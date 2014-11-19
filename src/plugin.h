/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  plugin interface
 *
 * @author Copyright (C) 2004-2012 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2012      by Victor Dodon
 *         <a href="mailto:dodonvictor at gmail dot com">dodonvictor at gmail dot com</a>
 * @author Copyright (C) 2004-2005 by Renchi Raju
 *         <a href="mailto:renchi dot raju at gmail dot com">renchi dot raju at gmail dot com</a>
 * @author Copyright (C) 2004-2005 by Jesper K. Pedersen
 *         <a href="mailto:blackie at kde dot org">blackie at kde dot org</a>
 * @author Copyright (C) 2004-2005 by Aurelien Gateau
 *         <a href="mailto:aurelien dot gateau at free dot fr">aurelien dot gateau at free dot fr</a>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef KIPI_PLUGIN_H
#define KIPI_PLUGIN_H

// Qt includes

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include <QtXml/QDomDocument>

// KDE includes

#include <kxmlguiclient.h>

// Local includes

#include "libkipi_export.h"

class KAction;
class KActionCollection;
class KComponentData;

namespace KIPI
{

class Interface;

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
 * @short Base class for the KIPI plugins
 *
 * TODO complete the documentation
 */
class LIBKIPI_EXPORT Plugin : public QObject, public KXMLGUIClient
{
    Q_OBJECT

    typedef QList<QDomElement>                        QDomElemList;
    typedef QHash<QString, QDomElemList>              QHashPath;
    typedef QMap<QWidget*, QMap<KAction*, Category> > ActionCategoryMap;

public:

    /**
     * Constructs a plugin
     *
     * @param instance the KComponentData associated with this plugin
     * @param parent the parent of this object
     * @param name the name of the plugin
     */
    Plugin(const KComponentData& instance, QObject* const parent, const char* name);

    /**
     * Standard destructor
     *
     * All the actions in the actionCollection are deleted before the plugin is
     * deleted
     */
    virtual ~Plugin();

    /**
     * Returns the plugin actions associated with the widget passed as argument, or with
     * the default widget, if widget is null or not provided. The actions are in
     * the same order as added to the plugin.
     */
    QList<KAction*> actions(QWidget* const widget = 0) const;

    /**
     * Returns the KIPI::Interface
     */
    Interface* interface() const;

    /**
     * Virtual method that must be overrided by the non abstract descendants and
     * must be called before any actions are added.
     *
     * @param widget The widget which holds the plugin. It will be set as the default widget.
     */
    virtual void setup(QWidget* const widget) = 0;

    /**
     * Returns the category of the specified plugin action, or InvalidCategory
     * if the action is not recognised
     */
    Category category(KAction* const action) const;

    /**
     * Force the plugin to reread and to reload its xml file
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
     * @note It just calls addAction with the default category, so the default
     * category must be set using setDefaultCategory before you use this method
     */
    void addAction(const QString& name, KAction* const action);

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
    void addAction(const QString& name, KAction* const action, Category cat);

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
     * Sets the name of the xml file associated with this KXMLGUIClient. You must
     * provide only the filename without slashes.
     *
     * The default xml file must be installed in the ${DATA_INSTALL_DIR}/kipi,
     * modifications are stored in the local config dir of the KGlobal::mainComponent
     *
     * \sa uiBaseName()
     */
    void setUiBaseName(const char* name);

    /**
     * Return the base name of the xml file associated with this KXMLGUIClient
     *
     * \sa setUiBaseName()
     */
    QString uiBaseName() const;

    /**
     * Adapt the xml file of the plugin with the one of the KXmlGuiWindow main window.
     * It's recommended to call it on every creation of the plugin.
     *
     * @note the xml file of the plugin must be set using setUiBaseName()
     */
    void setupXML();

private:

    /** For internal uses only
      */
    void addAction(KAction* const action);
    void addAction(KAction* const action, Category cat);

    void mergeXMLFile(KXMLGUIClient* const host);
    void clearActions();

private:

    class Private;
    Private* const d;
};

} // namespace KIPI

#endif  // KIPI_PLUGIN_H
