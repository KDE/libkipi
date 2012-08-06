/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2012-08-06
 * @brief  plugin config widget
 *
 * @author Copyright (C) 2004-2012 by Gilles Caulier
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

    ConfigWidget(QWidget* const parent = 0);
    ~ConfigWidget();

    void apply();

private Q_SLOTS:

    void slotCheckAll();
    void slotClearList();

private:

    class Private;
    Private* const d;
};


} // namespace KIPI

#endif /* KIPI_CONFIGWIDGET_H */
