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

#include "util/fs.hpp"
#include "video/renderer.hpp"

Texture::Texture(Renderer& renderer, const std::string& file, bool physfs) :
  m_renderer(renderer),
  m_sdl_texture(),
  m_drawable(false),
  m_cached_size()
{
  SDL_Surface* surface = nullptr;

  if (physfs)
  {
    surface = IMG_Load_RW(FS::get_rwops(file, FS::OP::READ), true);
  }
  else
  {
    surface = IMG_Load(file.c_str());
  }

  if (!surface)
  {
    throw std::runtime_error("Can't load image '" + file + "': "
                             + std::string(SDL_GetError()));
  }

  m_sdl_texture = SDL_CreateTextureFromSurface(renderer.get_sdl_renderer(),
                                               surface);
  SDL_FreeSurface(surface);

  if (!m_sdl_texture)
  {
    throw std::runtime_error("Can't create texture from image '" + file + "': "
                             + std::string(SDL_GetError()));
  }

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
  if (!m_sdl_texture)
  {
    throw std::runtime_error("Can't create texture: "
                             + std::string(SDL_GetError()));
  }
}

Texture::Texture(Renderer& renderer, SDL_Surface* surface, bool free_surface) :
  m_renderer(renderer),
  m_sdl_texture(),
  m_drawable(false),
  m_cached_size()
{
  if (!surface)
  {
    throw std::runtime_error("Can't create texture from null surface: "
                             + std::string(SDL_GetError()));
  }

  m_sdl_texture = SDL_CreateTextureFromSurface(renderer.get_sdl_renderer(),
                                               surface);

  if (free_surface)
    SDL_FreeSurface(surface);

  if (!m_sdl_texture)
  {
    throw std::runtime_error("Can't create texture from surface: "
                             + std::string(SDL_GetError()));
  }

  int w, h;
  SDL_QueryTexture(m_sdl_texture, nullptr, nullptr, &w, &h);
  m_cached_size.w = static_cast<float>(w);
  m_cached_size.h = static_cast<float>(h);
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
