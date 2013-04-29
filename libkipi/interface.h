/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  main kipi host application interface
 *
 * @author Copyright (C) 2004-2012 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2006-2012 by Marcel Wiesweg
 *         <a href="mailto:marcel dot wiesweg at gmx dot de">marcel dot wiesweg at gmx dot de</a>
 * @author Copyright (C) 2004-2005 by Renchi Raju
 *         <a href="mailto:renchi dot raju at gmail dot com">renchi dot raju at gmail dot com</a>
 * @author Copyright (C) 2004-2005 by Jesper K. Pedersen
 *         <a href="mailto:blackie at kde dot org">blackie at kde dot org</a>
 * @author Copyright (C) 2004-2005 by Aurelien Gateau
 *         <a href="mailto:aurelien dot gateau at free dot fr">aurelien dot gateau at free dot fr</a>
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

#ifndef KIPI_INTERFACE_H
#define KIPI_INTERFACE_H

// Qt includes

#include <QtCore/QFlags>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QList>

// KDE includes

#include <kurl.h>

// Local includes

#include "libkipi_export.h"

class QPixmap;
class QWidget;
class QAbstractItemModel;

class KFileItem;

/** @namespace KIPI */

namespace KIPI
{

class ImageCollection;
class ImageCollectionSelector;
class ImageInfo;
class ImageInfoShared;
class UploadWidget;

/*!
  @enum KIPI::Features
  Not all host applications support the full subset of features that KIPI
  allows access to. As an example <a href="http://www.digikam.org">digiKam</a> support comments
  for albums. Thus before a plugin expect a decant value for the comment,
  it should check whether KIPI::CollectionsHaveComments are set. It does so
  using KIPI::Interface::hasFeature().
*/
enum Features
{
    CollectionsHaveComments     = 1 << 0,  /** This feature specifies that albums have descriptions associated to them.                                                          */
    ImagesHasComments           = 1 << 1,  /** This feature specifies that images in the host application has descriptions associated to them.                                 */
    ImagesHasTime               = 1 << 2,  /** This feature specifies that images has a date associated with it, which the host application can display and set.               */
    HostSupportsDateRanges      = 1 << 3,  /** This feature specifies whether the host application supports that the user can specify a date range for images, like 1998-2000.   */
    HostAcceptNewImages         = 1 << 4,  /** This feature specifies that the host application do accept new images. Use \ref ImageCollection::uploadPath to find the
                                               location to place the image, and \ref KIPI::Interface::addImage() to tell the host application about the new image.             */
    ImagesHasTitlesWritable     = 1 << 5,  /** This feature specifies whether the plugin can change the title for images.                                                     */
    CollectionsHaveCategory     = 1 << 6,  /** This feature specifies that collections are category associated to them ('travels', 'friends', 'monuments', etc.).                */
    CollectionsHaveCreationDate = 1 << 7,  /** This feature specifies that collections are a creation date associated to them.                                                   */
    HostSupportsProgressBar     = 1 << 8,  /** This feature specifies whether the host application has a progress manager available to report progress information from plugins. */
    HostSupportsTags            = 1 << 9,  /** This feature specifies whether the host application supports keywords for images.                                                 */
    HostSupportsRating          = 1 << 10, /** This feature specifies whether the host application supports rating values for images.                                            */
    HostSupportsThumbnails      = 1 << 11, /** This feature specifies that host application can provide image thumbnails.                                                      */
    HostSupportsReadWriteLock   = 1 << 12, /** This feature specifies that host application has mechanism to lock/unlock items to prevent concurent operations.                */
    HostSupportsPickLabel       = 1 << 13, /** This feature specifies whether the host application supports pick label values for images, used for photograph workflow.          */
    HostSupportsColorLabel      = 1 << 14, /** This feature specifies whether the host application supports color label values for images, used to sort item with color flag.    */
    HostSupportsItemReservation = 1 << 15  /** This feature specifies whether the host application supports item reservation */
};

// NOTE: When a new item is add to Features, please don't forget to patch Interface::hasFeature().

enum EditHint
{
    UndefinedEditHint   = 0,
    /** The image data (pixels) have been edited */
    HintPixelContentChanged = 1 << 0,
    /** Metadata have been edited */
    HintMetadataChanged     = 1 << 1,
    /** All changes done (typically, to the metadata) are also communicated via image attributes.
     *  In other words, if the host updates its internal storage from attribute changes
     *  and ignores changes on disk, it already has all information. */
    HintChangeReflectedByAttributes
                            = 1 << 2,
    /** The operation indicated as "about to be done" has been aborted / did not result in a change. */
    HintEditAborted         = 1 << 3,

