//////////////////////////////////////////////////////////////////////////////
//
//    BATCHPROGRESSDIALOG.CPP
//
//    Copyright (C) 2004 Gilles Caulier <caulier dot gilles at free.fr>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU Library General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Library General Public License for more details.
//
//    You should have received a copy of the GNU Library General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin Steet, Fifth Floor, Boston, MA 02110-1301, USA.
//
//////////////////////////////////////////////////////////////////////////////


// Include files for Qt

#include <qvbox.h>
#include <qlayout.h>
#include <qdir.h>
#include <qwidget.h>
#include <qgroupbox.h>
#include <qwhatsthis.h>
#include <qcolor.h>
#include <qhgroupbox.h>
#include <qvgroupbox.h>
#include <qheader.h>
#include <qlistview.h>
#include <qframe.h>
#include <qlabel.h>
#include <qcolor.h>
#include <qpixmap.h>
#include <qpushbutton.h>

// Include files for KDE

#include <klocale.h>
#include <kprogress.h>
#include <kinstance.h>
#include <kconfig.h>
#include <kapplication.h>
#include <kdebug.h>
#include <kdialogbase.h>
#include <kiconloader.h>
#include <klistview.h>
#include <kstandarddirs.h>
#include <kapplication.h>
#include <kaboutdata.h>
#include <khelpmenu.h>
#include <kiconloader.h>
#include <kpopupmenu.h>

// Include files for libKipi.

#include <libkipi/version.h>

// Local includes

#include "batchprogressdialog.h"

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


struct BatchProgressDialog::Private {
};


/////////////////////////////////// CONSTRUCTOR ////////////////////////////////////////////

BatchProgressDialog::BatchProgressDialog( QWidget *parent, const QString &caption )
                   : KDialogBase( KDialogBase::Plain, caption, Help|Cancel,
                                  Cancel, parent, "KIPIBatchProgressDialog", true )
{
    d = new Private;
    QWidget* box = plainPage();
    QVBoxLayout *dvlay = new QVBoxLayout( box, 6 );

    //---------------------------------------------

    QFrame *headerFrame = new QFrame( box );
    headerFrame->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    QHBoxLayout* layout = new QHBoxLayout( headerFrame );
    layout->setMargin( 2 ); // to make sure the frame gets displayed
    layout->setSpacing( 0 );
    QLabel *pixmapLabelLeft = new QLabel( headerFrame, "pixmapLabelLeft" );
    pixmapLabelLeft->setScaledContents( false );
    layout->addWidget( pixmapLabelLeft );
    QLabel *labelTitle = new QLabel( caption, headerFrame, "labelTitle" );
    layout->addWidget( labelTitle );
    layout->setStretchFactor( labelTitle, 1 );
    dvlay->addWidget( headerFrame );

    QString dir;
    KGlobal::dirs()->addResourceType("kipi_banner_left", KGlobal::dirs()->kde_default("data") + "kipi/data");
    dir = KGlobal::dirs()->findResourceDir("kipi_banner_left", "banner_left.png");

    pixmapLabelLeft->setPaletteBackgroundColor( QColor(201, 208, 255) );
    pixmapLabelLeft->setPixmap( QPixmap( dir + "banner_left.png" ) );
    labelTitle->setPaletteBackgroundColor( QColor(201, 208, 255) );

    //---------------------------------------------

    // About data and help button.

    KAboutData* about = new KAboutData("kipiplugins",
                                       I18N_NOOP("Batch process dialog"),
                                       kipi_version,
                                       I18N_NOOP("A Kipi dialog for batch process operations"),
                                       KAboutData::License_GPL,
                                       "(c) 2004, Kipi development team",
                                       0,
                                       "http://extragear.kde.org/apps/kipi");

    QPushButton *helpButton = actionButton( Help );
    KHelpMenu* helpMenu = new KHelpMenu(this, about, false);
    helpMenu->menu()->removeItemAt(0);
    helpMenu->menu()->insertItem(i18n("Kipi Plugins Handbooks"), this, SLOT(slotHelp()), 0, -1, 0);
    helpButton->setPopup( helpMenu->menu() );

    //---------------------------------------------


    QGroupBox* groupBox1 = new QGroupBox( 2, Qt::Horizontal, box );

    m_actionsList = new KListView( groupBox1 );
    m_actionsList->addColumn(i18n( "Status" ));
    m_actionsList->addColumn(i18n( "Current Actions" ));
    m_actionsList->setSorting(-1);
    m_actionsList->setItemMargin(1);
    m_actionsList->header()->hide();
    m_actionsList->setResizeMode(QListView::LastColumn);
    QWhatsThis::add( m_actionsList, i18n("<p>This is the current tasks list released.") );
    dvlay->addWidget( groupBox1 );

    //---------------------------------------------

    m_progress = new KProgress( box, "Progress" );
    m_progress->setTotalSteps(100);
    m_progress->setValue(0);
    QWhatsThis::add( m_progress, i18n("<p>This is the list current percent task released.") );
    dvlay->addWidget( m_progress );
    resize( 600, 400 );
}


//////////////////////////////////// DESTRUCTOR /////////////////////////////////////////////

BatchProgressDialog::~BatchProgressDialog()
{
    delete d;
}


///////////////////////////////////// FONCTIONS /////////////////////////////////////////////

void BatchProgressDialog::addedAction(const QString &text, int type)
{
    m_item = new KIPI::BatchProgressItem(m_actionsList,
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

/////////////////////////////////////// SLOTS ///////////////////////////////////////////////


void BatchProgressDialog::slotHelp( void )
{
    KApplication::kApplication()->invokeHelp("",
                                             "kipi-plugins");
}

}  // NameSpace KIPI

#include "batchprogressdialog.moc"
