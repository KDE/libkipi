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
        QString name() const;
        KURL path() const;
        QString description() const;
        QDateTime time() const;
        QMap<QString,QVariant> attributes() const;
        QString toString( const QVariant& ) const;
        int size() const;

        // Interface for host application + general stuff
        ImageInfo( ImageInfoShared* );
        ImageInfo( const ImageInfo& );
        ~ImageInfo();

    private:
        ImageInfo() {} // Disabled
        ImageInfo& operator=( const ImageInfo& ) { return *this; } // Disabled
        mutable KIPI::ImageInfoShared* _data;
    };
}

#endif /* IMAGEINFO_H */

