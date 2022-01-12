/*
    SPDX-FileCopyrightText: 2007-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
                      bool  sixteenBit, bool hasAlpha);

    void setCancel(bool* const cancel);
    bool cancel() const;

    bool write2JPEG(const QString& destPath);
    bool write2PNG(const QString& destPath);
    bool write2TIFF(const QString& destPath);
    bool write2PPM(const QString& destPath);

private:

    int bytesDepth() const;

private:

    class Private;
    Private* const d;
};

}  // namespace KXMLKipiCmd

#endif /* KIPIWRITEIMAGE_H */
