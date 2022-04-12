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

#include "instance.h"

#include <vlc/vlc.h>

#include "error.h"

namespace Vlc {

Instance::Instance(const QStringList &args, QObject *parent)
    : QObject(parent), m_vlcInstance { nullptr }, m_status { false }
{
#if defined(Q_OS_WIN32)  // Will be removed on Windows if confirmed working
    char **argv = (char **)malloc(sizeof(char **) * args.count());

    for (int i = 0; i < args.count(); ++i) {
        argv[i] = (char *)qstrdup(args.at(i).toUtf8().data());
    }
#else
    char *argv[args.count()];

    for (int i = 0; i < args.count(); ++i) {
        argv[i] = static_cast<char *>(args.at(i).toUtf8().data());
    }
#endif

    m_vlcInstance = libvlc_new(args.count(), argv);

    Error::printErrorMsg();

    if (m_vlcInstance) {
        m_status = true;

        qDebug() << "Using libvlc version:" << version();
    } else {
        qCritical() << "libvlc failed to load!";
    }
}

Instance::~Instance()
{
    if (m_status && m_vlcInstance) {
        libvlc_release(m_vlcInstance);
    }
}

libvlc_instance_t *Instance::core()
{
    return m_vlcInstance;
}

bool Instance::status() const
{
    return m_status;
}

QString Instance::libVertion()
{
    return {};
}

QString Instance::version()
{
    return QString { libvlc_get_version() };
}

}  // namespace Vlc