    /** Short name, implies that only metadata changed, and all metadata changes are communicated via attributes. */
    HintMetadataOnlyChange  = HintMetadataChanged | HintChangeReflectedByAttributes
};
Q_DECLARE_FLAGS(EditHints, EditHint)

// ---------------------------------------------------------------------------------------------------------------

class LIBKIPI_EXPORT FileReadWriteLock
{
public:

    /**
     *  A Kipi FileReadWriteLock refers to application-wide reading/writing
     *  to a file on disk; it is created with createReadWriteLock for a URL.
     *  All semantics are identical to a recursive QReadWriteLock.
     *  You must unlock as often as you locked.
     *
     *  Note: locking will incur a mutex wait if the file is not free.
     *  Therefore, calling the lock methods, especially lockForWrite,
     *  from the UI thread shall be done with care, or rather avoided.
     *
     *  Note that you must not keep a lock for a longer time, but only for the imminent
     *  low-level reading or writing on disk.
     *
     *  See reserveForAction() API for longer lasting reservation which
     *  do not incur waits.
     *
     *  It is strongly recommended to use the FileReadLocker or FileWriteLocker
     *  convenience locks instead of creating and locking a FileReadWriteLock directly.
     */

    virtual ~FileReadWriteLock() {}
    virtual void lockForRead() = 0;
    virtual void lockForWrite() = 0;
    virtual bool tryLockForRead() = 0;
    virtual bool tryLockForRead(int timeout) = 0;
    virtual bool tryLockForWrite() = 0;
    virtual bool tryLockForWrite(int timeout) = 0;
    virtual void unlock() = 0;
};

// ---------------------------------------------------------------------------------------------------------------

class LIBKIPI_EXPORT Interface : public QObject
{
    Q_OBJECT

public:

    explicit Interface(QObject* const parent, const char* name=0);
    virtual ~Interface();

    /**
     * Tells whether the host application under which the plugin currently executes a given feature.
     * See KIPI::Features for details on the individual features.
     */
    bool hasFeature(Features feature) const;

    /**
     * Returns list of all images in current album.
     * If there are no current album, the returned
     * KIPI::ImageCollection::isValid() will return false.
     */
    virtual ImageCollection currentAlbum() = 0;

    /**
     * Current selection in a thumbnail view for example.
     * If there are no current selection, the returned
     * KIPI::ImageCollection::isValid() will return false.
     */
    virtual ImageCollection currentSelection() = 0;

    /**
     * Returns a list of albums.
     */
    virtual QList<ImageCollection> allAlbums() = 0;

    /**
     * Returns the image info container for item pointed by url.
     */
    virtual ImageInfo info(const KUrl&) = 0;

    /**
     * Tells to host application that a new image has been made available to it.
     * Returns true if the host application did accept the new image, otherwise err will be filled with
     * an error description.
     */
    virtual bool addImage(const KUrl&, QString& err);

    /**
     * Tells to host application that a new image has been removed.
     */
    virtual void delImage(const KUrl&);

    /**
     * Tells to host application that the following images has changed on disk
     */
    virtual void refreshImages(const KUrl::List&);

    /**
     * Tells to host application to render a thumbnail for an image. If this method is not
     * re-implemented in host, standard KIO::filePreview is used to generated a thumbnail.
     * Use gotThumbnail() signal to take thumb.
     */
    virtual void thumbnail(const KUrl& url, int size);

    /**
     * Ask to Kipi host application to render thumbnails for a list of images. If this method is not
     * re-implemented in host, standard KIO::filePreview is used to generated a thumbnail.
     * Use gotThumbnail() signal to take thumbs.
     */
    virtual void thumbnails(const KUrl::List& list, int size);

    /**
      Ask to Kipi host application to prepare progress manager for a new entry. This method must return from host 
      a string identification about progress item created. This id will be used later to change in host progress item
      value and text. Title is text used to name progress item in host application.
      Set canBeCanceled to true if you want that progress item provide a cancel button to close process from kipi host.
      Use progressCanceled() signal to manage feedback from kipi host when cancel button is pressed.
      Set hasThumb to true if you want that progress item support small thumbnail near progress bar.
      Use progresssThumbnailChanged() to change thumbnail in kipi host and progressValueChanged() to advance progress
      bar in percent. Use progressStatusChanged() to change description string of progress item.
      To close progress item in kipi host, for example when all is done in plugin, use progressCompleted() method.
      If you Host do not re-implement this method, value returned is a null string.
      You must re-implement this method if your host support HostSupportsProgressBar feature.
    */
    virtual QString progressScheduled(const QString& title, bool canBeCanceled, bool hasThumb) const;

