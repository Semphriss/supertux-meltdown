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

#include "SDL.h"

#include "util/color.hpp"
#include "util/log.hpp"
#include "video/drawing_context.hpp"

GameManager::GameManager() :
  m_window(),
  m_quit(false)
{
}

int
GameManager::run()
{
  SDL_Init(SDL_INIT_VIDEO);

  m_window = Window::create_window(Window::VideoSystem::SDL);
  m_window->set_title("SuperTux Meltdown");
  m_window->set_resizable(true);

  run_loops();

  m_window.reset();

  SDL_Quit();

  return 0;
}

int
GameManager::run_loops()
{
  while(!m_quit)
  {
    handle_events();
    handle_update();
    handle_draw();

    SDL_Delay(1);
  }

  return 0;
}

void
GameManager::handle_events()
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    switch (e.type)
    {
      case SDL_QUIT:
        m_quit = true;
        break;

      default:
        break;
    }
  }
}

void
GameManager::handle_update()
{
}

void
GameManager::handle_draw()
{
  DrawingContext context(m_window->get_renderer());

  context.draw_filled_rect(m_window->get_size(), Color(),
                            Renderer::Blend::NONE, 0);

  context.render();
}

