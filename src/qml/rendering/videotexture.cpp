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

#include "videotexture.h"

VideoTexture::VideoTexture()
{
    //    auto *ni = nativeInterface<QNativeInterface::QSGOpenGLTexture>();
}

// void VideoTexture::setStream(VideoStream *videoStream)
//{
//    m_videoStream = videoStream;
//}

qint64 VideoTexture::comparisonKey() const
{
    //    GLuint textureId = m_videoStream->getVideoFrame()->takeTexture();

    //    return static_cast<qint64>(textureId);
    return 0;
}

QSize VideoTexture::textureSize() const
{
    //    auto size = m_videoStream->getVideoFrame()->size();

    //    return size;
    return QSize {};
}

bool VideoTexture::hasAlphaChannel() const
{
    return false;
}

bool VideoTexture::hasMipmaps() const
{
    return false;
}

bool VideoTexture::updateTexture()
{
    //    QOpenGLFramebufferObject *fbo = m_videoStream->getVideoFrame();

    //    if (fbo != nullptr) {
    //        GLuint textureId = fbo->takeTexture();

    //        auto *ni = nativeInterface<QNativeInterface::QSGOpenGLTexture>();
    //        auto *gl = QOpenGLContext::currentContext()->functions();
    //    }

    return false;
}