    /** To manage progress state from plugin to host application. id is identification string of process item
     *  returned from host by progressScheduled() method.
     */
    virtual void progressValueChanged(const QString& id, float percent);
    virtual void progressStatusChanged(const QString& id, const QString& status);
    virtual void progressThumbnailChanged(const QString& id, const QPixmap& thumb);
    virtual void progressCompleted(const QString& id);

    /*!
      \brief Tells to host application to return a setting to share with plugins.

      <table><tr>
            <th>SETTING NAME</th>
            <th>VALUE RETURNED</th>
            <th>COMMENTS</th>
        </tr><tr>
            <th>"WriteMetadataUpdateFiletimeStamp"</th>
            <td>bool value</td>
            <td>Returns true if file timestamp are updated when metadata are saved.</td>
        </tr><tr>
            <th>"WriteMetadataToRAW"</th>
            <td>bool value</td>
            <td>Returns true if RAW files metadata can be writted to image.</td>
        </tr><tr>
            <th>"UseXMPSidecar4Reading"</th>
            <td>bool value</td>
            <td>Returns true if XMP sidecar is used to read metadata from item.</td>
        </tr><tr>
            <th>"MetadataWritingMode"</th>
            <td>integer</td>
            <td>Returns mode to write metadata to item. See KExiv2::MetadataWritingMode for details.</td>
        </tr><tr>
            <th>"FileExtensions"</th>
            <td>QString</td>
            <td>Returns all file extensions (image, sound, video) managed by host application, separated by blank spaces, (ex: "JPG PNG TIF NEF AVI MP3").</td>
        </tr><tr>
            <th>"ImagesExtensions"</th>
            <td>QString</td>
            <td>Returns images file extensions managed by host application, not incuding RAW formats, separated by blank spaces, (ex: "JPG PNG TIF").</td>
        </tr><tr>
            <th>"RawExtensions"</th>
            <td>QString</td>
            <td>Returns RAW file extensions managed by host application, separated by blank spaces, (ex: "NEF CR2 ARW PEF").</td>
        </tr><tr>
            <th>"VideoExtensions"</th>
            <td>QString</td>
            <td>Returns video file extensions managed by host application, separated by blank spaces, (ex: "AVI MOV MPG").</td>
        </tr><tr>
            <th>"AudioExtensions"</th>
            <td>QString</td>
            <td>Return audio file extensions managed by host application, separated by blank spaces, (ex: "MP3 WAV OGG").</td>
      </tr></table>

      This method return the default settings. Re-implement this method in your dedicated kipi interface
      to control kipi-plugins rules with your kipi host application settings.

      NOTE: If you want to manage host settings from kipi-plugins, use wrapper class KIPIPlugins::KPHostSettings,
      not this method directly.
    */
    virtual QVariant hostSetting(const QString& settingName);

    virtual ImageCollectionSelector* imageCollectionSelector(QWidget* parent)=0;
    virtual UploadWidget*            uploadWidget(QWidget* parent)=0;
    virtual QAbstractItemModel*      getTagTree() const;

    /**
     * Supported if HostSupportsItemReservation
     *
     * If an item is scheduled in a plugin for an action which will edit the object,
     * call this method. If the user tries to subject the reserved item to another operation,
     * possibly conflicting, a warning message or other action may be taken.
     *
     * Give the URL of the item and a QObject which acts as the holder of the reservation.
     * The object must not be null, and the reservation will be cancelled when the object is deleted.
     * descriptionOfAction is a user-presentable string describing the action for which
     * the reservation was made.
     *
     * Returns true if a reservation was made, or false if a reservation could not be made.
     */
    virtual bool reserveForAction(const KUrl& url, QObject* const reservingObject,
                                  const QString& descriptionOfAction) const;
    /**
     * Supported if HostSupportsItemReservation
     *
     * Clears a reservation made previously with reserveForAction for the given reservingObject.
     * You must clear any reservation you made, or, alternatively, delete the reserving object.
     */
    virtual void clearReservation(const KUrl& url, QObject* const reservingObject);

