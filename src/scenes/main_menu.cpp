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

#include "scenes/main_menu.hpp"

#include "make_unique.hpp"

#include "fs/file.hpp"
#include "game/game_manager.hpp"
#include "transitions/transition.hpp"
#include "video/drawing_context.hpp"

MainMenu::MainMenu(GameManager& game_manager) :
  Scene(game_manager),
  m_image_path(File::get_os_path("images/background.png"))
{
}

void
MainMenu::event(const SDL_Event& event)
{
  switch(event.type)
  {
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym)
      {
        case SDLK_ESCAPE:
          m_game_manager.pop_scene(Transition::Type::DISSOLVE);
          break;

        default:
          break;
      }
      break;

    default:
      break;
  }
}

void
MainMenu::update(float dt_sec)
{
}

void
MainMenu::draw(DrawingContext& context) const
{
  context.draw_filled_rect(m_game_manager.get_window().get_size(),
                           Color(0.7f, 0.9f, 1.0f),
                           Renderer::Blend::NONE, 0);

  auto& w = context.get_renderer().get_window();
  auto& tex = w.load_texture(m_image_path);
  context.draw_texture(tex, tex.get_size(), w.get_size(), 0.0f,
                       Color(1.0f, 1.0f, 1.0f), Renderer::Blend::BLEND, 1);
}

void
MainMenu::enter()
{
}

void
MainMenu::leave(bool to_parent)
{
}

bool
MainMenu::quit(bool can_abort)
{
  return true;
}
