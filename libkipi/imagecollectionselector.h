/* ============================================================
 * File   : imagecollectionselector.h
 * Authors: KIPI team developers (see AUTHORS files for details)
 *	    
 * Date   : 2004-07
 * Description :
 *
 * Copyright 2004 by the KIPI team
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Library General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * ============================================================ */

#ifndef IMAGECOLLECTIONSELECTOR_H
#define IMAGECOLLECTIONSELECTOR_H   

// Qt includes.

#include <qwidget.h>
#include <qvaluelist.h>
#include <qpixmap.h>

// KIPI includes.

#include "libkipi/imagecollection.h"

class QListViewItem;
class KFileItem;

namespace KIPI 
{

class Interface;
    
class ImageCollectionSelector : public QWidget 
{
Q_OBJECT
    class Private;

public:
    
    ImageCollectionSelector(QWidget* parent, Interface*, const char* name=0);
    ~ImageCollectionSelector();
    
    QValueList<ImageCollection> selectedImageCollections() const;
    
signals:    

    void selectionChanged(void);
    
private:
    
    Private* d;
    void fillList();
    void emitSelectionChanged();
    friend class ImageCollectionItem;

private slots:
    
    void slotSelectAll();
    void slotInvertSelection();
    void slotSelectNone();
    void slotSelectionChanged(QListViewItem* listItem);
    void slotGotPreview(const KFileItem*, const QPixmap&);
    void slotInitialShow();
};

} // KIPI


#endif /* IMAGECOLLECTIONSELECTOR_H */
