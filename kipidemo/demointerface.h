#ifndef DEMOINTERFACE_H
#define DEMOINTERFACE_H

#include <libkipi/interface.h>
using namespace KIPI;

class DemoInterface :public KIPI::Interface {
    Q_OBJECT

public:
    DemoInterface( QWidget* parent, const char* name = 0 );

    virtual ImageCollection* currentAlbum();
    virtual ImageCollection* currentSelection();
    virtual ImageInfo info( const KURL& );
};


#endif /* DEMOINTERFACE_H */

