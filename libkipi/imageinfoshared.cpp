#include "imageinfoshared.h"
#include <kdebug.h>
#include <qfileinfo.h>

KIPI::ImageInfoShared::ImageInfoShared( const KURL& url )
    :_url( url ), _count(1)
{
}

KURL KIPI::ImageInfoShared::path()
{
    return _url;
}

int KIPI::ImageInfoShared::size()
{
    if ( ! _url.isLocalFile() ) {
        kdFatal() << "KIPI::ImageInfoShared::size does not yet support non local files, please fix\n";
        return 0;
    }
    else
        return QFileInfo( _url.path() ).size();
}

QDateTime KIPI::ImageInfoShared::time()
{
    if ( ! _url.isLocalFile() ) {
        kdFatal() << "KIPI::ImageInfoShared::time does not yet support non local files, please fix\n";
        return QDateTime();
    }
    else
        return QFileInfo( _url.path() ).lastModified();
}

void KIPI::ImageInfoShared::addRef()
{
    _count++;
}

void KIPI::ImageInfoShared::removeRef()
{
    _count--;
    if ( _count == 0 ) {
        delete this;
    }
}

