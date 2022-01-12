/*
    SPDX-FileCopyrightText: 2009-2010 Patrick Spendrin <ps_ml@gmx.de>
    SPDX-FileCopyrightText: 2007-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KIPIWRITEHELP_H
#define KIPIWRITEHELP_H

// C++ includes

#include <cstdio>

// Qt includes

#include <QIODevice>

// C ANSI includes

extern "C"
{
#include <jpeglib.h>
#include <png.h>
}

namespace KXMLKipiCmd
{

/**
  * a replacement function for jpeg_stdio_dest
  * for convenience reasons, it uses a QIODevice instead of a QFile, but the main advantage is to not give over
  * a FILE* pointer on Windows which can break due to different MS C Runtime libraries.
  *
  * Prepare for output to a QIODevice.
  * The caller must have already opened the device, and is responsible
  * for closing it after finishing compression.
  */
void kipi_jpeg_qiodevice_dest(j_compress_ptr cinfo, QIODevice* const outfile);

/**
  * a replacement function for jpeg_stdio_src
  * for convenience reasons, it uses a QIODevice instead of a QFile, but the main advantage is to not give over
  * a FILE* pointer on Windows which can break due to different MS C Runtime libraries.
  *
  * Prepare for input from a QIODevice.
  * The caller must have already opened the device, and is responsible
  * for closing it after finishing reading.
  */
void kipi_jpeg_qiodevice_src(j_decompress_ptr cinfo, QIODevice* const infile);

/**
  * a callback function for writing a png image
  */
void kipi_png_write_fn(png_structp png_ptr, png_bytep data, png_size_t length);

/**
  * a callback function for flushing the buffers, currently unused, since no buffering happens
  */
void kipi_png_flush_fn(png_structp png_ptr);

/**
 * To manage Errors/Warnings handling provide by libtiff
 */
void kipi_tiff_warning(const char* module, const char* format, va_list warnings);
void kipi_tiff_error(const char* module, const char* format, va_list errors);

}  // namespace KXMLKipiCmd

#endif // KIPIWRITEHELP_H
