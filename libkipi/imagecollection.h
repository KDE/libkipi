#ifndef KIPI_IMAGECOLLECTION_H
#define KIPI_IMAGECOLLECTION_H
#include <kurl.h>
#include <qstring.h>

namespace KIPI
{
    class ImageCollectionShared;

    class ImageCollection
    {
    public:
        // Interface for plugins.
        QString name() const;
        QString comment() const;
        KURL::List images() const;
        KURL path() const;
        KURL uploadPath() const;
        KURL uploadRoot() const;
        bool isValid() const;

        // Interface for host application + general stuff
        ImageCollection( ImageCollectionShared* );
        ImageCollection( const ImageCollection& other );
        ~ImageCollection();
        ImageCollection();
        ImageCollection& operator=( const ImageCollection& );

    private:
        mutable KIPI::ImageCollectionShared* _data;
        void printNullError() const;
    };
}

#endif /* IMAGECOLLECTION_H */

