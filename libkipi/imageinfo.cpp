#include "imageinfo.h"
#include "KDStream.h"
#include "imageinfoshared.h"

QString KIPI::ImageInfo::toString( const QVariant& data ) const
{
    QString string;
    KDStream stream( &string );
    stream << data
           << flush ;
    return string;
}

/**
   PENDING(blackie) document
*/
QString KIPI::ImageInfo::name() const
{
    return _data->name();
}

/**
   PENDING(blackie) document
*/
KURL KIPI::ImageInfo::path() const
{
    return _data->path();
}

/**
   PENDING(blackie) document
*/
QString KIPI::ImageInfo::description() const
{
    return _data->description();
}

/**
   PENDING(blackie) document
*/
QDateTime KIPI::ImageInfo::time() const
{
    return _data->time();
}

/**
   PENDING(blackie) document
*/
QMap<QString,QVariant> KIPI::ImageInfo::attributes() const
{
    return _data->attributes();
}

/**
   PENDING(blackie) document
*/
int KIPI::ImageInfo::size() const
{
    return _data->size();
}

KIPI::ImageInfo::ImageInfo( ImageInfoShared* shared )
    : _data( shared )
{
}

KIPI::ImageInfo::ImageInfo( const KIPI::ImageInfo& rhs )
{
    _data = rhs._data;
    _data->addRef();
}

KIPI::ImageInfo::~ImageInfo()
{
    _data->removeRef();
}
