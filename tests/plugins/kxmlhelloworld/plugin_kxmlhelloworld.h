/*
    SPDX-FileCopyrightText: 2012-2018 Gilles Caulier <caulier dot gilles at gmail dot com>
    SPDX-FileCopyrightText: 2012 Victor Dodon <dodonvictor at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

/** Standard C++ header wrapper
 */
#ifndef PLUGIN_KXMLHELLOWORLD_H
#define PLUGIN_KXMLHELLOWORLD_H

// NOTE: Always limit include in headers file to speed-up compilation.

// Libkipi includes

#include "plugin.h"

/** To make source code more readable, we will declare KIPI namespace as well.
 */
using namespace KIPI;

/** Each plugin must be fully wrapped into a dedicated C++ namespace to prevent conflicts with symbols and identifiers.
 */
namespace KIPIKXMLHelloWorldPlugin
{

/** We will use KIPI::Plugin class as parent from libkipi
 *  See this API for details : http://api.kde.org/4.x-api/kdegraphics-apidocs/libs/libkipi/libkipi/html/index.html
 */
class Plugin_KXMLHelloWorld : public Plugin
{
    Q_OBJECT

public:

    /** Notice the constructor
        takes two arguments QObject* const parent (the parent of this object),
        and const QStringList& args (the arguments passed).
    */
    Plugin_KXMLHelloWorld(QObject* const parent, const QVariantList& args);
    ~Plugin_KXMLHelloWorld() override;

    /** This method setup the plugin actions and connect internal signals and slots to handle plugin actions.
     */
    void setup(QWidget* const widget) override;

private Q_SLOTS:

    /** There are slots to handle action events.
     */
    void slotActivateActionImages();
    void slotActivateActionTools();
    void slotActivateActionExport();
    void slotActivateActionImport();

private:

    /** Create and setup plugin actions
      */
    void setupActions();

private:

    /** We use d private internal container everywhere to speed up compilation. See implementation for details.
     */
    class Private;
    Private* const d;
};

}  // namespace KIPIKXMLHelloWorldPlugin

#endif // PLUGIN_KXMLHELLOWORLD_H
