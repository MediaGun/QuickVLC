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

#include "abstractvideoframe.h"

namespace Vlc {

AbstractVideoFrame::AbstractVideoFrame() : m_size { 0, 0 }
{
}

quint16 AbstractVideoFrame::width() const
{
    return m_size.width();
}

quint16 AbstractVideoFrame::height() const
{
    return m_size.height();
}

void AbstractVideoFrame::setWidth(quint16 width)
{
    m_size.setWidth(width);
}

void AbstractVideoFrame::setHeight(quint16 height)
{
    m_size.setHeight(height);
}

QSize AbstractVideoFrame::size() const
{
    return m_size;
}

void AbstractVideoFrame::setSize(const QSize &size)
{
    m_size = std::move(size);
}

bool AbstractVideoFrame::isValid()
{
    return !m_size.isEmpty();
}

void AbstractVideoFrame::clear()
{
    m_size.setWidth(0);
    m_size.setHeight(0);
}

}  // namespace Vlc
