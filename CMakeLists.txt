#  SuperTux Meltdown - Semphris' take on the popular Linux platformer
#  Copyright (C) 2022 Semphris <semphris@semphris.com>
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

project(stmeltdown)

cmake_minimum_required(VERSION 3.0)

file(GLOB_RECURSE STM_FILES src/*.cpp tests/*.cpp)
add_executable(stmeltdown ${STM_FILES})
target_include_directories(stmeltdown PUBLIC src/ tests/)

# This presumes Git exists and is in the PATH, which may or may not be true
execute_process(COMMAND git describe --tags --always
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                OUTPUT_VARIABLE STM_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET RESULT_VARIABLE GIT_SUCCESS)
if (NOT ("${GIT_SUCCESS}" STREQUAL "0"))
  message(WARNING "Could not run git, returned: ${GIT_SUCCESS}")
else()
  target_compile_definitions(stmeltdown PUBLIC -DSTM_VERSION="${STM_VERSION}")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mk/cmake)

find_package(SDL2 REQUIRED)
target_link_libraries(stmeltdown PUBLIC ${SDL2_LIBRARIES})
target_include_directories(stmeltdown PUBLIC ${SDL2_INCLUDE_DIRS})

find_package(SDL2_image REQUIRED)
target_link_libraries(stmeltdown PUBLIC ${SDL2_IMAGE_LIBRARIES})
target_include_directories(stmeltdown PUBLIC ${SDL2_IMAGE_INCLUDE_DIRS})

find_package(SDL2_ttf REQUIRED)
target_link_libraries(stmeltdown PUBLIC ${SDL2_TTF_LIBRARIES})
target_include_directories(stmeltdown PUBLIC ${SDL2_TTF_INCLUDE_DIRS})

# Installation

install(TARGETS stmeltdown)
install(DIRECTORY data/ DESTINATION share/stmeltdown)
install(FILES mk/install/stmeltdown.desktop DESTINATION share/applications)
install(FILES mk/install/stmeltdown.svg
        DESTINATION share/icons/hicolor/scalable/apps)

# Packaging

file(STRINGS .gitignore STM_IGNORE REGEX "^[^#]")
string(REGEX REPLACE "\\." "\\\\\\\\." STM_IGNORE_ESCAPED "${STM_IGNORE}")
string(REGEX REPLACE "\\*" ".*" STM_IGNORE_ESCAPED "${STM_IGNORE_ESCAPED}")
string(REGEX REPLACE "\\?" ".?" STM_IGNORE_ESCAPED "${STM_IGNORE_ESCAPED}")
message(STATUS "${STM_IGNORE_ESCAPED}")
set(CPACK_SOURCE_IGNORE_FILES "${STM_IGNORE_ESCAPED};\\\\.git/")

include(CPack)
