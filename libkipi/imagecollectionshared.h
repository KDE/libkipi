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
        virtual QString name() = 0;
        virtual QString comment() = 0;
        virtual KURL::List images() = 0;

    private:
        friend class ImageCollection;
        void addRef();
        void removeRef();
        int _count;
    };
}

#endif /* IMAGECOLLECTIONSHARED_H */

