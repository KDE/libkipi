/*
    SPDX-FileCopyrightText: 2009-2010 Michael G. Hansen <mike at mghansen dot de>
    SPDX-FileCopyrightText: 2011-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
      m_listWidget(nullptr)
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
        return ImageCollection(nullptr);
    }

    const int row = m_listWidget->row(selectedItems.at(0));

    return m_allAlbums.at(row);
}

void KipiUploadWidget::slotItemSelectionChanged()
{
    Q_EMIT selectionChanged();
}

} // namespace KXMLKipiCmd

#include "moc_kipiuploadwidget.cpp"
