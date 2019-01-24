/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 *
 *
 * @date   2012-06-28
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

#ifndef KIPISETUP_H
#define KIPISETUP_H

// Qt includes

#include <QScrollArea>
#include <QString>
#include <QDialog>

namespace KXMLKipiCmd
{

class KipiSetup : public QDialog
{
    Q_OBJECT

public:

    enum Page
    {
        LastPageUsed    = -1,
        KipiPluginsPage =  0,
        XmlFilesPage
    };

public:

    static bool runSetupDialog(QWidget* const parent = nullptr);

private Q_SLOTS:

    void slotOkClicked();
    void slotFilterChanged(const QString& filter);

private:

    KipiSetup(QWidget* const parent);
    ~KipiSetup() override;

private:

    friend class SetupXML;

    class Private;
    Private* const d;
};

// -------------------------------------------------------------------

class SetupXML : public QScrollArea
{
    Q_OBJECT

public:

    SetupXML(QWidget* const parent);
    ~SetupXML() override;

    static const QString CONFIG_GROUP_NAME;

public Q_SLOTS:

    void apply();

private:

    class Private;
    Private* const d;
};

} // namespace KXMLKipiCmd

#endif // KIPISETUP_H
