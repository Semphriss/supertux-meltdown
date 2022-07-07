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

#include "game/game_manager.hpp"

#include <chrono>
#include <iostream>
#include <memory>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "scenes/tilemap_editor.hpp"
#include "video/drawing_context.hpp"
#include "video/window.hpp"

GameManager::GameManager() :
  m_scene_manager(this)
{
}

int
GameManager::run(int argc, char** argv)
{
  if (SDL_Init(SDL_INIT_VIDEO))
  {
    std::cerr << "Could not init SDL: " << SDL_GetError() << std::endl;
    return 1;
  }

  if (!IMG_Init(IMG_INIT_PNG))
  {
    std::cerr << "Could not init SDL_image: " << IMG_GetError() << std::endl;
    return 1;
  }

  if (TTF_Init())
  {
    std::cerr << "Could not init SDL_ttf: " << TTF_GetError() << std::endl;
    return 1;
  }

  try
  {
    Window w;
    Renderer& r = w.get_renderer();
    DrawingContext dc;
    auto last_time = std::chrono::steady_clock::now();

    auto& controller = m_scene_manager.get_controller();
    m_scene_manager.push_scene(std::make_unique<TilemapEditor>(controller));

    while(!m_scene_manager.empty())
    {
      SDL_Event e;
      while (SDL_PollEvent(&e) && !m_scene_manager.empty())
      {
        m_scene_manager.event(e);

        if (e.type == SDL_QUIT)
        {
          m_scene_manager.quit();
          break;
        }
      }

      if (m_scene_manager.empty())
        break;

      // https://en.cppreference.com/w/cpp/chrono/steady_clock says:
      //   "This clock [...] is most suitable for measuring intervals."
      // std::chrono::high_resolution_clock is merely an alias to another clock,
      // which may or may not be steady_clock, and which can have huge jumps if
      // the user changes their computer time, including back in time. See the
      // notes: https://en.cppreference.com/w/cpp/chrono/high_resolution_clock
      auto time = std::chrono::steady_clock::now();
      float diff = static_cast<float>((time - last_time).count())
                    * static_cast<float>(decltype(time)::period::num)
                    / static_cast<float>(decltype(time)::period::den);
      last_time = time;

      m_scene_manager.update(0.01f);

      if (m_scene_manager.empty())
        break;

      dc.target_size = w.get_size();

      m_scene_manager.draw(dc);

      dc.render(r);
      dc.clear();

      SDL_Delay(10);
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << "Fatal error: " << e.what() << std::endl;
  }
  catch(const std::string& s)
  {
    std::cerr << "Fatal error: " << s << std::endl;
  }
  catch(const char* s)
  {
    std::cerr << "Fatal error: " << s << std::endl;
  }
  catch(...)
  {
    std::cerr << "Fatal error (unknown type)" << std::endl;
  }

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}
