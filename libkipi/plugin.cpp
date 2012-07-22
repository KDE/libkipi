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
 * @author Copyright (C) 2012 by Victor Dodon
 *         <a href="mailto:dodonvictor at gmail dot com">dodonvictor at gmail dot com</a>
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

class Plugin::Private
{
public:

    Private() :
        uiBaseName(QString())
    {
        defaultWidget = 0;
    }

    QMap<QWidget*, KActionCollection*> actionCollection;
    KComponentData                     instance;
    QMap<QWidget*, QList<KAction*> >   actions;
    QWidget*                           defaultWidget;
    QString                            uiBaseName;

public:

    class XMLParser
    {

    public:

        static QDomElement makeElement(QDomDocument& domDoc, const QDomElement& from);
        static void        buildPaths(const QDomElement& original, const QDomNodeList& localNodes, QHashPath& paths);
        static int         findByNameAttr(const QDomNodeList& list, const QDomElement& node);

    private:

        XMLParser();
        static void buildPaths(const QDomElement& original, const QDomNodeList& localNodes, QHashPath& paths, QDomElemList& stack);
    };
};

QDomElement Plugin::Private::XMLParser::makeElement(QDomDocument& domDoc, const QDomElement& from)
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

void Plugin::Private::XMLParser::buildPaths(const QDomElement& original, const QDomNodeList& localNodes,
                                            QHashPath& paths, QDomElemList& stack)
{
    stack.push_back(original.cloneNode(true).toElement());

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

// --------------------------------------------------------------------------------------------------------------

Plugin::Plugin(const KComponentData& instance, QObject* const parent, const char* name)
      : QObject(parent), d(new Private)
{
    d->instance = instance;
    setObjectName(name);

    setProperty("KipiBinaryVersion", kipi_binary_version);
}

Plugin::~Plugin()
{
    clearActions();
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
    clearActions();
    d->defaultWidget = widget;
    d->actions.insert(widget, QList<KAction*>());
    d->actionCollection.insert(widget, new KActionCollection(widget, d->instance));
}

Interface* Plugin::interface() const
{
    return (dynamic_cast<Interface*>(parent()));
}

void Plugin::setUiBaseName(const char* name)
{
    d->uiBaseName = QString(name);
}

QString Plugin::uiBaseName() const
{
    return d->uiBaseName;
}

void Plugin::mergeXMLFile(KXMLGUIClient *const host)
{
    if (!host)
    {
        kError() << "Host KXMLGUIClient is null!";
        return;
    }
    if (d->uiBaseName.isEmpty())
    {
        kError() << "UI file basename is not set! You must first call setUiBaseName.";
        return;
    }

    const QString componentName = KGlobal::mainComponent().componentName();
    const QString defaultUI     = KGlobal::dirs()->locate("data", QString("kipi/") + d->uiBaseName);
    const QString localUI       = KGlobal::dirs()->locateLocal("data", componentName + "/" + d->uiBaseName);

    QFile defaultUIFile(defaultUI);
    QDomDocument defaultDomDoc;
    if (!defaultUIFile.open(QFile::ReadOnly) || !defaultDomDoc.setContent(&defaultUIFile))
    {
        kError() << "Could not open default ui file";
        return;
    }
    defaultUIFile.close();

    const QDomDocument hostDoc = host->domDocument();
    if (hostDoc.isNull() || defaultDomDoc.isNull())
    {
        kError() << "Cannot merge the XML files, at least one is null!";
        return;
    }

    QDomElement hostGuiElem       = hostDoc.firstChildElement("kpartgui");
    QDomElement hostMenuBarElem   = hostGuiElem.firstChildElement("MenuBar");

    QDomDocument newPluginDoc(defaultDomDoc.doctype());
    QDomElement defGuiElem        = defaultDomDoc.firstChildElement("gui");

    QDomElement newGuiElem        = Private::XMLParser::makeElement(newPluginDoc, defGuiElem);
    QDomElement defMenuBarElem    = defGuiElem.firstChildElement("MenuBar");
    QDomElement newMenuBarElem    = Private::XMLParser::makeElement(newPluginDoc, defMenuBarElem);
    QDomElement defToolBarElem    = defGuiElem.firstChildElement("ToolBar");
    QDomElement defActionPropElem = defGuiElem.firstChildElement("ActionProperties");

    QHashPath paths;
    Private::XMLParser::buildPaths(hostMenuBarElem, defMenuBarElem.childNodes(), paths);

    for (QDomNode n = defMenuBarElem.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElemList path    = paths[n.toElement().attribute("name")];
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
                int idx = Private::XMLParser::findByNameAttr(current.childNodes(), path[i]);
                origCurr = path[i];
                if (idx == -1)
                {
                    if (!path[i].isNull())
                    {
                        QDomElement newChild = Private::XMLParser::makeElement(newPluginDoc, path[i]);
                        QDomElement textElem = origCurr.firstChildElement("text");

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

    QFile localUIFile(localUI);
    QDomDocument localDomDoc;
    if (!localUIFile.exists() || !localUIFile.open(QFile::ReadOnly) || !localDomDoc.setContent(&localUIFile))
    {
        newGuiElem.appendChild(defToolBarElem.cloneNode());
        newGuiElem.appendChild(defActionPropElem.cloneNode());
    }
    else
    {
        QDomElement localGuiElem = localDomDoc.firstChildElement("gui");
        QDomElement localToolBarElem = localGuiElem.firstChildElement("ToolBar");
        QDomElement localActionPropElem = localGuiElem.firstChildElement("ActionProperties");

        newGuiElem.appendChild(localToolBarElem.cloneNode());
        newGuiElem.appendChild(localActionPropElem.cloneNode());
    }
    localUIFile.close();
    QFile writeFile(localUI);
    if (!writeFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        kError() << "Could not open " << localUI << " for writing!";
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
    foreach (QAction* action, actions)
    {
        actionCollection()->removeAction(action);
    }
}

} // namespace KIPI
