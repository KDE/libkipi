#include "imagecollectionshared.h"

#include <kdebug.h>

KIPI::ImageCollectionShared::ImageCollectionShared()
     : _count(1)
{
}

void KIPI::ImageCollectionShared::addRef()
{
    _count++;
}

void KIPI::ImageCollectionShared::removeRef()
{
    _count--;
    if ( _count == 0 ) {
        // qDebug("Deleting!");
        delete this;
    }
}

KURL KIPI::ImageCollectionShared::path()
{
    kdWarning(51000) << "This method should only be invoked if the host application supports the KIPI::Features\n"
            "AlbumEQDir - if the host application do support that, then this function should\n"
            "have been overriden in the host application.\n";
    return KURL();
}

KURL KIPI::ImageCollectionShared::uploadPath()
{
    kdWarning(51000) << "This method should only be invoked if the host application supports the KIPI::Features\n"
            "AcceptNewImages - if the host application do support that, then this function should\n"
            "have been overriden in the host application.\n";
    return KURL();
}

KURL KIPI::ImageCollectionShared::uploadRoot()
{
    KURL path = uploadPath();
    if ( path.isValid() ) {
        path.setPath("/");
        return path;
    }
    else
        return KURL( "file:/" );
}

QString KIPI::ImageCollectionShared::comment()
{
    kdWarning(51000) << "This method should only be invoked if the host application supports the KIPI::Features\n"
            "AlbumsHaveComments - if the host application do support that, then this function should\n"
            "have been overriden in the host application.\n";
    return QString::null;
}
