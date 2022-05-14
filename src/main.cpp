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
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include <iostream>

#include "util/color.hpp"
#include "util/rect.hpp"
#include "util/size.hpp"
#include "util/vector.hpp"
#include "video/font.hpp"
#include "video/texture.hpp"
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

  // Scope ensures all SDL-dependant objects (window, renderer, etc.) are
  // destroyed before de-initing libraries
  {
    Window w;
    Renderer& r = w.get_renderer();
    Texture t(r, "../data/images/background.png");
    Font f("../data/fonts/SuperTux-Medium.ttf", 16);

    auto ft = f.draw_text(r, "Lorem ipsum dolor", 150.0f);

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

      auto window_size = w.get_size();

      r.draw_texture(t, t.get_size(), window_size, Color(1.0f, 1.0f, 1.0f),
                    Renderer::Blend::BLEND);

      Rect font_rect(Vector(0.0f, window_size.h - ft->get_size().h),
                    ft->get_size());
      r.draw_texture(*ft, ft->get_size(), font_rect, Color(0.0f, 0.0f, 0.0f),
                    Renderer::Blend::BLEND);

      r.flush();

      SDL_Delay(10);
    }
  }

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}
