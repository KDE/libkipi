/* ============================================================
 * File   : testimagecollectionselector.h 
 * Authors: KIPI team developers (see AUTHORS files for details)
 *	    
 * Date   : 2004-07
 * Description :
 *
 * Copyright 2004 by the KIPI team
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Library General
 * Public License as published bythe Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * ============================================================ */
#ifndef TESTIMAGECOLLECTIONSELECTORWINDOW_H
#define TESTIMAGECOLLECTIONSELECTORWINDOW_H

class TestWindow : public QVBox {
Q_OBJECT
public:
    TestWindow(KIPI::Interface* interface)
    : QVBox(0) {
        _selector=new KIPI::ImageCollectionSelector(this, interface);
        QPushButton* button=new QPushButton("Selected image collections", this);
        connect(button, SIGNAL(clicked()), this, SLOT(slotDumpImageCollections()) );
    }

public slots:
    void slotDumpImageCollections() {
        kdDebug() << "Selected image collections:" << endl;
        QValueList<KIPI::ImageCollection> list=_selector->selectedImageCollections();
        QValueList<KIPI::ImageCollection>::ConstIterator it=list.begin();
        for( ; it!=list.end(); ++it) {
            kdDebug() << "- " << (*it).name() << endl;
        }
    }

private:
    KIPI::ImageCollectionSelector* _selector;
};

#endif // TESTIMAGECOLLECTIONSELECTORWINDOW_H
