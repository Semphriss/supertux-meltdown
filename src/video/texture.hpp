//  SuperTux Meltdown - Semphris' take on the popular Linux platformer
//  Copyright (C) 2022 Semphris <semphris@semphris.com>
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

#ifndef HEADER_STM_VIDEO_TEXTURE_HPP
#define HEADER_STM_VIDEO_TEXTURE_HPP

#include <string>

#include "SDL2/SDL.h"

#include "util/size.hpp"

class Renderer;

class Texture final
{
public:
  Texture(Renderer& renderer, const std::string file);
  Texture(Renderer& renderer, const Size& size);
  ~Texture();

  SDL_Texture* get_sdl_texture() const;
  Size get_size() const;

  const Renderer& get_renderer() const;

private:
  Renderer& m_renderer;
  SDL_Texture* m_sdl_texture;
  bool m_drawable;
  Size m_cached_size;

private:
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;
};

#endif
