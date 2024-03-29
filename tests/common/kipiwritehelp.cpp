/*
    SPDX-FileCopyrightText: 1994-1996 Thomas G. Lane.
    SPDX-FileCopyrightText: 2009-2010 Patrick Spendrin <ps_ml@gmx.de>
    SPDX-FileCopyrightText: 2007-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    This file is based on jdatadst.c from libjpeg.

    SPDX-License-Identifier: GPL-2.0-or-later
*/

//#define ENABLE_DEBUG_MESSAGES 1

#include "kipiwritehelp.h"

// Qt includes

#include <QIODevice>
#include <QDebug>

// LibJPEG includes

extern "C"
{
#include <jerror.h>
}

/* choose an efficiently fwrite'able size */
#define BUFFER_SIZE  4096

namespace KXMLKipiCmd
{

//-- JPG helper methods ---------------------------------------------------------------------

/**
 Expanded data destination object for input/output for jpeg
 */
typedef struct
{
    struct jpeg_destination_mgr pub; /* public fields */

    QIODevice* outDevice;   /* target stream */
    JOCTET*    buffer;      /* start of buffer */
} my_destination_mgr;

typedef my_destination_mgr* my_dest_ptr;

typedef struct
{
    struct jpeg_source_mgr pub; /* public fields */

    QIODevice* inDevice;
    JOCTET     buffer[BUFFER_SIZE];
} my_source_mgr;

/**
 * Initialize destination --- called by jpeg_start_compress
 * before any data is actually written.
 */
void init_destination (j_compress_ptr cinfo)
{
    my_dest_ptr dest           = (my_dest_ptr) cinfo->dest;

    /* Allocate the output buffer --- it will be released when done with image */
    dest->buffer               = (JOCTET*) (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
                                 BUFFER_SIZE * (size_t)sizeof(JOCTET));

    dest->pub.next_output_byte = dest->buffer;
    dest->pub.free_in_buffer   = BUFFER_SIZE;
}

/**
 * Empty the output buffer --- called whenever buffer fills up.
 */
boolean empty_output_buffer (j_compress_ptr cinfo)
{
    my_dest_ptr dest = (my_dest_ptr) cinfo->dest;

    if (dest->outDevice->write((char*)dest->buffer, BUFFER_SIZE) != (size_t) BUFFER_SIZE)
        ERREXIT(cinfo, JERR_FILE_WRITE);

    dest->pub.next_output_byte = dest->buffer;
    dest->pub.free_in_buffer   = BUFFER_SIZE;

    return true;
}

/**
 * Terminate destination --- called by jpeg_finish_compress
 * after all data has been written.  Usually needs to flush buffer.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */
void term_destination (j_compress_ptr cinfo)
{
    my_dest_ptr dest = (my_dest_ptr) cinfo->dest;
    size_t datacount = BUFFER_SIZE - dest->pub.free_in_buffer;

    /* Write any data remaining in the buffer */
    if (datacount > 0)
    {
        if ((size_t)dest->outDevice->write((char*)dest->buffer, datacount) != (size_t)datacount)
            ERREXIT(cinfo, JERR_FILE_WRITE);
    }
}

void kipi_jpeg_qiodevice_dest (j_compress_ptr cinfo, QIODevice* const outDevice)
{
    my_dest_ptr dest;

    /* The destination object is made permanent so that multiple JPEG images
    * can be written to the same file without re-executing jpeg_stdio_dest.
    * This makes it dangerous to use this manager and a different destination
    * manager serially with the same JPEG object, because their private object
    * sizes may be different.  Caveat programmer.
    */
    if (cinfo->dest == nullptr)
    {
        /* first time for this JPEG object? */
        cinfo->dest = (struct jpeg_destination_mgr*)
                      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
                      (size_t)sizeof(my_destination_mgr));
    }

    dest                          = (my_dest_ptr) cinfo->dest;
    dest->pub.init_destination    = init_destination;
    dest->pub.empty_output_buffer = empty_output_buffer;
    dest->pub.term_destination    = term_destination;
    dest->outDevice               = outDevice;
}

boolean fill_input_buffer(j_decompress_ptr cinfo)
{
    my_source_mgr* const src = (my_source_mgr*)cinfo->src;
    Q_ASSERT(src->inDevice);
    int readSize             = src->inDevice->read((char*)src->buffer, BUFFER_SIZE);

    if (readSize > 0)
    {
        src->pub.next_input_byte = src->buffer;
        src->pub.bytes_in_buffer = readSize;
    }
    else
    {
        /**
        * JPEG file is broken. We feed the decoder with fake EOI, as specified
        * in the libjpeg documentation.
        */
        static JOCTET fakeEOI[2] = { JOCTET(0xFF), JOCTET(JPEG_EOI)};
        qWarning() << "Image is incomplete";
        src->pub.next_input_byte = fakeEOI;
        src->pub.bytes_in_buffer = 2;
    }

    return true;
}

void init_source(j_decompress_ptr cinfo)
{
    fill_input_buffer(cinfo);
}

void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
    my_source_mgr* const src = (my_source_mgr*)cinfo->src;

    if (num_bytes > 0)
    {
        while (num_bytes > (long) src->pub.bytes_in_buffer)
        {
            num_bytes -= (long) src->pub.bytes_in_buffer;
            fill_input_buffer(cinfo);
            /**
            * we assume that fill_input_buffer will never return FALSE, so
            * suspension need not be handled.
            */
        }
        src->pub.next_input_byte += (size_t) num_bytes;
        src->pub.bytes_in_buffer -= (size_t) num_bytes;
    }
}

void term_source(j_decompress_ptr)
{
}

void kipi_jpeg_qiodevice_src(j_decompress_ptr cinfo, QIODevice* const ioDevice)
{
    Q_ASSERT(!cinfo->src);
    my_source_mgr* const src   = (my_source_mgr*)
                                 (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
                                 sizeof(my_source_mgr));
    cinfo->src                 = (jpeg_source_mgr*)src;

    src->pub.init_source       = init_source;
    src->pub.fill_input_buffer = fill_input_buffer;
    src->pub.skip_input_data   = skip_input_data;
    src->pub.resync_to_restart = jpeg_resync_to_restart;
    src->pub.term_source       = term_source;
    src->inDevice              = ioDevice;
}

//-- PNG helper methods ---------------------------------------------------------------------

void kipi_png_write_fn(png_structp png_ptr, png_bytep data, png_size_t length)
{
    QIODevice* const out = (QIODevice*)png_get_io_ptr(png_ptr);
    uint nr              = out->write((char*)data, length);

    if (nr != length)
    {
        png_error(png_ptr, "Write Error");
        return;
    }
}

void kipi_png_flush_fn(png_structp png_ptr)
{
    Q_UNUSED(png_ptr);
}

//-- TIF helper methods ---------------------------------------------------------------------

void kipi_tiff_warning(const char* module, const char* format, va_list warnings)
{
#ifdef ENABLE_DEBUG_MESSAGES
    char message[4096];
    vsnprintf(message, 4096, format, warnings);
    qDebug() << module << "::" << message ;
#else
    Q_UNUSED(module);
    Q_UNUSED(format);
    Q_UNUSED(warnings);
#endif
}

void kipi_tiff_error(const char* module, const char* format, va_list errors)
{
#ifdef ENABLE_DEBUG_MESSAGES
    char message[4096];
    vsnprintf(message, 4096, format, errors);
    qDebug() << module << "::" << message ;
#else
    Q_UNUSED(module);
    Q_UNUSED(format);
    Q_UNUSED(errors);
#endif
}

}  // namespace KXMLKipiCmd
