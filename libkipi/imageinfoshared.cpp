#include "imageinfoshared.h"
#include <kdebug.h>
#include <qfileinfo.h>
#include "interface.h"

KIPI::ImageInfoShared::ImageInfoShared( Interface* interface, const KURL& url )
    : _url( url ), _count(1), _interface( interface )
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

QDateTime KIPI::ImageInfoShared::time( KIPI::TimeSpec )
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

int KIPI::ImageInfoShared::angle()
{
    return 0;
}

void KIPI::ImageInfoShared::setAngle( int )
{
}

bool KIPI::ImageInfoShared::isTimeExact()
{
    return true;
}

void KIPI::ImageInfoShared::setTime( const QDateTime& /*time*/, TimeSpec /*spec*/ )
{
}

void KIPI::ImageInfoShared::setTitle( const QString& )
{
    kdWarning(51000) << "This method should only be invoked if the host application supports the KIPI::ImageTitlesWritable\n"
        "If the host application do support that, then this function should\n"
        "have been overriden in the host application.\n";
}

void KIPI::ImageInfoShared::cloneData( ImageInfoShared* other )
{
    if ( _interface->hasFeature( ImageTitlesWritable ) )
        setTitle( other->title() );

    if ( _interface->hasFeature( ImagesHasComments ) )
        setDescription( other->description() );

    clearAttributes();
    addAttributes( other->attributes() );

    setTime( other->time( FromInfo ), FromInfo );
    if ( _interface->hasFeature( SupportsDateRanges ) )
        setTime( other->time( ToInfo ), ToInfo );

    setAngle( other->angle() );
}
