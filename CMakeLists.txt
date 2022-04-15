#  SuperTux Meltdown - Semphris' take on the popular Linux platformer
#  Copyright (C) 2021-2022 Semphris <semphris@semphris.com>
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

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mk/cmake ${CMAKE_MODULE_PATH})

include(GetGitRevisionDescription)
git_describe(STM_VER)

string(REGEX REPLACE "^v([0-9]+)\\..*" "\\1" STM_VER_MAJOR "${STM_VER}")
string(REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" STM_VER_MINOR "${STM_VER}")
string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" STM_VER_PATCH "${STM_VER}")
string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.[0-9]+-([0-9]+)-.*" "\\1" STM_VER_BUILD "${STM_VER}")

if(STM_VER_BUILD STREQUAL STM_VER)
  set(VER_SHORT "${STM_VER_MAJOR}.${STM_VER_MINOR}.${STM_VER_PATCH}")
  set(STM_IS_RELEASE ON)
else()
  set(VER_SHORT "${STM_VER_MAJOR}.${STM_VER_MINOR}.${STM_VER_PATCH}.${STM_VER_BUILD}")
  set(STM_IS_RELEASE OFF)
endif()

cmake_policy(SET CMP0048 NEW)

project(supertux-meltdown VERSION "${VER_SHORT}")

if(EMSCRIPTEN)
  # Requires target_link_options
  cmake_minimum_required(VERSION 3.13)
else()
  cmake_minimum_required(VERSION 3.0)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(STM_INSTALL_LIBRARY "Install the Meltdown library" ${BUILD_SHARED_LIBS})

file(GLOB_RECURSE STM_LIB_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp)
list(SORT STM_LIB_FILES)
add_library(supertux-meltdown_lib ${STM_LIB_FILES})
target_include_directories(supertux-meltdown_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)
target_compile_definitions(supertux-meltdown_lib PUBLIC -DSTM_VER="${STM_VER}")
if(UBUNTU_TOUCH)
  target_compile_definitions(supertux-meltdown_lib PUBLIC -DUBUNTU_TOUCH=1)
endif(UBUNTU_TOUCH)

add_executable(supertux-meltdown ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_include_directories(supertux-meltdown PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)
target_link_libraries(supertux-meltdown PUBLIC supertux-meltdown_lib)

if(EMSCRIPTEN)
  target_link_options(supertux-meltdown PUBLIC -sEXPORTED_FUNCTIONS=['_main','_getExceptionMessage'] PUBLIC -sEXPORTED_RUNTIME_METHODS=['ccall','cwrap'] PUBLIC -sEXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap'])
  target_link_options(supertux-meltdown PUBLIC --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/data/@/data/
                                                -sALLOW_MEMORY_GROWTH=1 --use-preload-plugins
                                                -fsanitize=address)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -sSDL2_IMAGE_FORMATS='[\"png\",\"jpg\"]' -fexceptions")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -sSDL2_IMAGE_FORMATS='[\"png\",\"jpg\"]' -fexceptions")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mk/emscripten/supertux-meltdown.html ${CMAKE_CURRENT_BINARY_DIR}/supertux-meltdown.html COPYONLY)

  set(HARBOR_BUILD_TEST OFF CACHE BOOL "" FORCE)
endif()

file(GLOB_RECURSE STM_TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*/*.cpp)
list(SORT STM_TEST_FILES)
set(HARBOR_USER_TEST_FILES ${STM_TEST_FILES} CACHE STRING "" FORCE)
set(HARBOR_USER_TEST_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src/ CACHE STRING "" FORCE)
set(HARBOR_USER_TEST_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/tests/ CACHE STRING "" FORCE)
set(HARBOR_USER_TEST_LINKS supertux-meltdown_lib CACHE STRING "" FORCE)
set(HARBOR_USER_TEST_COV supertux-meltdown_lib CACHE STRING "" FORCE)

# Harbor
option(HARBOR_BUILD_EXEC OFF)
option(HARBOR_INSTALL_DATA OFF)
option(HARBOR_INSTALL_LIB ${BUILD_SHARED_LIBS})
option(HARBOR_USE_SCRIPTING_LUA OFF)
set(HARBOR_USER_TEST_SKIP_HARBOR ON CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/harbor)
target_link_libraries(supertux-meltdown_lib PUBLIC harbor_lib)

# Move run_tests from build/external/harbor/ to just build/ (and enable CTest)
if(HARBOR_BUILD_TEST)
  enable_testing()
  set_target_properties(run_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

if(UBUNTU_TOUCH)
  install(TARGETS supertux-meltdown RUNTIME DESTINATION .)
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/ DESTINATION data)

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mk/clickable/manifest.json.in ${CMAKE_CURRENT_BINARY_DIR}/manifest.json)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/manifest.json DESTINATION .)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/mk/clickable/supertux-meltdown.apparmor DESTINATION .)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/mk/linux/supertux-meltdown.desktop DESTINATION .)
else(UBUNTU_TOUCH)
  install(TARGETS supertux-meltdown RUNTIME DESTINATION bin)
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/ DESTINATION share/games/supertux-meltdown)
endif(UBUNTU_TOUCH)

if(STM_INSTALL_LIBRARY)
  install(TARGETS supertux-meltdown_lib)
endif(STM_INSTALL_LIBRARY)
