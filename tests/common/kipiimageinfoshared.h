/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 *
 *
 * @date   2009-11-21
 * @brief  kipi host test application
 *
 * @author Copyright (C) 2009-2010 by Michael G. Hansen
 *         <a href="mailto:mike at mghansen dot de">mike at mghansen dot de</a>
 * @author Copyright (C) 2011-2018 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef KIPIIMAGEINFOSHARED_H
#define KIPIIMAGEINFOSHARED_H

// Libkipi includes

#include "imageinfoshared.h"

using namespace KIPI;

namespace KXMLKipiCmd
{

class KipiImageInfoShared : public ImageInfoShared
{
public:

    KipiImageInfoShared(Interface* const interface, const QUrl& url);
    ~KipiImageInfoShared() override;

    QMap<QString, QVariant> attributes() override;
    void                    addAttributes(const QMap<QString, QVariant>& attributes) override;
    void                    delAttributes(const QStringList& attributes) override;
    void                    clearAttributes() override;

private:

    class Private;
    Private* const d;
};

} // namespace KXMLKipiCmd

#endif // KIPIIMAGEINFOSHARED_H
