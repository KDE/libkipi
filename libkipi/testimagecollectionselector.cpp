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
    TestInterface(QObject* parent) : Interface(parent) {
        KURL::List list;
        _collections << KIPI::ImageCollection(new TestImageCollection("First album", list));
        _collections << KIPI::ImageCollection(new TestImageCollection("Second album", list));
        _collections << KIPI::ImageCollection(new TestImageCollection("Third album", list));
    }
    ImageCollection currentAlbum() {
        return _collections.first();
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
};

int main(int argc, char* argv[]) {
	KCmdLineArgs::init(argc, argv, argv[0], "test app", "1.0");
    KApplication app;
    TestInterface interface(0);

    TestWindow testWindow(&interface);
    app.setMainWidget(&testWindow);
    testWindow.show();
    app.exec();
    return 0;
}
