/** ===========================================================
 * @file
 *
 * This file is a part of kipi-plugins project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * Date        : 2007-19-09
 * Description : Interface to write image data to common picture format.
 *
 * Copyright (C) 2007-2016 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

//#define ENABLE_DEBUG_MESSAGES 1

#include "kipiwriteimage.h"
#include "kipiwritehelp.h"

// C ANSI includes

extern "C"
{
#ifndef _WIN32
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#endif
#include <sys/types.h>
#include <tiffvers.h>
}

// Qt includes

#include <QFile>
#include <QDebug>
#include <QDataStream>
#include <QStandardPaths>

namespace KXMLKipiCmd
{

class KIPIWriteImage::Private
{
public:

    Private()
    {
        sixteenBit     = false;
        hasAlpha       = false;
        width          = 0;
        height         = 0;
        cancel         = 0;
    }

    bool*      cancel;

    bool       sixteenBit;
    bool       hasAlpha;

    uint       width;
    uint       height;

    QByteArray data;         // BGR(A) image data
                             // data[0] = blue, data[1] = green, data[2] = red, data[3] = alpha.
};

KIPIWriteImage::KIPIWriteImage()
    : d(new Private)
{
}

KIPIWriteImage::~KIPIWriteImage()
{
    delete d;
}

int KIPIWriteImage::bytesDepth() const
{
    if (d->sixteenBit)
    {
        if (d->hasAlpha)
            return 8;
        else
            return 6;
    }

    if (d->hasAlpha)
        return 4;

    return 3;
}

void KIPIWriteImage::setCancel(bool* const cancel)
{
    d->cancel = cancel;
}

bool KIPIWriteImage::cancel() const
{
    if (d->cancel)
        return *d->cancel;

    return false;
}

void KIPIWriteImage::setImageData(const QByteArray& data, uint width, uint height,
                                bool  sixteenBit, bool hasAlpha)
{
    d->data       = data;
    d->width      = width;
    d->height     = height;
    d->sixteenBit = sixteenBit;
    d->hasAlpha   = hasAlpha;
}

bool KIPIWriteImage::write2JPEG(const QString& destPath)
{
    QFile file(destPath);

    if (!file.open(QIODevice::ReadWrite))
    {
        qDebug() << "Failed to open JPEG file for writing" ;
        return false;
    }

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr       jerr;

    // Init JPEG compressor.
    cinfo.err              = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    kp_jpeg_qiodevice_dest(&cinfo, &file);
    cinfo.image_width      = d->width;
    cinfo.image_height     = d->height;
    cinfo.input_components = 3;
    cinfo.in_color_space   = JCS_RGB;
    jpeg_set_defaults(&cinfo);

    // bug #149578: set encoder horizontal and vertical chroma subsampling
    // factor to 2x1, 1x1, 1x1 (4:2:2) : Medium subsampling.
    // See this page for details: http://en.wikipedia.org/wiki/Chroma_subsampling
    cinfo.comp_info[0].h_samp_factor = 2;
    cinfo.comp_info[0].v_samp_factor = 1;
    cinfo.comp_info[1].h_samp_factor = 1;
    cinfo.comp_info[1].v_samp_factor = 1;
    cinfo.comp_info[2].h_samp_factor = 1;
    cinfo.comp_info[2].v_samp_factor = 1;

    // bug #154273: use 99 compression level instead 100 to reduce output JPEG file size.
    jpeg_set_quality(&cinfo, 99, true);
    jpeg_start_compress(&cinfo, true);

    // Write image data
    uchar* line   = new uchar[d->width*3];
    uchar* dstPtr = 0;

    if (!d->sixteenBit)     // 8 bits image.
    {
        uchar* srcPtr = (uchar*)d->data.data();

        for (uint j=0; j < d->height; ++j)
        {
            if (cancel())
            {
                delete [] line;
                jpeg_destroy_compress(&cinfo);
                file.close();
                return false;
            }

            dstPtr = line;

            for (uint i = 0; i < d->width; ++i)
            {
                dstPtr[2] = srcPtr[0];  // Blue
                dstPtr[1] = srcPtr[1];  // Green
                dstPtr[0] = srcPtr[2];  // Red

                d->hasAlpha ? srcPtr += 4 : srcPtr += 3;
                dstPtr += 3;
            }

            jpeg_write_scanlines(&cinfo, &line, 1);
        }
    }
    else                    // 16 bits image
    {
        unsigned short* srcPtr = reinterpret_cast<unsigned short*>(d->data.data());

        for (uint j=0; j < d->height; ++j)
        {
            if (cancel())
            {
                delete [] line;
                jpeg_destroy_compress(&cinfo);
                file.close();
                return false;
            }

            dstPtr = line;

            for (uint i = 0; i < d->width; ++i)
            {
                dstPtr[2] = (srcPtr[0] * 255UL)/65535UL;    // Blue
                dstPtr[1] = (srcPtr[1] * 255UL)/65535UL;    // Green
                dstPtr[0] = (srcPtr[2] * 255UL)/65535UL;    // Red

                d->hasAlpha ? srcPtr += 4 : srcPtr += 3;
                dstPtr += 3;
            }

            jpeg_write_scanlines(&cinfo, &line, 1);
        }
    }

    delete [] line;

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    file.close();

    return true;
}

bool KIPIWriteImage::write2PPM(const QString& destPath)
{
    FILE* const file = fopen((const char*)(QFile::encodeName(destPath)).constData(), "wb");

    if (!file)
    {
        qDebug() << "Failed to open ppm file for writing" ;
        return false;
    }

    fprintf(file, "P6\n%d %d\n255\n", d->width, d->height);

    // Write image data
    uchar* const line = new uchar[d->width*3];
    uchar* dstPtr     = 0;

    if (!d->sixteenBit)     // 8 bits image.
    {
        uchar* srcPtr = (uchar*)d->data.data();

        for (uint j=0; j < d->height; ++j)
        {
            if (cancel())
            {
                delete [] line;
                fclose(file);
                return false;
            }

            dstPtr = line;

            for (uint i = 0; i < d->width; ++i)
            {
                dstPtr[2] = srcPtr[0];  // Blue
                dstPtr[1] = srcPtr[1];  // Green
                dstPtr[0] = srcPtr[2];  // Red

                d->hasAlpha ? srcPtr += 4 : srcPtr += 3;
                dstPtr += 3;
            }

            fwrite(line, 1, d->width*3, file);
        }
    }
    else                    // 16 bits image
    {
        unsigned short* srcPtr = reinterpret_cast<unsigned short*>(d->data.data());

        for (uint j=0; j < d->height; ++j)
        {
            if (cancel())
            {
                delete [] line;
                fclose(file);
                return false;
            }

            dstPtr = line;

            for (uint i = 0; i < d->width; ++i)
            {
                dstPtr[2] = (srcPtr[0] * 255UL)/65535UL;    // Blue
                dstPtr[1] = (srcPtr[1] * 255UL)/65535UL;    // Green
                dstPtr[0] = (srcPtr[2] * 255UL)/65535UL;    // Red

                d->hasAlpha ? srcPtr += 4 : srcPtr += 3;
                dstPtr += 3;
            }

            fwrite(line, 1, d->width*3, file);
        }
    }

    delete [] line;
    fclose(file);

    return true;
}

bool KIPIWriteImage::write2PNG(const QString& destPath)
{
    /*
    check this out for b/w support:
    http://lxr.kde.org/source/playground/graphics/krita-exp/kis_png_converter.cpp#607
    */
    QFile file(destPath);

    if (!file.open(QIODevice::ReadWrite))
    {
        qDebug() << "Failed to open PNG file for writing" ;
        return false;
    }

    uchar*       data       = 0;
    int          bitsDepth  = d->sixteenBit ? 16 : 8;
    png_color_8  sig_bit;
    png_bytep    row_ptr;
    png_structp  png_ptr    = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop    info_ptr   = png_create_info_struct(png_ptr);

    png_set_write_fn(png_ptr, (void*)&file, kp_png_write_fn, kp_png_flush_fn);

    if (QSysInfo::ByteOrder == QSysInfo::LittleEndian)      // Intel
        png_set_bgr(png_ptr);
    else                                                    // PPC
        png_set_swap_alpha(png_ptr);

    if (d->hasAlpha)
    {
        png_set_IHDR(png_ptr, info_ptr, d->width, d->height, bitsDepth,
                     PNG_COLOR_TYPE_RGB_ALPHA,  PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        if (d->sixteenBit)
            data = new uchar[d->width * 8 * sizeof(uchar)];
        else
            data = new uchar[d->width * 4 * sizeof(uchar)];
    }
    else
    {
        png_set_IHDR(png_ptr, info_ptr, d->width, d->height, bitsDepth,
                     PNG_COLOR_TYPE_RGB,        PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        if (d->sixteenBit)
            data = new uchar[d->width * 6 * sizeof(uchar)];
        else
            data = new uchar[d->width * 3 * sizeof(uchar)];
    }

    sig_bit.red   = bitsDepth;
    sig_bit.green = bitsDepth;
    sig_bit.blue  = bitsDepth;
    sig_bit.alpha = bitsDepth;
    png_set_sBIT(png_ptr, info_ptr, &sig_bit);
    png_set_compression_level(png_ptr, 9);

    // Write Software info.
    QString libpngver(QLatin1String(PNG_HEADER_VERSION_STRING));
    libpngver.replace(QLatin1Char('\n'), QLatin1Char(' '));
    QByteArray softAscii = libpngver.toLatin1();
    png_text text;
    text.key         = (png_charp)"Software";
    text.text        = softAscii.data();
    text.compression = PNG_TEXT_COMPRESSION_zTXt;
    png_set_text(png_ptr, info_ptr, &(text), 1);

    png_write_info(png_ptr, info_ptr);
    png_set_shift(png_ptr, &sig_bit);
    png_set_packing(png_ptr);

    uchar* ptr = (uchar*)d->data.data();
    uint   x, y, j;

    for (y = 0; y < d->height; ++y)
    {
        if (cancel())
        {
            delete [] data;
            file.close();
            png_destroy_write_struct(&png_ptr, (png_infopp) & info_ptr);
            png_destroy_info_struct(png_ptr, (png_infopp) & info_ptr);
            return false;
        }

        j = 0;

        for (x = 0; x < d->width*bytesDepth(); x+=bytesDepth())
        {
            if (d->sixteenBit)
            {
                if (d->hasAlpha)
                {
                    data[j++] = ptr[x+1];  // Blue
                    data[j++] = ptr[ x ];
                    data[j++] = ptr[x+3];  // Green
                    data[j++] = ptr[x+2];
                    data[j++] = ptr[x+5];  // Red
                    data[j++] = ptr[x+4];
                    data[j++] = ptr[x+7];  // Alpha
                    data[j++] = ptr[x+6];
                }
                else
                {
                    data[j++] = ptr[x+1];  // Blue
                    data[j++] = ptr[ x ];
                    data[j++] = ptr[x+3];  // Green
                    data[j++] = ptr[x+2];
                    data[j++] = ptr[x+5];  // Red
                    data[j++] = ptr[x+4];
                }
            }
            else
            {
                if (d->hasAlpha)
                {
                    data[j++] = ptr[ x ];  // Blue
                    data[j++] = ptr[x+1];  // Green
                    data[j++] = ptr[x+2];  // Red
                    data[j++] = ptr[x+3];  // Alpha
                }
                else
                {
                    data[j++] = ptr[ x ];  // Blue
                    data[j++] = ptr[x+1];  // Green
                    data[j++] = ptr[x+2];  // Red
                }
            }
        }

        row_ptr = (png_bytep) data;

        png_write_rows(png_ptr, &row_ptr, 1);
        ptr += (d->width * bytesDepth());
    }

    delete [] data;

    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, (png_infopp) & info_ptr);
    png_destroy_info_struct(png_ptr, (png_infopp) & info_ptr);
    file.close();

    return true;
}

