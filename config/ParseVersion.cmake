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

# Version parser #
##################

file(READ VERSION QUICKVLC_VERSION)
string(REGEX REPLACE "\n" "" QUICKVLC_VERSION "${QUICKVLC_VERSION}")

message(STATUS "Building Mediagun ${QUICKVLC_VERSION}")

# Check Git revision
find_program(GIT git)

if (GIT AND NOT NO_GIT)
    execute_process(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${GIT} rev-parse --short HEAD
        OUTPUT_VARIABLE GIT_OUT OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(NOT GIT_OUT STREQUAL "")
        set(PROJECT_REVISION "${GIT_OUT}")
        message(STATUS "Git Revision: ${PROJECT_REVISION}")
    endif()
elseif(GITBUILD)
    set(PROJECT_REVISION "${GITBUILD}")
    message(STATUS "Git Revision: ${PROJECT_REVISION}")
else()
    set(GIT_OUT 0)
endif()
