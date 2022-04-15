//  SuperTux Meltdown - Semphris' take on the popular Linux platformer
//  Copyright (C) 2021-2022 Semphris <semphris@semphris.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "game/resource_manager.hpp"

#include <memory>

#include "physfs.h"
#ifdef EMSCRIPTEN
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#else
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#endif

#include "util/log.hpp"
#include "video/font.hpp"

ResourceManager&
ResourceManager::get_resource_manager(const char* arg0)
{
  static ResourceManager rm(arg0);
  return rm;
}

ResourceManager::~ResourceManager()
{
  Font::flush_fonts();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  if (!PHYSFS_deinit())
  {
    log_error << "PhysFS failed to unload ("
              << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())
              << "), game must be closed completely." << std::endl;
  }
}

ResourceManager::ResourceManager(const char* arg0)
{
  log_info << "Compiled against SDL " << SDL_MAJOR_VERSION << "."
           << SDL_MINOR_VERSION << "." << SDL_PATCHLEVEL << std::endl;

  SDL_version ver;
  SDL_GetVersion(&ver);

  log_info << "Linked against SDL " << std::to_string(ver.major) << "."
           << std::to_string(ver.minor) << "." << std::to_string(ver.patch)
           << std::endl;

  if (!PHYSFS_init(arg0))
  {
    throw std::runtime_error("Could not initialize PhysFS: " + std::string(
                             PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
  }

  if (SDL_Init(SDL_INIT_VIDEO))
  {
    throw std::runtime_error("Could not initialize SDL: " +
                             std::string(SDL_GetError()));
  }

  if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2") == SDL_FALSE)
  {
    log_warn << "Could not set antialiasing: " << SDL_GetError() << std::endl;
  }

  /** @bug Tiff and WebP can't seem to be compiled with Emscripten (include
      headers tiffio.h and webp/decode.h not found) */
  if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
  {
    throw std::runtime_error("Could not initialize SDL_image: " +
                             std::string(IMG_GetError()));
  }

  if (TTF_Init())
  {
    throw std::runtime_error("Could not initialize SDL_ttf: " +
                             std::string(TTF_GetError()));
  }
}
