/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  plugin interface
 *
 * @author Copyright (C) 2004-2016 by Gilles Caulier
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

#include "plugin.h"

// Qt includes

#include <QApplication>
#include <QWidget>
#include <QFile>
#include <QDir>
#include <QAction>
#include <QStandardPaths>

// KDE includes

#include <kactioncollection.h>

// Local includes

#include "libkipi_version.h"
#include "libkipi_debug.h"
#include "interface.h"
#include "pluginloader.h"

namespace KIPI
{

class Q_DECL_HIDDEN Plugin::Private
{
public:

    Private() :
        uiBaseName(QString())
    {
        defaultWidget   = 0;
        defaultCategory = InvalidCategory;
    }

    ActionCategoryMap actionsCat;
    QWidget*          defaultWidget;
    QString           uiBaseName;
    Category          defaultCategory;

public:

    class XMLParser
    {

    public:

        static QDomElement makeElement(QDomDocument& domDoc, const QDomElement& from);
        static void        buildPaths(const QDomElement& original, const QDomNodeList& localNodes, QHashPath& paths);
        static int         findByNameAttr(const QDomNodeList& list, const QDomElement& node);
        static void        removeDisabledActions(QDomElement& elem);

    private:

        XMLParser();
        static void buildPaths(const QDomElement& original, const QDomNodeList& localNodes, QHashPath& paths, QDomElemList& stack);
    };
};

QDomElement Plugin::Private::XMLParser::makeElement(QDomDocument& domDoc, const QDomElement& from)
{
    if (domDoc.isNull() || from.isNull())
        return QDomElement();

    QDomElement elem            = domDoc.createElement(from.tagName());
    QDomNamedNodeMap attributes = from.attributes();

    for (int i = 0; i < attributes.size(); ++i)
    {
        QDomAttr attr = attributes.item(i).toAttr();

        if (attr.name() != QString::fromLatin1("alreadyVisited"))
            elem.setAttributeNode(attr);
    }

    return elem;
}

void Plugin::Private::XMLParser::buildPaths(const QDomElement& original, const QDomNodeList& localNodes, QHashPath& paths)
{
    /*
     * For each child element of "local", we will construct the path from the
     * "original" element to first appearance of the respective child in the
     * subtree.
     */
    QDomElemList stack;
    buildPaths(original, localNodes, paths, stack);
}

int Plugin::Private::XMLParser::findByNameAttr(const QDomNodeList& list, const QDomElement& node)
{
    const QString nodeName = node.toElement().attribute(QString::fromLatin1("name"));
    const QString nodeTag  = node.toElement().tagName();

    for (int i = 0; i < list.size(); ++i)
    {
        QDomElement e = list.at(i).toElement();

        if (e.tagName() == nodeTag && e.attribute(QString::fromLatin1("name")) == nodeName)
            return i;
    }

    return -1;
}

void Plugin::Private::XMLParser::removeDisabledActions(QDomElement& elem)
{
    QDomNodeList actionList      = elem.elementsByTagName(QString::fromLatin1("Action"));
    QStringList  disabledActions = PluginLoader::instance()->disabledPluginActions();
    QDomElemList disabledElements;

    for(int i = 0; i < actionList.size(); ++i)
    {
        QDomElement el = actionList.item(i).toElement();

        if (el.isNull())
            continue;

        if (disabledActions.contains(el.attribute(QString::fromLatin1("name"))))
        {
            disabledElements << el;
        }
    }

    foreach(QDomElement element, disabledElements)
    {
        //qCDebug(LIBKIPI_LOG) << "Plugin action '" << element.attribute("name") << "' is disabled.";
        QDomElement parent = element.parentNode().toElement();
        parent.removeChild(element);
    }
}

void Plugin::Private::XMLParser::buildPaths(const QDomElement& original, const QDomNodeList& localNodes,
                                            QHashPath& paths, QDomElemList& stack)
{
    stack.push_back(original.cloneNode(true).toElement());

    int idx;

    if ((idx = findByNameAttr(localNodes, original)) != -1)
    {
        paths[localNodes.item(idx).toElement().attribute(QString::fromLatin1("name"))] = stack;
    }

    if (!original.hasChildNodes())
    {
        stack.pop_back();
        return;
    }

    for (QDomNode n = original.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElement e = n.toElement();

        if (e.tagName() == QString::fromLatin1("Menu") && e.hasChildNodes())
        {
            buildPaths(e, localNodes, paths, stack);
        }
    }

    stack.pop_back();
}

// --------------------------------------------------------------------------------------------------------------

Plugin::Plugin(QObject* const parent, const char* name)
      : QObject(parent), d(new Private)
{
    setObjectName(QString::fromLatin1(name));
}

Plugin::~Plugin()
{
    clearActions();
    delete d;
}

QList<QAction *> Plugin::actions(QWidget* const widget) const
{
    QWidget* const w = !widget ? d->defaultWidget : widget;

    if (!d->actionsCat.contains(w))
    {
        qCWarning(LIBKIPI_LOG) << "Error in plugin. It needs to call Plugin::setup(QWidget*) "
                               << "as the very first line when overriding the setup method.";
    }

    return d->actionsCat[w].keys();
}

void Plugin::addAction(const QString& name, QAction* const action)
{
    if (!action || name.isEmpty())
        return;

    if (!PluginLoader::instance()->disabledPluginActions().contains(name))
    {
        actionCollection()->addAction(name, action);
        addAction(action);
    }
    else
    {
        //qCDebug(LIBKIPI_LOG) << "Action '" << name << "' is disabled.";
    }
}

void Plugin::addAction(QAction* const action)
{
    addAction(action, d->defaultCategory);
}

void Plugin::addAction(const QString& name, QAction* const action, Category cat)
{
    if (!action || name.isEmpty())
        return;

    if (!PluginLoader::instance()->disabledPluginActions().contains(name))
    {
        actionCollection()->addAction(name, action);
        addAction(action, cat);
    }
    else
    {
        //qCDebug(LIBKIPI_LOG) << "Action '" << name << "' is disabled.";
    }
}

void Plugin::addAction(QAction* const action, Category cat)
{
    if (cat == InvalidCategory)
    {
        qCWarning(LIBKIPI_LOG) << "Error in plugin. Action '" << action->objectName() << "has "
                                  "invalid category. You must set default plugin category or "
                                  "to use a valid Category";
    }

    d->actionsCat[d->defaultWidget].insert(action, cat);
}

void Plugin::setup(QWidget* const widget)
{
    clearActions();
    d->defaultWidget = widget;
    d->actionsCat.insert(widget, QMap<QAction*, Category>());
}

Category Plugin::category(QAction* const action) const
{
    QMap<QAction *, Category>::const_iterator it = d->actionsCat[d->defaultWidget].constFind(action);

    if (it != d->actionsCat[d->defaultWidget].constEnd())
    {
        return it.value();
    }
    else
    {
        if (d->defaultCategory == InvalidCategory)
        {
            qCWarning(LIBKIPI_LOG) << "Error in plugin. Invalid category. "
                                      "You must set default plugin category.";
        }

        return d->defaultCategory;
    }
}

Interface* Plugin::interface() const
{
    return (dynamic_cast<Interface*>(parent()));
}

void Plugin::setUiBaseName(const char* name)
{
    if (name && *name)
        d->uiBaseName = QString::fromLatin1(name);
}

QString Plugin::uiBaseName() const
{
    return d->uiBaseName;
}

void Plugin::mergeXMLFile(KXMLGUIClient *const host)
{
    if (!host)
    {
        qCCritical(LIBKIPI_LOG) << "Host KXMLGUIClient is null!";
        return;
    }

    if (d->uiBaseName.isEmpty())
    {
        qCCritical(LIBKIPI_LOG) << "UI file basename is not set! You must first call setUiBaseName.";
        return;
    }

    const QString componentName = QApplication::applicationName();
    const QString defaultUI     = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QString::fromLatin1("kxmlgui5/kipi/") + d->uiBaseName);
    const QString localUIdir    = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QString::fromLatin1("/kxmlgui5/") +
                                                                   componentName;
    const QString localUI       = localUIdir + QString::fromLatin1("/") + d->uiBaseName;

