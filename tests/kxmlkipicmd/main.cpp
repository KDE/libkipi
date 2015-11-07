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
 * @author Copyright (C) 2011-2015 by Gilles Caulier
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

// LibKipi includes

#include "libkipi_version.h"

// local includes:

#include "kipitestmainwindow.h"

using namespace KXMLKipiCmd;

int main(int argc, char* argv[])
{
    KAboutData aboutData(QLatin1String("kxmlkipicmd"),
                         QLatin1String("kxmlkipicmd"),
                         QLatin1String(KIPI_VERSION_STRING),                            // libkipi version
                         QLatin1String("Kipi host test application using KDE XML-GUI"),
                         KAboutLicense::GPL,
                         QLatin1String("(c) 2009-2010 Michael G. Hansen\n"
                                       "(c) 2011-2014 Gilles Caulier\n"
                                       "(c) 2012 Victor Dodon "),
                         QString(),                                                     // optional text
                         QLatin1String("http://www.digikam.org"),                       // URI of homepage
                         QLatin1String("kde-imaging@kde.org")                           // bugs e-mail address
                        );

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                                   QStringLiteral("kf5/kipi/pics/kipi-icon.svg"))));

    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);

    parser.addOption(QCommandLineOption(QStringList() <<  QLatin1String("c"),              QLatin1String("Selected collections"), QLatin1String("selectedcollectionss")));
    parser.addOption(QCommandLineOption(QStringList() <<  QLatin1String("i"),              QLatin1String("Selected images"),      QLatin1String("selectedimages")));
    parser.addOption(QCommandLineOption(QStringList() <<  QLatin1String("allc"),           QLatin1String("All collections"),      QLatin1String("allcollections")));
    parser.addOption(QCommandLineOption(QStringList() <<  QLatin1String("+[images]"),      QLatin1String("List of images")));
    parser.addOption(QCommandLineOption(QStringList() <<  QLatin1String("+[collections]"), QLatin1String("Selected collections")));

    parser.process(app);
    aboutData.processCommandLine(&parser);

    QList<QUrl> listSelectedImages;
    QList<QUrl> listSelectedAlbums;
    QList<QUrl> listAllAlbums;

    // Determine which with list we start

    QList<QUrl>* startList = 0;

    if (parser.isSet(QString::fromLatin1("i")))
    {
        startList = &listSelectedImages;
        startList->append(QUrl(parser.value(QString::fromLatin1("i"))));
    }
    else if (parser.isSet(QString::fromLatin1("c")))
    {
        startList = &listSelectedAlbums;
        startList->append(QUrl(parser.value(QString::fromLatin1("c"))));
    }
    else if (parser.isSet(QString::fromLatin1("allc")))
    {
        startList = &listAllAlbums;
        startList->append(QUrl(parser.value(QString::fromLatin1("allc"))));
    }

    // Append the remaining arguments to the lists

    const QStringList args = parser.positionalArguments();

    for (int i = 0; i < args.count(); ++i)
    {
        const QString argValue = args.value(i);

        if (argValue == QString::fromLatin1("-i"))
        {
            startList = &listSelectedImages;
        }
        else if (argValue == QString::fromLatin1("-c"))
        {
            startList = &listSelectedAlbums;
        }
        else if (argValue == QString::fromLatin1("-allc"))
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
