/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 *
 *
 * @date   2012-08-06
 * @brief  Plugins config widget.
 *
 * @author Copyright (C) 2004-2018 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2012      by Victor Dodon
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

#ifndef KIPI_CONFIGWIDGET_H
#define KIPI_CONFIGWIDGET_H

// Qt includes

#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QTreeWidget>

// Local includes

#include "plugin.h"
#include "pluginloader.h"
#include "libkipi_export.h"

namespace KIPI
{

class LIBKIPI_EXPORT ConfigWidget : public QTreeWidget
{
    Q_OBJECT

public:

    /** Default constructor.
     */
    ConfigWidget(QWidget* const parent=0);
    ~ConfigWidget();

    /** Apply all changes about plugins selected to be hosted in KIPI host application.
     */
    void apply();

    /** Return the number of plugins actived in the list.
     */
    int actived() const;

    /** Return the total number of plugins in the list.
     */
    int count()   const;

    /** Return the number of visible plugins in the list.
     */
    int visible() const;

    /** Select all plugins in the list.
     */
    void selectAll() Q_DECL_OVERRIDE;

    /** Clear all selected plugins in the list.
     */
    void clearAll();

    /** Set the string used to filter the plugins list. signalSearchResult() is emitted when all is done.
     */
    void setFilter(const QString& filter, Qt::CaseSensitivity cs);

    /** Return the current string used to filter the plugins list.
     */
    QString filter() const;

Q_SIGNALS:

    /** Signal emitted when filtering is done through slotSetFilter().
     *  Number of plugins found is sent when item relevant of filtering match the query.
     */
    void signalSearchResult(int);

private:

    class Private;
    Private* const d;
};

} // namespace KIPI


#endif /* KIPI_CONFIGWIDGET_H */
