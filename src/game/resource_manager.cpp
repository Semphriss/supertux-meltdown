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

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "util/log.hpp"
#include "video/font.hpp"

ResourceManager&
ResourceManager::get_resource_manager()
{
  static ResourceManager rm;
  return rm;
}

ResourceManager::~ResourceManager()
{
  Font::flush_fonts();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

ResourceManager::ResourceManager()
{
  log_info << "SDL " << SDL_MAJOR_VERSION << "." << SDL_MINOR_VERSION << "."
           << SDL_PATCHLEVEL << std::endl;

  if (SDL_Init(SDL_INIT_VIDEO))
  {
    throw std::runtime_error("Could not initialize SDL: " +
                             std::string(SDL_GetError()));
  }

  if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2") == SDL_FALSE)
  {
    log_error << "Could not set antialiasing: " << SDL_GetError() << std::endl;
  }

  if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP))
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
