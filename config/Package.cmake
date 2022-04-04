#############################################################################
# VLC-Qt - Qt and libvlc connector library
# Copyright (C) 2015 Tadej Novak <tadej@tano.si>
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library. If not, see <http://www.gnu.org/licenses/>.
#############################################################################

# Enable package helpers
include(CMakePackageConfigHelpers)

### QuickVLC main package configuration

write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/package/QuickVLC/QuickVLCConfigVersion.cmake"
    VERSION ${QUICKVLC_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_file(config/package/QuickVLCConfig.cmake
    "${CMAKE_BINARY_DIR}/package/QuickVLC/QuickVLCConfig.cmake"
)

set(QuickVLCConfigPackageLocation ${CMAKE_INSTALL_LIBDIR}/cmake/QuickVLC)

install(
    FILES
        config/package/QuickVLCConfig.cmake
        "${CMAKE_BINARY_DIR}/package/QuickVLC/QuickVLCConfigVersion.cmake"
    DESTINATION ${QuickVLCConfigPackageLocation}
    COMPONENT Devel
)

### QuickVLC Core package configuration

write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/package/QuickVLCCore/QuickVLCCoreConfigVersion.cmake"
    VERSION ${QUICKVLC_VERSION}
    COMPATIBILITY AnyNewerVersion
)

export(EXPORT QuickVLCCoreTargets
       FILE "${CMAKE_BINARY_DIR}/package/QuickVLCCore/QuickVLCCoreTargets.cmake"
       NAMESPACE QuickVLC::
)

configure_file(config/package/QuickVLCCoreConfig.cmake
    "${CMAKE_BINARY_DIR}/package/QuickVLCCore/QuickVLCCoreConfig.cmake"
)

set(QuickVLCCoreConfigPackageLocation ${CMAKE_INSTALL_LIBDIR}/cmake/QuickVLCCore)

install(EXPORT QuickVLCCoreTargets
        FILE QuickVLCCoreTargets.cmake
        NAMESPACE QuickVLC::
        DESTINATION ${QuickVLCCoreConfigPackageLocation}
)

install(
    FILES
        config/package/QuickVLCCoreConfig.cmake
        "${CMAKE_BINARY_DIR}/package/QuickVLCCore/QuickVLCCoreConfigVersion.cmake"
    DESTINATION ${QuickVLCCoreConfigPackageLocation}
    COMPONENT Devel
)

### QuickVLC Qml package configuration

write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/package/QuickVLCQml/QuickVLCQmlConfigVersion.cmake"
    VERSION ${QUICKVLC_VERSION}
    COMPATIBILITY AnyNewerVersion
)

export(EXPORT QuickVLCQmlTargets
       FILE "${CMAKE_BINARY_DIR}/package/QuickVLCQml/QuickVLCQmlTargets.cmake"
       NAMESPACE QuickVLC::
)

configure_file(config/package/QuickVLCQmlConfig.cmake
    "${CMAKE_BINARY_DIR}/package/QuickVLCQmlQuickVLCQmlConfig.cmake"
)

set(QuickVLCQmlConfigPackageLocation ${CMAKE_INSTALL_LIBDIR}/cmake/QuickVLCQml)

install(EXPORT QuickVLCQmlTargets
        FILE QuickVLCQmlTargets.cmake
        NAMESPACE QuickVLC::
        DESTINATION ${QuickVLCQmlConfigPackageLocation}
)

install(
    FILES
        config/package/QuickVLCQmlConfig.cmake
        "${CMAKE_BINARY_DIR}/package/QuickVLCQml/QuickVLCQmlConfigVersion.cmake"
    DESTINATION ${QuickVLCQmlConfigPackageLocation}
    COMPONENT Devel
)
