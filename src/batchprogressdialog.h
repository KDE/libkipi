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

/** @file batchprogressdialog.h */

#ifndef BATCHPROGRESSDIALOG_H
#define BATCHPROGRESSDIALOG_H

// Include files for KDE

#include <kdialogbase.h>
#include "libkipi/libkipi_export.h"

class KListView;
class KProgress;

/** 
  @namespace KIPI 
  This is the namespace
*/
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

class LIBKIPI_EXPORT BatchProgressDialog : public KDialogBase
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

 private:
 
   struct Private;
   Private* d;
};

}  // NameSpace KIPI

#endif  // BATCHPROGRESSDIALOG_H
