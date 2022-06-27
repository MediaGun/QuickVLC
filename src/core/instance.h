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

#include <QObject>
#include <QStringList>

#include "core_shared_export.h"
#include "vlc.h"

struct libvlc_instance_t;

namespace Vlc {

class QUICKVLC_CORE_EXPORT Instance : public QObject
{
    Q_OBJECT

public:
    Instance(const QStringList &args, QObject *parent = nullptr);

    ~Instance();

    libvlc_instance_t *core();

    bool status() const;

    static QString libVertion();

    static QString version();

    static QString libVersion();

    Enum::LogLevel logLevel() const;

    void setLogLevel(Enum::LogLevel level);

private:
    libvlc_instance_t *m_vlcInstance;
    bool m_status;
    Enum::LogLevel m_logLevel;
};

}  // namespace Vlc
