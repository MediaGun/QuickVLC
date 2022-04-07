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

#include "videomaterialshader.h"

#include <QQuickWindow>
#include <QSGTexture>

#include "videomaterial.h"

VideoMaterialShader::VideoMaterialShader()
{
    setShaderFileName(VertexStage, QLatin1String("://shaders/rendering/shaders/videomaterialshader.vert.qsb"));
    setShaderFileName(FragmentStage, QLatin1String("://shaders/rendering/shaders/videomaterialshader.frag.qsb"));
}

VideoMaterialShader::~VideoMaterialShader()
{
}

bool VideoMaterialShader::updateUniformData(RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    Q_UNUSED(newMaterial)
    Q_UNUSED(oldMaterial)

    bool changed = false;
    QByteArray *buf = state.uniformData();

    if (state.isMatrixDirty()) {
        const QMatrix4x4 m = state.combinedMatrix();
        memcpy(buf->data(), m.constData(), 64);
        changed = true;
    }

    return changed;
}

void VideoMaterialShader::updateSampledImage(
    RenderState &state, int binding, QSGTexture **texture, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    Q_UNUSED(state)

    Q_ASSERT(oldMaterial == nullptr || newMaterial->type() == oldMaterial->type());

    VideoMaterial *mat = static_cast<VideoMaterial *>(newMaterial);

    if (binding == 1) {
        QSGTexture *t = mat->getTexture();

        //        t->setAnisotropyLevel(QSGTexture::Anisotropy2x);
        //        t->setFiltering(QSGTexture::Linear);
        //        t->setHorizontalWrapMode(QSGTexture::Repeat);
        //        t->setVerticalWrapMode(QSGTexture::Repeat);

        //        t->commitTextureOperations(state.rhi(), state.resourceUpdateBatch());

        *texture = t;
    }
}
