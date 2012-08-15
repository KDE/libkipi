/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2012-02-16
 * Description : an Hello World plugin using KDE XML-GUI technology.
 *
 * Copyright (C) 2012 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2012 by Victor Dodon   <dodonvictor at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

/** Take a care about includes order, to prevent compilation problem.
 *  1/ moc file.
 *  2/ C ansi if really necessary.
 *  3/ C++ (always prefered than C ansi.
 *  4/ Extra libraries such as openCV for ex.
 *  4/ Qt.
 *  5/ KDE.
 *  6/ Local files.
 *
 *  Also, use C++ classes include header styles with Qt4,
 *  but do not use it with KDE4 header (use C ANSI style instead).
 */

/// No need to include plugin_helloworld.h, it will be done through Qt moc file.
#include "plugin_kxmlhelloworld.moc"

// Qt includes

#include <QPointer>
#include <QDomDocument>

// KDE includes

#include <kaction.h>
#include <kactioncollection.h>
#include <kdebug.h>
#include <kgenericfactory.h>
#include <klibloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <kcomponentdata.h>
#include <kdialog.h>

/// This is all libkipi header includes in this tool.

#include "imagecollection.h"
#include "imagecollectionselector.h"
#include "interface.h"

/** This is all Kipi-plugins common includes used in this tool.
 *  Look into kipi-plugins/common/libkipiplugins/ for details.
 */

/// You must wrap all your plugin code to a dedicated namespace
namespace KIPIKXMLHelloWorldPlugin
{

/** We will use KPToolDialog class from kipi-plugins to display plugin dialogs. It offers some facilities to
    set data and rules about plugins, especially to wrap properlly tool with KDE bugilla. We use KPAboutData container
    for that.
*/

/** Using private container everywhere is clear to speed up compilation and reduce source code depencies through header files.
 *  See this url for details : http://techbase.kde.org/Policies/Binary_Compatibility_Issues_With_C%2B%2B#Using_a_d-Pointer
 */
class Plugin_KXMLHelloWorld::Private
{
public:

    Private()
    {
        /// It's always clean to init pointers to zero. If crash appear,
        /// debugger will show a null pointer instead a non initialized one.
        actionImages = 0;
        actionTools  = 0;
        actionExport = 0;
        actionImport = 0;
    }

