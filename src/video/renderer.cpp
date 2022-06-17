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
#include "util/vector.hpp"
#include "video/drawing_context.hpp"
#include "video/texture.hpp"
#include "video/window.hpp"

Renderer::Renderer(const Window& window) :
  m_sdl_renderer(SDL_CreateRenderer(window.get_sdl_window(), -1, 0))
{
  if (!m_sdl_renderer)
  {
    throw std::runtime_error("Can't create renderer:"
                             + std::string(SDL_GetError()));
  }
}

Renderer::~Renderer()
{
  if (m_sdl_renderer)
    SDL_DestroyRenderer(m_sdl_renderer);

  for (auto* context : m_bound_contexts)
  {
    context->unbind(this);
  }
}

void
Renderer::flush()
{
  SDL_RenderPresent(m_sdl_renderer);
  SDL_SetRenderDrawColor(m_sdl_renderer, 0, 0, 0, 0);
  SDL_RenderClear(m_sdl_renderer);
}

void
Renderer::draw_filled_rect(const Rect& rect, const Color& color, Blend blend)
{
  SDL_SetRenderDrawColor(m_sdl_renderer,
                         static_cast<Uint8>(color.r * 255.f),
                         static_cast<Uint8>(color.g * 255.f),
                         static_cast<Uint8>(color.b * 255.f),
                         static_cast<Uint8>(color.a * 255.f));

  SDL_SetRenderDrawBlendMode(m_sdl_renderer, static_cast<SDL_BlendMode>(blend));

  SDL_FRect sdl_rect;
  sdl_rect.x = rect.x1;
  sdl_rect.y = rect.y1;
  sdl_rect.w = rect.width();
  sdl_rect.h = rect.height();

  SDL_RenderFillRectF(m_sdl_renderer, &sdl_rect);
}

void
Renderer::draw_line(const Vector& p1, const Vector& p2, const Color& color,
                    Blend blend)
{
  SDL_SetRenderDrawColor(m_sdl_renderer,
                         static_cast<Uint8>(color.r * 255.f),
                         static_cast<Uint8>(color.g * 255.f),
                         static_cast<Uint8>(color.b * 255.f),
                         static_cast<Uint8>(color.a * 255.f));

  SDL_SetRenderDrawBlendMode(m_sdl_renderer, static_cast<SDL_BlendMode>(blend));

  SDL_RenderDrawLineF(m_sdl_renderer, p1.x, p1.y, p2.x, p2.y);
}

void
Renderer::draw_texture(const Texture& texture, const Rect& src, const Rect& dst,
                       const Color& color, Blend blend)
{
  SDL_SetTextureColorMod(texture.get_sdl_texture(),
                         static_cast<Uint8>(color.r * 255.f),
                         static_cast<Uint8>(color.g * 255.f),
                         static_cast<Uint8>(color.b * 255.f));

  SDL_SetTextureAlphaMod(texture.get_sdl_texture(),
                         static_cast<Uint8>(color.a * 255.f));

  SDL_SetTextureBlendMode(texture.get_sdl_texture(),
                          static_cast<SDL_BlendMode>(blend));

  SDL_Rect s;
  SDL_FRect d;
  s.x = static_cast<int>(src.x1);
  s.y = static_cast<int>(src.y1);
  s.w = static_cast<int>(src.width());
  s.h = static_cast<int>(src.height());
  d.x = dst.x1;
  d.y = dst.y1;
  d.w = dst.width();
  d.h = dst.height();

  // TODO: Add support for angle, center point and flip
  SDL_RenderCopyExF(m_sdl_renderer, texture.get_sdl_texture(), &s, &d, 0.0f,
                    NULL, SDL_FLIP_NONE);
}

SDL_Renderer*
Renderer::get_sdl_renderer() const
{
  return m_sdl_renderer;
}

void
Renderer::bind_lifetime(DrawingContext& context)
{
  m_bound_contexts.push_back(&context);
}
