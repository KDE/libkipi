#include "imagecollection.h"

/**
   returns the common prefix for the images if there is a common prefix.
**/
KURL KIPI::ImageCollection::root() const
{
    return KURL();
}

/**
   returns the comment for the collection of images or QString::null if that doesn't make any sense.
   A comment makes sense for an album, but not for a KIPI::Interface::currentSelection().
*/
QString KIPI::ImageCollection::comment() const
{
    return QString::null;
}
