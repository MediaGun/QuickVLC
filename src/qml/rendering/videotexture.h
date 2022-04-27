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

#include <private/qrhi_p.h>

#include <QSGDynamicTexture>

class VideoTexture : public QSGDynamicTexture
{
public:
    VideoTexture();

    qint64 comparisonKey() const override;
    QSize textureSize() const override;
    bool hasAlphaChannel() const override;
    bool hasMipmaps() const override;
    bool updateTexture() override;

    // QSGTexture interface
    QRhiTexture *rhiTexture() const override;
    void commitTextureOperations(QRhi *rhi, QRhiResourceUpdateBatch *resourceUpdates) override;

    void setRhiTexture(QRhiTexture *texture);
    void setData(QRhiTexture::Format f, const QSize &s, const uchar *data, int bytes);
    void setNativeObject(quint64 obj, const QSize &s, QRhiTexture::Format f = QRhiTexture::RGBA8);

private:
    void updateRhiTexture(QRhi *rhi, QRhiResourceUpdateBatch *resourceUpdates);

    QRhiTexture::Format m_format;
    QSize m_size;
    QByteArray m_data;

    QScopedPointer<QRhiTexture> m_texture;
    quint64 m_nativeObject = 0;
};
