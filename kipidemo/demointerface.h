#ifndef DEMOINTERFACE_H
#define DEMOINTERFACE_H

#include <libkipi/interface.h>
#include <libkipi/imageinfoshared.h>
using namespace KIPI;

class DemoInterface :public KIPI::Interface {
    Q_OBJECT

public:
    DemoInterface( QWidget* parent, const char* name = 0 );

    virtual ImageCollection currentAlbum();
    virtual ImageCollection currentSelection();
    virtual ImageInfo info( const KURL& );
    virtual  QValueList<ImageCollection> allAlbums();
};

class DemoImageInfo :public ImageInfoShared
{
public:
    DemoImageInfo( const KURL& url );
    virtual QString name();
    virtual QString description();
    virtual QMap<QString,QVariant> attributes();
};

#endif /* DEMOINTERFACE_H */

