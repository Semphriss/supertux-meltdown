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

project(STMELTDOWN)

cmake_minimum_required(VERSION 3.0)

file(GLOB_RECURSE STM_FILES src/*/*.cpp)
add_executable(stmeltdown src/main.cpp ${STM_FILES})
target_include_directories(stmeltdown PUBLIC src/)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mk/cmake)

find_package(SDL2 REQUIRED)
target_link_libraries(stmeltdown PUBLIC ${SDL2_LIBRARIES})
target_include_directories(stmeltdown PUBLIC ${SDL2_INCLUDE_DIRS})

find_package(SDL2_image REQUIRED)
target_link_libraries(stmeltdown PUBLIC ${SDL2_IMAGE_LIBRARIES})
target_include_directories(stmeltdown PUBLIC ${SDL2_IMAGE_INCLUDE_DIRS})
