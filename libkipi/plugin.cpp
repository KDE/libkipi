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

#include "plugin.moc"

// Qt includes

#include <QWidget>
#include <QFile>

// KDE includes

#include <kaction.h>
#include <kactioncollection.h>
#include <kcomponentdata.h>
#include <kstandarddirs.h>
#include <kdebug.h>

// Local includes

#include "version.h"
#include "interface.h"

namespace KIPI
{

QDomElement Plugin::XMLParser::makeElement(QDomDocument domDoc, const QDomElement& from)
{
    QDomElement elem            = domDoc.createElement(from.tagName());
    QDomNamedNodeMap attributes = from.attributes();
    for (int i = 0; i < attributes.size(); ++i)
    {
        QDomAttr attr = attributes.item(i).toAttr();
        if (attr.name() != "alreadyVisited")
            elem.setAttributeNode(attr);
    }
    return elem;
}

int Plugin::XMLParser::findByNameAttr(const QDomNodeList& list, const QDomElement &node)
{
    const QString nodeName = node.toElement().attribute("name");
    const QString nodeTag  = node.toElement().tagName();
    for (int i = 0; i < list.size(); ++i)
    {
        QDomElement e = list.at(i).toElement();
        if (e.tagName() == nodeTag && e.attribute("name") == nodeName)
            return i;
    }
    return -1;
}

void Plugin::XMLParser::buildPaths(QDomElement original, const QDomNodeList& localNodes, QHashPath& paths, QDomElemList stack)
{
    stack.push_back(original);

    int idx;
    if ((idx = findByNameAttr(localNodes, original)) != -1)
    {
        paths[localNodes.item(idx).toElement().attribute("name")] = stack;
    }

    if (!original.hasChildNodes())
    {
        stack.pop_back();
        return;
    }

    for (QDomNode n = original.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElement e = n.toElement();
        if (e.tagName() == "Menu" && e.hasChildNodes())
        {
            buildPaths(e, localNodes, paths, stack);
        }
    }

    stack.pop_back();
}

void Plugin::XMLParser::buildPaths(QDomElement original, const QDomNodeList& localNodes, QHashPath& paths)
{
    /*
     * For each child element of "local", we will construct the path from the
     * "original" element to first appearance of the respective child in the
     * subtree.
     */
    QDomElemList stack;
    buildPaths(original, localNodes, paths, stack);
}

class Plugin::Private
{
public:

    Private()
    {
        defaultWidget = 0;
    }

