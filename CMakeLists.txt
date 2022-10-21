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

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE STM_FILES src/*.cpp)
add_executable(stmeltdown ${STM_FILES})
target_include_directories(stmeltdown PUBLIC src/)

# Equivalent of -DSTM_VERSION="$(git describe --tags --always)"
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

# Find and link libraries and include directories
if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
  set(USE_FLAGS "-sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2")
  set(USE_FLAGS "${USE_FLAGS} -sSDL2_IMAGE_FORMATS='[\"png\"]'")
  set(USE_FLAGS "${USE_FLAGS} -sNO_DISABLE_EXCEPTION_CATCHING")
  set(USE_LINK_FLAGS "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/data/@/data")
  set(USE_LINK_FLAGS "${USE_LINK_FLAGS} --use-preload-plugin")
  set(USE_LINK_FLAGS "${USE_LINK_FLAGS} --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/mk/emscripten/template.html")

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_LINK_FLAGS}")

  set(CMAKE_EXECUTABLE_SUFFIX .html)

  # PhysFS needs to be built manually
  set(PHYSFS_EMSCRIPTEN ${CMAKE_BINARY_DIR}/physfs/build.wasm/)
  if(NOT EXISTS ${PHYSFS_EMSCRIPTEN})
    message(FATAL_ERROR "Please run ../mk/emscripten/build-physfs.sh before "
                        "running CMake.")
  endif()

  target_include_directories(stmeltdown PUBLIC ${CMAKE_BINARY_DIR}/physfs/src)
  target_link_libraries(stmeltdown PUBLIC ${PHYSFS_EMSCRIPTEN}/libphysfs.a)
else()
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

  find_package(PhysFS REQUIRED)
  target_link_libraries(stmeltdown PUBLIC ${PHYSFS_LIBRARY})
  target_include_directories(stmeltdown PUBLIC ${PHYSFS_INCLUDE_DIRS})
endif()

# Installation
# ============

# Required
install(TARGETS stmeltdown DESTINATION bin)
install(DIRECTORY data/ DESTINATION share/stmeltdown)

# Optional
install(FILES mk/install/stmeltdown.desktop DESTINATION share/applications)
install(FILES mk/install/stmeltdown.svg
        DESTINATION share/icons/hicolor/scalable/apps)
install(FILES mk/unix/bash_completion.sh
        DESTINATION share/bash-completion/completions/stmeltdown)
install(FILES mk/unix/man DESTINATION share/man/stmeltdown.6)

# Packaging
# =========

# Source (cpack --config CPackSourceConfig.cmake -G ZIP)
file(STRINGS .gitignore STM_IGNORE REGEX "^[^#]")
string(REGEX REPLACE "\\." "\\\\\\\\." STM_IGNORE_ESCAPED "${STM_IGNORE}")
string(REGEX REPLACE "\\*" ".*" STM_IGNORE_ESCAPED "${STM_IGNORE_ESCAPED}")
string(REGEX REPLACE "\\?" ".?" STM_IGNORE_ESCAPED "${STM_IGNORE_ESCAPED}")
set(CPACK_SOURCE_IGNORE_FILES "${STM_IGNORE_ESCAPED};\\\\.git/")

include(CPack)
