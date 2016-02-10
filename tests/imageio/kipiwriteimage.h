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

#ifndef KIPIWRITEIMAGE_H
#define KIPIWRITEIMAGE_H

// C++ includes

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

// C ANSI includes

extern "C"
{
#include <jpeglib.h>
#include <tiffio.h>
#include <tiff.h>
#include <png.h>
}

// Qt includes

#include <QByteArray>
#include <QString>

namespace KXMLKipiCmd
{

class KIPIWriteImage
{
public:

    KIPIWriteImage();
    ~KIPIWriteImage();

    void setImageData(const QByteArray& data, uint width, uint height,
                      bool  sixteenBit, bool hasAlpha,
                      const QByteArray& iccProfile);

    void setCancel(bool* const cancel);
    bool cancel() const;

    bool write2JPEG(const QString& destPath);
    bool write2PNG(const QString& destPath);
    bool write2TIFF(const QString& destPath);
    bool write2PPM(const QString& destPath);

private:

    void   writeRawProfile(png_struct* const ping, png_info* const ping_info, char* const profile_type,
                           char* const profile_data, png_uint_32 length);

    size_t concatenateString(char* const destination, const char* source, const size_t length);
    size_t copyString(char* const destination, const char* source, const size_t length);
    long   formatString(char* const string, const size_t length, const char* format, ...);
    long   formatStringList(char* const string, const size_t length, const char* format, va_list operands);

    int bytesDepth() const;

    static void kipi_tiff_warning(const char* module, const char* format, va_list warnings);
    static void kipi_tiff_error(const char* module, const char* format, va_list errors);

private:

    class Private;
    Private* const d;
};

}  // namespace KXMLKipiCmd

#endif /* KIPIWRITEIMAGE_H */
