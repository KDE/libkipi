/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2009-11-13
 * Description : export plugin
 *
 * Copyright (C) 2004-2009 by Aleix Pol Gonzalez <aleixpol@kde.org>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef EXPORTINTERFACE_H
#define EXPORTINTERFACE_H

#include <QtPlugin>
#include <KUrl>

// Local includes.

#include "libkipi_export.h"
#include "extension.h"


class KJob;

namespace KIPI
{

class LIBKIPI_EXPORT ExportInterface
{
public:
    virtual ~ExportInterface();
    
    virtual KJob* exportFiles(const QString& albumname)=0;

};

}

KIPI_DECLARE_EXTENSION_INTERFACE_NS(KIPI, ExportInterface, "org.kde.kipi.ExportPlugin")
Q_DECLARE_INTERFACE( KIPI::ExportInterface, "org.kde.kipi.ExportPlugin" )

#endif
