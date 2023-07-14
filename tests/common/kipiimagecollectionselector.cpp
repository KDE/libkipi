/*
    SPDX-FileCopyrightText: 2009-2010 Michael G. Hansen <mike at mghansen dot de>
    SPDX-FileCopyrightText: 2011-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
    Q_EMIT selectionChanged();
}

} // namespace KXMLKipiCmd

#include "moc_kipiimagecollectionselector.cpp"
