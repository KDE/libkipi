/*
    SPDX-FileCopyrightText: 2012 Victor Dodon <dodonvictor at gmail dot com>
    SPDX-FileCopyrightText: 2012-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KIPITESTPLUGINLOADER_H
#define KIPITESTPLUGINLOADER_H

// Qt includes

#include <QObject>
#include <QList>
#include <QString>

// Libkipi includes

#include "plugin.h"
#include "pluginloader.h"

class QAction;

class KActionCollection;
class KXmlGuiWindow;

using namespace KIPI;

namespace KXMLKipiCmd
{

class KipiInterface;

class KipiTestPluginLoader : public QObject
{
    Q_OBJECT

public:

    KipiTestPluginLoader(KXmlGuiWindow* const parent, KipiInterface* const interface);

    QList<PluginLoader::Info*> pluginList();
    QList<QAction*>            kipiActionsByCategory(Category cat) const;
    KActionCollection*         pluginsActionCollection() const;

    static KipiTestPluginLoader* instance();


private Q_SLOTS:

    /** Called by PluginLoader when plugins list must be reloaded
     */
    void slotKipiPluginsPlug();

private:

    ~KipiTestPluginLoader() override;

    void loadPlugins();
    void checkEmptyCategory(Category cat);
    QString categoryName(Category cat) const;
    QString categoryShortName(Category cat) const;

private:

    static KipiTestPluginLoader* m_instance;

    class Private;
    Private* const d;
};

} // namespace KXMLKipiCmd

#endif // KIPITESTPLUGINLOADER_H
