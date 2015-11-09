/** ===========================================================
 * @file
 *
 * This file is a part of kipi-plugins project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2009-11-21
 * @brief  kipi host test application
 *
 * @author Copyright (C) 2009-2010 by Michael G. Hansen
 *         <a href="mailto:mike at mghansen dot de">mike at mghansen dot de</a>
 * @author Copyright (C) 2011-2015 by Gilles Caulier
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

#include "kipiuploadwidget.h"

// Qt includes:

#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

// Libkipi includes

#include "imagecollection.h"

// Local includes

#include "kipiinterface.h"

namespace KXMLKipiCmd
{

KipiUploadWidget::KipiUploadWidget(KipiInterface* const interface, QWidget* const parent)
    : UploadWidget(parent),
      m_interface(interface),
      m_listWidget(0)
{
    QVBoxLayout* const layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(QString::fromLatin1("Please select a target album:")));  // no need i18n.
    m_listWidget              = new QListWidget(this);
    layout->addWidget(m_listWidget);

    setLayout(layout);

    connect(m_listWidget, &QListWidget::itemSelectionChanged, this, &KipiUploadWidget::slotItemSelectionChanged);

    // add all albums to the list widget:
    m_allAlbums = m_interface->allAlbums();

    for (QList<ImageCollection>::const_iterator it = m_allAlbums.constBegin(); it != m_allAlbums.constEnd(); ++it)
    {
        m_listWidget->addItem(it->name());

        // is the album selected?
        const QUrl itemPath = it->url();
        m_listWidget->item(m_listWidget->count()-1)->setSelected(m_interface->m_selectedAlbums.contains(itemPath));
    }
}

KipiUploadWidget::~KipiUploadWidget()
{
}

ImageCollection KipiUploadWidget::selectedImageCollection() const
{
    // return the selected albums (should be only one):
    const QList<QListWidgetItem*> selectedItems = m_listWidget->selectedItems();

    if (selectedItems.isEmpty())
    {
        // this should not happen!!! the calling application will probably crash now...
        qDebug() << "Nothing selected... impossible!";
        return ImageCollection(0);
    }

    const int row = m_listWidget->row(selectedItems.at(0));

    return m_allAlbums.at(row);
}

void KipiUploadWidget::slotItemSelectionChanged()
{
    emit(selectionChanged());
}

} // namespace KXMLKipiCmd
