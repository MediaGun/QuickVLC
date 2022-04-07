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

#include "videostream.h"

#include <functional>

#include "core/mediaplayer.h"
#include "videooutput.h"

VideoStream::VideoStream(QObject *parent) : Vlc::OpenGLVideoStream(parent)
{
}

VideoStream::~VideoStream()
{
}

void VideoStream::init(Vlc::MediaPlayer *player)
{
    m_player = player;

    setCallbacks(player);
}

void VideoStream::deinit()
{
    unsetCallbacks(m_player);

    m_player = nullptr;
}

void VideoStream::registerVideoOutput(VideoOutput *output)
{
    if (m_attachedOutputs.find(output) != m_attachedOutputs.end()) {
        return;
    }

    m_attachedOutputs.insert(output);
}

void VideoStream::deregisterVideoOutput(VideoOutput *output)
{
    m_attachedOutputs.erase(output);
}

void VideoStream::frameUpdated()
{
    //    auto frame = getVideoFrame();

    //    std::shared_ptr<const Vlc::VideoFrame> frame = std::dynamic_pointer_cast<const
    //    Vlc::VideoFrame>(getVideoFrame());

    //    if (!frame) {
    //        return;
    //    }

    //    for (auto *output : m_attachedOutputs) {
    //        QMetaObject::invokeMethod(output, &VideoOutput::presentFrame, Qt::QueuedConnection, QGenericArgument {},
    //            Q_ARG(std::shared_ptr<Vlc::VideoFrame>, frame));
    //    }

    //    std::for_each(m_attachedOutputs.begin(), m_attachedOutputs.end(),
    //        std::bind2nd(std::mem_fun(&VideoOutput::presentFrame), frame));

    auto *fbo = getFbo();

    std::for_each(
        m_attachedOutputs.begin(), m_attachedOutputs.end(), std::bind2nd(std::mem_fun(&VideoOutput::presentFbo), fbo));
}
