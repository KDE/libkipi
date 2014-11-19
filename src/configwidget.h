/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2012-08-06
 * @brief  plugin config widget
 *
 * @author Copyright (C) 2004-2014 by Gilles Caulier
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

#include <QtGui/QScrollArea>

// Local includes

#include "libkipi_export.h"

namespace KIPI
{

class LIBKIPI_EXPORT ConfigWidget : public QScrollArea
{
    Q_OBJECT

public:

    /** Standard constructor.
     */
    ConfigWidget(QWidget* const parent = 0);
    ~ConfigWidget();

    /** Apply all changes about plugin selected to be hosted in KIPI host application.
     */
    void apply();

    /** Set a filter widget (as KLineEdit) to be able to list only wanted plugin using a string query.
     *  Use slotSetFilter() to apply filetring. signalSearchResult is emitted when it's done.
     */
    void setFilterWidget(QWidget* const wdg);

Q_SIGNALS:

    /** Signal emitted when filetring is done through slotSetFilter(). True is sent when item relevant of filetring query give items in list.
     */
    void signalSearchResult(bool);

public Q_SLOTS:

    /** Set plugins list filtetring string properties. signalSearchResult() is emitted when all is done.
     */
    void slotSetFilter(const QString& filter, Qt::CaseSensitivity cs);

private Q_SLOTS:

    /** For internal uses only.
     */
    void slotCheckAll();
    void slotClearList();
    void slotItemClicked();
    void slotProcessUrl(const QString&);

private:

    class Private;
    Private* const d;
};

} // namespace KIPI

#endif /* KIPI_CONFIGWIDGET_H */
