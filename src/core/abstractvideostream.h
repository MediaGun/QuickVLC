/******************************************************************************
* This file is part of QuickVLC - Qt and libvlc connection library.
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>, 
*               2022 Alexey Avramchik (OU Bamboo group) <aa@bam-boo.eu>
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

#include <vlc/vlc.h>

#include "core_shared_export.h"

namespace Vlc {

class MediaPlayer;

class QUICKVLC_CORE_EXPORT AbstractVideoStream
{
public:
    explicit AbstractVideoStream();
    virtual ~AbstractVideoStream();

    void setCallbacks(Vlc::MediaPlayer *player);

    void unsetCallbacks(Vlc::MediaPlayer *player);

protected:
    virtual bool resize(const libvlc_video_render_cfg_t *cfg, libvlc_video_output_cfg_t *render_cfg) = 0;
    virtual bool setup(const libvlc_video_setup_device_cfg_t *cfg, libvlc_video_setup_device_info_t *out) = 0;
    virtual void cleanup() = 0;
    virtual void swap() = 0;
    virtual bool makeCurrent(bool isCurrent) = 0;
    virtual void *getProcAddress(const char *current) = 0;

private:
    static bool resize_cb(void *opaque, const libvlc_video_render_cfg_t *cfg, libvlc_video_output_cfg_t *render_cfg);
    static bool setup_cb(
        void **opaque, const libvlc_video_setup_device_cfg_t *cfg, libvlc_video_setup_device_info_t *out);
    static void cleanup_cb(void *opaque);
    static void swap_cb(void *opaque);
    static bool make_current_cb(void *opaque, bool current);
    static void *get_proc_address_cb(void *opaque, const char *current);
};

}  // namespace Vlc
