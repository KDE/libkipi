/*
    SPDX-FileCopyrightText: 2009-2010 Michael G. Hansen <mike at mghansen dot de>
    SPDX-FileCopyrightText: 2011-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
