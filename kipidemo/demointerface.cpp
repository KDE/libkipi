#include "demointerface.h"
#include <qwidget.h>
#include <kdebug.h>
DemoInterface::DemoInterface( QWidget* parent, const char* name )
    :KIPI::Interface( parent, name )
{
}

ImageCollection* DemoInterface::currentAlbum()
{
    kdDebug() << "DemoInterface::currentAlbum\n";
    return 0;
}

ImageCollection* DemoInterface::currentSelection()
{
    kdDebug() << "DemoInterface::currentSelection\n";
    return 0;
}

ImageInfo DemoInterface::info( const KURL& )
{
    kdDebug() << "DemoInterface::info\n";
    return ImageInfo();
}

#include "demointerface.moc"
