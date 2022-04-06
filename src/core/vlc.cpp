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

#include "vlc.h"

namespace Vlc {

Enum::Enum(QObject *parent) : QObject { parent }
{
}

QStringList Enum::logLevel()
{
    QStringList list;

    list << "debug"
         << "debug"
         << "notice"
         << "warning"
         << "error"
         << "disabled";

    return list;
}

QStringList Enum::audioCodec()
{
    QStringList list;

    list << "none"
         << "mpga"
         << "mp3"
         << "mp4a"
         << "vorb"
         << "flac";

    return list;
}

QStringList Enum::audioOutput()
{
    QStringList list;

    list << "default";

    return list;
}

QStringList Enum::audioOutputHuman()
{
    QStringList list;

    list << "default";

    return list;
}

QStringList Enum::deinterlacing()
{
    QStringList list;

    list << ""
         << "discard"
         << "blend"
         << "man"
         << "bob"
         << "linear"
         << "x"
         << "yadif"
         << "yadif2x"
         << "phosphor"
         << "ivtc";

    return list;
}

QStringList Enum::mux()
{
    QStringList list;

    list << "ts"
         << "ps"
         << "mp4"
         << "ogg"
         << "avi";

    return list;
}

QStringList Enum::ratio()
{
    QStringList list;

    list << ""
         << "ignore"
         << "16:9"
         << "16:10"
         << "185:100"
         << "221:100"
         << "235:100"
         << "239:100"
         << "4:3"
         << "5:4"
         << "5:3"
         << "1:1";

    return list;
}

QStringList Enum::ratioHuman()
{
    QStringList list;

    list << ""
         << ""
         << "16:9"
         << "16:10"
         << "1.85:1"
         << "2.21:1"
         << "2.35:1"
         << "2.39:1"
         << "4:3"
         << "5:4"
         << "5:3"
         << "1:1";

    return list;
}

QSizeF Enum::ratioSize(const Enum::Ratio &ratio)
{
    switch (ratio) {
    case Enum::R_16_9:
        return QSizeF(16, 9);
        break;
    case Enum::R_16_10:
        return QSizeF(16, 10);
        break;
    case Enum::R_185_100:
        return QSizeF(185, 100);
        break;
    case Enum::R_221_100:
        return QSizeF(221, 100);
        break;
    case Enum::R_235_100:
        return QSizeF(235, 100);
        break;
    case Enum::R_239_100:
        return QSizeF(239, 100);
        break;
    case Enum::R_4_3:
        return QSizeF(4, 3);
        break;
    case Enum::R_5_4:
        return QSizeF(5, 4);
        break;
    case Enum::R_5_3:
        return QSizeF(5, 3);
        break;
    case Enum::R_1_1:
        return QSizeF(1, 1);
        break;
    case Enum::Original:
    case Enum::Ignore:
    default:
        return QSizeF(0, 0);
        break;
    }

    return QSizeF(0, 0);
}

QList<float> Enum::scale()
{
    QList<float> list;

    list << (float)0 << (float)1.05 << (float)1.1 << (float)1.2 << (float)1.3 << (float)1.4 << (float)1.5 << (float)1.6
         << (float)1.7 << (float)1.8 << (float)1.9 << (float)2.0;

    return list;
}

QStringList Enum::videoCodec()
{
    QStringList list;

    list << "none"
         << "mpgv"
         << "mp4v"
         << "h264"
         << "theora";

    return list;
}

QStringList Enum::videoOutput()
{
    QStringList list;

    list << "default";

#if defined(Q_OS_LINUX)
    list << "x11"
         << "xvideo"
         << "glx";
#elif defined(Q_OS_WIN32)
    list << "directx"
         << "direct3d";
#endif
    list << "opengl";

    return list;
}

QStringList Enum::videoOutputHuman()
{
    QStringList list;

    list << "default";

#if defined(Q_OS_LINUX)
    list << "X11"
         << "XVideo"
         << "OpenGL";
#elif defined(Q_OS_WIN32)
    list << "DirectX"
         << "Direct3D"
         << "OpenGL";
#endif

    return list;
}

}  // namespace Vlc
