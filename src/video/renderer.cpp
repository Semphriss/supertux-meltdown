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

#include <stdexcept>

#include "util/color.hpp"
#include "util/rect.hpp"
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

void
Renderer::flush()
{
  SDL_RenderPresent(m_sdl_renderer);
  SDL_SetRenderDrawColor(m_sdl_renderer, 0, 0, 0, 0);
  SDL_RenderClear(m_sdl_renderer);
}

void
Renderer::draw_filled_rect(const Rect& rect, const Color& color)
{
  SDL_SetRenderDrawColor(m_sdl_renderer,
                         static_cast<Uint8>(color.r * 255.f),
                         static_cast<Uint8>(color.g * 255.f),
                         static_cast<Uint8>(color.b * 255.f),
                         static_cast<Uint8>(color.a * 255.f));

  SDL_FRect sdl_rect;
  sdl_rect.x = rect.x1;
  sdl_rect.y = rect.y1;
  sdl_rect.w = rect.width();
  sdl_rect.h = rect.height();
  SDL_RenderFillRectF(m_sdl_renderer, &sdl_rect);
}
