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

#include "editor/editor_tilemap.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "util/fs.hpp"
#include "util/math.hpp"
#include "video/drawing_context.hpp"

static const std::vector<std::string> g_tiles = {
  "",
  "images/tiles/block.png",
  "images/tiles/brick.png"
};

// g_tile_null should probably be called g_tile_default, the code allows it to
// point to a non-null tile (g_tiles[g_tile_null] is allowed to be non-empty)
static const int g_tile_null = 0;

static const Size g_tile_size(32.0f, 32.0f);

EditorTilemap::EditorTilemap() :
  m_tilemap(),
  m_camera(),
  m_tilebox(*this, g_tiles),
  m_tilemap_offset(0.0f, 0.0f),
  m_tile_id(g_tile_null),
  m_mouse_pos()
{
  m_tilemap.resize(5);

  for (auto& row : m_tilemap)
    row.resize(10, g_tile_null);
}

void
EditorTilemap::event(const SDL_Event& event)
{
  if (m_tilebox.event(event))
    return;

  m_camera.event(event);

  switch (event.type)
  {
    case SDL_MOUSEBUTTONDOWN:
      switch (event.button.button)
      {
        case SDL_BUTTON_LEFT:
          {
            float x = static_cast<float>(event.button.x);
            float y = static_cast<float>(event.button.y);

            Vector tile_coord = screen_to_tilemap(Vector(x, y));

            resize_tilemap_to(tile_coord);

            tile_coord += m_tilemap_offset;

            m_tilemap.at(tile_coord.y).at(tile_coord.x) = m_tile_id;
          }
          break;

        default:
          break;
      }
      break;

    case SDL_MOUSEMOTION:
      m_mouse_pos = Vector(event.motion.x, event.motion.y);
      break;

    default:
      break;
  }
}

void
EditorTilemap::update(float dt_sec)
{
  m_camera.update(dt_sec);
}

void
EditorTilemap::draw(DrawingContext& context) const
{
  context.draw_filled_rect(context.target_size, Color(0.1f, 0.2f, 0.4f),
                           Blend::NONE);

  if (m_tilemap.size() != 0 && m_tilemap.at(0).size() != 0)
  {
    context.push_transform();
    m_camera.apply_transform(context);

    context.get_transform().move(-m_tilemap_offset * g_tile_size.vector());

    // Tiles
    for (int y = 0.0f; y < m_tilemap.size(); y++)
    {
      for (float x = 0.0f; x < m_tilemap.at(y).size(); x++)
      {
        // This does not use g_tile_null because other tiles may need to be
        // empty
        if (g_tiles[m_tilemap.at(y).at(x)].empty())
          continue;

        Rect tile_rect(Vector(g_tile_size) * Vector(x, y), g_tile_size);

        context.draw_texture(g_tiles.at(m_tilemap.at(y).at(x)), true, {},
                             tile_rect, Color(1.0f, 1.0f, 1.0f), Blend::BLEND);
      }
    }

    // Grid
    if (m_tilemap.size() > 0)
    {
      for (float x = 0.0f; x <= m_tilemap.at(0).size(); x++)
      {
        context.draw_line(Vector(x * 32.0f, 0.0f),
                          Vector(x * 32.0f, m_tilemap.size() * 32.0f),
                          Color(1.0f, 1.0f, 1.0f, 0.5f), Blend::BLEND);
      }

      for (int y = 0.0f; y <= m_tilemap.size(); y++)
      {
        context.draw_line(Vector(0.0f, y * 32.0f),
                          Vector(m_tilemap.at(0).size() * 32.0f, y * 32.0f),
                          Color(1.0f, 1.0f, 1.0f, 0.5f), Blend::BLEND);
      }
    }

    context.pop_transform();
  }

  if (m_tile_id < g_tiles.size() && !g_tiles[m_tile_id].empty())
  {
    Rect pos(tilemap_to_screen(screen_to_tilemap(m_mouse_pos)),
             g_tile_size * m_camera.get_zoom());

    context.draw_texture(g_tiles[m_tile_id], true, Rect(), pos,
                         Color(1.0f, 1.0f, 1.0f,  0.5f), Blend::BLEND);
  }

  m_tilebox.draw(context);

  /** @todo de-hardcode the tilebox width here */
  context.draw_text("Press Ctrl+S to save and Ctrl+O to load",
                    "fonts/SuperTux-Medium.ttf", true, 12, TextAlign::TOP_LEFT,
                    Rect(context.target_size).with_x1(128).grown(-8.0f),
                    Color(1.0f, 1.0f, 1.0f), Blend::BLEND);
}