    /** These plugin actions will pluged into menu KIPI host application.
     */
    KAction*   actionImages;
    KAction*   actionTools;
    KAction*   actionExport;
    KAction*   actionImport;
};

/** Macro from KDE KParts to create the factory for this plugin.
 *  The first argument is the name of the plugin library
 *  and the second is the generic factory templated from
 *  the class for your plugin.
 */
K_PLUGIN_FACTORY(KXMLHelloWorldFactory, registerPlugin<Plugin_KXMLHelloWorld>();)

/** Macro from KDE KParts to export the symbols for this plugin
 *  NOTE: The plugin library is the name used in CMakeList.txt to link bin file,
 *  and with X-KDE-Library value from .desktop file.
 */
K_EXPORT_PLUGIN(KXMLHelloWorldFactory("kipiplugin_kxmlhelloworld") )

/** The plugin constructor. Note that plugin name passed as string in 3rd arguement of KIPI::Plugin parent class
 *  is the same than Name value from .desktop file.
 */
Plugin_KXMLHelloWorld::Plugin_KXMLHelloWorld(QObject* const parent, const QVariantList&)
    : Plugin(KXMLHelloWorldFactory::componentData(), parent, "KXMLHelloWorld"),
      /// Private container is allocated here.
      d(new Private)
{
    kDebug() << "Plugin_KXMLHelloWorld plugin loaded";

    /** This is needed to setup the plugin gui and to merge with the kipi host
     *  application gui.
     *  The name of the UI file must be: nameofpluginui.rc, where "nameofplugin"
     *  is the name given to the plugin factory, usualy: kipiplugin_<name> .
     *  UI file of the plugin must be installed in kipi data dir.
     */
    setUiBaseName("kipiplugin_kxmlhelloworldui.rc");

    /** We need to call setupXML so the XML file and the GUI of the plugin to
      * be merged with those of the KIPI host app
      */
    setupXML();
}

Plugin_KXMLHelloWorld::~Plugin_KXMLHelloWorld()
{
    /// Don't forget to clear d private container allocation in destructor to prevent memory leak.
    delete d;
}

void Plugin_KXMLHelloWorld::setup(QWidget* const widget)
{
    /** Each plugin must overload Plugin::setup method.
     *  We pass the widget which host plugin in KIPI host application
     */
    Plugin::setup(widget);

    /** This is the interface instance to plugin host application. Note that you can get it everywhere in your plugin using
     *  instance of KIPI::PluginLoader singleton which provide a method for that.
     *  Since libkipi 2.0.0, KIPI host interface is also available from KIPI::Plugin::interface().
     */
    if (!interface())
        return;

    /** We will enable plugin actions only if the KIPI interface is not null
      */
    setupActions();
}

void Plugin_KXMLHelloWorld::setupActions()
{
    /** We define plugin action which will be plug in KIPI host application.
     *  Note that if you set keyboard shortcut to an action you must take a care
     *  about already existing one from other tool to prevent conflict.
     *  Don't forget to define an unique string name to your action, to be able to disable it
     *  in KIPI host application if necessary. You must check of course name already used in
     *  others tool before to prevent redondancy.
     */

    /** We need to call setDefaultCategory in case the plugin loader cannot
      * recognize the category of an action
      */
    setDefaultCategory(ExportPlugin);

    /** An action dedicated to be plugged in digiKam Image menu.
     */
    d->actionImages = new KAction(this);
    d->actionImages->setText(i18n("KXML Hello World Image..."));
    d->actionImages->setIcon(KIcon("script-error"));
    d->actionImages->setShortcut(KShortcut(Qt::ALT + Qt::SHIFT + Qt::CTRL + Qt::Key_F1));

    /** Connect plugin action signal to dedicated slot.
     */
    connect(d->actionImages, SIGNAL(triggered(bool)),
            this, SLOT(slotActivateActionImages()));
    /** We need to register actions in plugin instance
     */
    addAction("kxmlhelloworld-actionImage", d->actionImages, ImagesPlugin);

    /** This will get items selection from KIPI host application.
     */
    ImageCollection selection = interface()->currentSelection();
    d->actionImages->setEnabled(selection.isValid() && !selection.images().isEmpty());

    /** Another action dedicated to be plugged in digiKam Tool menu.
     */
    d->actionTools = new KAction(this);
    d->actionTools->setText(i18n("KXML Hello World Tools..."));
    d->actionTools->setIcon(KIcon("script-error"));
    d->actionTools->setShortcut(KShortcut(Qt::ALT+Qt::SHIFT+Qt::CTRL+Qt::Key_F2));
    connect(d->actionTools, SIGNAL(triggered(bool)),
            this, SLOT(slotActivateActionTools()));

    addAction("kxmlhelloworld-actionTools", d->actionTools, ToolsPlugin);

    /** We will get current selected album in the digikam tree view
      */
    ImageCollection currAlbum = interface()->currentAlbum();
    bool enable = currAlbum.isValid() && !currAlbum.images().isEmpty();
    d->actionTools->setEnabled(enable);

    /** Another action dedicated to be plugged in digiKam Export menu.
     */
    d->actionExport = new KAction(this);
    d->actionExport->setText(i18n("KXML Hello World Export..."));
    d->actionExport->setIcon(KIcon("script-error"));
    d->actionExport->setShortcut(KShortcut(Qt::ALT+Qt::SHIFT+Qt::CTRL+Qt::Key_F3));
    connect(d->actionExport, SIGNAL(triggered(bool)),
            this, SLOT(slotActivateActionExport()));

    addAction("kxmlhelloworld-actionExport", d->actionExport, ExportPlugin);

    /** Another action dedicated to be plugged in digiKam Import menu.
     */
    d->actionImport = new KAction(this);
    d->actionImport->setText(i18n("KXML Hello World Import..."));
    d->actionImport->setIcon(KIcon("script-error"));
    d->actionImport->setShortcut(KShortcut(Qt::ALT+Qt::SHIFT+Qt::CTRL+Qt::Key_F4));
    connect(d->actionImport, SIGNAL(triggered(bool)),
            this, SLOT(slotActivateActionImport()));

    addAction("kxmlhelloworld-actionImport", d->actionImport, ImportPlugin);

    /** If selection change in KIPI host application, this signal will be fired, and plugin action enabled accordingly.
     */
    connect(interface(), SIGNAL(selectionChanged(bool)),
            d->actionImages, SLOT(setEnabled(bool)));

    connect(interface(), SIGNAL(currentAlbumChanged(bool)),
            d->actionTools, SLOT(setEnabled(bool)));
}

void Plugin_KXMLHelloWorld::slotActivateActionImages()
{
    /** When actionImages is actived, we display list of items selected in a message box.
     *  This example show a simple dialog with current items selected in KIPI host application.
     *  You can branch here your dedicated dialog to process items as you want.
     */
    ImageCollection images = interface()->currentSelection();

    if (images.isValid() && !images.images().isEmpty())
    {
        QStringList names;
        foreach (const KUrl& url, images.images())
            names << url.fileName();

        KMessageBox::informationList(0, i18n("This is the list of selected items"), names);
    }
}

void Plugin_KXMLHelloWorld::slotActivateActionTools()
{
    /** When actionTools is actived, we display a dedicated widget to select albums from kipi host application
     *  for post processing purpose. When selection is done, we display it in a message box.
     */
    QPointer<KDialog> dlg = new KDialog(0);
    ImageCollectionSelector* selector = interface()->imageCollectionSelector(dlg);
    dlg->setMainWidget(selector);
    dlg->exec();

    QList<ImageCollection> list = selector->selectedImageCollections();

    if (!list.isEmpty())
    {
        QStringList names;
        foreach (const ImageCollection& col, list)
            names << col.name();

        KMessageBox::informationList(0, i18n("This is the list of selected albums"), names);
    }

    delete dlg;
}

void Plugin_KXMLHelloWorld::slotActivateActionExport()
{
    /** When actionExport is actived, we display a dedicated widget from libkipiplugins which will show
     *  and permit to manage current items selection from kipi host application for batch post-processing purpose.
     */
/*
    QPointer<KPToolDialog> dlg = new KPToolDialog(0);
    KPImagesList* listView     = new KPImagesList(dlg);
    listView->setControlButtonsPlacement(KPImagesList::ControlButtonsRight);
    listView->setAllowRAW(true);
    listView->loadImagesFromCurrentSelection();
    dlg->setMainWidget(listView);
    dlg->setAboutData(new HelloWorldAbout);
    dlg->exec();

    KUrl::List list = listView->imageUrls();

    if (!list.isEmpty())
    {
        QStringList names;
        foreach (const KUrl& col, list)
            names << col.fileName();

        KMessageBox::informationList(0, i18n("This is the list of items to process"), names);
    }

    delete dlg;
*/
}

void Plugin_KXMLHelloWorld::slotActivateActionImport()
{
    /** When actionImport is actived, we display a dedicated widget from libkipiplugins which will preview
     *  the first selected item of current selection from kipi host application.
     */

    ImageCollection images = interface()->currentSelection();

    if (images.isValid() && !images.images().isEmpty())
    {
/*        QPointer<KPToolDialog> dlg = new KPToolDialog(0);
        KPPreviewManager* mng      = new KPPreviewManager(dlg);
        dlg->setMainWidget(mng);
        dlg->setAboutData(new HelloWorldAbout);
        mng->load(images.images().first().path());
        dlg->exec();
        delete dlg;*/
    }
}

}  // namespace KIPIHelloWorldPlugin
