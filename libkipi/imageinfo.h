#ifndef IMAGEINFO_H
#define IMAGEINFO_H
#include <kurl.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qmap.h>
#include <qvariant.h>


namespace KIPI
{
    class ImageInfoShared;

    class ImageInfo
    {
    public:
        // Interface for plugins.
        QString name();
        KURL path();
        QString descrion();
        QDateTime time();
        QMap<QString,QVariant> attributes();
        QString toString( const QVariant& );
        int size();

        // Interface for host application
        ImageInfo( ImageInfoShared* );

        // general stuff
        ImageInfo( const ImageInfo& );
        ~ImageInfo();

    private:
        ImageInfo() {} // Disabled
        ImageInfo& operator=( const ImageInfo& ) { return *this; } // Disabled
        KIPI::ImageInfoShared* _data;
    };
}

#endif /* IMAGEINFO_H */

