#include "imageinfo.h"
#include "KDStream.h"

QString KIPI::ImageInfo::toString( const QVariant& data )
{
    QString string;
    KDStream stream( &string );
    stream << data
           << flush ;
    return string;
}
