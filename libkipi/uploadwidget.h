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

    private:
        KFileTreeView* m_treeView;
    };

}

#endif /* KIPI_UPLOADWIDGET_H */