    qCDebug(LIBKIPI_LOG) << "UI file :" << defaultUI;

    QFile        defaultUIFile(defaultUI);
    QDomDocument defaultDomDoc;

    if (!defaultUIFile.open(QFile::ReadOnly) || !defaultDomDoc.setContent(&defaultUIFile))
    {
        qCCritical(LIBKIPI_LOG) << "Could not open default ui file " << defaultUI << " for ui basename " << d->uiBaseName;
        return;
    }

    defaultUIFile.close();
    const QDomDocument hostDoc    = host->domDocument();

    if (hostDoc.isNull() || defaultDomDoc.isNull())
    {
        qCCritical(LIBKIPI_LOG) << "Cannot merge the XML files, at least one is null!";
        return;
    }

    QDomElement hostGuiElem       = hostDoc.firstChildElement(QString::fromLatin1("kpartgui"));
    QDomElement hostMenuBarElem   = hostGuiElem.firstChildElement(QString::fromLatin1("MenuBar"));

    QDomDocument newPluginDoc(defaultDomDoc.doctype());
    QDomElement  defGuiElem       = defaultDomDoc.firstChildElement(QString::fromLatin1("gui"));

    Private::XMLParser::removeDisabledActions(defGuiElem);

    QDomElement newGuiElem        = Private::XMLParser::makeElement(newPluginDoc, defGuiElem);
    QDomElement defMenuBarElem    = defGuiElem.firstChildElement(QString::fromLatin1("MenuBar"));
    QDomElement newMenuBarElem    = Private::XMLParser::makeElement(newPluginDoc, defMenuBarElem);
    QDomElement defToolBarElem    = defGuiElem.firstChildElement(QString::fromLatin1("ToolBar"));
    QDomElement defActionPropElem = defGuiElem.firstChildElement(QString::fromLatin1("ActionProperties"));

