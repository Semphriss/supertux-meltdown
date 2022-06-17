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

#ifndef HEADER_STM_VIDEO_RENDERER_HPP
#define HEADER_STM_VIDEO_RENDERER_HPP

#include <vector>

#include "SDL2/SDL.h"

class Color;
class DrawingContext;
class Rect;
class Texture;
class Vector;
class Window;

enum class Blend
{
  NONE = SDL_BLENDMODE_NONE,
  BLEND = SDL_BLENDMODE_BLEND,
  ADD = SDL_BLENDMODE_ADD,
  MODULATE = SDL_BLENDMODE_MOD
};

class Renderer final
{
public:
  Renderer(const Window& window);
  ~Renderer();

  void flush();

  void draw_filled_rect(const Rect& rect, const Color& color, Blend blend);
  void draw_line(const Vector& p1, const Vector& p2, const Color& color,
                 Blend blend);
  void draw_texture(const Texture& texture, const Rect& src, const Rect& dst,
                    const Color& color, Blend blend);

  SDL_Renderer* get_sdl_renderer() const;

  void bind_lifetime(DrawingContext& context);

private:
  SDL_Renderer* m_sdl_renderer;
  std::vector<DrawingContext*> m_bound_contexts;

private:
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
};

#endif
