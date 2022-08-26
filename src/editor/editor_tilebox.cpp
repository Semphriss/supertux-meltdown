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

#include "editor/editor_tilebox.hpp"

#include "editor/editor_tilemap.hpp"

// Is allowed to differ from src/editor/editor_tilemap.cpp's g_tile_size
static const Size g_tile_size(32.0f, 32.0f);

static const int TILEBOX_WIDTH = std::ceil(g_tile_size.w * 4);

EditorTilebox::EditorTilebox(EditorTilemap& parent,
                             const std::vector<std::string>& tileset,
                             const std::string& data_folder) :
  m_parent(parent),
  m_tileset(tileset),
  m_data_folder(data_folder),
  m_mouse_pos(),
  m_mouse_down(false)
{
}

bool
EditorTilebox::event(const SDL_Event& event)
{
  switch (event.type)
  {
    case SDL_MOUSEBUTTONDOWN:
      if (event.button.x < TILEBOX_WIDTH)
      {
        m_mouse_down = true;
        return true;
      }
      break;

    case SDL_MOUSEBUTTONUP:
      if (m_mouse_down)
      {
        if (m_mouse_pos.x < TILEBOX_WIDTH)
        {
          size_t id = int(m_mouse_pos.x / 32.0f)
                    + int(m_mouse_pos.y / 32.0f) * 4;

          if (id < m_tileset.size())
          {
            m_parent.set_tile_id(id);
          }
        }
        m_mouse_down = false;
        return true;
      }
      break;

    case SDL_MOUSEMOTION:
      m_mouse_pos = Vector(event.motion.x, event.motion.y);
      return m_mouse_down;
      break;

    default:
      break;
  }

  return false;
}

void
EditorTilebox::update(float dt_sec)
{
}

void
EditorTilebox::draw(DrawingContext& context) const
{
  context.draw_filled_rect(Rect(0, 0, TILEBOX_WIDTH, context.target_size.h),
                           Color(1.0f, 1.0f, 1.0f, 0.5f), Blend::BLEND);

  for (size_t i = 0; i < m_tileset.size(); i++)
  {
    const auto& tile = m_tileset.at(i);
    size_t x = i % 4;
    size_t y = i / 4;

    Rect pos(Vector(x, y) * g_tile_size.vector(), g_tile_size);

    if (!tile.empty())
    {
      context.draw_texture(m_data_folder + tile, Rect(), pos,
                           Color(1.0f, 1.0f, 1.0f), Blend::BLEND);
    }

    if (pos.contains(m_mouse_pos))
    {
      float o = m_mouse_down ? 0.5f : 0.25f;
      context.draw_filled_rect(pos, Color(1.0f, 1.0f, 1.0f, o), Blend::BLEND);
    }
  }
}
