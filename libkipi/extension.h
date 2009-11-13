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

#ifndef _EXTENSION_H
#define _EXTENSION_H

#include <QtDesigner/QAbstractExtensionFactory>
#define KIPI_DECLARE_EXTENSION_INTERFACE_NS(Namespace, IFace, IId) \
namespace Namespace { \
const char * const IFace##_iid = IId; \
} \
template <> inline Namespace::IFace *qt_extension<Namespace::IFace *>( \
QAbstractExtensionManager *manager, QObject *object) \
{ QObject *extension = manager->extension(object, Q_TYPEID(Namespace::IFace)); \
return extension ? static_cast<Namespace::IFace *>(extension->qt_metacast( \
Namespace::IFace##_iid)) : static_cast<Namespace::IFace *>(0); }

#endif
