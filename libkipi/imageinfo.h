#ifndef IMAGEINFO_H
#define IMAGEINFO_H
#include <kurl.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qmap.h>
#include <qvariant.h>

namespace KIPI
{
    class ImageInfo
    {
    public:
        QString name;
        KURL path;
        QString descrion;
        QDateTime time;
        QMap<QString,QVariant> attributes;
        QString toString( const QVariant& );
    };
}

#endif /* IMAGEINFO_H */

