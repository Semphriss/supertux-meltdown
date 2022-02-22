//  SuperTux Meltdown - Semphris' take on the popular Linux platformer
//  Copyright (C) 2021 Semphris <semphris@protonmail.com>
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

#include "game/game_manager.hpp"

#include "make_unique.hpp"

#include "SDL.h"

#include "scenes/main_menu.hpp"
#include "util/color.hpp"
#include "util/log.hpp"
#include "video/drawing_context.hpp"
#include <memory>

int
GameManager::run()
{
  SDL_Init(SDL_INIT_VIDEO);

  m_window = Window::create_window(Window::VideoSystem::SDL);
  m_window->set_title("SuperTux Meltdown");
  m_window->set_resizable(true);

  push_scene(std::make_unique<MainMenu>(*this));

  m_last_frame = std::chrono::steady_clock::now();

  run_loops();

  m_window.reset();

  SDL_Quit();

  return 0;
}

const Window&
GameManager::get_window() const
{
  return *m_window;
}

void
GameManager::push_scene(std::unique_ptr<Scene> scene)
{
  m_scenes.push_back(std::move(scene));
}

void
GameManager::pop_scene()
{
  m_scenes.pop_back();
}

int
GameManager::run_loops()
{
  while(!m_scenes.empty())
  {
    handle_events();

    if (m_scenes.empty())
      break;

    handle_update();

    if (m_scenes.empty())
      break;

    handle_draw();

    SDL_Delay(1);
  }

  return 0;
}

void
GameManager::handle_events()
{
  SDL_Event e;
  while (SDL_PollEvent(&e) && !m_scenes.empty())
  {
    m_scenes.back()->event(e);

    switch (e.type)
    {
      case SDL_QUIT:
        m_scenes.clear();
        break;

      default:
        break;
    }
  }
}

void
GameManager::handle_update()
{
  auto t = std::chrono::steady_clock::now();
  auto diff = t - m_last_frame;
  float dt_sec = static_cast<float>(diff.count()) / 1000000000.f;
  m_last_frame = t;

  m_scenes.back()->update(dt_sec);
}

void
GameManager::handle_draw()
{
  DrawingContext context(m_window->get_renderer());
  m_scenes.back()->draw(context);
  context.render();
}
