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

#include "SDL2/SDL.h"

class Color;
class Rect;
class Window;

class Renderer final
{
public:
  Renderer(const Window& window);
  ~Renderer();

  void flush();

  void draw_filled_rect(const Rect& rect, const Color& color);

private:
  SDL_Renderer* m_sdl_renderer;

private:
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
};

#endif
