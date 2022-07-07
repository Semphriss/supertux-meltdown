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

#include "video/drawing_context.hpp"

static const std::vector<std::string> g_tiles = {
  "",
  "../data/images/tiles/block.png",
  "../data/images/tiles/brick.png"
};

static const Size g_tile_size(32.0f, 32.0f);

TilemapEditor::TilemapEditor(SceneController& scene_controller) :
  Scene(scene_controller),
  m_tilemap()
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
      {
        int x = event.button.x;
        int y = event.button.y;

        int tile_x = x / 32;
        int tile_y = y / 32;

        if (tile_y < m_tilemap.size() && tile_x < m_tilemap.at(tile_y).size())
        {
          ++m_tilemap.at(tile_y).at(tile_x) %= g_tiles.size();
        }
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

  if (m_tilemap.size() == 0 || m_tilemap.at(0).size() == 0)
    return;

  // Tiles
  for (int y = 0.0f; y < 5.0f; y++)
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
}
