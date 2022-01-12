/*
    SPDX-FileCopyrightText: 2004-2018 Gilles Caulier <caulier dot gilles at gmail dot com>
    SPDX-FileCopyrightText: 2012 Victor Dodon <dodonvictor at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KIPI_CONFIGWIDGET_H
#define KIPI_CONFIGWIDGET_H

// Std includes

#include <memory>

// Qt includes

#include <QTreeWidgetItem>
#include <QTreeWidget>

// Local includes

#include "plugin.h"
#include "pluginloader.h"
#include "libkipi_export.h"

namespace KIPI
{

/**
 * @class ConfigWidget configwidget.h <KIPI/ConfigWidget>
 *
 * The ConfigWidget class.
 */
class LIBKIPI_EXPORT ConfigWidget : public QTreeWidget
{
    Q_OBJECT

public:

    /** Default constructor.
     */
    ConfigWidget(QWidget* const parent = nullptr);
    ~ConfigWidget() override;

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
    void selectAll() override;

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

    /** Signal emitted when filtering is done through setFilter().
     *  Number of plugins found is sent when item relevant of filtering match the query.
     */
    void signalSearchResult(int);

private:

    class Private;
    std::unique_ptr<Private> const d;
};

} // namespace KIPI


#endif /* KIPI_CONFIGWIDGET_H */
