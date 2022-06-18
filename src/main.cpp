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

#include <iostream>
#include <memory>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "scenes/main_menu.hpp"
#include "scenes/scene.hpp"
#include "util/color.hpp"
#include "util/rect.hpp"
#include "util/size.hpp"
#include "util/vector.hpp"
#include "video/drawing_context.hpp"
#include "video/window.hpp"

int main()
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
    std::unique_ptr<Scene> scene = std::make_unique<MainMenu>();

    while(scene)
    {
      SDL_Event e;
      while (SDL_PollEvent(&e))
      {
        scene->event(e);

        if (e.type == SDL_QUIT)
        {
          scene = nullptr;
          break;
        }
      }

      if (!scene)
        break;

      scene->update(0.01f);

      dc.target_size = w.get_size();

      scene->draw(dc);

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
