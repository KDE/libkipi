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
#include <kinstance.h>

KIPI::Plugin::Plugin( KInstance* instance, QObject *parent, const char* name)
    : QObject( parent, name), m_actionCollection( 0 ), m_instance( instance )
{
}

KIPI::Plugin::~Plugin()
{
}

KActionCollection* KIPI::Plugin::actionCollection()
{
    if (!m_actionCollection)
        m_actionCollection = new KActionCollection(this, "m_actions", m_instance );

    return m_actionCollection;
}

void KIPI::Plugin::addAction( KAction* action )
{
    m_actions.append( action );
}

KActionPtrList KIPI::Plugin::actions()
{
    return m_actions;
}

