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

class QUICKVLC_CORE_EXPORT OpenGLVideoStream : public AbstractVideoStream, protected QOpenGLFunctions
{
public:
    explicit OpenGLVideoStream(QQuickItem *parent = nullptr);
    ~OpenGLVideoStream();

    std::shared_ptr<AbstractVideoFrame> getVideoFrame() override;

    void windowChanged(QQuickWindow *window) override;
    void initContext() override;

private:
    libvlc_video_engine_t videoEngine() override;
    bool updateOutput(const libvlc_video_render_cfg_t *cfg, libvlc_video_output_cfg_t *render_cfg) override;
    bool setup(const libvlc_video_setup_device_cfg_t *cfg, libvlc_video_setup_device_info_t *out) override;
    void cleanup() override;
    void swap() override;
    bool selectPlane(size_t plane, void *output) override;
    bool makeCurrent(bool isCurrent) override;
    void *getProcAddress(const char *current) override;

    QOpenGLContext *m_context = nullptr;
    QOffscreenSurface *m_surface = nullptr;
    QSemaphore m_videoReady;
    QQuickWindow *m_window = nullptr;

    unsigned m_width = 0;
    unsigned m_height = 0;

    QMutex m_text_lock;

    std::unique_ptr<QOpenGLFramebufferObject> m_buffers[4];
    size_t m_idx_render = 0;
    size_t m_idx_swap1 = 1;
    size_t m_idx_swap2 = 2;
    size_t m_idx_display = 3;

    bool m_updated = false;

    std::shared_ptr<OpenGLVideoFrame> m_videoFrame;
};

}  // namespace Vlc
