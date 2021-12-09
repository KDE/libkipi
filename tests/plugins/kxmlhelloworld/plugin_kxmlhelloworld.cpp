/* ============================================================
 *
 * This file is a part of KDE project
 *
 * Date        : 2012-02-16
 * Description : an Hello World plugin using KDE XML-GUI technology.
 *
 * Copyright (C) 2012-2018 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2012      by Victor Dodon   <dodonvictor at gmail dot com>
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
 *  1/ class header file.
 *  2/ C ansi if really necessary.
 *  3/ C++ (always preferred than C ansi.
 *  4/ Extra libraries such as openCV for ex.
 *  4/ Qt.
 *  5/ KDE.
 *  6/ Local files.
 *  7/ At end of file .moc file of class.
 *
 *  Also, use C++ classes include header styles with Qt5,
 *  but do not use it with KF5 headers (use C ANSI style instead).
 */

// include here header file. See also loc file to plug at end of implementation.

#include "plugin_kxmlhelloworld.h"

// Qt includes

#include <QPointer>
#include <QDomDocument>
#include <QAction>
#include <QDebug>
#include <QKeySequence>
#include <QUrl>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

// KF includes

#include <KPluginFactory>

/// This is all libkipi headers included in this tool.

#include "imagecollection.h"
#include "imagecollectionselector.h"
#include "interface.h"

// Special header used to include kipi-plugins debug space when plugin is includes in Kipi-plugins project.
//#include "kipiplugins_debug.h"

/** Put here all Kipi-plugins common includes used to create a new tool.
 *  Look into kipi-plugins/common/libkipiplugins/ API documentation for details.
 */

/// You must wrap all your plugin code to a dedicated namespace
namespace KIPIKXMLHelloWorldPlugin
{

/** Under Kipi-plugins, you can use KPToolDialog class from kipi-plugins to display plugin dialogs. It offers some facilities to
    set data and rules about plugins, especially to wrap properly tool with KDE bugzilla. We use KPAboutData container
    for that.
*/

/** Using private container everywhere is clear to speed up compilation and reduce source code dependencies through header files.
 *  See this url for details : http://techbase.kde.org/Policies/Binary_Compatibility_Issues_With_C%2B%2B#Using_a_d-Pointer
 */
class Plugin_KXMLHelloWorld::Private
{
public:

    Private()
    {
        /// It's always clean to init pointers to zero. If crash appear,
        /// debugger will show a null pointer instead a non initialized one.
        actionImages = nullptr;
        actionTools  = nullptr;
        actionExport = nullptr;
        actionImport = nullptr;
    }

