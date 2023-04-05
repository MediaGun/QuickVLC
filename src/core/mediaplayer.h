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

#include <vlc/vlc.h>

#include <QObject>
#include <QTimer>

#include "core_shared_export.h"
#include "vlc.h"

struct libvlc_event_t;
struct libvlc_event_manager_t;
struct libvlc_media_t;
struct libvlc_media_player_t;
struct libvlc_media_player_time_point_t;

namespace Vlc {

class Instance;
class Media;

class QUICKVLC_CORE_EXPORT MediaPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MediaPlayer(Instance *instance);

    ~MediaPlayer();

    libvlc_media_player_t *core() const;
    libvlc_event_manager_t *eventManager() const;

    int length() const;

    libvlc_media_t *currentMediaCore();

    void open(Media *media);

    void openOnly(Media *media);

    int time() const;

    bool seekable() const;

    float position() const;

    float rate() const;

    float sampleAspectRatio() const;

    float playbackRate() const;

    Enum::PlaybackState playbackState() const;

    QSize resolution() const;

public slots:
    void setTime(int time);

    void setPosition(float pos);

    void setPlaybackRate(float rate);

    void play();

    void pause();

    void togglePause();

    void resume();

    void stop();

signals:
    void backward();
    void buffering(float buffer);
    void buffering(int buffer);
    void forward();
    void lengthChanged(int length);
    void mediaChanged(libvlc_media_t *media);
    void pausableChanged(bool pausable);
    void positionChanged(float position);
    void seekableChanged(bool seekable);
    void timeChanged(int time);
    void rateChanged(float rate);

    // player states
    void nothingSpecial();
    void opening();
    void paused();
    void playing();
    void error();
    void stopping();
    void stopped();
    void playbackStateChanged();

private:
    static void libvlc_callback(const libvlc_event_t *event, void *data);

    static void onTimeUpdate(const libvlc_media_player_time_point_t *value, void *data);
    static void onTimeDiscontinuity(int64_t system_date_us, void *data);

    void updateTime(std::chrono::microseconds system_now, bool forceUpdate);
    bool interpolateTime(std::chrono::microseconds system_now);
    void updatePositionFromTimer();
    void updateTimeFromTimer();

    void createCoreConnections();
    void removeCoreConnections();

    libvlc_media_player_t *m_vlcMediaPlayer = nullptr;
    libvlc_event_manager_t *m_vlcEvents = nullptr;

    Media *m_media = nullptr;

    Enum::PlaybackState m_playerState = Enum::PlaybackState::Idle;

    QTimer m_positionTimer;
    QTimer m_timeTimer;
    libvlc_media_player_time_point_t m_playerTime;
    std::chrono::microseconds m_time;
    std::chrono::microseconds m_length;
    double m_position = 0.0;
    float m_rate = 0.0;
};

}  // namespace Vlc
