/* ============================================================
 * File  : thumbnailjob.h
 * Author: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Date  : 2003-10-14
 * Description :
 *
 * Copyright 2003 by Renchi Raju

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

#ifndef THUMBNAILJOB_H
#define THUMBNAILJOB_H

#include <kio/job.h>
#include <kurl.h>
#include <qcstring.h>

class QPixmap;
class QImage;

namespace KIPI
{

class ThumbnailJobPriv;

class ThumbnailJob : public KIO::Job
{
    Q_OBJECT

public:

    ThumbnailJob(const KURL& url, int size, bool dir=false, bool highlight=true);
    ThumbnailJob(const KURL::List& urlList, int size, bool highlight=true);
    ~ThumbnailJob();

    void addItem(const  KURL& url);
    void addItems(const KURL::List& urlList);

signals:

    void signalThumbnail(const KURL& url, const QPixmap& pix);
    void signalCompleted();
    void signalFailed(const KURL& url);

private:

    ThumbnailJobPriv *d;

private:

    void createThumbnailDirs();
    void processNext();
    bool statThumbnail();
    void createThumbnail();
    void createFolderThumbnail();
    void emitThumbnail(QImage& thumb);
    void highlight(QImage& thumb);
    void createShmSeg();

protected slots:

    void slotResult(KIO::Job *job);
    void slotThumbData(KIO::Job *job, const QByteArray &data);
};

}

#endif /* THUMBNAILJOB_H */
