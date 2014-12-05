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
 * @author Copyright (C) 2011-2014 by Gilles Caulier
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

// Qt includes

#include <QUrl>
#include <QDebug>
#include <QApplication>
#include <QStandardPaths>
#include <QCommandLineParser>
#include <QCommandLineOption>

// KDE includes

#include <kaboutdata.h>
#include <klocalizedstring.h>

// LibKipi includes

#include "libkipi_version.h"

// local includes:

#include "kipitestmainwindow.h"

using namespace KXMLKipiCmd;

int main(int argc, char* argv[])
{
    KAboutData aboutData("kxmlkipicmd",
                         ki18n("kxmlkipicmd").toString(),
                         QString(KIPI_VERSION_STRING),            // libkipi version
                         ki18n("Kipi host test application using KDE XML-GUI").toString(),
                         KAboutLicense::GPL,
                         ki18n("(c) 2009-2010 Michael G. Hansen\n"
                               "(c) 2011-2014 Gilles Caulier\n"
                               "(c) 2012 Victor Dodon ").toString(),
                         QString(),                               // optional text
                         QString("http://www.digikam.org"),       // URI of homepage
                         QString("kde-imaging@kde.org")           // bugs e-mail address
                        );


    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(QStandardPaths::locate(QStandardPaths::GenericDataLocation, "kipi/data/kipi-icon.svg")));

    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);

    parser.addOption(QCommandLineOption(QStringList() <<  QLatin1String("selectedalbums"), i18n("Selected albums"), QLatin1String("album")));
    parser.addOption(QCommandLineOption(QStringList() <<  QLatin1String("selectedimages"), i18n("Selected images"), QLatin1String("images")));
    parser.addOption(QCommandLineOption(QStringList() <<  QLatin1String("allalbums"),      i18n("All albums"),      QLatin1String("albums")));
    parser.addOption(QCommandLineOption(QStringList() <<  QLatin1String("+[images]"),      i18n("List of images")));
    parser.addOption(QCommandLineOption(QStringList() <<  QLatin1String("+[albums]"),      i18n("Selected albums")));

    parser.process(app);
    aboutData.processCommandLine(&parser);

    QList<QUrl> listSelectedImages;
    QList<QUrl> listSelectedAlbums;
    QList<QUrl> listAllAlbums;

    // Determine which with list we start

    QList<QUrl>* startList = 0;

    if (parser.isSet("selectedimages"))
    {
        startList = &listSelectedImages;
        startList->append(QUrl(parser.value("selectedimages")));
    }
    else if (parser.isSet("selectedalbums"))
    {
        startList = &listSelectedAlbums;
        startList->append(QUrl(parser.value("selectedalbums")));
    }
    else if (parser.isSet("allalbums"))
    {
        startList = &listAllAlbums;
        startList->append(QUrl(parser.value("allalbums")));
    }

    // Append the remaining arguments to the lists

    const QStringList args = parser.positionalArguments();

    for (int i = 0; i < args.count(); ++i)
    {
        const QString argValue = args.value(i);

        if (argValue == "--selectedimages")
        {
            startList = &listSelectedImages;
        }
        else if (argValue == "--selectedalbums")
        {
            startList = &listSelectedAlbums;
        }
        else if (argValue == "--allalbums")
        {
            startList = &listAllAlbums;
        }
        else
        {
            if (startList == 0)
            {
                qCritical() << "StartList is null. "
                            << "Please specify how the filenames you provided should be used.";
                return 0;
            }
            else
            {
                startList->append(QUrl(args.value(i)));
            }
        }
    }

    KipiTestMainWindow* const mainWindow = new KipiTestMainWindow(listSelectedImages, listSelectedAlbums, listAllAlbums);

    QObject::connect(mainWindow, &KipiTestMainWindow::destroyed, 
                     &app, &QApplication::quit);

    app.setActiveWindow(mainWindow);

    mainWindow->show();
    return app.exec();
}
