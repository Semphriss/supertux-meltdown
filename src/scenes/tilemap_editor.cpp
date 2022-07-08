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

#include "scenes/tilemap_editor.hpp"

#include <memory>

#include "util/math.hpp"
#include "video/drawing_context.hpp"

static const std::vector<std::string> g_tiles = {
  "",
  "../data/images/tiles/block.png",
  "../data/images/tiles/brick.png"
};

static const Size g_tile_size(32.0f, 32.0f);

TilemapEditor::TilemapEditor(SceneController& scene_controller) :
  Scene(scene_controller),
  m_tilemap(),
  m_camera(0.0f, 0.0f),
  m_moving_camera(false),
  m_zoom(1.0f),
  m_mouse_pos()
{
  m_tilemap.resize(5);

  for (auto& row : m_tilemap)
    row.resize(10);
}

void
TilemapEditor::event(const SDL_Event& event)
{
  switch (event.type)
  {
    case SDL_MOUSEBUTTONDOWN:
      switch (event.button.button)
      {
        case SDL_BUTTON_LEFT:
          {
            int x = event.button.x;
            int y = event.button.y;

            if (x - m_camera.x < 0 || y - m_camera.y < 0)
              return;

            int tile_x = (x - m_camera.x) / (32 * m_zoom);
            int tile_y = (y - m_camera.y) / (32 * m_zoom);

            if (tile_y < m_tilemap.size() && tile_x < m_tilemap.at(tile_y).size())
            {
              ++m_tilemap.at(tile_y).at(tile_x) %= g_tiles.size();
            }
          }
          break;

        case SDL_BUTTON_RIGHT:
          m_moving_camera = true;
          break;

        default:
          break;
      }
      break;

    case SDL_MOUSEBUTTONUP:
      switch (event.button.button)
      {
        case SDL_BUTTON_RIGHT:
          m_moving_camera = false;
          break;

        default:
          break;
      }
      break;

    case SDL_MOUSEMOTION:
      m_mouse_pos = Vector(event.motion.x, event.motion.y);
      if (m_moving_camera)
      {
        m_camera += Vector(event.motion.xrel, event.motion.yrel);
      }
      break;

    case SDL_MOUSEWHEEL:
      {
        float m_old_zoom = m_zoom;
        m_zoom += static_cast<float>(event.wheel.y) / 8.0f;
        m_zoom = Math::clamp(m_zoom, 0.5f, 2.0f);
        m_camera = m_mouse_pos - (m_mouse_pos - m_camera) * m_zoom / m_old_zoom;
      }
      break;

    default:
      break;
  }
  
}

void
TilemapEditor::update(float dt_sec)
{
}

void
TilemapEditor::draw(DrawingContext& context) const
{
  context.draw_filled_rect(context.target_size, Color(0.1f, 0.2f, 0.4f),
                           Blend::NONE);

  context.push_transform();
  context.get_transform().move(m_camera);
  context.get_transform().scale(Size(m_zoom, m_zoom));

  if (m_tilemap.size() == 0 || m_tilemap.at(0).size() == 0)
    return;

  // Tiles
  for (int y = 0.0f; y < m_tilemap.size(); y++)
  {
    for (float x = 0.0f; x < m_tilemap.at(y).size(); x++)
    {
      if (g_tiles[m_tilemap.at(y).at(x)].empty())
        continue;

      Rect tile_rect(Vector(g_tile_size) * Vector(x, y), g_tile_size);

      context.draw_texture(g_tiles[m_tilemap.at(y).at(x)], {}, tile_rect,
                           Color(1.0f, 1.0f, 1.0f), Blend::BLEND);
    }
  }

  // Grid
  for (float x = 0.0f; x <= m_tilemap.at(0).size(); x++)
  {
    context.draw_line(Vector(x * 32.0f, 0.0f), Vector(x * 32.0f, 5.0f * 32.0f),
                      Color(1.0f, 1.0f, 1.0f, 0.5f), Blend::BLEND);
  }

  for (int y = 0.0f; y <= m_tilemap.size(); y++)
  {
    context.draw_line(Vector(0.0f, y * 32.0f), Vector(10.0f * 32.0f, y * 32.0f),
                      Color(1.0f, 1.0f, 1.0f, 0.5f), Blend::BLEND);
  }

  context.pop_transform();
}
