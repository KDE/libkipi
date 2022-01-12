/*
    SPDX-FileCopyrightText: 2009-2010 Michael G. Hansen <mike at mghansen dot de>
    SPDX-FileCopyrightText: 2011-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef __KIPIUPLOADWIDGET_H
#define __KIPIUPLOADWIDGET_H

// Libkipi includes

#include "uploadwidget.h"

class QListWidget;

using namespace KIPI;

namespace KXMLKipiCmd
{

class KipiInterface;

class KipiUploadWidget : public UploadWidget
{
    Q_OBJECT

public:

    KipiUploadWidget(KipiInterface* const interface, QWidget* const parent);
    ~KipiUploadWidget() override;

    ImageCollection selectedImageCollection() const override;

public Q_SLOTS:

    void slotItemSelectionChanged();

private:

    KipiInterface* const   m_interface;
    QListWidget*           m_listWidget;
    QList<ImageCollection> m_allAlbums;
};

} // namespace KXMLKipiCmd

#endif // __KIPIUPLOADWIDGET_H
