#ifndef KIPI_UPLOADWIDGET_H
#define KIPI_UPLOADWIDGET_H

#include <qwidget.h>
#include <kfiletreeview.h>
#include <libkipi/interface.h>

namespace KIPI
{
    class UploadWidget :public QWidget {
        Q_OBJECT

    public:
        UploadWidget( KIPI::Interface* interface, QWidget* parent, const char* name = 0 );
        KURL path() const;

    public slots:
        void mkdir();

    protected slots:
        void load();
        void slotAlbumCreated(KIO::Job* job);

    private:
        KFileTreeView* m_treeView;
        KFileTreeBranch* m_item;
        QStringList m_pendingPath;
        QString m_handled;
        KURL m_root;
    };

}

#endif /* KIPI_UPLOADWIDGET_H */

