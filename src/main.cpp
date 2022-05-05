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

#include "SDL2/SDL.h"

#include <iostream>

#include "util/color.hpp"
#include "util/rect.hpp"
#include "video/window.hpp"

int main()
{
  if (SDL_Init(SDL_INIT_VIDEO))
  {
    std::cerr << "Could not init SDL: " << SDL_GetError() << std::endl;
    return 1;
  }

  // Scope ensures all SDL-dependant objects (window, renderer, etc.) are
  // destroyed before de-initing libraries
  {
    Window w;
    Renderer& r = w.get_renderer();

    bool quit = false;
    while(!quit)
    {
      SDL_Event e;
      while (SDL_PollEvent(&e))
      {
        if (e.type == SDL_QUIT)
        {
          quit = true;
          break;
        }
      }

      r.draw_filled_rect(Rect(0.0f, 0.0f, 640.0f, 400.0f),
                        Color(0.1f, 0.2f, 0.4f));
      r.flush();

      SDL_Delay(10);
    }
  }

  SDL_Quit();

  return 0;
}
