/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-05-04
 * Description : Batch progress dialog
 *
 * Copyright (C) 2004-2007 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

// Include files for Qt

#include <QBrush>
#include <QWidget>
#include <QColorGroup>
#include <QListWidget>
#include <QProgressBar>

// Include files for KDE

#include <klocale.h>
#include <kiconloader.h>
#include <kdebug.h>
#include <kvbox.h>

// Include files for libKipi.

#include "version.h"

// Local includes

#include "batchprogressdialog.h"
#include "batchprogressdialog.moc"

namespace KIPI
{

class BatchProgressItem : public QListWidgetItem
{
public:

BatchProgressItem(QListWidget * parent, const QString& message, int messageType)
    : QListWidgetItem(message, parent)
{
   // Set the icon.

   switch( messageType )
   {
     case StartingMessage:
        setIcon(SmallIcon( "run" ));
        break;
     case SuccessMessage:
        setIcon(SmallIcon( "ok" ));
        break;
     case WarningMessage:
        setIcon(SmallIcon( "flag" ));
        setForeground( QBrush(Qt::darkYellow) );
        break;
     case ErrorMessage:
        setIcon(SmallIcon( "stop" ));
        setForeground( QBrush(Qt::red) );
        break;
     case ProgressMessage:
        setIcon(SmallIcon( "info" ));
        break;
     default:
        setIcon(SmallIcon( "info" ));
   }

   // Set the message text.

   setText(message);
}

};

// ----------------------------------------------------------------------

struct BatchProgressDialog::Private 
{
};

BatchProgressDialog::BatchProgressDialog( QWidget *parent, const QString &caption )
                   : KDialog( parent )
{
    d = new Private;

    setCaption(caption);
    setButtons( KDialog::Cancel );
    setDefaultButton(KDialog::Cancel);
    setModal(true);

    KVBox *box = new KVBox( this );
    setMainWidget( box );

    //---------------------------------------------

    m_actionsList = new QListWidget( box );
    m_actionsList->setSortingEnabled(false);
    m_actionsList->setWhatsThis( i18n("<p>This is the current tasks list released.") );

    //---------------------------------------------

    m_progress = new QProgressBar( box );
    m_progress->setRange(0, 100);
    m_progress->setValue(0);
    m_progress->setWhatsThis( i18n("<p>This is the list current percent task released.") );
    resize( 600, 400 );
}

BatchProgressDialog::~BatchProgressDialog()
{
    delete d;
}

void BatchProgressDialog::addedAction(const QString &text, int type)
{
    BatchProgressItem *item = new BatchProgressItem(m_actionsList, text, type);
    m_actionsList->setCurrentItem(item);
}

void BatchProgressDialog::reset()
{
    m_actionsList->clear();
    m_progress->setValue(0);
}

void BatchProgressDialog::setProgress(int current, int total)
{
    m_progress->setMaximum(total);
    m_progress->setValue(current);
}

}  // NameSpace KIPI
