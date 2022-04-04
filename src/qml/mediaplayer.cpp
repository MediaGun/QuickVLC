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

#include "core/common.h"

MediaPlayer::MediaPlayer(QQuickItem *parent) : MediaSource { parent }, m_media { nullptr }, m_autoplay { true }
{
    m_instance = new Vlc::Instance(Vlc::Common::args(), this);
    m_player = new Vlc::MediaPlayer(m_instance);

    connect(m_player, &Vlc::MediaPlayer::lengthChanged, this, &MediaPlayer::durationChanged);
    connect(m_player, &Vlc::MediaPlayer::positionChanged, this, &MediaPlayer::positionChanged);
    connect(m_player, &Vlc::MediaPlayer::seekableChanged, this, &MediaPlayer::seekableChanged);
    connect(m_player, &Vlc::MediaPlayer::timeChanged, this, &MediaPlayer::positionChanged);

    setPlayer(m_player);
}

MediaPlayer::~MediaPlayer()
{
    m_player->stop();

    removePlayer();

    if (m_media) {
        delete m_media;
    }

    delete m_player;
    delete m_instance;
}

void MediaPlayer::play()
{
    m_player->play();
}

void MediaPlayer::pause()
{
    m_player->pause();
}

void MediaPlayer::stop()
{
    m_player->stop();
}

bool MediaPlayer::autoplay() const
{
    return m_autoplay;
}

void MediaPlayer::setAutoplay(bool autoplay)
{
    if (m_autoplay == autoplay) {
        return;
    }

    m_autoplay = autoplay;

    emit autoplayChanged();
}

QUrl MediaPlayer::source() const
{
    if (m_media) {
        return QUrl { m_media->currentLocation() };
    } else {
        return QUrl {};
    }
}

void MediaPlayer::setSource(const QUrl &url)
{
    if (url == this->source()) {
        return;
    }

    m_player->stop();

    if (m_media) {
        m_media->deleteLater();
    }

    if (url.isLocalFile()) {
        m_media = new Vlc::Media(url.toLocalFile(), true, m_instance);
    } else {
        m_media = new Vlc::Media(url.toString(QUrl::FullyEncoded), false, m_instance);
    }

    if (m_autoplay) {
        m_player->open(m_media);
    } else {
        m_player->openOnly(m_media);
    }

    emit sourceChanged();
}

qint64 MediaPlayer::duration() const
{
    return m_player->length();
}

qint64 MediaPlayer::position() const
{
    return m_player->time();
}

void MediaPlayer::setPosition(qint64 position)
{
    m_player->setTime(position);
}

bool MediaPlayer::seekable() const
{
    return m_player->seekable();
}

int MediaPlayer::state() const
{
    return m_player->state();
}
