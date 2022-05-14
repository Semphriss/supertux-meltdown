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

#include "video/window.hpp"

Window::Window() :
  m_sdl_window(SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, 640, 400, 0)),
  m_renderer(*this)
{
  if (!m_sdl_window)
  {
    throw std::runtime_error("Can't create window:"
                             + std::string(SDL_GetError()));
  }
}

Window::~Window()
{
  if (m_sdl_window)
    SDL_DestroyWindow(m_sdl_window);
}

Renderer&
Window::get_renderer()
{
  return m_renderer;
}

Size
Window::get_size() const
{
  int w, h;
  SDL_GetWindowSize(m_sdl_window, &w, &h);
  return Size(static_cast<float>(w), static_cast<float>(h));
}

SDL_Window*
Window::get_sdl_window() const
{
  return m_sdl_window;
}
