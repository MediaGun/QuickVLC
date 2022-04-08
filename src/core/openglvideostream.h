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

#include <QMutex>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>
#include <QSemaphore>

#include "abstractvideostream.h"
#include "core_shared_export.h"
#include "videoframe.h"

namespace Vlc {

class QUICKVLC_CORE_EXPORT OpenGLVideoStream : public QObject, public AbstractVideoStream, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLVideoStream(QObject *parent = nullptr);
    ~OpenGLVideoStream();

    std::shared_ptr<VideoFrame> getVideoFrame();
    QOpenGLFramebufferObject *getFbo();

public slots:
    void initContext();

private:
    Q_INVOKABLE virtual void frameUpdated() = 0;

    libvlc_video_engine_t videoEngine() override;
    bool resize(const libvlc_video_render_cfg_t *cfg, libvlc_video_output_cfg_t *render_cfg) override;
    bool setup(const libvlc_video_setup_device_cfg_t *cfg, libvlc_video_setup_device_info_t *out) override;
    void cleanup() override;
    void swap() override;
    bool makeCurrent(bool isCurrent) override;
    void *getProcAddress(const char *current) override;

    QOpenGLContext *m_context;
    QOffscreenSurface *m_surface;
    QSemaphore m_videoReady;

    unsigned m_width = 0;
    unsigned m_height = 0;

    QMutex m_text_lock;

    std::unique_ptr<QOpenGLFramebufferObject> m_buffers[3];
    size_t m_idx_render = 0;
    size_t m_idx_swap = 1;
    size_t m_idx_display = 2;

    bool m_updated = false;

    std::shared_ptr<VideoFrame> m_videoFrame;
};

}  // namespace Vlc