    /** These plugin actions will plugged into menu KIPI host application.
     */
    QAction* actionImages;
    QAction* actionTools;
    QAction* actionExport;
    QAction* actionImport;
};

/** Macro from KDE KParts to create the factory for this plugin.
 *  The first argument is the name of the plugin library
 *  and the second is the generic factory templated from
 *  the class for your plugin.
 */
K_PLUGIN_FACTORY(KXMLHelloWorldFactory, registerPlugin<Plugin_KXMLHelloWorld>();)

/** The plugin constructor. Note that plugin name passed as string in 3rd argument of KIPI::Plugin parent class
 *  is the same than Name value from .desktop file.
 */
Plugin_KXMLHelloWorld::Plugin_KXMLHelloWorld(QObject* const parent, const QVariantList&)
    : Plugin(parent, "KXMLHelloWorld"),
      /// Private container is allocated here.
      d(new Private)
{
    // plugin do not use libkipi debug space. There is a debug space dedicate to kipi-plugins. See kipiplugins_debug.h header.
    //qCDebug(KIPIPLUGINS_LOG) << "Plugin_KXMLHelloWorld plugin loaded";

    /** This is needed to setup the plugin gui and to merge with the kipi host
     *  application gui.
     *  The name of the UI file must be: nameofpluginui.rc, where "nameofplugin"
     *  is the name given to the plugin factory, usually: kipiplugin_<name> .
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
     *  others tool before to prevent redundancy.
     */

    /** We need to call setDefaultCategory in case the plugin loader cannot
      * recognize the category of an action
      */
    setDefaultCategory(ExportPlugin);

    /** An action dedicated to be plugged in Image menu.
     */
    d->actionImages = new QAction(this);
    d->actionImages->setText(QLatin1String("KXML Hello World Image..."));
    d->actionImages->setIcon(QIcon::fromTheme(QString::fromLatin1("script-error")));
    d->actionImages->setShortcut(QKeySequence(Qt::ALT + Qt::SHIFT + Qt::CTRL + Qt::Key_F1));

    /** Connect plugin action signal to dedicated slot.
     */
    connect(d->actionImages, SIGNAL(triggered(bool)),
            this, SLOT(slotActivateActionImages()));

    /** We need to register actions in plugin instance
     */
    addAction(QString::fromLatin1("kxmlhelloworld-actionImage"), d->actionImages, ImagesPlugin);

    /** This will get items selection from KIPI host application.
     */
    ImageCollection selection = interface()->currentSelection();
    d->actionImages->setEnabled(selection.isValid() && !selection.images().isEmpty());

    /** Another action dedicated to be plugged in Tool menu.
     */
    d->actionTools = new QAction(this);
    d->actionTools->setText(QLatin1String("KXML Hello World Tools..."));
    d->actionTools->setIcon(QIcon::fromTheme(QString::fromLatin1("script-error")));
    d->actionTools->setShortcut(QKeySequence(Qt::ALT+Qt::SHIFT+Qt::CTRL+Qt::Key_F2));

    connect(d->actionTools, SIGNAL(triggered(bool)),
            this, SLOT(slotActivateActionTools()));

    addAction(QString::fromLatin1("kxmlhelloworld-actionTools"), d->actionTools, ToolsPlugin);

    /** We will get current selected album in the host tree view
     */
    ImageCollection currAlbum = interface()->currentAlbum();
    bool enable               = currAlbum.isValid() && !currAlbum.images().isEmpty();
    d->actionTools->setEnabled(enable);

    /** Another action dedicated to be plugged in host Export menu.
     */
    d->actionExport = new QAction(this);
    d->actionExport->setText(QLatin1String("KXML Hello World Export..."));
    d->actionExport->setIcon(QIcon::fromTheme(QString::fromLatin1("script-error")));
    d->actionExport->setShortcut(QKeySequence(Qt::ALT+Qt::SHIFT+Qt::CTRL+Qt::Key_F3));

    connect(d->actionExport, SIGNAL(triggered(bool)),
            this, SLOT(slotActivateActionExport()));

    addAction(QString::fromLatin1("kxmlhelloworld-actionExport"), d->actionExport, ExportPlugin);

    /** Another action dedicated to be plugged in host Import menu.
     */
    d->actionImport = new QAction(this);
    d->actionImport->setText(QLatin1String("KXML Hello World Import..."));
    d->actionImport->setIcon(QIcon::fromTheme(QString::fromLatin1("script-error")));
    d->actionImport->setShortcut(QKeySequence(Qt::ALT+Qt::SHIFT+Qt::CTRL+Qt::Key_F4));

    connect(d->actionImport, SIGNAL(triggered(bool)),
            this, SLOT(slotActivateActionImport()));

    addAction(QString::fromLatin1("kxmlhelloworld-actionImport"), d->actionImport, ImportPlugin);

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

        const auto imageUrls = images.images();
        for (const QUrl& url : imageUrls)
            names << url.fileName();

        QMessageBox::information(nullptr, QLatin1String("This is the list of selected items"), names.join(QString::fromLatin1("\n")));
    }
}

void Plugin_KXMLHelloWorld::slotActivateActionTools()
{
    /** When actionTools is actived, we display a dedicated widget to select albums from kipi host application
     *  for post processing purpose. When selection is done, we display it in a message box.
     */
    QPointer<QDialog> dlg                   = new QDialog(nullptr);
    ImageCollectionSelector* const selector = interface()->imageCollectionSelector(dlg);
    QDialogButtonBox* const buttons         = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dlg);
    QVBoxLayout* const vbox                 = new QVBoxLayout(dlg);
    vbox->addWidget(selector);
    vbox->addWidget(buttons);
    dlg->setLayout(vbox);

    connect(buttons->button(QDialogButtonBox::Ok), &QPushButton::clicked,
            dlg.data(), &QDialog::accept);

    connect(buttons->button(QDialogButtonBox::Cancel), &QPushButton::clicked,
            dlg.data(), &QDialog::reject);

    dlg->exec();

    const QList<ImageCollection> list = selector->selectedImageCollections();

    if (!list.isEmpty())
    {
        QStringList names;

        for (const ImageCollection& col : list)
            names << col.name();

        QMessageBox::information(nullptr, QLatin1String("This is the list of selected albums"), names.join(QString::fromLatin1("\n")));
    }

    delete dlg;
}

void Plugin_KXMLHelloWorld::slotActivateActionExport()
{
    /** When actionExport is actived, we can display a dedicated widget from libkipiplugins which will show
     *  and permit to manage current items selection from kipi host application for batch post-processing purpose.
     */

    QMessageBox::information(nullptr, QLatin1String("Information"), QLatin1String("Plugin_KXMLHelloWorld::slotActivateActionExport() activated"));
}

void Plugin_KXMLHelloWorld::slotActivateActionImport()
{

    ImageCollection images = interface()->currentSelection();

    if (images.isValid() && !images.images().isEmpty())
    {
        /** When actionImport is actived, we can display a dedicated widget from libkipiplugins which will preview
         *  the first selected item of current selection from kipi host application.
         */
    }

    QMessageBox::information(nullptr, QLatin1String("Information"), QLatin1String("Plugin_KXMLHelloWorld::slotActivateActionImport() activated"));
}

}  // namespace KIPIHelloWorldPlugin

// Moc file must be included at end because it's a plugin (it's a specific and special case where we need to do it).

#include "plugin_kxmlhelloworld.moc"
