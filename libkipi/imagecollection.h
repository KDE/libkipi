#ifndef IMAGECOLLECTION_H
#define IMAGECOLLECTION_H
#include <kurl.h>
#include <qstring.h>

namespace KIPI
{
    class ImageCollection
    {
    public:
        virtual ~ImageCollection() {}
        virtual QString name() const = 0;
        virtual QString comment() const;
        virtual KURL::List images() const = 0;
        virtual KURL root() const ;
    };
}

#endif /* IMAGECOLLECTION_H */