    QHashPath paths;
    Private::XMLParser::buildPaths(hostMenuBarElem, defMenuBarElem.childNodes(), paths);

    for (QDomNode n = defMenuBarElem.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElemList path    = paths[n.toElement().attribute(QString::fromLatin1("name"))];
        QDomElement current  = newMenuBarElem;
        QDomElement origCurr = defMenuBarElem;

        if (path.empty())
        {
            newMenuBarElem.appendChild(n.cloneNode());
        }
        else
        {
            for (int i = 1; i < path.size() - 1; ++i)
            {
                int idx  = Private::XMLParser::findByNameAttr(current.childNodes(), path[i]);
                origCurr = path[i];

                if (idx == -1)
                {
                    if (!path[i].isNull())
                    {
                        QDomElement newChild = Private::XMLParser::makeElement(newPluginDoc, path[i]);
                        QDomElement textElem = origCurr.firstChildElement(QString::fromLatin1("text"));

                        if (!textElem.isNull())
                        {
                            newChild.appendChild(textElem.cloneNode());
                        }

                        current.appendChild(newChild);
                        current = newChild;
                    }
                }
                else
                {
                    current = current.childNodes().item(idx).toElement();
                }
            }
        }

        if (!current.isNull())
            current.appendChild(n.cloneNode());
    }

    newGuiElem.appendChild(newMenuBarElem);
    QFile        localUIFile(localUI);
    QDomDocument localDomDoc;
// be safe rather than sorry
// create the appname folder in kxmlgui5
    QDir localUIDir(localUIdir);
    if (!localUIDir.exists())
        QDir().mkpath(localUIdir);

   if (!localUIFile.exists() || !localUIFile.open(QFile::ReadOnly) || !localDomDoc.setContent(&localUIFile))
    {
        newGuiElem.appendChild(defToolBarElem.cloneNode());
        newGuiElem.appendChild(defActionPropElem.cloneNode());
    }
    else
    {
        QDomElement localGuiElem        = localDomDoc.firstChildElement(QString::fromLatin1("gui"));

        Private::XMLParser::removeDisabledActions(localGuiElem);

        QDomElement localToolBarElem    = localGuiElem.firstChildElement(QString::fromLatin1("ToolBar"));
        QDomElement localActionPropElem = localGuiElem.firstChildElement(QString::fromLatin1("ActionProperties"));

        newGuiElem.appendChild(localToolBarElem.cloneNode());
        newGuiElem.appendChild(localActionPropElem.cloneNode());
    }

    localUIFile.close();
    QFile writeFile(localUI);

    if (!writeFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        qCCritical(LIBKIPI_LOG) << "Could not open " << localUI << " for writing!";
        return;
    }

    newPluginDoc.appendChild(newGuiElem);

    writeFile.write(newPluginDoc.toString().toUtf8());
    writeFile.close();

    setXMLFile(d->uiBaseName);
}

void Plugin::clearActions()
{
    QList<QAction*> actions = actionCollection()->actions();

    foreach (QAction* const action, actions)
    {
        actionCollection()->removeAction(action);
    }
}

void Plugin::setupXML()
{
    mergeXMLFile(dynamic_cast<KXMLGUIClient*>(interface()->parent()));
}

void Plugin::rebuild()
{
    QString file = xmlFile();

    if (!file.isEmpty())
    {
        setXMLGUIBuildDocument(QDomDocument());
        setXMLFile(file, false);
    }
}

void Plugin::setDefaultCategory(Category cat)
{
    d->defaultCategory = cat;
}

Category Plugin::defaultCategory() const
{
    return d->defaultCategory;
}

} // namespace KIPI
