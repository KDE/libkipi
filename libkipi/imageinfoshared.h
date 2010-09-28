/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-19
 * @brief  image info shared
 *
 * @author Copyright (C) 2004-2010 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2004-2005 by Renchi Raju
 *         <a href="mailto:renchi dot raju at gmail dot com">renchi dot raju at gmail dot com</a>
 * @author Copyright (C) 2004-2005 by Jesper K. Pedersen
 *         <a href="mailto:blackie at kde dot org">blackie at kde dot org</a>
 * @author Copyright (C) 2004-2005 by Aurelien Gateau
 *         <a href="mailto:aurelien dot gateau at free dot fr">aurelien dot gateau at free dot fr</a>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef KIPI_IMAGEINFOSHARED_H
#define KIPI_IMAGEINFOSHARED_H

// Qt includes.

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QDateTime>
#include <QtCore/QVariant>

// KDE includes.

#include <kurl.h>

// Local includes.

#include "imageinfo.h"
#include "libkipi_export.h"

namespace KIPI
{

class Interface;

class LIBKIPI_EXPORT ImageInfoShared
{

public:

    ImageInfoShared( KIPI::Interface* const interface, const KUrl& url );
    virtual ~ImageInfoShared() {}
    virtual QString title() {return QString::null;}
    virtual void setTitle( const QString& );

    virtual QString description() = 0;
    virtual void setDescription( const QString& ) = 0;

    virtual QMap<QString,QVariant> attributes() = 0;
    virtual void clearAttributes() = 0;
    virtual void addAttributes( const QMap<QString,QVariant>& ) = 0;
    virtual void delAttributes( const QStringList& ) = 0;

    virtual KUrl path();
    virtual QDateTime time( KIPI::TimeSpec spec );
    virtual void setTime( const QDateTime& time, TimeSpec spec = FromInfo );

    virtual bool isTimeExact();

    virtual int size();

    virtual int angle();
    virtual void setAngle( int );

    QString toString( const QVariant& );

    virtual void cloneData( ImageInfoShared* const other );

protected:

    KUrl _url;

private:

    ImageInfoShared() {} // Disable

    void addRef();
    void removeRef();

private:

    int        m_count;
    Interface* m_interface;

private:

    friend class ImageInfo;
};

} // namespace KIPI

#endif /* IMAGEINFOSHARED_H */