void
EditorTilemap::load_tilemap(const std::string& file)
{
  auto* surface = SDL_LoadBMP_RW(FS::get_rwops(file.c_str(), FS::OP::READ),
                                 true);

  if (!surface)
  {
    throw std::runtime_error("Can't open BMP at '" + file + "': "
                            + std::string(SDL_GetError()));
  }

  auto* data = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ARGB8888, 0);
  SDL_FreeSurface(surface);

  if (!data)
  {
    throw std::runtime_error("Can't convert '" + file + "' to ARGB8888: "
                            + std::string(SDL_GetError()));
  }

  auto w = data->w;
  auto h = data->h;
  auto bpp = data->format->BytesPerPixel;

  m_tilemap.clear();
  m_tilemap.resize(h);

  for (int y = 0; y < h; y++)
  {
    m_tilemap.at(y).resize(w);

    for (int x = 0; x < w; x++)
    {
      int tile = *((Uint32*) ((Uint8*) data->pixels + y * data->pitch
                  + x * bpp)) ^ 0xff000000;

      if (tile >= g_tiles.size())
      {
        throw std::runtime_error("Can't load file " + file + ": tile overflow "
                                + std::to_string(tile) + " at location "
                                + std::to_string(x) + "x" + std::to_string(y));
      }

      m_tilemap.at(y).at(x) = tile;
    }
  }

  SDL_FreeSurface(data);
}

void
EditorTilemap::save_tilemap(const std::string& file) const
{
  auto h = m_tilemap.size(), w = h ? m_tilemap.at(0).size() : 0;

  auto format = SDL_PIXELFORMAT_ARGB8888;
#if SDL_VERSION_ATLEAST(2, 0, 5)
  auto* surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, format);
#else
  SDL_Surface* surface;

  {
    Uint32 r, g, b, a;
    int bpp; // Unused, but required by SDL
    auto success = SDL_PixelFormatEnumToMasks(format, &bpp, &r, &g, &b, &a);

    if (success == SDL_FALSE)
    {
      throw std::runtime_error("Can't obtain image format info: "
                               + std::string(SDL_GetError()));
    }

    surface = SDL_CreateRGBSurface(0, w, h, 32, r, g, b, a);
  }
#endif

  if (!surface)
  {
    throw std::runtime_error("Can't create image surface: "
                             + std::string(SDL_GetError()));
  }

  auto bpp = surface->format->BytesPerPixel;

  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      auto* p = (Uint32 *) ((Uint8 *) surface->pixels + y * surface->pitch
                                          + x * surface->format->BytesPerPixel);
      *p = m_tilemap.at(y).at(x) ^ 0xff000000;
    }
  }

  SDL_SaveBMP_RW(surface, FS::get_rwops(file.c_str(), FS::OP::WRITE), true);
  SDL_FreeSurface(surface);
}

void
EditorTilemap::set_tile_id(size_t id)
{
  /** @todo Should this code check that id < g_tiles.size(), or should the
            caller do it? */
  m_tile_id = id;
}

void
EditorTilemap::resize_tilemap_to(const Vector& tilemap_point)
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

/** Does NOT take tilemap offset in consideration. */
Vector
EditorTilemap::screen_to_tilemap(const Vector& screen_point) const
{
  return (m_camera.apply_transform(screen_point) / g_tile_size.vector())
          .floor();
}

/** Does NOT take tilemap offset in consideration. */
Vector
EditorTilemap::tilemap_to_screen(const Vector& tilemap_point) const
{
  return tilemap_point * g_tile_size.vector() * m_camera.get_zoom()
         + m_camera.get_pos();
}
