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

#include "mainwindow.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

static const char description[] =
    I18N_NOOP("A demo of libkipi");

static const char version[] = "0.1";

static KCmdLineOptions options[] =
{
    KCmdLineLastOption
};

int main(int argc, char **argv)
{
    KAboutData about("kipidemo", I18N_NOOP("KIPIDemo"), version, description,
                     KAboutData::License_GPL, "(C) 2004 Jesper K. Pedersen", 0, 0, "blackie@kde.org");
    about.addAuthor( "Jesper K. Pedersen", 0, "blackie@kde.org" );
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions( options );
    KApplication app;

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    MainWindow* mainWin = new MainWindow();
    app.setMainWidget( mainWin );
    mainWin->show();

    args->clear();

    // mainWin has WDestructiveClose flag by default, so it will delete itself.
    return app.exec();
}
