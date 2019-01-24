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
