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
  m_image(),
  m_font_path()
{
  reset_caches();
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

    case SDL_FINGERDOWN:
      m_game_manager.pop_scene(Transition::Type::DISSOLVE);
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

  auto& w = m_game_manager.get_window();
  context.draw_texture(*m_image, m_image->get_size(), w.get_size(), 0.0f,
                       Color(1.0f, 1.0f, 1.0f), Renderer::Blend::BLEND, 1);

  context.draw_text("Copyright (c) 2021-2022, Semphris\nThis program comes with"
                    " ABSOLUTELY NO WARRANTY.\nThis is free software, and you"
                    " are welcome to redistribute it\nunder certain conditions;"
                    " see the LICENSE file for details.", w.get_size(),
                    Renderer::TextAlign::BOTTOM_LEFT, m_font_path, 12,
                    Color(1.0f, 1.0f, 1.0f), Renderer::Blend::BLEND, true, 2);

  context.draw_text(Window::get_video_system_tag(w.get_type()), w.get_size(),
                    Renderer::TextAlign::BOTTOM_RIGHT, m_font_path, 12,
                    Color(1.0f, 1.0f, 1.0f), Renderer::Blend::BLEND, true, 2);
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

void
MainMenu::reset_caches()
{
  auto& w = m_game_manager.get_window();

  m_image = &w.load_texture(File::get_os_path("images/background.png"));
  m_font_path = File::get_os_path("fonts/SuperTux-Medium.ttf");
}
