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

#include <iostream>

Window::Window() :
  m_sdl_window(SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, 640, 400, 0)),
  m_renderer(*this)
{
  if (!m_sdl_window)
  {
    throw std::runtime_error("Can't create window: "
                             + std::string(SDL_GetError()));
  }
}

Window::~Window()
{
  if (m_sdl_window)
    SDL_DestroyWindow(m_sdl_window);
}

bool
Window::get_bordered() const
{
  return !static_cast<bool>(SDL_GetWindowFlags(m_sdl_window)
                            & SDL_WINDOW_BORDERLESS);
}

float
Window::get_opacity() const
{
#if SDL_VERSION_ATLEAST(2, 0, 5)
  float opacity;
  if (SDL_GetWindowOpacity(m_sdl_window, &opacity))
  {
    std::string error(SDL_GetError());
    throw std::runtime_error("Could not get window opacity: " + error);
  }
  return opacity;
#else
  std::cerr << "Cannot get Window opacity because it requires SDL >= 2.0.5"
            << " and the compiled version is " << SDL_MAJOR_VERSION << "."
            << SDL_MINOR_VERSION << "." << SDL_PATCHLEVEL << std::endl;
#endif
}

Vector
Window::get_pos() const
{
  int x, y;
  SDL_GetWindowPosition(m_sdl_window, &x, &y);
  return Vector(static_cast<float>(x), static_cast<float>(y));
}

bool
Window::get_resizable() const
{
  return static_cast<bool>(SDL_GetWindowFlags(m_sdl_window)
                            & SDL_WINDOW_RESIZABLE);
}

Size
Window::get_size() const
{
  int w, h;
  SDL_GetWindowSize(m_sdl_window, &w, &h);
  return Size(static_cast<float>(w), static_cast<float>(h));
}

Window::Status
Window::get_status() const
{
  const auto& flags = SDL_GetWindowFlags(m_sdl_window);

  if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
    return Status::FULLSCREEN_DESKTOP;

  if (flags & SDL_WINDOW_FULLSCREEN)
    return Status::FULLSCREEN;

  if (flags & SDL_WINDOW_MAXIMIZED)
    return Status::MAXIMIZED;

  if (flags & SDL_WINDOW_MINIMIZED)
    return Status::MINIMIZED;

  return Status::NORMAL;
}

std::string
Window::get_title() const
{
  return std::string(SDL_GetWindowTitle(m_sdl_window));
}

bool
Window::get_visible() const
{
  return static_cast<bool>(SDL_GetWindowFlags(m_sdl_window) & SDL_WINDOW_SHOWN);
}

void
Window::set_bordered(bool bordered)
{
  SDL_SetWindowBordered(m_sdl_window, bordered ? SDL_TRUE : SDL_FALSE);
}

void
Window::set_opacity(float opacity)
{
#if SDL_VERSION_ATLEAST(2, 0, 5)
  if (SDL_SetWindowOpacity(m_sdl_window, opacity))
  {
    std::string error(SDL_GetError());
    throw std::runtime_error("Could not set window opacity " + error);
  }
#else
  std::cerr << "Cannot set Window opacity because it requires SDL >= 2.0.5"
            << " and the compiled version is " << SDL_MAJOR_VERSION << "."
            << SDL_MINOR_VERSION << "." << SDL_PATCHLEVEL << std::endl;
#endif
}

void
Window::set_pos(Vector pos)
{
  int x = static_cast<int>(pos.x);
  int y = static_cast<int>(pos.y);

  SDL_SetWindowPosition(m_sdl_window, x, y);
}

void
Window::set_resizable(bool resizable)
{
#if SDL_VERSION_ATLEAST(2, 0, 5)
  SDL_SetWindowResizable(m_sdl_window, resizable ? SDL_TRUE : SDL_FALSE);
#else
  std::cerr << "Cannot set Window resizable because it requires SDL >= 2.0.5"
            << " and the compiled version is " << SDL_MAJOR_VERSION << "."
            << SDL_MINOR_VERSION << "." << SDL_PATCHLEVEL << std::endl;
#endif
}

void
Window::set_size(const Size& size)
{
  SDL_SetWindowSize(m_sdl_window, static_cast<int>(size.w),
                    static_cast<int>(size.h));
}

void
Window::set_status(Status status)
{
  switch (status)
  {
    case Status::NORMAL:
      if (SDL_SetWindowFullscreen(m_sdl_window, 0))
      {
        std::string error(SDL_GetError());
        throw std::runtime_error("Could not restore window: " + error);
      }
      SDL_RestoreWindow(m_sdl_window);
      break;

    case Status::MINIMIZED:
      if (SDL_SetWindowFullscreen(m_sdl_window, 0))
      {
        std::string error(SDL_GetError());
        throw std::runtime_error("Could not restore window: " + error);
      }
      SDL_MinimizeWindow(m_sdl_window);
      break;

    case Status::MAXIMIZED:
      if (SDL_SetWindowFullscreen(m_sdl_window, 0))
      {
        std::string error(SDL_GetError());
        throw std::runtime_error("Could not restore window: " + error);
      }
      SDL_MaximizeWindow(m_sdl_window);
      break;

    case Status::FULLSCREEN:
      if (SDL_SetWindowFullscreen(m_sdl_window, SDL_WINDOW_FULLSCREEN))
      {
        std::string error(SDL_GetError());
        throw std::runtime_error("Could not make window fullscreen: " + error);
      }
      break;

    case Status::FULLSCREEN_DESKTOP:
      if (SDL_SetWindowFullscreen(m_sdl_window, SDL_WINDOW_FULLSCREEN_DESKTOP))
      {
        std::string error(SDL_GetError());
        throw std::runtime_error("Could not make window fullscreen: " + error);
      }
      break;
  }
}

void
Window::set_title(const std::string& title)
{
  SDL_SetWindowTitle(m_sdl_window, title.c_str());
}

void
Window::set_visible(bool visible)
{
  if (visible)
  {
    SDL_ShowWindow(m_sdl_window);
  }
  else
  {
    SDL_HideWindow(m_sdl_window);
  }
}

Renderer&
Window::get_renderer()
{
  return m_renderer;
}

SDL_Window*
Window::get_sdl_window() const
{
  return m_sdl_window;
}
