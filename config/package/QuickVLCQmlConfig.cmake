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

include(CMakeFindDependencyMacro)

find_dependency(QuickVLCCore)

include("${CMAKE_CURRENT_LIST_DIR}/QuickVLCQmlTargets.cmake")

set_target_properties(QuickVLC::Qml
    PROPERTIES
        INTERFACE_LINK_LIBRARIES QuickVLC::Core
)

if (NOT STATIC AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(QuickVLCQmlFramework ${CMAKE_CURRENT_LIST_DIR}/../../QuickVLCQml.framework)
    set(QuickVLCQmlPluginDir ${CMAKE_CURRENT_LIST_DIR}/../../../qml/QuickVLC)
else()
    get_target_property(QuickVLCQmlLocation QuickVLC::Qml INTERFACE_INCLUDE_DIRECTORIES)

    string(REGEX REPLACE "/include" "" QuickVLCQmlLocation "${QuickVLCQmlLocation}")
endif()
