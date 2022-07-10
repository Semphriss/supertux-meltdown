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

#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "util/math.hpp"
#include "video/drawing_context.hpp"

static const std::vector<std::string> g_tiles = {
  "",
  "../data/images/tiles/block.png",
  "../data/images/tiles/brick.png"
};

static const int g_tile_null = 0;

static const Size g_tile_size(32.0f, 32.0f);

TilemapEditor::TilemapEditor(SceneController& scene_controller) :
  Scene(scene_controller),
  m_tilemap(),
  m_camera(0.0f, 0.0f),
  m_moving_camera(false),
  m_zoom(1.0f),
  m_mouse_pos(),
  m_tilemap_offset(0.0f, 0.0f)
{
  m_tilemap.resize(5);

  for (auto& row : m_tilemap)
    row.resize(10, g_tile_null);
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
            float x = static_cast<float>(event.button.x);
            float y = static_cast<float>(event.button.y);

            int tile_x = std::floor((x - m_camera.x) / (32.0f * m_zoom));
            int tile_y = std::floor((y - m_camera.y) / (32.0f * m_zoom));

            resize_tilemap_to(Vector(tile_x, tile_y));

            tile_x += m_tilemap_offset.x;
            tile_y += m_tilemap_offset.y;

            ++m_tilemap.at(tile_y).at(tile_x) %= g_tiles.size();
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

    case SDL_KEYDOWN:
      if (event.key.keysym.mod & KMOD_CTRL)
      {
        switch (event.key.keysym.sym)
        {
          case SDLK_s:
            try
            {
              save("../data/levels/level.bin");
            }
            catch (const std::exception& e)
            {
              std::cerr << e.what() << std::endl;
            }
            break;

          case SDLK_o:
            try
            {
              load("../data/levels/level.bin");
            }
            catch (const std::exception& e)
            {
              std::cerr << e.what() << std::endl;
            }
            break;
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

  context.push_transform();
  context.get_transform().move(m_camera);
  context.get_transform().scale(Size(m_zoom, m_zoom));

  if (m_tilemap.size() == 0 || m_tilemap.at(0).size() == 0)
    return;

  context.get_transform().move(-m_tilemap_offset * g_tile_size.vector());

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
  if (m_tilemap.size() > 0)
  {
    for (float x = 0.0f; x <= m_tilemap.at(0).size(); x++)
    {
      context.draw_line(Vector(x * 32.0f, 0.0f), Vector(x * 32.0f, m_tilemap.size() * 32.0f),
                        Color(1.0f, 1.0f, 1.0f, 0.5f), Blend::BLEND);
    }

    for (int y = 0.0f; y <= m_tilemap.size(); y++)
    {
      context.draw_line(Vector(0.0f, y * 32.0f), Vector(m_tilemap.at(0).size() * 32.0f, y * 32.0f),
                        Color(1.0f, 1.0f, 1.0f, 0.5f), Blend::BLEND);
    }
  }

  context.pop_transform();

  context.draw_text("Press Ctrl+S to save and Ctrl+O to load",
                    "../data/fonts/SuperTux-Medium.ttf", 12,
                    TextAlign::TOP_LEFT, Rect(context.target_size).grown(-8.0f),
                    Color(1.0f, 1.0f, 1.0f), Blend::BLEND);
}

void
TilemapEditor::load(const std::string& file)
{
  std::ifstream input(file, std::ios::binary);
  std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

  if (buffer.size() < 8)
  {
    throw std::runtime_error("Can't load file " + file + ": no dimensions");
  }

  auto w = ((buffer[0] * 256 + buffer[1]) * 256 + buffer[2]) * 256 + buffer[3];
  auto h = ((buffer[4] * 256 + buffer[5]) * 256 + buffer[6]) * 256 + buffer[7];

  if (buffer.size() < 8 + w * h)
  {
    throw std::runtime_error("Can't load file " + file + ": wrong size ("
                              + std::to_string(w) + "x" + std::to_string(h)
                              + " != " + std::to_string(buffer.size() - 8));
  }

  m_tilemap.clear();
  m_tilemap.resize(h);

  for (int y = 0; y < h; y++)
  {
    m_tilemap.at(y).resize(w);

    for (int x = 0; x < w; x++)
    {
      int tile = buffer[y * w + x + 8];

      if (tile >= g_tiles.size())
      {
        throw std::runtime_error("Can't load file " + file + ": tile overflow "
                                + std::to_string(tile) + " at location "
                                + std::to_string(x) + "x" + std::to_string(y));
      }

      m_tilemap.at(y).at(x) = tile;
    }
  }
}

void
TilemapEditor::save(const std::string& file) const
{
  std::ofstream output(file, std::ios::binary);

  auto h = m_tilemap.size(), w = h ? m_tilemap.at(0).size() : 0;
  char buffer[w * h + 8];

  buffer[0] = (w / 16777216) % 256;
  buffer[1] = (w / 65536) % 256;
  buffer[2] = (w / 255) % 256;
  buffer[3] = w % 256;
  buffer[4] = (h / 16777216) % 256;
  buffer[5] = (h / 65536) % 256;
  buffer[6] = (h / 255) % 256;
  buffer[7] = h % 256;

  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      buffer[y * w + x + 8] = m_tilemap.at(y).at(x);
    }
  }

  output.write(buffer, w * h + 8);
}

void
TilemapEditor::resize_tilemap_to(const Vector& tilemap_point)
{
  int x = static_cast<int>(tilemap_point.x + m_tilemap_offset.x);
  int y = static_cast<int>(tilemap_point.y + m_tilemap_offset.y);

  int width = (m_tilemap.size() > 0) ? m_tilemap.at(0).size() : 1;

  if (y < 0)
  {
    m_tilemap.resize(m_tilemap.size() - y);

    for (int i = m_tilemap.size() - 1; i >= -y; i--)
    {
      m_tilemap.at(i) = std::move(m_tilemap.at(i + y));
    }
  }
  else if (y >= m_tilemap.size())
  {
    m_tilemap.resize(y + 1);
  }

  if (x < 0)
  {
    for (auto& row : m_tilemap)
    {
      row.resize(width - x, g_tile_null);

      for (int i = row.size() - 1; i >= -x; i--)
      {
        row.at(i) = std::move(row.at(i + x));
      }

      for (int i = 0; i < -x; i++)
      {
        row.at(i) = g_tile_null;
      }
    }
  }
  else if (x >= width)
  {
    for (auto& row : m_tilemap)
    {
      row.resize(x + 1, g_tile_null);
    }
  }
  else
  {
    for (auto& row : m_tilemap)
    {
      row.resize(width, g_tile_null);
    }
  }

  m_tilemap_offset += Vector((x < 0) ? -x : 0, (y < 0) ? -y : 0);
}
