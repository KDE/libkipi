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

#ifndef KIPI_PLUGIN_H
#define KIPI_PLUGIN_H

// Qt includes

#include <QtCore/QObject>
#include <QtCore/QList>

// Local includes

#include "libkipi_export.h"

class KAction;
class KActionCollection;
class KComponentData;

/** @namespace KIPI */
namespace KIPI
{

enum Category
{
    ImagesPlugin = 0,
    ToolsPlugin,
    ImportPlugin,
    ExportPlugin,
    BatchPlugin,
    CollectionsPlugin
};

class LIBKIPI_EXPORT Plugin : public QObject
{
    Q_OBJECT

public:

    Plugin(const KComponentData& instance, QObject* const parent, const char* name);
    virtual ~Plugin();

    QList<KAction*>    actions(QWidget* parent = 0);
    KActionCollection* actionCollection(QWidget* parent = 0) const;

    virtual void       setup(QWidget* widget) = 0;
    virtual Category   category(KAction* action) const = 0;

protected:

    void addAction(KAction* const action);

private:

    class PluginPrivate;
    PluginPrivate* const d;
};

} // namespace KIPI

#endif  // KIPI_PLUGIN_H
