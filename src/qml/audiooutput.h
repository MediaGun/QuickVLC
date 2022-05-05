/******************************************************************************
 * This file is part of QuickVLC - Qt and libvlc connection library.
 * Copyright (C) 2022 Alexey Avramchik (OU Bamboo group) <aa@bam-boo.eu>
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
#include <QQuickItem>

#include "core/audio.h"
#include "core/mediaplayer.h"
#include "qml_shared_export.h"

class QUICKVLC_QML_EXPORT AudioOutput : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged)
    Q_PROPERTY(float volume READ volume WRITE setVolume NOTIFY volumeChanged)

public:
    AudioOutput();
    ~AudioOutput();

    void init(Vlc::MediaPlayer *mediaPlayer);

    bool muted();
    void setMuted(bool isMuted);

    float volume();
    void setVolume(float newVolume);

signals:
    void mutedChanged();
    void volumeChanged();

private:
    Vlc::Audio *m_audio;
};
