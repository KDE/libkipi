#ifndef KIPI_IMAGEINFO_H
#define KIPI_IMAGEINFO_H
#include <kurl.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qmap.h>
#include <qvariant.h>


namespace KIPI
{
    class ImageInfoShared;
    enum TimeSpec { FromInfo, ToInfo };

    class ImageInfo
    {
    public:
        // Interface for plugins.
        QString title() const;
        void setTitle( const QString& name );

        QString description() const;
        void setDescription( const QString& description);

        QMap<QString,QVariant> attributes() const;
        void clearAttributes();
        void addAttributes( const QMap<QString,QVariant>& );

        KURL path() const;
        QDateTime time( TimeSpec spec = FromInfo ) const;
        void setTime( const QDateTime& time, TimeSpec spec = FromInfo );
        bool isTimeExact() const;
        int size() const;

        int angle() const;
        void setAngle( int );

        QString toString( const QVariant& ) const;

        void cloneData( const ImageInfo& other );

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

#endif /* KIPI_IMAGEINFO_H */

