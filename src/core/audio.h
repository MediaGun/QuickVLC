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

#include "core_shared_export.h"

struct libvlc_media_player_t;
struct libvlc_event_manager_t;
struct libvlc_event_t;

namespace Vlc {

class MediaPlayer;

class QUICKVLC_CORE_EXPORT Audio : public QObject
{
    Q_OBJECT

public:
    explicit Audio(MediaPlayer *player);
    ~Audio();

    bool muted() const;
    void setMuted(bool isMuted);

    int volume() const;
    void setVolume(int volume);

signals:
    void muteChanged(bool isMuted);
    void volumeChanged(int volume);

private:
    static void libvlc_callback(const libvlc_event_t *event, void *data);

    void createCoreConnections();
    void removeCoreConnections();

    libvlc_media_player_t *m_mediaPlayer;
    libvlc_event_manager_t *m_eventManager;
};

}  // namespace Vlc
