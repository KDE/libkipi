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

void KIPI::ImageInfo::setName( const QString& name )
{
    _data->setName( name );
}

void KIPI::ImageInfo::setDescription( const QString& description )
{
    _data->setDescription( description );
}

void KIPI::ImageInfo::clearAttributes()
{
    _data->clearAttributes();
}

void KIPI::ImageInfo::addAttributes( const QMap<QString,QVariant>& attributes )
{
    _data->addAttributes( attributes );
}

/**
   Returns the angle the application rotates the image with when displaying it.
   Certain host applications may choose to rotate the image on disk, and will always return 0,
   while other host application will rotate the image when displaying it, and will thus not rotate
   the image on disk.
*/
int KIPI::ImageInfo::angle()
{
    return _data->angle();
}

/**
   See \ref angle
*/
void KIPI::ImageInfo::setAngle( int angle )
{
    _data->setAngle( angle );
}

