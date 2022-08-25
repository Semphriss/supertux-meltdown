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

#ifndef HEADER_STM_VIDEO_WINDOW_HPP
#define HEADER_STM_VIDEO_WINDOW_HPP

#include "SDL2/SDL.h"

#include "util/size.hpp"
#include "util/vector.hpp"
#include "video/renderer.hpp"

class Window final
{
public:
  enum class Status {
    NORMAL,
    MINIMIZED,
    MAXIMIZED,
    FULLSCREEN,
    FULLSCREEN_DESKTOP
  };

public:
  Window();
  ~Window();

  bool get_bordered() const;
  float get_opacity() const;
  Vector get_pos() const;
  bool get_resizable() const;
  Size get_size() const;
  Status get_status() const;
  std::string get_title() const;
  bool get_visible() const;

  void set_bordered(bool bordered);
  void set_opacity(float opacity);
  void set_pos(Vector pos);
  void set_resizable(bool resizable);
  void set_size(const Size& size);
  void set_status(Status status);
  void set_title(const std::string& title);
  void set_visible(bool visible);

  Renderer& get_renderer();

  SDL_Window* get_sdl_window() const;

private:
  SDL_Window* m_sdl_window;
  Renderer m_renderer;

private:
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
};

#endif
