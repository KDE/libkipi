#include "imagecollection.h"
#include "imagecollectionshared.h"
#include <kdebug.h>

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
    if ( rhs._data ) {
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

KIPI::ImageCollection& KIPI::ImageCollection::operator=( const KIPI::ImageCollection& rhs )
{
    if ( rhs._data == _data )
        return *this;

    if ( _data )
        _data->removeRef();
    if ( !rhs._data ) {
        kdFatal() << "You are now using a null ImageCollection, you should have constructed one with"
            "the constructor taking a ImageCollectionShared* as argument!\n";
    }
    _data = rhs._data;
    _data->addRef();
    return *this;
}

/*!
  Returns the directory for the image collections.
  The host application may, however, return anything in case it does not
  support the KIPI::Features AlbumEQDir, e.g. the directory of the first
  image in the collection, the root of the image collection (in case all
  images has a common root), or even an empty URL.
*/
KURL KIPI::ImageCollection::path() const
{
    return _data->path();
}

/*!
  Returns the directory to place images into.
  This function should only be called if KIPI::Features AcceptNewImages
  is available.
  The function may choose to return the directory for the image collection
  or if images from the collection are not available in a common directory,
  then instead a common upload directory.
  In contrast to \ref path, this function must return a valid url.
*/
KURL KIPI::ImageCollection::uploadPath() const
{
    return _data->uploadPath();
}

