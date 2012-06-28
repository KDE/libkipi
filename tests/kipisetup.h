/** ===========================================================
 * @file
 *
 * This file is a part of kipi-plugins project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2012-06-28
 * @brief  kipi host test application using KDE XML-GUI technology
 *
 * @author Copyright (C) 2012 by Victor Dodon
 *         <a href="mailto:dodonvictor at gmail dot com">dodonvictor at gmail dot com</a>
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

// KDE includes

#include <kpagedialog.h>

namespace KXMLKipiCmd
{

class KipiSetup : public KPageDialog
{
    Q_OBJECT

public:

    enum Page
    {
        LastPageUsed    = -1,
        KipiPluginsPage =  0,
        XmlFilesPage
    };

    static bool exec(QWidget* parent = 0);

private Q_SLOTS:

    void slotButtonClicked(int button);

private:

    KipiSetup(QWidget* parent);
    ~KipiSetup();

    void okClicked();

    class KipiSetupPriv;
    KipiSetupPriv* const d;
};

// -------------------------------------------------------------------

class SetupXML : public QScrollArea
{
    Q_OBJECT

public:

    SetupXML(QWidget* const parent);
    ~SetupXML();

public Q_SLOTS:

    void apply();

private:

    class SetupXMLPriv;
    SetupXMLPriv* const d;

};

} // namespace KXMLKipiCmd

#endif // KIPISETUP_H
