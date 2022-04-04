#############################################################################
# VLC-Qt - Qt and libvlc connector library
# Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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

macro(symlink_framework_test Target Path Framework)
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/src/lib")

        add_custom_command(TARGET ${Target}
            PRE_LINK
            COMMAND "${CMAKE_COMMAND}" -E create_symlink "${CMAKE_BINARY_DIR}/src/${Path}/${Framework}.framework" "${Framework}.framework"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/src/lib"
        )
    endif()
endmacro()

macro(generate_pkgconfig LibraryName LibraryFolder)
    configure_file(
        lib${LibraryName}.pc.in
        lib${LibraryName}.pc
        @ONLY
    )

    if (PKGCONFIG)
        install(FILES ${CMAKE_BINARY_DIR}/src/${LibraryFolder}/lib${LibraryName}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
    endif()
endmacro()

macro(generate_windows_rc LibraryName LibraryFolder LibrarySrcs)
    configure_file(
        ${LibraryName}.rc.in
        ${LibraryName}.rc
    )

    # Compile resources with windres
    if (MINGW)
        add_custom_command(
            OUTPUT ${CMAKE_BINARY_DIR}/src/${LibraryFolder}/${LibraryName}.obj
            COMMAND ${CMAKE_RC_COMPILER}
            -i ${CMAKE_BINARY_DIR}/src/${LibraryFolder}/${LibraryName}.rc
            -o ${CMAKE_BINARY_DIR}/src/${LibraryFolder}/${LibraryName}.obj
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src/${LibraryFolder}"
        )

        list(APPEND ${LibrarySrcs} ${CMAKE_BINARY_DIR}/src/${LibraryFolder}/${LibraryName}.obj)
    endif()

    if (MSVC)
        list(APPEND ${LibrarySrcs} ${CMAKE_BINARY_DIR}/src/${LibraryFolder}/${LibraryName}.rc)
    endif()
endmacro()
