#include "imagecollection.h"
#include "imagecollectionshared.h"
#include <kdebug.h>

/**
   returns the common prefix for the images if there is a common prefix.
**/
KURL KIPI::ImageCollection::root() const
{
    if ( _data )
        return _data->root();
    else {
        kdFatal() << "You are now using a null ImageCollection, you should have constructed one with"
            "the constructor taking a ImageCollectionShared* as argument!\n";
    }
    return KURL();
}

/**
   returns the comment for the collection of images or QString::null if that doesn't make any sense.
   A comment makes sense for an album, but not for a KIPI::Interface::currentSelection().
*/
QString KIPI::ImageCollection::comment() const
{
    if ( _data )
        return _data->comment();
    else {
        kdFatal() << "You are now using a null ImageCollection, you should have constructed one with"
            "the constructor taking a ImageCollectionShared* as argument!\n";
        return QString::null;
    }
}

/**
   PENDING(blackie) document
*/
QString KIPI::ImageCollection::name() const
{
    if ( _data )
        return _data->name();
    else {
        kdFatal() << "You are now using a null ImageCollection, you should have constructed one with"
            "the constructor taking a ImageCollectionShared* as argument!\n";
        return QString::null;
    }
}

/**
   PENDING(blackie) document
*/
KURL::List KIPI::ImageCollection::images() const
{
    if ( _data )
        return _data->images();
    else {
        kdFatal() << "You are now using a null ImageCollection, you should have constructed one with"
            "the constructor taking a ImageCollectionShared* as argument!\n";
        return KURL::List();
    }
}

KIPI::ImageCollection::ImageCollection( ImageCollectionShared* data )
    : _data( data )
{
}

KIPI::ImageCollection::~ImageCollection()
{
    if ( _data )
        _data->removeRef();
}

KIPI::ImageCollection::ImageCollection( const ImageCollection& rhs )
{
    if ( _data ) {
        _data = rhs._data;
        _data->addRef();
    }
    else {
        kdFatal() << "You are now using a null ImageCollection, you should have constructed one with"
            "the constructor taking a ImageCollectionShared* as argument!\n";
    }
}

KIPI::ImageCollection::ImageCollection()
{
    _data = 0;
}

KIPI::ImageCollection& KIPI::ImageCollection::operator=( const KIPI::ImageCollection::ImageCollection& rhs )
{
    if ( rhs._data == _data )
        return *this;

    if ( _data )
        _data->removeRef();
    _data = rhs._data;
    _data->addRef();
    return *this;
}
