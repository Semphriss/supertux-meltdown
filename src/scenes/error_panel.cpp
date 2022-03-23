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

#include "scenes/error_panel.hpp"

#include "make_unique.hpp"

#include "fs/file.hpp"
#include "game/game_manager.hpp"
#include "transitions/transition.hpp"
#include "video/drawing_context.hpp"

ErrorPanel::ErrorPanel(GameManager& game_manager, const Scene& target_scene) :
  Scene(game_manager),
  m_scene(target_scene),
  m_image(),
  m_font_path()
{
  reset_caches();
}

void
ErrorPanel::event(const SDL_Event& event)
{
  switch(event.type)
  {
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym)
      {
        case SDLK_ESCAPE:
          m_game_manager.pop_scene();
          break;

        default:
          break;
      }
      break;

    case SDL_FINGERDOWN:
      m_game_manager.pop_scene();
      break;


    case SDL_WINDOWEVENT:
      switch(event.window.event)
      {
        case SDL_WINDOWEVENT_RESIZED:
          reset_caches();
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
ErrorPanel::update(float dt_sec)
{
}

void
ErrorPanel::draw(DrawingContext& context) const
{
  auto& w = m_game_manager.get_window();

  Rect rect(w.get_size());

  rect.x1 += 32.f;
  rect.y1 += 32.f;
  rect.x2 -= 32.f;
  rect.y2 -= 32.f;

  context.draw_texture(*m_image, m_image->get_size(), w.get_size(), 0.0f,
                       Color(0.2f, 0.2f, 0.2f), Renderer::Blend::BLEND, 1);

  context.draw_text("An error occured", rect, Renderer::TextAlign::TOP_MID,
                    m_font_path, 24, Color(1.0f, 1.0f, 1.0f),
                    Renderer::Blend::BLEND, true, 2);

  context.draw_text("Lorem ipsum dolor sit amet consectetur adiscipling elit\nerewr",
                    rect, Renderer::TextAlign::BOTTOM_MID, m_font_path, 12,
                    Color(1.0f, 1.0f, 1.0f), Renderer::Blend::BLEND, true, 2);
}

void
ErrorPanel::enter()
{
}

void
ErrorPanel::leave(bool to_parent)
{
}

bool
ErrorPanel::quit(bool can_abort)
{
  return true;
}

void
ErrorPanel::reset_caches()
{
  auto& w = m_game_manager.get_window();

  m_image = w.create_texture(w.get_size());
  DrawingContext dc(w.get_renderer());
  m_scene.draw(dc);
  dc.render(m_image.get());

  m_font_path = File::get_os_path("fonts/SuperTux-Medium.ttf");
}

bool
ErrorPanel::recover_from_error(const std::exception& err)
{
  return true;
}
