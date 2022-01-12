/*
    SPDX-FileCopyrightText: 2004-2018 Gilles Caulier <caulier dot gilles at gmail dot com>
    SPDX-FileCopyrightText: 2006-2012 Marcel Wiesweg <marcel dot wiesweg at gmx dot de>
    SPDX-FileCopyrightText: 2004-2005 Renchi Raju <renchi dot raju at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Jesper K. Pedersen <blackie at kde dot org>
    SPDX-FileCopyrightText: 2004-2005 Aurelien Gateau <aurelien dot gateau at free dot fr>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KIPI_IMAGEINFOSHARED_H
#define KIPI_IMAGEINFOSHARED_H

// Std includes

#include <memory>

// Qt includes

#include <QString>
#include <QMap>
#include <QDateTime>
#include <QVariant>
#include <QUrl>

// Local includes

#include "imageinfo.h"
#include "libkipi_export.h"

namespace KIPI
{

class Interface;
class FileReadWriteLock;

/**
 * @class ImageInfoShared imageinfoshared.h <KIPI/ImageInfoShared>
 *
 * See ImageInfo documentation for details.
 */
class LIBKIPI_EXPORT ImageInfoShared
{

public:

    ImageInfoShared(Interface* const interface, const QUrl &url);
    virtual ~ImageInfoShared();

    QUrl url() const;

    /** Convenience methods calling the respective methods in the interface for this item.
     */
    bool               reserveForAction(QObject* const reservingObject, const QString& descriptionOfAction) const;
    void               clearReservation(QObject* const reservingObject);
    bool               itemIsReserved(QString* const descriptionOfAction = nullptr) const;
    FileReadWriteLock* createReadWriteLock() const;

    /** These methods must be re-implemented in your KIPI host application to manage image attributes with plugins.
     */
    virtual QMap<QString, QVariant> attributes() = 0;
    virtual void                    clearAttributes() = 0;
    virtual void                    addAttributes(const QMap<QString, QVariant>&) = 0;
    virtual void                    delAttributes(const QStringList&) = 0;

    virtual void cloneData(ImageInfoShared* const other);

protected:

    QUrl _url;

private:

    ImageInfoShared();    // Disable

    /// Used by ImageInfo class.
    void addRef();
    void removeRef();

private:

    class Private;
    std::unique_ptr<Private> const d;

    friend class ImageInfo;
};

} // namespace KIPI

#endif /* IMAGEINFOSHARED_H */
