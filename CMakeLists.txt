#  SuperTux Meltdown - Semphris' take on the popular Linux platformer
#  Copyright (C) 2022 Semphris <semphris@protonmail.com>
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

project(supertux-meltdown)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE STM_LIB_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp)
list(SORT STM_LIB_FILES)
add_library(supertux-meltdown_lib ${STM_LIB_FILES})
target_include_directories(supertux-meltdown_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)

add_executable(supertux-meltdown ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_include_directories(supertux-meltdown PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)
target_link_libraries(supertux-meltdown PUBLIC supertux-meltdown_lib)

# Harbor
option(HARBOR_BUILD_EXEC OFF)
option(HARBOR_BUILD_TEST OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/harbor)
target_link_libraries(supertux-meltdown_lib PUBLIC harbor_lib)
