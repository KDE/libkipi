/**
   \mainpage libkipi reference page.

   This page will tell you both how to develop a new
   plugin for KIPI, and how to add KIPI support to a new
   application (referred to as host application).
   
   You can find more information about KIPI at this <a
   href="http://extragear.kde.org/apps/kipi">url</a>.
   
   This documentation has been writen by Renchi Raju, Gilles Caulier,
    Jesper K. Pedersen, and Aurelien Gateau. 

   <h1>Developing a new Plugin</h1>
   The easiest way to start a new plugin, is to copy the hello world
   plugin, which you may find in kdeextragear-libs-1/kipi-plugins. Please
   also read <a
   href="http://developer.kde.org/documentation/tutorials/developing-a-plugin-structure/index.html">
   this tutorial</a> which tells in general about developing a plugin
   structure for an application, but it also gives some background
   information on how plugins are loaded, and especially on the macros
   involved. The tutorial has been written by one of the KIPI authors, so
   it matches fairly good the model of KIPI (except the part on merging
   GUI's from plugins).




   <h2>General things to do to implement a plugin</h2>
   Here is a list of things to do.
   <ul>
   <li> Create a class that inherits KIPI::Plugin. In the constructor you
   must create instances of KAction for your application. The topmost
   actions should be given to the Plugin super class using \ref
   KIPI::Plugin::addAction().
   <li> As described in the tutorial <a href="http://developer.kde.org/documentation/tutorials/developing-a-plugin-structure/index.html#understanding_loading">here</a>, you must call the macro
   <tt>K_EXPORT_COMPONENT_FACTORY</tt>.
   <li> You also need to create a desktop file, as described <a href="http://developer.kde.org/documentation/tutorials/developing-a-plugin-structure/index.html#makefile_am_client">here</a>
   </ul>





   <h2>Retrieving information from the host application</h2>
   The above was the required step to build the plugin. The following will
   tell you about the functions with which you communicate with the host
   application.

   The origin of all your communication with the host application is
   \ref KIPI::Interface. You obtain a pointer to this by casting the parent
   pointer given to your plugin during construction.

   Selection of images can be obtained using
   KIPI::Interface::currentSelection(), KIPI::Interface::currentAlbum() and
   KIPI::Interface::allAlbums(). These methods uses KIPI::ImageCollection
   to wrap a selection of images.

   Finally to obtain information about individual images, call
   KIPI::Interface::info(), which returns a KIPI::ImageInfo, with the
   information.

   When developing plugins, you should pay special attention to the fact the different
   host applications may have different feature sets they
   support. Information about what the host application support can be
   obtained from KIPI::Interface::hasFeature().




   <h2>Utilities for Plugins</h2>
   KIPI has a number of utilities useful when developing plugins:
   <ul>
   <li> use KIPI::ImageCollectionDialog to ask the user for an image.
   <li> use KIPI::UploadWidget when the user should specify a directory to
   write images into.
   <li> KIPI::ThumbnailJob helps you load thumb nails of images.
   </ul>




   <h1>Adding KIPI support to a new host application.</h1>
   To add KIPI support to your application you must inherit
   KIPI::Interface, and give an instance of this class to
   KIPI::PluginLoader. See \ref KIPI::PluginLoader for details on loading plugins.

   To be able to create an instance of your subclass of KIPI::Interface,
   you must of course implement all the pure virtual method of the super
   class, among others this requires you to create and return instances of
   KIPI::ImageCollection and KIPI::ImageInfo. The only way you can do so
   are by giving the classes a pointer to a subclass of
   KIPI::ImageCollectionShared and KIPI::ImageInfoShared. These sub classes
   are also your responsibility to implement.

   The reason why things are that complicated, are to solve the problem of
   ownership of pointers to the classes in question. The
   KIPI::ImageCollectionShared pointer you give to KIPI::ImageCollection
   constructor will be the owned by the ImageCollection instance.
*/
