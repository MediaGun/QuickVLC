###############################################################################
# This file is part of QuickVLC - Qt and libvlc connection library.
# Copyright (C) 2016 Tadej Novak <tadej@tano.si>, 
#               2022 Alexey Avramchik (OU Bamboo group) <aa@bam-boo.eu>
#
# QuickVLC is free software: you can redistribute it and/or modify it 
# under the terms of the GNU General Public License as published 
# by the Free Software Foundation, either version 3 of the License, 
# or (at your option) any later version.
#
# QuickVLC is distributed in the hope that it will be useful, 
# but WITHOUT ANY WARRANTY; without even the implied warranty 
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License 
# along with QuickVLC. If not, see <https://www.gnu.org/licenses/>.
###############################################################################

if (CMAKE_VERSION VERSION_LESS 3.16)
    message(FATAL_ERROR "QuickVLC requires at least CMake version 3.16")
endif()

if (NOT QuickVLC_FIND_COMPONENTS)
    set(QuickVLC_NOT_FOUND_MESSAGE "The QuickVLC package requires at least one component")

    set(QuickVLC_FOUND False)

    return()
endif()

set (_QuickVLC_FIND_PARTS_REQUIRED)

if (QuickVLC_FIND_REQUIRED)
    set(_QuickVLC_FIND_PARTS_REQUIRED REQUIRED)
endif()

set(_QuickVLC_FIND_PARTS_QUIET)

if (QuickVLC_FIND_QUIETLY)
    set(_QuickVLC_FIND_PARTS_QUIET QUIET)
endif()

get_filename_component(_QuickVLC_install_prefix "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

set(_QuickVLC_NOTFOUND_MESSAGE)

foreach (module ${QuickVLC_FIND_COMPONENTS})
    find_package (QuickVLC${module}
        ${_QuickVLC_FIND_PARTS_QUIET}
        ${_QuickVLC_FIND_PARTS_REQUIRED}
        PATHS "${_QuickVLC_install_prefix}" NO_DEFAULT_PATH
    )

    if (NOT QuickVLC${module}_FOUND)
        if (QuickVLC_FIND_REQUIRED_${module})
            set(_QuickVLC_NOTFOUND_MESSAGE "${_QuickVLC_NOTFOUND_MESSAGE}Failed to find VLC-Qt component \"${module}\" config file at \"${_QuickVLC_install_prefix}/QuickVLC${module}/QuickVLC${module}Config.cmake\"\n")
        elseif(NOT QuickVLC_FIND_QUIETLY)
            message(WARNING "Failed to find VLC-Qt component \"${module}\" config file at \"${_QuickVLC_install_prefix}/QuickVLC${module}/QuickVLC${module}Config.cmake\"")
        endif()
    endif()
endforeach()

if (_QuickVLC_NOTFOUND_MESSAGE)
    set(QuickVLC_NOT_FOUND_MESSAGE "${_QuickVLC_NOTFOUND_MESSAGE}")

    set(QuickVLC_FOUND False)
endif()
