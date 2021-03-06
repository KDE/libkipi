/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 *
 *
 * @date   2012-06-20
 * @brief  kipi host test application using KDE XML-GUI technology
 *
 * @author Copyright (C) 2012 by Victor Dodon
 *         <a href="mailto:dodonvictor at gmail dot com">dodonvictor at gmail dot com</a>
 * @author Copyright (C) 2012-2018 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

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
