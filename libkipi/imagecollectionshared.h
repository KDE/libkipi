#ifndef KIPI_IMAGECOLLECTIONSHARED_H
#define KIPI_IMAGECOLLECTIONSHARED_H
#include <qstring.h>
#include <kurl.h>
#include "imagecollection.h"

namespace KIPI
{
    class ImageCollectionShared
    {
    public:
        ImageCollectionShared();
        virtual ~ImageCollectionShared() {}
        virtual QString name() const = 0;
        virtual QString comment() const = 0;
        virtual KURL::List images() const = 0;
        virtual KURL root() const = 0;

    private:
        friend class ImageCollection;
        void addRef();
        void removeRef();
        int _count;
    };
}

#endif /* IMAGECOLLECTIONSHARED_H */

