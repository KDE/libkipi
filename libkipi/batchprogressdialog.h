//////////////////////////////////////////////////////////////////////////////
//
//    BATCHPROGRESSDIALOG.H
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
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//////////////////////////////////////////////////////////////////////////////


#ifndef BATCHPROGRESSDIALOG_H
#define BATCHPROGRESSDIALOG_H

// Include files for KDE

#include <kdialogbase.h>

class QGroupBox;

class KListView;
class KProgress;

namespace KIPI
{

class BatchProgressItem;

enum ActionMessageType
{
    StartingMessage = 0,
    SuccessMessage,
    WarningMessage,
    ErrorMessage,
    ProgressMessage
};

class BatchProgressDialog : public KDialogBase
{
Q_OBJECT

 public:

   BatchProgressDialog( QWidget *parent=0, const QString &caption=QString::null );
   ~BatchProgressDialog();

   void addedAction(const QString &text, int type);
   void reset();
   void setProgress(int current, int total);
   
 protected:
   KIPI::BatchProgressItem *m_item;
   KListView               *m_actionsList;
   KProgress               *m_progress;
   QGroupBox               *groupBox1;
};

}  // NameSpace KIPI

#endif  // BATCHPROGRESSDIALOG_H
