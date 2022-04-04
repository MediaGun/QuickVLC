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

# Compiler Configuration #
##########################

# Detect building for mobile
if(CMAKE_SYSTEM_NAME MATCHES "Android" OR IOS)
    set(MOBILE ON)
endif()

# Require C++ standard minimum 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Automatically run moc, uic and/or rcc, if required
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Verbose makefile
set(CMAKE_VERBOSE_MAKEFILE ON)


list(APPEND QML_DIRS "${CMAKE_SOURCE_DIR}/qml")
set(QML_IMPORT_PATH "${QML_DIRS}" CACHE STRING "Qt Creator extra qml import paths")

# Show all warnings
if(NOT MSVC)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
endif()

# Coverage reports
if(COVERAGE AND NOT MOBILE AND NOT MSVC)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -coverage")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -coverage")
endif()

# Make final release build smaller
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -s")
endif()

# Support macOS 10.6 or later (64-bit only)
if(NOT IOS AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment version")

    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ferror-limit=0")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ferror-limit=0")

    # TODO: Enable back
    message(WARNING "Enable MacOS additional compiler settings!")

#    set(WITH_HOMEBREW OFF CACHE BOOL "Build using Homebrew provided Qt")

#    if(WITH_HOMEBREW)
#        set(CMAKE_MACOSX_RPATH OFF)
#    else()
#        set(CMAKE_MACOSX_RPATH ON)
#        set(CMAKE_INSTALL_RPATH "@loader_path/${CMAKE_INSTALL_LIBDIR}")
#        set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
#    endif()

#    set(CMAKE_OSX_ARCHITECTURES x86_64)
endif()
