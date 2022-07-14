/******************************************************************************
 * This file is part of QuickVLC - Qt and libvlc connection library.
 * Copyright (C) 2016 Tadej Novak <tadej@tano.si>,
 *               2022 Alexey Avramchik (OU Bamboo group) <aa@bam-boo.eu>
 *
 * QuickVLC is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * QuickVLC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QuickVLC. If not, see <https://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "media.h"

#include <vlc/vlc.h>

#include <QDir>

#include "error.h"

namespace Vlc {

Media::Media(const QString &location, bool localFile, Instance *instance) : QObject(instance)
{
    initMedia(location, localFile, instance);
}

Media::Media(const QString &location, Instance *instance)
{
    initMedia(location, false, instance);
}

Media::Media(libvlc_media_t *media)
{
    m_vlcMedia = libvlc_media_duplicate(media);
    m_vlcEvents = libvlc_media_event_manager(m_vlcMedia);

    createCoreConnections();

    Error::printErrorMsg();
}

Media::~Media()
{
    removeCoreConnections();

    libvlc_media_release(m_vlcMedia);

    Error::printErrorMsg();
}

libvlc_media_t *Media::core()
{
    return m_vlcMedia;
}

QString Media::currentLocation() const
{
    return m_currentLocation;
}

qint64 Media::duration() const
{
    return libvlc_media_get_duration(m_vlcMedia);
}

void Media::initMedia(const QString &location, bool localFile, Instance *instance)
{
    Q_UNUSED(instance)

    m_currentLocation = location;

    QString loc = location;

    if (localFile) {
        loc = QDir::toNativeSeparators(loc);

        m_vlcMedia = libvlc_media_new_path(loc.toUtf8().data());
    } else {
        m_vlcMedia = libvlc_media_new_location(loc.toUtf8().data());
    }

    m_vlcEvents = libvlc_media_event_manager(m_vlcMedia);

    createCoreConnections();

    Error::printErrorMsg();
}

void Media::libvlc_callback(const libvlc_event_t *event, void *data)
{
    auto *core = static_cast<Media *>(data);

    switch (event->type) {
    case libvlc_MediaMetaChanged:
        emit core->metaChanged(Enum::Meta(event->u.media_meta_changed.meta_type));
        break;
    case libvlc_MediaSubItemAdded:
        emit core->subitemAdded(event->u.media_subitem_added.new_child);
        break;
    case libvlc_MediaDurationChanged:
        emit core->durationChanged(event->u.media_duration_changed.new_duration);
        break;
    case libvlc_MediaParsedChanged:
        emit core->parsedChanged(event->u.media_parsed_changed.new_status);
        emit core->parsedChanged((bool)event->u.media_parsed_changed.new_status);
        break;
    default:
        break;
    }
}

void Media::createCoreConnections()
{
    QList<libvlc_event_e> list;

    list << libvlc_MediaMetaChanged << libvlc_MediaSubItemAdded << libvlc_MediaDurationChanged
         << libvlc_MediaParsedChanged;

    for (auto &event : list) {
        libvlc_event_attach(m_vlcEvents, event, libvlc_callback, this);
    }
}

void Media::removeCoreConnections()
{
    QList<libvlc_event_e> list;

    list << libvlc_MediaMetaChanged << libvlc_MediaSubItemAdded << libvlc_MediaDurationChanged
         << libvlc_MediaParsedChanged;

    for (auto &event : list) {
        libvlc_event_detach(m_vlcEvents, event, libvlc_callback, this);
    }
}

}  // namespace Vlc
