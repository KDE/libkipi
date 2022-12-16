/*
    SPDX-FileCopyrightText: 2004-2018 Gilles Caulier <caulier dot gilles at gmail dot com>
    SPDX-FileCopyrightText: 2012 Victor Dodon <dodonvictor at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Renchi Raju <renchi dot raju at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Jesper K. Pedersen <blackie at kde dot org>
    SPDX-FileCopyrightText: 2004-2005 Aurelien Gateau <aurelien dot gateau at free dot fr>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KIPI_PLUGIN_H
#define KIPI_PLUGIN_H

// Std includes

#include <memory>

// Qt includes

#include <QObject>
#include <QList>
#include <QHash>
#include <QDomElement>
#include <QDomNode>
#include <QDomDocument>

// KF includes

#include <KXMLGUIClient>

// Local includes

#include "libkipi_export.h"

class QAction;

namespace KIPI
{

class Interface;

/**
 * The Category enum.
 */
enum Category
{
    InvalidCategory = -1,
    ImagesPlugin    = 0,
    ToolsPlugin,
    ImportPlugin,
    ExportPlugin,
    BatchPlugin,
    CollectionsPlugin
};

/**
 * @class Plugin plugin.h <KIPI/Plugin>
 *
 * @short Base class for the KIPI plugins
 *
 */
class LIBKIPI_EXPORT Plugin : public QObject, public KXMLGUIClient
{
    Q_OBJECT

    typedef QList<QDomElement>                        QDomElemList;
    typedef QHash<QString, QDomElemList>              QHashPath;
    typedef QMap<QWidget*, QMap<QAction*, Category> > ActionCategoryMap;

public:

    /**
     * Constructs a plugin
     *
     * @param parent the parent of this object
     * @param name the name of the plugin
     */
    Plugin(QObject* const parent, const char* name);

    /**
     * Standard destructor
     *
     * All the actions in the actionCollection are deleted before the plugin is
     * deleted.
     */
    ~Plugin() override;

    /**
     * Returns the plugin actions associated with passed @p widget, or with
     * the default widget, if @p widget is a nullptr. The actions are in
     * the same order as added to the plugin.
     */
    QList<QAction*> actions(QWidget* const widget = nullptr) const;

    /**
     * Returns the KIPI::Interface
     */
    Interface* interface() const;

    /**
     * Virtual method that must be overridden by the non abstract descendants and
     * must be called before any actions are added.
     *
     * @param widget The widget which holds the plugin. It will be set as the default widget.
     */
    virtual void setup(QWidget* const widget) = 0;

    /**
     * Returns the category of the specified plugin action, or InvalidCategory
     * if the action is not recognised
     */
    Category category(QAction* const action) const;

    /**
     * Force the plugin to reread and to reload its XML file
     */
    void rebuild();

protected:

    /**
     * Register an action to the plugin instance and add it to the action collection.
     *
     * The action is added only if the action name is not in the disabled actions
     * list of the PluginLoader singleton class.
     *
     * @param name the name by which the action will be added to the action collection
     * @param action the action to add
     *
     * @note It just calls addAction(const QString&, QAction* const, Category)
     * with the default category, so the default
     * category must be set using setDefaultCategory() before you use this method
     */
    void addAction(const QString& name, QAction* const action);

    /**
     * Register action to the plugin instance and add it to the action collection
     *
     * The action is added only if the action name is not in the disabled actions
     * list of the PluginLoader singleton class.
     *
     * @param name the name by which the action will be added to the action collection
     * @param action the action to add
     * @param cat the category of the action
     */
    void addAction(const QString& name, QAction* const action, Category cat);

    /**
     * Sets the default category of the plugin actions
     *
     * \sa defaultCategory()
     */
    void setDefaultCategory(Category cat);

    /**
     * Returns the default category of the plugin actions
     *
     * \sa setDefaultCategory()
     */
    Category defaultCategory() const;

    /**
     * Sets the name of the XML file associated with this KXMLGUIClient. You must
     * provide only the filename without slashes.
     *
     * The default XML file must be installed in the dir ${KDE_INSTALL_KXMLGUIDIR}/kipi,
     * modifications will be stored in the local config dir.
     *
     * \sa uiBaseName()
     */
    void setUiBaseName(const char* name);

    /**
     * Return the base name of the XML file associated with this KXMLGUIClient
     *
     * \sa setUiBaseName()
     */
    QString uiBaseName() const;

    /**
     * Adapt the XML file of the plugin with the one of the KXmlGuiWindow main window.
     * It's recommended to call it on every creation of the plugin.
     *
     * @note the XML file of the plugin must be set using setUiBaseName()
     */
    void setupXML();

private:

    /** For internal uses only
      */
    void addAction(QAction* const action);
    void addAction(QAction* const action, Category cat);

    void mergeXMLFile(KXMLGUIClient* const host);
    void clearActions();

private:

    class Private;
    std::unique_ptr<Private> const d;
};

} // namespace KIPI

#endif  // KIPI_PLUGIN_H
