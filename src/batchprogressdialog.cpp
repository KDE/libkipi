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

#include <QDir>
#include <QWidget>
#include <QGroupBox>
#include <QColor>
#include <QListView>
#include <QFrame>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QProgressBar>

// Include files for KDE

#include <klocale.h>
#include <kconfig.h>
#include <kapplication.h>
#include <kdebug.h>
#include <kdialog.h>
#include <kiconloader.h>
#include <k3listview.h>
#include <kstandarddirs.h>
#include <kapplication.h>
#include <kaboutdata.h>
#include <khelpmenu.h>
#include <kiconloader.h>
#include <kmenu.h>

// Include files for libKipi.

#include "version.h"

// Local includes

#include "batchprogressdialog.h"
#include "batchprogressdialog.moc"

namespace KIPI
{

class BatchProgressItem : public KListViewItem
{
public:

BatchProgressItem(KListView * parent, QListViewItem *after, const QString &message, int messageType)
                  : KListViewItem( parent, after), m_messagetype(messageType)
{
   // Set the icon.

   switch( m_messagetype )
   {
     case KIPI::StartingMessage:
        setPixmap( 0, SmallIcon( "run" ) );
        break;
     case KIPI::SuccessMessage:
        setPixmap( 0, SmallIcon( "ok" ) );
        break;
     case KIPI::WarningMessage:
        setPixmap( 0, SmallIcon( "flag" ) );
        break;
     case KIPI::ErrorMessage:
        setPixmap( 0, SmallIcon( "stop" ) );
        break;
     case KIPI::ProgressMessage:
        setPixmap( 0, SmallIcon( "info" ) );
        break;
     default:
        setPixmap( 0, SmallIcon( "info" ) );
   }

   // Set the message text.

   setText(1, message);
}

private:

   int m_messagetype;

   void paintCell (QPainter *p, const QColorGroup &cg, int column, int width, int alignment)
   {
      QColorGroup _cg( cg );

      if ( m_messagetype == KIPI::ErrorMessage )
          {
          _cg.setColor( QColorGroup::Text, Qt::red );
          KListViewItem::paintCell( p, _cg, column, width, alignment );
          return;
          }

      if ( m_messagetype == KIPI::WarningMessage )
          {
          _cg.setColor( QColorGroup::Text, Qt::darkYellow );
          KListViewItem::paintCell( p, _cg, column, width, alignment );
          return;
          }

      KListViewItem::paintCell( p, cg, column, width, alignment );
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

    KVBox *box = new KVBox( this );
    setMainWidget( box );

    //---------------------------------------------

    QFrame *headerFrame = new QFrame( box );
    headerFrame->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    QHBoxLayout* layout = new QHBoxLayout( headerFrame );
    layout->setMargin( 2 ); // to make sure the frame gets displayed
    layout->setSpacing( 0 );
    QLabel *pixmapLabelLeft = new QLabel( headerFrame );
    pixmapLabelLeft->setScaledContents( false );
    layout->addWidget( pixmapLabelLeft );
    QLabel *labelTitle = new QLabel( caption, headerFrame );
    layout->addWidget( labelTitle );
    layout->setStretchFactor( labelTitle, 1 );

    QString dir;
    KGlobal::dirs()->addResourceType("kipi_banner_left", KGlobal::dirs()->kde_default("data") + "kipi/data");
    dir = KGlobal::dirs()->findResourceDir("kipi_banner_left", "banner_left.png");

    pixmapLabelLeft->setPaletteBackgroundColor( QColor(201, 208, 255) );
    pixmapLabelLeft->setPixmap( QPixmap( dir + "banner_left.png" ) );
    labelTitle->setPaletteBackgroundColor( QColor(201, 208, 255) );

    //---------------------------------------------


    m_actionsList = new KListView( box );
    m_actionsList->addColumn(i18n( "Status" ));
    m_actionsList->addColumn(i18n( "Current Actions" ));
    m_actionsList->setSorting(-1);
    m_actionsList->setItemMargin(1);
    m_actionsList->header()->hide();
    m_actionsList->setResizeMode(QListView::LastColumn);
    m_actionsList->setWhatsThis( i18n("<p>This is the current tasks list released.") );

    //---------------------------------------------

    m_progress = new KProgress( box, "Progress" );
    m_progress->setTotalSteps(100);
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
    m_item = new BatchProgressItem(m_actionsList,
                                   m_actionsList->lastItem(),
                                   text, type);

    m_actionsList->ensureItemVisible(m_item);
}


void BatchProgressDialog::reset()
{
    m_actionsList->clear();
    m_progress->setValue(0);
}


void BatchProgressDialog::setProgress(int current, int total)
{
    m_progress->setTotalSteps(total);
    m_progress->setValue(current);
}

}  // NameSpace KIPI
