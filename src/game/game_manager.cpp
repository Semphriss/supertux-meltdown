//  SuperTux Meltdown - Semphris' take on the popular Linux platformer
//  Copyright (C) 2021-2022 Semphris <semphris@semphris.com>
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
#include "transitions/transition.hpp"
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

  push_scene(std::make_unique<MainMenu>(*this), Transition::Type::DISSOLVE);

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
GameManager::push_scene(std::unique_ptr<Scene> scene,
                        Transition::Type transition, float time)
{
  if (m_transition)
  {
    log_warn << "Attempt to push scene during a transition" << std::endl;
    return;
  }

  auto* from = m_scenes.empty() ? nullptr : m_scenes.back().get();
  auto* to = scene.get();

  m_scenes.push_back(std::move(scene));
  m_transition = Transition::create_transition(from, to, time, transition);

  if (from)
    from->leave(false);
}

void
GameManager::pop_scene(Transition::Type transition, float time)
{
  if (m_transition)
  {
    log_warn << "Attempt to pop scene during a transition" << std::endl;
    return;
  }

  if (m_scenes.size() < 1)
  {
    log_warn << "Cannot pop empty scene stack" << std::endl;
    return;
  }

  m_popping_scene = std::move(m_scenes.back());
  m_scenes.pop_back();

  auto* from = m_popping_scene.get();
  auto* to = m_scenes.empty() ? nullptr : m_scenes.back().get();

  m_transition = Transition::create_transition(from, to, time, transition);

  from->leave(true);
}

int
GameManager::run_loops()
{
  while(!empty())
  {
    handle_events();

    if (empty())
      break;

    handle_update();

    if (empty())
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
  while (SDL_PollEvent(&e) && !empty())
  {
    if (!m_transition)
    {
      m_scenes.back()->event(e);
    }
    else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
    {
      m_transition = nullptr;
    }

    switch (e.type)
    {
      case SDL_QUIT:
        for (auto it = m_scenes.rbegin(); it != m_scenes.rend(); it++)
        {
          if (!(*it)->quit(true))
          {
            break;
          }
          else
          {
            m_scenes.pop_back();
          }
        }
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

  if(m_transition && m_transition->update(dt_sec))
  {
    m_transition = nullptr;

    if (!m_scenes.empty())
    {
      m_scenes.back()->enter();
    }
  }

  if (!m_transition && !m_scenes.empty())
  {
    m_scenes.back()->update(dt_sec);
  }
}

void
GameManager::handle_draw()
{
  DrawingContext context(m_window->get_renderer());

  if (m_transition)
  {
    m_transition->draw(context);
  }
  else
  {
    m_scenes.back()->draw(context);
  }

  context.render();
}

bool
GameManager::empty() const
{
  return m_scenes.empty() && !m_transition;
}
