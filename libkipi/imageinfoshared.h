#ifndef KIPI_IMAGEINFOSHARED_H
#define KIPI_IMAGEINFOSHARED_H
#include <kurl.h>
#include <qstring.h>
#include <qmap.h>
#include <qdatetime.h>
#include <qvariant.h>
#include <libkipi/imageinfo.h>

namespace KIPI
{
    class ImageInfoShared
    {
    public:
        ImageInfoShared( const KURL& url );
        virtual ~ImageInfoShared() {}
        virtual QString name() = 0;
        virtual void setName( const QString& ) = 0;

        virtual QString description() = 0;
        virtual void setDescription( const QString& ) = 0;

        virtual QMap<QString,QVariant> attributes() = 0;
        virtual void clearAttributes() = 0;
        virtual void addAttributes( const QMap<QString,QVariant>& ) = 0;

        virtual KURL path();
        virtual QDateTime time( KIPI::TimeSpec spec );
        virtual void setTime( const QDateTime& time, TimeSpec spec = FromInfo );

        virtual bool isTimeExact();

        virtual int size();

        virtual int angle();
        virtual void setAngle( int );

        QString toString( const QVariant& );

    protected:
        KURL _url;

    private:
        ImageInfoShared() {} // Disable

        friend class ImageInfo;
        void addRef();
        void removeRef();
        int _count;
    };
}

#endif /* IMAGEINFOSHARED_H */
