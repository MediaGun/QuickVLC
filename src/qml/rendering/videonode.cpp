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

#include "videonode.h"

VideoNode::VideoNode() : m_geometry { QSGGeometry::defaultAttributes_TexturedPoint2D(), 4 }
{
    setGeometry(&m_geometry);
    setMaterial(&m_material);
}

void VideoNode::setTexture(const std::shared_ptr<QSGTexture> &texture)
{
    m_material.setTexture(texture);

    markDirty(QSGNode::DirtyMaterial);
}

void VideoNode::setRect(const QRectF &rect, const QRectF &sourceRect)
{
    QSGGeometry::updateTexturedRectGeometry(&m_geometry, rect, sourceRect);

    markDirty(QSGNode::DirtyGeometry);
}

void VideoNode::updateFrame()
{
    m_material.updateFrame();
}