    QMap<QWidget*, KActionCollection*> actionCollection;
    KComponentData                     instance;
    QMap<QWidget*, QList<KAction*> >   actions;
    QWidget*                           defaultWidget;
};

Plugin::Plugin(const KComponentData& instance, QObject* const parent, const char* name)
      : QObject(parent), d(new Private)
{
    d->instance = instance;
    setObjectName(name);

    setProperty("KipiBinaryVersion", kipi_binary_version);
}

Plugin::~Plugin()
{
    delete d;
}

KActionCollection* Plugin::widgetActionCollection(QWidget* const widget) const
{
    QWidget* w = !widget ? d->defaultWidget : widget;

    if (!d->actions.contains(w))
    {
        kWarning() << "Error in plugin. It needs to call Plugin::setup(QWidget*) "
                   << "as the very first line when overriding the setup method.";
    }

    return d->actionCollection[w];
}

QList<KAction*> Plugin::actions(QWidget* const widget) const
{
    QWidget* w = !widget ? d->defaultWidget : widget;

    if (!d->actions.contains(w))
    {
        kWarning() << "Error in plugin. It needs to call Plugin::setup(QWidget*) "
                   << "as the very first line when overriding the setup method.";
    }

    return d->actions[w];
}

void Plugin::addAction(KAction* const action)
{
    d->actions[d->defaultWidget].append(action);
}

void Plugin::setup(QWidget* const widget)
{
    d->defaultWidget = widget;
    d->actions.insert(widget, QList<KAction*>());
    d->actionCollection.insert(widget, new KActionCollection(widget, d->instance));
}

Interface* Plugin::interface() const
{
    return (dynamic_cast<Interface*>(parent()));
}

void Plugin::mergeXMLFile(KXMLGUIClient* const host)
{
    if (!host)
    {
        kError() << "Host KXMLGUIClient is null! Cannot merge!";
        return;
    }
    const QDomDocument hostDoc = host->domDocument();
    QDomDocument pluginDoc     = domDocument();
    if (hostDoc.isNull() || pluginDoc.isNull())
    {
        kError() << "Cannot merge the XML files, at least one is null.";
        return;
    }

    QDomElement hostGuiElem     = hostDoc.firstChildElement("kpartgui");
    QDomElement hostMenuBarElem = hostGuiElem.firstChildElement("MenuBar");

    QDomDocument newPluginDoc(pluginDoc.doctype());
    QDomElement guiElem = pluginDoc.firstChildElement("gui");
    if (guiElem.isNull())
    {
        return;
    }
    QDomElement newGuiElem     = XMLParser::makeElement(newPluginDoc, guiElem);
    QDomElement menuBarElem    = guiElem.firstChildElement("MenuBar");
    QDomElement newMenuBarElem = XMLParser::makeElement(newPluginDoc, menuBarElem);
    QDomElement toolBarElem    = guiElem.firstChildElement("ToolBar");

    QHashPath paths;
    XMLParser::buildPaths(hostMenuBarElem, menuBarElem.childNodes(), paths);
    for (QDomNode n = menuBarElem.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElemList path   = paths[n.toElement().attribute("name")];
        QDomElement current = newMenuBarElem;
        QDomElement origCur = menuBarElem;
        if (path.empty())
        {
            newMenuBarElem.appendChild(n.cloneNode());
        }
        else
        {
            for (int i = 1; i < path.size() - 1; ++i)
            {
                int idx = XMLParser::findByNameAttr(current.childNodes(), path[i]);
                origCur = path[i];
                if (idx == -1)
                {
                    QDomElement newChild = XMLParser::makeElement(newPluginDoc, path[i]);
                    QDomElement textElem = origCur.firstChildElement("text");
                    if (!textElem.isNull())
                    {
                        newChild.appendChild(textElem.cloneNode());
                    }
                    current.appendChild(newChild);
                    current = newChild;
                }
                else
                {
                    current = current.childNodes().item(idx).toElement();
                }
            }
        }
        current.appendChild(n.cloneNode());
    }

    newGuiElem.appendChild(newMenuBarElem);
    newGuiElem.appendChild(toolBarElem.cloneNode());
    newPluginDoc.appendChild(newGuiElem);

    const QString pluginName      = "kipiplugin_" + objectName().toLower();
    const QString component       = KGlobal::mainComponent().componentName();
    const QString newPluginFile   =
            KStandardDirs::locateLocal("data", component + "/default-" + pluginName + "ui.rc");
    QFile file(newPluginFile);
    if (!file.open(QFile::WriteOnly))
    {
        kWarning() << "Cannot write to " << newPluginFile;
        return;
    }
    file.write(newPluginDoc.toString().toUtf8());
    file.flush();

    setXMLFiles();
}

void Plugin::setXMLFiles()
{
    const QString pluginName      = "kipiplugin_" + objectName().toLower();
    const QString component       = KGlobal::mainComponent().componentName();
    const QString newPluginFile   =
            KStandardDirs::locateLocal("data", component + "/default-" + pluginName + "ui.rc");
    const QString localPluginFile =
            KStandardDirs::locateLocal("data", component + "/" + pluginName + "ui.rc");

    setXMLFile(newPluginFile);
    setLocalXMLFile(localPluginFile);
}

} // namespace KIPI