bool KIPIWriteImage::write2TIFF(const QString& destPath)
{
    uint32 w          = d->width;
    uint32 h          = d->height;
    uchar* const data = (uchar*)d->data.data();

    // TIFF error handling. If an errors/warnings occurs during reading,
    // libtiff will call these methods

    TIFFSetWarningHandler(kipi_tiff_warning);
    TIFFSetErrorHandler(kipi_tiff_error);

    // Open target file

    TIFF* const tif = TIFFOpen((const char*)(QFile::encodeName(destPath)).constData(), "w");

    if (!tif)
    {
        qDebug() << "Failed to open TIFF file for writing" ;
        return false;
    }

    int bitsDepth = d->sixteenBit ? 16 : 8;

    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH,          w);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH,         h);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC,         PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG,        PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_ORIENTATION,         ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT,      RESUNIT_NONE);
    TIFFSetField(tif, TIFFTAG_COMPRESSION,         COMPRESSION_ADOBE_DEFLATE);
    TIFFSetField(tif, TIFFTAG_ZIPQUALITY,          9);
    // NOTE : this tag values aren't defined in libtiff 3.6.1. '2' is PREDICTOR_HORIZONTAL.
    //        Use horizontal differencing for images which are
    //        likely to be continuous tone. The TIFF spec says that this
    //        usually leads to better compression.
    //        See this url for more details:
    //        http://www.awaresystems.be/imaging/tiff/tifftags/predictor.html
    TIFFSetField(tif, TIFFTAG_PREDICTOR,           2);

    uint16 sampleinfo[1];

    if (d->hasAlpha)
    {
        sampleinfo[0] = EXTRASAMPLE_ASSOCALPHA;
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 4);
        TIFFSetField(tif, TIFFTAG_EXTRASAMPLES,    1, sampleinfo);
    }
    else
    {
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
    }

    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE,       (uint16)bitsDepth);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP,        TIFFDefaultStripSize(tif, 0));

    QString libtiffver(QLatin1String(TIFFLIB_VERSION_STR));
    libtiffver.replace(QLatin1Char('\n'), QLatin1Char(' '));
    TIFFSetField(tif, TIFFTAG_SOFTWARE, (const char*)libtiffver.toLatin1().data());


    // Write full image data in tiff directory IFD0

    uchar*  pixel=0;
    double  alpha_factor;
    uint32  x, y;
    uint8   r8, g8, b8, a8=0;
    uint16  r16, g16, b16, a16=0;
    int     i=0;

    uint8* const buf = (uint8 *)_TIFFmalloc(TIFFScanlineSize(tif));

    if (!buf)
    {
        qDebug() << "Cannot allocate memory buffer for main TIFF image." ;
        TIFFClose(tif);
        return false;
    }

    for (y = 0; y < h; ++y)
    {
        if (cancel())
        {
            _TIFFfree(buf);
            TIFFClose(tif);
            return false;
        }

        i = 0;

        for (x = 0; x < w; ++x)
        {
            pixel = &data[((y * w) + x) * bytesDepth()];

            if ( d->sixteenBit )        // 16 bits image.
            {
                b16 = (uint16)(pixel[0]+256*pixel[1]);
                g16 = (uint16)(pixel[2]+256*pixel[3]);
                r16 = (uint16)(pixel[4]+256*pixel[5]);

                if (d->hasAlpha)
                {
                    // TIFF makes you pre-multiply the rgb components by alpha

                    a16          = (uint16)(pixel[6]+256*pixel[7]);
                    alpha_factor = ((double)a16 / 65535.0);
                    r16          = (uint16)(r16*alpha_factor);
                    g16          = (uint16)(g16*alpha_factor);
                    b16          = (uint16)(b16*alpha_factor);
                }

                // This might be endian dependent

                buf[i++] = (uint8)(r16);
                buf[i++] = (uint8)(r16 >> 8);
                buf[i++] = (uint8)(g16);
                buf[i++] = (uint8)(g16 >> 8);
                buf[i++] = (uint8)(b16);
                buf[i++] = (uint8)(b16 >> 8);

                if (d->hasAlpha)
                {
                    buf[i++] = (uint8)(a16) ;
                    buf[i++] = (uint8)(a16 >> 8) ;
                }
            }
            else                            // 8 bits image.
            {
                b8 = (uint8)pixel[0];
                g8 = (uint8)pixel[1];
                r8 = (uint8)pixel[2];

                if (d->hasAlpha)
                {
                    // TIFF makes you pre-multiply the rgb components by alpha

                    a8           = (uint8)(pixel[3]);
                    alpha_factor = ((double)a8 / 255.0);
                    r8           = (uint8)(r8*alpha_factor);
                    g8           = (uint8)(g8*alpha_factor);
                    b8           = (uint8)(b8*alpha_factor);
                }

                // This might be endian dependent

                buf[i++] = r8;
                buf[i++] = g8;
                buf[i++] = b8;

                if (d->hasAlpha)
                    buf[i++] = a8;
            }
        }

        if (!TIFFWriteScanline(tif, buf, y, 0))
        {
            qDebug() << "Cannot write main TIFF image to target file." ;
            _TIFFfree(buf);
            TIFFClose(tif);
            return false;
        }
    }

    _TIFFfree(buf);
    TIFFWriteDirectory(tif);
    TIFFClose(tif);

    return true;
}

// To manage Errors/Warnings handling provide by libtiff

void KIPIWriteImage::kipi_tiff_warning(const char* module, const char* format, va_list warnings)
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

void KIPIWriteImage::kipi_tiff_error(const char* module, const char* format, va_list errors)
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
