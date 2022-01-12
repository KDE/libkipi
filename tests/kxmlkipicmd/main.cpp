/*
    SPDX-FileCopyrightText: 2009-2010 Michael G. Hansen <mike at mghansen dot de>
    SPDX-FileCopyrightText: 2011-2018 Gilles Caulier <caulier dot gilles at gmail dot com>
    SPDX-FileCopyrightText: 2012 Victor Dodon <dodonvictor at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

// Qt includes

#include <QUrl>
#include <QDebug>
#include <QApplication>
#include <QStandardPaths>
#include <QCommandLineParser>
#include <QCommandLineOption>

// LibKipi includes

#include "libkipi_version.h"

#ifdef HAVE_KEXIV2
#   include <kexiv2/kexiv2.h>
#endif

// Local includes

#include "kipitestmainwindow.h"

using namespace KXMLKipiCmd;

int main(int argc, char* argv[])
{
#ifdef HAVE_KEXIV2
    KExiv2Iface::KExiv2::initializeExiv2();
#endif

    QApplication app(argc, argv);
    app.setApplicationName(QLatin1String("kxmlkipicmd"));
    app.setApplicationVersion(QLatin1String(KIPI_VERSION_STRING));
    app.setOrganizationName(QLatin1String("http://www.kde.org"));
    app.setWindowIcon(QIcon(QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                                   QLatin1String(":/images/kipi-icon.svg"))));

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.setApplicationDescription(QLatin1String("Demo Kipi host test application using KDE XML-GUI"));

    parser.addOption(QCommandLineOption(QStringList() <<  QLatin1String("c"),              QLatin1String("Selected collections"), QLatin1String("selectedcollectionss")));
    parser.addOption(QCommandLineOption(QStringList() <<  QLatin1String("i"),              QLatin1String("Selected images"),      QLatin1String("selectedimages")));
    parser.addOption(QCommandLineOption(QStringList() <<  QLatin1String("allc"),           QLatin1String("All collections"),      QLatin1String("allcollections")));
    parser.addOption(QCommandLineOption(QStringList() <<  QLatin1String("+[images]"),      QLatin1String("List of images")));
    parser.addOption(QCommandLineOption(QStringList() <<  QLatin1String("+[collections]"), QLatin1String("Selected collections")));
    parser.process(app);

    QList<QUrl> listSelectedImages;
    QList<QUrl> listSelectedAlbums;
    QList<QUrl> listAllAlbums;

    // Determine which with list we start

    QList<QUrl>* startList = nullptr;

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
            if (startList == nullptr)
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

    int ret = app.exec();

#ifdef HAVE_KEXIV2
    KExiv2Iface::KExiv2::cleanupExiv2();
#endif

    return ret;
}
