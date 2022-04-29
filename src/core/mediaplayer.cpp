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

#include <vlc/vlc.h>

#include "error.h"
#include "instance.h"
#include "media.h"

namespace Vlc {

MediaPlayer::MediaPlayer(Instance *instance) : QObject { instance }
{
    m_vlcMediaPlayer = libvlc_media_player_new(instance->core());
    m_vlcEvents = libvlc_media_player_event_manager(m_vlcMediaPlayer);

    libvlc_video_set_key_input(m_vlcMediaPlayer, false);
    libvlc_video_set_mouse_input(m_vlcMediaPlayer, false);

    Error::printErrorMsg();

    m_media = nullptr;

    createCoreConnections();

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

int MediaPlayer::length() const
{
    auto length = libvlc_media_player_get_length(m_vlcMediaPlayer);

    Error::printErrorMsg();

    return length;
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
    auto time = libvlc_media_player_get_time(m_vlcMediaPlayer);

    Error::printErrorMsg();

    return time;
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

    return libvlc_media_player_get_position(m_vlcMediaPlayer);
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
    case libvlc_MediaPlayerTimeChanged:
        emit core->timeChanged(event->u.media_player_time_changed.new_time);
        break;
    case libvlc_MediaPlayerPositionChanged:
        emit core->positionChanged(event->u.media_player_position_changed.new_position);
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
    case libvlc_MediaPlayerLengthChanged:
        emit core->lengthChanged(event->u.media_player_length_changed.new_length);
        break;
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

void MediaPlayer::createCoreConnections()
{
    QList<libvlc_event_e> events;

    events << libvlc_MediaPlayerMediaChanged << libvlc_MediaPlayerNothingSpecial << libvlc_MediaPlayerOpening
           << libvlc_MediaPlayerBuffering << libvlc_MediaPlayerPlaying << libvlc_MediaPlayerPaused
           << libvlc_MediaPlayerStopped << libvlc_MediaPlayerForward << libvlc_MediaPlayerBackward
           << libvlc_MediaPlayerEncounteredError << libvlc_MediaPlayerTimeChanged << libvlc_MediaPlayerPositionChanged
           << libvlc_MediaPlayerSeekableChanged << libvlc_MediaPlayerPausableChanged << libvlc_MediaPlayerSnapshotTaken
           << libvlc_MediaPlayerLengthChanged << libvlc_MediaPlayerVout;

    for (auto &event : events) {
        libvlc_event_attach(m_vlcEvents, event, libvlc_callback, this);
    }
}

void MediaPlayer::removeCoreConnections()
{
    QList<libvlc_event_e> events;

    events << libvlc_MediaPlayerMediaChanged << libvlc_MediaPlayerNothingSpecial << libvlc_MediaPlayerOpening
           << libvlc_MediaPlayerBuffering << libvlc_MediaPlayerPlaying << libvlc_MediaPlayerPaused
           << libvlc_MediaPlayerStopped << libvlc_MediaPlayerForward << libvlc_MediaPlayerBackward
           << libvlc_MediaPlayerEncounteredError << libvlc_MediaPlayerTimeChanged << libvlc_MediaPlayerPositionChanged
           << libvlc_MediaPlayerSeekableChanged << libvlc_MediaPlayerPausableChanged << libvlc_MediaPlayerSnapshotTaken
           << libvlc_MediaPlayerLengthChanged << libvlc_MediaPlayerVout;

    for (auto &event : events) {
        libvlc_event_detach(m_vlcEvents, event, libvlc_callback, this);
    }
}

}  // namespace Vlc
