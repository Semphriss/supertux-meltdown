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

#include "scenes/level_editor.hpp"

#include "make_unique.hpp"

#ifdef EMSCRIPTEN
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

#include "game/i_game_manager.hpp"
#include "objects/tile_map.hpp"
#include "video/drawing_context.hpp"

LevelEditor::LevelEditor(IGameManager& game_manager) :
  Scene(game_manager),
  m_sector(std::make_unique<GameObjectManager>()),
  m_current_tilemap(m_sector->add_object<TileMap>().second),
  m_camera()
{
  reset_caches();
}

void
LevelEditor::event(const SDL_Event& event)
{
  m_camera.event(event);

  switch (event.type)
  {
    case SDL_MOUSEBUTTONDOWN:
      if (m_current_tilemap && event.button.button == SDL_BUTTON_LEFT)
      {
        Vector mouse_pos(event.button.x, event.button.y);
        auto p = sp_to_tp(mouse_pos, *m_current_tilemap);
        m_current_tilemap->set_tile(sp_to_tp(mouse_pos, *m_current_tilemap), 0);
      }
      break;
  }
}

void
LevelEditor::update(float dt_sec)
{
  m_sector->update(dt_sec);
  m_camera.update(dt_sec);
}

void
LevelEditor::draw(DrawingContext& context) const
{
  auto& w = m_game_manager.get_window();
  context.draw_filled_rect(w.get_size(), Color(0.1f, 0.2f, 0.5f),
                           Renderer::Blend::BLEND, 0);

  context.push_transform();
  context.get_transform().move(-m_camera.get_position());
  context.get_transform().scale(Size(1.0f, 1.0f) * m_camera.get_zoom());

  m_sector->draw(context);

  context.pop_transform();
}

void
LevelEditor::enter()
{
}

void
LevelEditor::leave(bool to_parent)
{
}

bool
LevelEditor::quit(bool can_abort)
{
  return true;
}

void
LevelEditor::reset_caches()
{
}

bool
LevelEditor::recover_from_error(const std::exception& err)
{
  return true;
}

Vector
LevelEditor::sp_to_tp(const Vector& screen_point, const TileMap& tilemap) const
{
  return ((screen_point + m_camera.get_position() + tilemap.get_position()
          * m_camera.get_zoom()) / m_camera.get_zoom()
          / tilemap.get_tile_size()).floor();
}
