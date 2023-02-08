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

#include "mediaplayer.h"
#include <chrono>
#include <vlc/vlc.h>

#include "error.h"
#include "instance.h"
#include "media.h"

namespace Vlc {

using sec = std::chrono::seconds;
using msec = std::chrono::milliseconds;
using usec = std::chrono::microseconds;
using std::chrono::duration_cast;


template<typename To, typename From>
int64_t timeAs(const From &from)
{
    return duration_cast<To>(from).count();
}

MediaPlayer::MediaPlayer(Instance *instance) : QObject { instance }
{
    m_vlcMediaPlayer = libvlc_media_player_new(instance->core());
    m_vlcEvents = libvlc_media_player_event_manager(m_vlcMediaPlayer);

    libvlc_video_set_key_input(m_vlcMediaPlayer, false);
    libvlc_video_set_mouse_input(m_vlcMediaPlayer, false);

    Error::printErrorMsg();

    m_media = nullptr;

    createCoreConnections();


    m_timeTimer.setSingleShot(true);
    m_timeTimer.setTimerType(Qt::PreciseTimer);
    connect(&m_positionTimer, &QTimer::timeout, this, &MediaPlayer::updatePositionFromTimer);
    connect(&m_timeTimer, &QTimer::timeout, this, &MediaPlayer::updateTimeFromTimer);


    Error::printErrorMsg();
}

MediaPlayer::~MediaPlayer()
{
    removeCoreConnections();


    libvlc_media_player_release(m_vlcMediaPlayer);

    Error::printErrorMsg();
}

libvlc_media_player_t *MediaPlayer::core() const
{
    return m_vlcMediaPlayer;
}

libvlc_event_manager_t *MediaPlayer::eventManager() const
{
    return m_vlcEvents;
}

int MediaPlayer::length() const
{
    return timeAs<msec>(m_length);
}

libvlc_media_t *MediaPlayer::currentMediaCore()
{
    auto *media = libvlc_media_player_get_media(m_vlcMediaPlayer);

    Error::printErrorMsg();

    return media;
}

void MediaPlayer::open(Media *media)
{
    openOnly(media);

    play();
}

void MediaPlayer::openOnly(Media *media)
{
    m_media = media;

    libvlc_media_player_set_media(m_vlcMediaPlayer, media->core());

    Error::printErrorMsg();
}

int MediaPlayer::time() const
{
    return timeAs<msec>(m_time);
}

float MediaPlayer::rate() const
{
    return m_rate;
}

bool MediaPlayer::seekable() const
{
    if (!libvlc_media_player_get_media(m_vlcMediaPlayer)) {
        return false;
    }

    bool seekable = libvlc_media_player_is_seekable(m_vlcMediaPlayer);

    Error::printErrorMsg();

    return seekable;
}

float MediaPlayer::position() const
{
    if (!m_vlcMediaPlayer) {
        return -1;
    }

    return m_position;
}

float MediaPlayer::sampleAspectRatio() const
{
    if (!m_vlcMediaPlayer) {
        return 0.0;
    }

    float sar = 0.0;

    return sar;
}

Enum::PlaybackState MediaPlayer::playbackState() const
{
    if (!libvlc_media_player_get_media(m_vlcMediaPlayer)) {
        return Enum::Idle;
    }

    auto state = libvlc_media_player_get_state(m_vlcMediaPlayer);

    Error::printErrorMsg();

    return Enum::PlaybackState(state);
}

void MediaPlayer::setTime(int time)
{
    if (!(playbackState() == Enum::Buffering || playbackState() == Enum::Playing || playbackState() == Enum::Paused)) {
        return;
    }

    bool changed = libvlc_media_player_set_time(m_vlcMediaPlayer, time, false);

    if (playbackState() == Enum::Paused && changed == 0) {
        emit timeChanged(time);
    }

    Error::printErrorMsg();
}

void MediaPlayer::setPosition(float pos)
{
    libvlc_media_player_set_position(m_vlcMediaPlayer, pos, false);

    Error::printErrorMsg();
}

void MediaPlayer::setPlaybackRate(float rate)
{
    libvlc_media_player_set_rate(m_vlcMediaPlayer, rate);

    Error::printErrorMsg();
}

void MediaPlayer::play()
{
    if (!m_vlcMediaPlayer) {
        return;
    }

    //#if defined(Q_OS_WIN32)
    //    libvlc_media_player_set_hwnd(_vlcMediaPlayer, (void *)_currentWId);
    //#elif defined(Q_OS_DARWIN)
    //    libvlc_media_player_set_nsobject(_vlcMediaPlayer, (void *)_currentWId);
    //#elif defined(Q_OS_UNIX)
    //    libvlc_media_player_set_xwindow(m_vlcMediaPlayer, 0);
    //#endif

    libvlc_media_player_play(m_vlcMediaPlayer);

    Error::printErrorMsg();
}

void MediaPlayer::pause()
{
    if (!m_vlcMediaPlayer) {
        return;
    }

    if (libvlc_media_player_can_pause(m_vlcMediaPlayer)) {
        libvlc_media_player_set_pause(m_vlcMediaPlayer, true);
    }

    Error::printErrorMsg();
}

void MediaPlayer::togglePause()
{
    if (!m_vlcMediaPlayer) {
        return;
    }

    if (libvlc_media_player_can_pause(m_vlcMediaPlayer)) {
        libvlc_media_player_pause(m_vlcMediaPlayer);
    }

    Error::printErrorMsg();
}

void MediaPlayer::resume()
{
    if (!m_vlcMediaPlayer) {
        return;
    }

    if (libvlc_media_player_can_pause(m_vlcMediaPlayer)) {
        libvlc_media_player_set_pause(m_vlcMediaPlayer, false);
    }

    Error::printErrorMsg();
}

void MediaPlayer::stop()
{
    if (!m_vlcMediaPlayer) {
        return;
    }

    libvlc_media_player_stop_async(m_vlcMediaPlayer);

    Error::printErrorMsg();
}

void MediaPlayer::libvlc_callback(const libvlc_event_t *event, void *data)
{
    auto *core = static_cast<MediaPlayer *>(data);

    switch (event->type) {
    case libvlc_MediaPlayerMediaChanged:
        emit core->mediaChanged(event->u.media_player_media_changed.new_media);
        break;
    case libvlc_MediaPlayerNothingSpecial:
        emit core->nothingSpecial();
        break;
    case libvlc_MediaPlayerOpening:
        emit core->opening();
        break;
    case libvlc_MediaPlayerBuffering:
        emit core->buffering(event->u.media_player_buffering.new_cache);
        emit core->buffering(qRound(event->u.media_player_buffering.new_cache));
        break;
    case libvlc_MediaPlayerPlaying:
        emit core->playing();
        break;
    case libvlc_MediaPlayerPaused:
        emit core->paused();
        break;
    case libvlc_MediaPlayerStopped:
        emit core->stopped();
        break;
    case libvlc_MediaPlayerForward:
        emit core->forward();
        break;
    case libvlc_MediaPlayerBackward:
        emit core->backward();
        break;
        //    case libvlc_MediaPlayerEndReached:
        //        emit core->end();
        //        break;
    case libvlc_MediaPlayerEncounteredError:
        emit core->error();
        break;
    case libvlc_MediaPlayerSeekableChanged:
        emit core->seekableChanged(event->u.media_player_seekable_changed.new_seekable);
        break;
    case libvlc_MediaPlayerPausableChanged:
        emit core->pausableChanged(event->u.media_player_pausable_changed.new_pausable);
        break;
        //    case libvlc_MediaPlayerTitleChanged:
        //        emit core->titleChanged(event->u.media_player_title_changed.new_title);
        //        break;
        //    case libvlc_MediaPlayerSnapshotTaken:
        //        emit core->snapshotTaken(event->u.media_player_snapshot_taken.psz_filename);
        //        break;
        //    case libvlc_MediaPlayerVout:
        //        emit core->vout(event->u.media_player_vout.new_count);
        //        break;
    default:
        break;
    }

    if (event->type >= libvlc_MediaPlayerNothingSpecial && event->type <= libvlc_MediaPlayerEncounteredError) {
        emit core->playbackStateChanged();
    }
}

void MediaPlayer::updateTime(usec system_now, bool forceUpdate)
{
    // Update time properties
    emit timeChanged(timeAs<usec>(m_time));

    if (m_playerTime.system_date_us != INT64_MAX && (forceUpdate || !m_timeTimer.isActive())) {
        // Tell the timer to wait until the next second is reached.
        usec next_update_date = usec(libvlc_media_player_time_point_get_next_date(
            &m_playerTime, timeAs<usec>(system_now), timeAs<usec>(m_time), timeAs<usec>(sec(1))));

        usec nextUpdateInterval = next_update_date - system_now;

        if (nextUpdateInterval > usec(0)) {
            // The timer can be triggered a little before. In that case, it's
            // likely that we didn't reach the next next second. It's better to
            // add a very small delay in order to be triggered after the next
            // seconds.
            static msec imprecisionDelay = msec(30);
            m_timeTimer.start(duration_cast<msec>(nextUpdateInterval + imprecisionDelay));
        }
    }
}

bool MediaPlayer::interpolateTime(usec system_now)
{
    int64_t newTime;
    if (libvlc_media_player_time_point_interpolate(&m_playerTime, timeAs<usec>(system_now), &newTime, &m_position) == 0) {
        m_time = usec(newTime);
        return true;
    }
    return false;
}


void MediaPlayer::onTimeUpdate(const libvlc_media_player_time_point_t *value, void *data)
{
    auto *core = static_cast<MediaPlayer *>(data);
    QMetaObject::invokeMethod(core, [core, timePoint = *value]() {
        core->m_playerTime = timePoint;
        bool lengthOrRateChanged = false;

        if (core->m_rate != timePoint.rate) {
            core->m_rate = timePoint.rate;
            lengthOrRateChanged = true;
            emit core->rateChanged(core->m_rate);
        }

        if (core->m_length != usec(timePoint.length_us)) {
            core->m_length = usec(timePoint.length_us);
            lengthOrRateChanged = true;
            emit core->lengthChanged(timeAs<usec>(core->m_length));
        }

        usec now = usec(libvlc_clock());

        if (core->interpolateTime(now)) {
            if (lengthOrRateChanged || !core->m_positionTimer.isActive()) {
                emit core->positionChanged(timeAs<usec>(core->m_time));

                if (core->m_playerTime.system_date_us != INT64_MAX) {

                    //NOTE: upates of the position has an arbitrary scale
                    msec interval = duration_cast<msec>(core->m_length / (core->m_playerTime.rate * 5000));
                    if (interval < msec(15))
                        interval = msec(15);
                    core->m_positionTimer.start(interval);
                }

            }
            core->updateTime(now, lengthOrRateChanged);
        }
    });
}

void MediaPlayer::onTimeDiscontinuity(int64_t system_date_us, void *data)
{
    auto *core = static_cast<MediaPlayer *>(data);
    usec systemDate = usec(system_date_us);
    QMetaObject::invokeMethod(core, [core, systemDate]() {
        if (systemDate > usec(0) && core->interpolateTime(systemDate)) {
            // The discontinuity event got a valid system date, update the time
            // properties.
            core->positionChanged(core->m_position);
            core->updateTime(systemDate, false);
        }

        // And stop the timers.
        core->m_positionTimer.stop();
        core->m_timeTimer.stop();
    });
}

void MediaPlayer::updatePositionFromTimer()
{
    usec system_now = usec(libvlc_clock());
    if (interpolateTime(system_now))
        emit positionChanged(m_position);
}

void MediaPlayer::updateTimeFromTimer()
{
    usec system_now = usec(libvlc_clock());
    if (interpolateTime(system_now))
        updateTime(system_now, false);
}

void MediaPlayer::createCoreConnections()
{
    QList<libvlc_event_e> events;

    events << libvlc_MediaPlayerMediaChanged << libvlc_MediaPlayerNothingSpecial << libvlc_MediaPlayerOpening
           << libvlc_MediaPlayerBuffering << libvlc_MediaPlayerPlaying << libvlc_MediaPlayerPaused
           << libvlc_MediaPlayerStopped << libvlc_MediaPlayerForward << libvlc_MediaPlayerBackward
           << libvlc_MediaPlayerEncounteredError
           << libvlc_MediaPlayerSeekableChanged << libvlc_MediaPlayerPausableChanged << libvlc_MediaPlayerSnapshotTaken
           << libvlc_MediaPlayerVout;

    for (auto &event : events) {
        libvlc_event_attach(m_vlcEvents, event, libvlc_callback, this);
    }

    libvlc_media_player_watch_time(m_vlcMediaPlayer, timeAs<usec>(msec(500)), &MediaPlayer::onTimeUpdate,
        &MediaPlayer::onTimeDiscontinuity, this);
}

void MediaPlayer::removeCoreConnections()
{
    libvlc_media_player_unwatch_time(m_vlcMediaPlayer);

    QList<libvlc_event_e> events;

    events << libvlc_MediaPlayerMediaChanged << libvlc_MediaPlayerNothingSpecial << libvlc_MediaPlayerOpening
           << libvlc_MediaPlayerBuffering << libvlc_MediaPlayerPlaying << libvlc_MediaPlayerPaused
           << libvlc_MediaPlayerStopped << libvlc_MediaPlayerForward << libvlc_MediaPlayerBackward
           << libvlc_MediaPlayerEncounteredError
           << libvlc_MediaPlayerSeekableChanged << libvlc_MediaPlayerPausableChanged << libvlc_MediaPlayerSnapshotTaken
           << libvlc_MediaPlayerVout;

    for (auto &event : events) {
        libvlc_event_detach(m_vlcEvents, event, libvlc_callback, this);
    }
}

}  // namespace Vlc
