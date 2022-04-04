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


cmake_minimum_required(VERSION 3.16)
cmake_policy(VERSION 3.16)

set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_SOURCE_DIR}/config
    ${CMAKE_SOURCE_DIR}/config/platform
)

include(ParseVersion)

project(QuickVLC
    VERSION ${QUICKVLC_VERSION})

SET(PROJECT_LIBRARY QuickVLC)
SET(PROJECT_AUTHOR "Alexey Avramchik (2022)")
SET(PROJECT_OWNER "Bamboo Agile")
SET(PROJECT_COPYRIGHT "Copyright (C) 2022 ${PROJECT_OWNER}.")
SET(PROJECT_DESCRIPTION "QuickVLC — Qt Qml component and bindings for libVlc")

set(QUICKVLC_CORE Core)
set(QUICKVLC_QML Qml)
set(QUICKVLC_PLUGIN_QML PluginQml)
set(QUICKVLC_CORE_NAME ${PROJECT_LIBRARY}Core)
set(QUICKVLC_QML_NAME ${PROJECT_LIBRARY}Qml)
set(QUICKVLC_PLUGIN_QML_NAME ${PROJECT_LIBRARY})

INCLUDE(GNUInstallDirs)
INCLUDE(Macros)
INCLUDE(CompilerConfig)
INCLUDE(Dependencies)
INCLUDE(Platform)

# Compile and set
add_subdirectory(src)
add_subdirectory(docs)

# Testing

# Package
include(Package)
