/** ===========================================================
 * @file
 *
 * This file is a part of kipi-plugins project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * Date        : 2009-13-11
 * Description : Helper functions for writing images
 *
 * Copyright (C) 2009-2010 by Patrick Spendrin <ps_ml@gmx.de>
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
