/* ============================================================
 * File   : testimagecollectionselector.cpp
 * Authors: KIPI team developers (see AUTHORS files for details)
 *	    
 * Date   : 2004-07
 * Description :
 *
 * Copyright 2004 by the KIPI team
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published bythe Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */
// Qt
#include <qpushbutton.h>
#include <qvbox.h>

// KDE
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kdebug.h>

// KIPI
#include "libkipi/imagecollectionselector.h"
#include "libkipi/imagecollectionshared.h"
#include "libkipi/imageinfoshared.h"
#include "libkipi/interface.h"

// Local
#include "testimagecollectionselector.moc"

using namespace KIPI;

class TestImageCollection : public ImageCollectionShared {
public:
	TestImageCollection(const QString& name, KURL::List images)
	: ImageCollectionShared(), mName(name), mImages(images) {}

	QString name() { return mName; }
	QString comment() { return QString::null; }
	
	KURL::List images() { return mImages; }

private:
	QString mName;
	KURL::List mImages;
};



class TestImageInfo : public ImageInfoShared {
public:
	TestImageInfo(Interface* interface, const KURL& url) : ImageInfoShared(interface, url) {}
    
	QString title() {
		return _url.fileName();
	}
	
	QString description() {
		return QString::null;
	}

	void setDescription(const QString&) {}
	
	QMap<QString,QVariant> attributes() {
		return QMap<QString,QVariant>();
	}

	void clearAttributes() {}

	void addAttributes(const QMap<QString, QVariant>&) {}
};


class TestInterface : public Interface {
public:
    TestInterface(bool currentNotInAll) : Interface(0) {
        KURL::List list1;
        KURL::List list2;
        KURL::List list3;
        KURL::List list4;
        list1 << KURL("file://url1");
        list2 << KURL("file://url2");
        list3 << KURL("file://url3");
        list4 << KURL("file://url4");
        _collections << KIPI::ImageCollection(new TestImageCollection("First album", list1));
        _collections << KIPI::ImageCollection(new TestImageCollection("Second album", list2));
        _collections << KIPI::ImageCollection(new TestImageCollection("Third album", list3));

        if (currentNotInAll) {
            _currentAlbum=KIPI::ImageCollection(new TestImageCollection("Current album", list4));
        } else {
            _currentAlbum=_collections.first();
        }
    }
    ImageCollection currentAlbum() {
        return _currentAlbum;
    }
    ImageCollection currentSelection() {
        return _collections.last();
    }
    QValueList<ImageCollection> allAlbums() {
        return _collections;
    }
    
    ImageInfo info(const KURL& url) { return ImageInfo(new TestImageInfo(this, url)); }
    
    int features() const { return 0; }
    
    QValueList<ImageCollection> _collections;
    ImageCollection _currentAlbum;
};

static KCmdLineOptions sOptions[] = {
    { "current-not-in-all", "Simulate an interface were the current album is not in allAlbums()", 0 },
    { 0, 0, 0 }
};

int main(int argc, char* argv[]) {
	KCmdLineArgs::init(argc, argv, argv[0], "test app", "1.0");
    KCmdLineArgs::addCmdLineOptions(sOptions);
    
    KApplication app;

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    TestInterface interface(args->isSet("current-not-in-all"));
    args->clear();

    TestWindow testWindow(&interface);
    app.setMainWidget(&testWindow);
    testWindow.show();
    app.exec();
    return 0;
}
