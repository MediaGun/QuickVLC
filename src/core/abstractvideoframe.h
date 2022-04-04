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

#include <QSize>

#include "core_shared_export.h"

namespace Vlc {

class QUICKVLC_CORE_EXPORT AbstractVideoFrame
{
public:
    AbstractVideoFrame();

    virtual ~AbstractVideoFrame() = 0;

    quint16 width() const;
    quint16 height() const;

    void setWidth(quint16 width);
    void setHeight(quint16 height);

    QSize size() const;
    void setSize(const QSize &size);

    bool isValid();

    void clear();

private:
    QSize m_size;
};

}  // namespace Vlc
