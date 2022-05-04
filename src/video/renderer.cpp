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

#include "video/renderer.hpp"

#include "video/window.hpp"

Renderer::Renderer(const Window& window) :
  m_sdl_renderer(SDL_CreateRenderer(window.get_sdl_window(), -1, 0))
{
}

Renderer::~Renderer()
{
  if (m_sdl_renderer)
    SDL_DestroyRenderer(m_sdl_renderer);
}
