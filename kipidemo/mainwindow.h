/***************************************************************************
 *   Copyright (C) 2004 by Jesper K. Pedersen                              *
 *   blackie@kde.org                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <kmainwindow.h>
class QTextEdit;
class MyPluginInterface;

class MainWindow : public KMainWindow
{
    Q_OBJECT
public:
    MainWindow();

protected:
    void loadPlugins();

private:
    QTextEdit* _editor;
    QObject* _pluginInterface;
};

#endif // _MAINWINDOW_H_
