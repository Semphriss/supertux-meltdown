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

#include "video/texture.hpp"

#include "SDL2/SDL_image.h"

#include "video/renderer.hpp"

Texture::Texture(Renderer& renderer, const std::string file) :
  m_renderer(renderer),
  m_sdl_texture(SDL_CreateTextureFromSurface(renderer.get_sdl_renderer(),
                                             IMG_Load(file.c_str()))),
  m_drawable(false),
  m_cached_size()
{
  int w, h;
  SDL_QueryTexture(m_sdl_texture, nullptr, nullptr, &w, &h);
  m_cached_size.w = static_cast<float>(w);
  m_cached_size.h = static_cast<float>(h);
}

Texture::Texture(Renderer& renderer, const Size& size) :
  m_renderer(renderer),
  m_sdl_texture(SDL_CreateTexture(renderer.get_sdl_renderer(), 0,
                                  SDL_TEXTUREACCESS_TARGET,
                                  static_cast<int>(size.w),
                                  static_cast<int>(size.h))),
  m_drawable(true),
  m_cached_size(size)
{
}

Texture::~Texture()
{
  if (m_sdl_texture)
    SDL_DestroyTexture(m_sdl_texture);
}

SDL_Texture*
Texture::get_sdl_texture() const
{
  return m_sdl_texture;
}

Size
Texture::get_size() const
{
  return m_cached_size;
}

const Renderer&
Texture::get_renderer() const
{
  return m_renderer;
}
