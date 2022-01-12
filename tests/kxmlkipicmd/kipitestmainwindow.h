/*
    SPDX-FileCopyrightText: 2012 Victor Dodon <dodonvictor at gmail dot com>
    SPDX-FileCopyrightText: 2012-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KIPITESTMAINWINDOW_H
#define KIPITESTMAINWINDOW_H

// Qt includes

#include <QUrl>

// KF includes

#include <KXmlGuiWindow>

namespace KXMLKipiCmd
{

class KipiTestMainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:

    KipiTestMainWindow(const QList<QUrl>& selectedImages,
                       const QList<QUrl>& selectedAlbums,
                       const QList<QUrl>& allAlbums);
    ~KipiTestMainWindow() override;

    static KipiTestMainWindow* instance();

private:

    void setupActions();
    void loadPlugins();

private Q_SLOTS:

    void slotShowMenuBar();
    void slotEditKeys();
    void slotConfToolbars();
    void slotNewToolbarConfig();
    void slotSetup();

private:

    class Private;
    Private* const d;

    static KipiTestMainWindow* m_instance;
};

} // namespace KXMLKipiCmd

#endif // KIPITESTMAINWINDOW_H