    /**
     * Supported if HostSupportsItemReservation
     *
     * Returns if the item is reserved. You can pass a pointer to a QString; if the return value
     * is true, the string will be set to the descriptionOfAction set with reserveForAction.
     */
    virtual bool itemIsReserved(const KUrl& url, QString* const descriptionOfAction = 0) const;

    /**
     * Supported if HostSupportsReadWriteLock
     * Creates a ReadWriteLock for the given URL.
     * You must unlock the ReadWriteLock as often as you locked.
     * Deleting the object does not unlock it.
     * The implementation KIPI host application must be thread-safe.
     *
     */
    virtual FileReadWriteLock* createReadWriteLock(const KUrl& url) const;

    /**
     * Supported if HostSupportsEditHints
     *
     * Before an edit operation starts when it has finished, specify a hint for it.
     * Change hints are optional and may allow optimizations.
     *
     * When aboutToEdit has been called, editingFinished must be called afterwards.
     * It is strongly recommended to use the EditHintScope instead of these methods.
     */
    virtual void aboutToEdit(const KUrl& url, EditHints hints);
    virtual void editingFinished(const KUrl& url, EditHints hints);

    /**
     * Returns a string version of libkipi release ID.
     */
    static QString version();

Q_SIGNALS:

    /**
     * Emit when item selection has changed from host application user interface.
     * Boolean argument is true if items are select or not in collection.
     */
    void selectionChanged(bool hasSelection);

    /**
     * Emit when current album selection as changed from host application user interface. 
     * Boolean argument is true if album are select or not in collection.
     */
    void currentAlbumChanged(bool hasSelection);

    /** Emit when host application has redered item thumbnail. See thumbnail() and thumbnails() 
     * methods for details.
     */
    void gotThumbnail(const KUrl&, const QPixmap&);

    /**
     * This signal is emit from kipi host when a progress item is canceled. id is identification string of progress item.
     */
    void progressCanceled(const QString& id);

    /**
     * Supported if HostSupportsItemReservation
     *
     * Emitted from reservedForAction and clearReservation, respectively.
     * */
    void reservedForAction(const KUrl& url, const QString& descriptionOfAction);
    void reservationCleared(const KUrl& url);

protected:

    /**
     * Return a bitwise or of the KIPI::Features that thus application support.
     */
    virtual int features() const = 0;

private Q_SLOTS:

    void gotKDEPreview(const KFileItem& item, const QPixmap& pix);
    void failedKDEPreview(const KFileItem&);

private:

    bool hasFeature(const QString& feature) const;

private:

    friend class PluginLoader;
};

// ---------------------------------------------------------------------------------------------------------------

/**
 * Convenience classes creating a FileReadWriteLock and locking it for you.
 * It is strongly recommended to use FileReadWriteLock only through these
 * classes, created on the stack, as unlocking will be done automatically for you.
 *
 * The API is modelled according to the QReadLocker/QWriteLocker classes.
 *
 * Note that operations are no-ops and fileReadWriteLock() is 0 if not HostSupportsReadWriteLock.
 */
class LIBKIPI_EXPORT FileReadLocker
{
public:

    FileReadLocker(Interface* const iface, const KUrl& url);
    FileReadLocker(ImageInfoShared* const info);
    ~FileReadLocker();

    FileReadWriteLock* fileReadWriteLock() const;
    void unlock();
    void relock();

private:

    FileReadWriteLock* const d;
};

// ---------------------------------------------------------------------------------------------------------------

class LIBKIPI_EXPORT FileWriteLocker
{
public:

    FileWriteLocker(Interface* const iface, const KUrl& url);
    FileWriteLocker(ImageInfoShared* const info);
    ~FileWriteLocker();

    FileReadWriteLock* fileReadWriteLock() const;
    void unlock();
    void relock();

private:

    FileReadWriteLock* const d;
};

// ---------------------------------------------------------------------------------------------------------------

class LIBKIPI_EXPORT EditHintScope
{
public:

    EditHintScope(Interface* const iface, const KUrl& url, EditHints hints);
    ~EditHintScope();

    void changeAborted();

private:

    Interface* const iface;
    KUrl const       url;
    EditHints        hints;
};

}  // namespace KIPI

Q_DECLARE_OPERATORS_FOR_FLAGS(KIPI::EditHints)

#endif /* KIPI_INTERFACE_H */
