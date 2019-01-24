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

#include "kipiimagecollectionselector.h"

// Qt includes

#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

// Libkipi includes

#include "imagecollection.h"

// Local includes:

#include "kipiinterface.h"

namespace KXMLKipiCmd
{

KipiImageCollectionSelector::KipiImageCollectionSelector(KipiInterface* const interface, QWidget* const parent)
    : ImageCollectionSelector(parent),
      m_interface(interface),
      m_listWidget(nullptr)
{
    QVBoxLayout* const layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(QString::fromLatin1("Please select albums:")));
    m_listWidget              = new QListWidget(this);
    m_listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    layout->addWidget(m_listWidget);

    setLayout(layout);

    connect(m_listWidget, &QListWidget::itemSelectionChanged,
            this, &KipiImageCollectionSelector::slotItemSelectionChanged);

    // add all albums to the list widget:
    m_allAlbums = m_interface->allAlbums();

    for (QList<ImageCollection>::const_iterator it = m_allAlbums.constBegin();
         it!=m_allAlbums.constEnd(); ++it)
    {
        m_listWidget->addItem(it->name());

        // is the album selected?
        const QUrl itemPath = it->url();
        m_listWidget->item(m_listWidget->count()-1)->setSelected(m_interface->m_selectedAlbums.contains(itemPath));
    }

    //TODO: select albums specified on the command line
}

QList<ImageCollection> KipiImageCollectionSelector::selectedImageCollections() const
{
    // return the selected albums:
    QList<ImageCollection> result;
    const QList<QListWidgetItem*> selectedItems = m_listWidget->selectedItems();

    for (QList<QListWidgetItem*>::const_iterator it = selectedItems.constBegin();
         it!=selectedItems.constEnd(); ++it)
    {
        const int row = m_listWidget->row(*it);
        result.append(m_allAlbums.at(row));
    }

    //  result.append(m_interface->currentAlbum());
    qDebug() << QString::fromLatin1("selectedImageCollections: %1 items selected").arg(result.size());
    return result;
}

void KipiImageCollectionSelector::slotItemSelectionChanged()
{
    emit(selectionChanged());
}

} // namespace KXMLKipiCmd
