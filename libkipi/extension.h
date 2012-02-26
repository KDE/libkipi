/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date  2009-11-13
 * @brief export plugin
 *
 * @author Copyright (C) 2009 by Aleix Pol Gonzalez
 *         <a href="mailto:aleixpol at kde dot org">aleixpol at kde dot org</a>
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

#ifndef _EXTENSION_H
#define _EXTENSION_H

// Qt includes

#include <QtDesigner/QAbstractExtensionFactory>

#define KIPI_DECLARE_EXTENSION_INTERFACE_NS(Namespace, IFace, IId)                                                                    \
                                                                                                                                      \
namespace Namespace                                                                                                                   \
{                                                                                                                                     \
    const char* const IFace##_iid = IId;                                                                                              \
} /* namespace Namespace */                                                                                                           \
                                                                                                                                      \
template <> inline Namespace::IFace* qt_extension<Namespace::IFace*>(QAbstractExtensionManager* const manager, QObject* const object) \
{                                                                                                                                     \
    QObject* extension = manager->extension(object, Q_TYPEID(Namespace::IFace));                                                      \
    return extension ? static_cast<Namespace::IFace*>(extension->qt_metacast(Namespace::IFace##_iid))                                 \
                     : static_cast<Namespace::IFace*>(0);                                                                             \
}                                                                                                                                     \

#endif // _EXTENSION_H
