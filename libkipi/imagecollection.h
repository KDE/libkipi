#ifndef IMAGECOLLECTION_H
#define IMAGECOLLECTION_H
#include <kurl.h>
#include <qstring.h>

namespace KIPI
{
    class ImageCollection
    {
    public:
        virtual QString name() const = 0;
        virtual QValueList<KURL> images() const = 0;
        virtual bool valid() const = 0;
    };
}

#endif /* IMAGECOLLECTION_H */

