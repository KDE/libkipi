/* ============================================================
 * File  : plugin.cpp
 * Author: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Date  : 2004-02-18
 * Description :
 *
 * Copyright 2004 by Renchi Raju

 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published bythe Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include <kaction.h>

#include "plugin.h"

namespace KIPI
{

Plugin::Plugin( Interface* interface, QObject *parent, const char* name)
    : QObject(parent, name), m_interface( interface )
{
    m_actions = 0;
}

Plugin::~Plugin()
{
}

KActionCollection* Plugin::actionCollection()
{
    if (!m_actions)
        m_actions = new KActionCollection(this);

    return m_actions;
}

bool Plugin::mergeContextMenu() const
{
    return false;
}

bool Plugin::mergeToolBar() const
{
    return false;
}

}

