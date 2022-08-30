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

#include "openglvideostream.h"

#include <QOpenGLContext>
#include <QQuickItem>

namespace Vlc {

OpenGLVideoStream::OpenGLVideoStream(QQuickItem *parent)
    : AbstractVideoStream { parent }
{
    m_context = new QOpenGLContext(parent);
    m_surface = new QOffscreenSurface(nullptr, parent);
}

OpenGLVideoStream::~OpenGLVideoStream()
{
    cleanup();
}

void OpenGLVideoStream::initContext()
{
    //    Q_ASSERT(QSGRendererInterface::isApiRhiBased(QSGRendererInterface::OpenGL));

    if (m_context->isValid()) {
        return;
    }

    auto *context = QOpenGLContext::currentContext();

    m_surface->setFormat(context->format());
    m_surface->create();

    m_context->setFormat(context->format());
    m_context->setShareContext(context);
    m_context->create();

    initializeOpenGLFunctions();

    m_videoReady.release();
}

libvlc_video_engine_t OpenGLVideoStream::videoEngine()
{
    return m_context->isOpenGLES() ? libvlc_video_engine_gles2 : libvlc_video_engine_opengl;
}

std::shared_ptr<AbstractVideoFrame> OpenGLVideoStream::getVideoFrame()
{
    QMutexLocker locker(&m_text_lock);

    if (m_updated) {
        std::swap(m_idx_swap, m_idx_display);

        if (m_buffers[m_idx_display]) {
            m_videoFrame = std::make_shared<VideoFrame>(m_buffers[m_idx_display].get());
        } else {
            m_videoFrame = {};
        }

        m_updated = false;
    }

    return m_videoFrame;
}

bool OpenGLVideoStream::updateOutput(const libvlc_video_render_cfg_t *cfg, libvlc_video_output_cfg_t *render_cfg)
{
    {
        QMutexLocker locker(&m_text_lock);

        for (auto &buffer : m_buffers) {
            buffer = std::make_unique<QOpenGLFramebufferObject>(cfg->width, cfg->height);
        }
    }

    m_width = cfg->width;
    m_height = cfg->height;

    m_buffers[m_idx_render]->bind();

    render_cfg->opengl_format = GL_RGBA;
    render_cfg->full_range = true;
    render_cfg->colorspace = libvlc_video_colorspace_BT709;
    render_cfg->primaries = libvlc_video_primaries_BT709;
    render_cfg->transfer = libvlc_video_transfer_func_SRGB;
    render_cfg->orientation = libvlc_video_orient_top_left;

    return true;
}

bool OpenGLVideoStream::setup(const libvlc_video_setup_device_cfg_t *cfg, libvlc_video_setup_device_info_t *out)
{
    Q_UNUSED(cfg)
    Q_UNUSED(out)

    if (!QOpenGLContext::supportsThreadedOpenGL()) {
        return false;
    }

    /* Wait for rendering view to be ready. */
    m_videoReady.acquire();

    m_width = 0;
    m_height = 0;

    return true;
}

void OpenGLVideoStream::cleanup()
{
    m_videoReady.release();

    QMutexLocker locker(&m_text_lock);

    if (m_width == 0 && m_height == 0) {
        return;
    }

    for (auto &buffer : m_buffers) {
        buffer.reset(nullptr);
    }
}

void OpenGLVideoStream::swap()
{
    QMutexLocker locker(&m_text_lock);

    m_updated = true;

    QMetaObject::invokeMethod(this, &OpenGLVideoStream::frameUpdated, Qt::QueuedConnection);

    //    frameUpdated();

    std::swap(m_idx_swap, m_idx_render);

    m_buffers[m_idx_render]->bind();
}

bool OpenGLVideoStream::selectPlane(size_t plane, void *output)
{
    //N/A
    return true;
}

bool OpenGLVideoStream::makeCurrent(bool isCurrent)
{
    if (isCurrent) {
        m_context->makeCurrent(m_surface);
    } else {
        m_context->doneCurrent();
    }

    return true;
}

void *OpenGLVideoStream::getProcAddress(const char *current)
{
    auto addr = m_context->getProcAddress(current);

#if QT_CONFIG(xcb_glx_plugin)
    // blacklist egl lookup on GLX
    // https://dri.freedesktop.org/wiki/glXGetProcAddressNeverReturnsNULL/
    if (m_context->nativeInterface<QNativeInterface::QGLXContext>()) {
        if (QString(current).startsWith("egl")) {
            return nullptr;
        }
    }
#endif

    return reinterpret_cast<void *>(addr);
}

}  // namespace Vlc
