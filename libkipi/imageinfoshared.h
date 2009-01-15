/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-02-19
 * Description : image info shared.
 *
 * Copyright (C) 2004-2009 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2004-2005 by Renchi Raju <renchi.raju at kdemail.net>
 * Copyright (C) 2004-2005 by Jesper K. Pedersen <blackie at kde.org>
 * Copyright (C) 2004-2005 by Aurelien Gateau <aurelien dot gateau at free.fr>
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

    ImageInfoShared( KIPI::Interface* interface, const KUrl& url );
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

    virtual void cloneData( ImageInfoShared* other );

protected:

    KUrl _url;

private:

    ImageInfoShared() {} // Disable

    friend class ImageInfo;
    void addRef();
    void removeRef();

private:

    int        _count;
    Interface* _interface;
};

} // namespace KIPI

#endif /* IMAGEINFOSHARED_H */
