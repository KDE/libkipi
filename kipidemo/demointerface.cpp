#include "demointerface.h"
#include <qwidget.h>
#include <kdebug.h>
#include <kurl.h>
#include <qfileinfo.h>

class MyImageCollection :public KIPI::ImageCollection
{
public:
    MyImageCollection( const QString& name, KURL::List urls )
        :ImageCollection(), _name(name), _urls(urls)
        {
        }
    virtual QString name() const
        {
            return _name;
        }

    virtual KURL::List images() const
        {
            return _urls;
        }

    virtual bool valid() const
        {
            return true;
        }

    virtual KURL root() const
        {
            return KURL( "file:/tmp/kimdaba-demo-blackie" );
        }

    virtual QString comment() const
        {
            return "a demo comment";
        }

private:
    QString _name;
    KURL::List _urls;
};

DemoInterface::DemoInterface( QWidget* parent, const char* name )
    :KIPI::Interface( parent, name )
{
}

ImageCollection* DemoInterface::currentAlbum()
{
    KURL::List list;
    list << KURL( "file:/tmp/kimdaba-demo-blackie/img001.jpg" ) << KURL( "file:/tmp/kimdaba-demo-blackie/img002.jpg" )  << KURL( "file:/tmp/kimdaba-demo-blackie/img003.jpg" ) << KURL( "file:/tmp/kimdaba-demo-blackie/img004.jpg" );

    return new MyImageCollection( "Hello collection", list );
}

ImageCollection* DemoInterface::currentSelection()
{
    return new MyImageCollection( "Hello collection", KURL::List() );
}

ImageInfo DemoInterface::info( const KURL& url )
{
    return ImageInfo( new DemoImageInfo( url ) );
}

QValueList<ImageCollection*> DemoInterface::allAlbums()
{
    KURL::List list1;
    list1 << KURL( "file:/tmp/kimdaba-demo-blackie/img001.jpg" ) << KURL( "file:/tmp/kimdaba-demo-blackie/img002.jpg" )
         << KURL( "file:/tmp/kimdaba-demo-blackie/img003.jpg" ) << KURL( "file:/tmp/kimdaba-demo-blackie/img004.jpg" );

    KURL::List list2;
    list2 << KURL( "file:/tmp/kimdaba-demo-blackie/img005.jpg" ) << KURL( "file:/tmp/kimdaba-demo-blackie/img006.jpg" )
         << KURL( "file:/tmp/kimdaba-demo-blackie/img008.jpg" ) << KURL( "file:/tmp/kimdaba-demo-blackie/img008.jpg" );

    MyImageCollection* album1 = new MyImageCollection( "Album 1", list1 );
    MyImageCollection* album2 = new MyImageCollection( "Album 1", list2 );

    QValueList<ImageCollection*> albumlist;
    albumlist  << album1 << album2;
    return albumlist;

}

DemoImageInfo::DemoImageInfo( const KURL& url )
    :ImageInfoShared( url )
{
}

QString DemoImageInfo::name()
{
    return _url.fileName();
}

QString DemoImageInfo::descrion()
{
    return QString( "description for %1" ).arg( name() );
}

QMap<QString,QVariant> DemoImageInfo::attributes()
{
    return QMap<QString,QVariant>();
}

#include "demointerface.moc"
