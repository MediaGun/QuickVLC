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

#pragma once

#include <QObject>

#include "instance.h"
#include "vlc.h"

struct libvlc_event_t;
struct libvlc_event_manager_t;
struct libvlc_media_t;

namespace Vlc {

class QUICKVLC_CORE_EXPORT Media : public QObject
{
    Q_OBJECT
public:
    explicit Media(const QString &location, bool localFile, Instance *instance);

    explicit Media(const QString &location, Instance *instance);

    Media(libvlc_media_t *media);

    ~Media();

    libvlc_media_t *core();

    bool parsed() const;

    void parse();

    QString currentLocation() const;

    qint64 duration() const;

signals:
    void metaChanged(const Enum::Meta &meta);
    void subitemAdded(libvlc_media_t *subitem);
    void durationChanged(int duration);
    void parsedChanged(bool status);
    void freed(libvlc_media_t *media);
    void playbackStateChanged(const Enum::PlaybackState &state);

private:
    void initMedia(const QString &location, bool localFile, Instance *instance);

    static void libvlc_callback(const libvlc_event_t *event, void *data);

    void createCoreConnections();
    void removeCoreConnections();

    libvlc_media_t *m_vlcMedia;
    libvlc_event_manager_t *m_vlcEvents;

    QString m_currentLocation;
};

}  // namespace Vlc
