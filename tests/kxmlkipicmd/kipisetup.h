/*
    SPDX-FileCopyrightText: 2012 Victor Dodon <dodonvictor at gmail dot com>
    SPDX-FileCopyrightText: 2012-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
