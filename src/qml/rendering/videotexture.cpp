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
}

qint64 VideoTexture::comparisonKey() const
{
    if (m_nativeObject) {
        return m_nativeObject;
    }

    if (m_texture) {
        return quint64(quintptr(m_texture.data()));
    }

    return quint64(quintptr(this));
}

QSize VideoTexture::textureSize() const
{
    return m_size;
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

QRhiTexture *VideoTexture::rhiTexture() const
{
    return m_texture.data();
}

void VideoTexture::commitTextureOperations(QRhi *rhi, QRhiResourceUpdateBatch *resourceUpdates)
{
    updateRhiTexture(rhi, resourceUpdates);
}

void VideoTexture::setRhiTexture(QRhiTexture *texture)
{
    m_texture.reset(texture);
}

void VideoTexture::setData(QRhiTexture::Format f, const QSize &s, const uchar *data, int bytes)
{
    m_size = s;
    m_format = f;
    m_data = { reinterpret_cast<const char *>(data), bytes };
}

void VideoTexture::setNativeObject(quint64 obj, const QSize &s, QRhiTexture::Format f)
{
    setData(f, s, nullptr, 0);

    if (m_nativeObject != obj) {
        m_nativeObject = obj;
        m_texture.reset();
    }
}

void VideoTexture::updateRhiTexture(QRhi *rhi, QRhiResourceUpdateBatch *resourceUpdates)
{
    bool needsRebuild = m_texture && m_texture->pixelSize() != m_size;

    if (!m_texture) {
        QRhiTexture::Flags flags;

        if (hasMipmaps()) {
            flags |= QRhiTexture::MipMapped | QRhiTexture::UsedWithGenerateMips;
        }

        m_texture.reset(rhi->newTexture(m_format, m_size, 1, flags));

        needsRebuild = true;
    }

    if (needsRebuild) {
        m_texture->setPixelSize(m_size);
        bool created = m_nativeObject ? m_texture->createFrom({ m_nativeObject, 0 }) : m_texture->create();

        if (!created) {
            qWarning("Failed to build texture (size %dx%d)", m_size.width(), m_size.height());
            return;
        }
    }

    if (!m_data.isEmpty()) {
        QRhiTextureSubresourceUploadDescription subresDesc(m_data.constData(), m_data.size());
        subresDesc.setSourceSize(m_size);
        subresDesc.setDestinationTopLeft(QPoint(0, 0));
        QRhiTextureUploadEntry entry(0, 0, subresDesc);
        QRhiTextureUploadDescription desc({ entry });
        resourceUpdates->uploadTexture(m_texture.data(), desc);
        m_data.clear();
    }
}
