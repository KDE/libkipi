/** ===========================================================
 * @file
 *
 * This file is a part of kipi-plugins project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2009-11-21
 * @brief  kipi host test application
 *
 * @author Copyright (C) 2009-2010 by Michael G. Hansen
 *         <a href="mailto:mike at mghansen dot de">mike at mghansen dot de</a>
 * @author Copyright (C) 2011-2012 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
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

// KDE includes

#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kstandarddirs.h>

// LibKipi includes

#include <libkipi/version.h>

// local includes:

#include "kipitestmainwindow.h"

using namespace KXMLKipiCmd;

int main(int argc, char* argv[])
{
    const KAboutData aboutData("kxmlkipicmd",
                               "kipi",
                               ki18n("kxmlkipicmd"),
                               kipi_version,                   // libkipi version
                               ki18n("Kipi host test application using KDE XML-GUI"),
                               KAboutData::License_GPL,
                               ki18n("(c) 2009-2010 Michael G. Hansen\n"
                                     "(c) 2011-2012 Gilles Caulier\n"
                                     "(c) 2012 Victor Dodon "),
                               KLocalizedString(),             // optional text
                               "http://www.digikam.org",       // URI of homepage
                               "kde-imaging@kde.org"           // bugs e-mail address
                              );

    KCmdLineArgs::init(argc, argv, &aboutData);

    KApplication app;
    app.setWindowIcon(QIcon(KStandardDirs::locate("data", "kipi/data/kipi-icon.svg")));

    KGlobal::locale()->insertCatalog("libkipi");
    KGlobal::locale()->insertCatalog("libkdcraw");

    KipiTestMainWindow* mainWindow = new KipiTestMainWindow();
    QObject::connect(mainWindow, SIGNAL(destroyed(QObject*)),
                     &app, SLOT(quit()));

    app.setTopWidget(mainWindow);

    mainWindow->show();
    return app.exec();
}
