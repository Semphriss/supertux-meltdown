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

#include "objects/tile_map.hpp"

#include "video/drawing_context.hpp"

bool
TileMap::is(const Object& object)
{
  return typeid(object) == typeid(TileMap);
}

constexpr const char*
TileMap::classname()
{
  return "tilemap";
}

TileMap::TileMap(GameObjectManager& gom) :
  Object(gom),
  m_tiles(Size(1.0f, 1.0f)),
  m_position(),
  m_autoresize(true),
  m_tile_size(32.0f, 32.0f)
{
}

void
TileMap::update(float dt_sec)
{
}

void
TileMap::draw(DrawingContext& context) const
{
  context.push_transform();
  context.get_transform().move(-m_position);
  context.get_transform().scale(m_tile_size);

  Size size = m_tiles.get_size();

  for (int x = 0; x <= size.w; x++)
  {
    for (int y = 0; y < size.h; y++)
    {
      context.draw_line(Vector(x, y), Vector(x, y + 1.0f),
                        Color(1.0f, 1.0f, 1.0f, 0.5f),
                        Renderer::Blend::BLEND, 1);
    }
  }

  for (int x = 0; x < size.w; x++)
  {
    for (int y = 0; y <= size.h; y++)
    {
      context.draw_line(Vector(x, y), Vector(x + 1, y),
                        Color(1.0f, 1.0f, 1.0f, 0.5f),
                        Renderer::Blend::BLEND, 1);
    }
  }

  context.pop_transform();
}

std::string
TileMap::get_classname() const
{
  return classname();
}

void
TileMap::set_tile(const Vector& pos, const TileID& tile)
{
  if (m_autoresize)
    m_position += m_tiles.autoresize(pos) * m_tile_size;

  m_tiles.set_tile(pos, tile);
}

void
TileMap::set_tiles(const Vector& pos, const TileRange<TileID>& tiles)
{
  if (m_autoresize)
    m_position += m_tiles.autoresize(Rect(pos, tiles.get_size())) * m_tile_size;

  m_tiles.set_tiles(pos, tiles);
}

TileMap::TileID
TileMap::get_tile(const Vector& pos) const
{
  return m_tiles.get_tile(pos);
}

TileRange<TileMap::TileID>
TileMap::get_tiles(const Rect& rect) const
{
  return m_tiles.get_tiles(rect);
}

void
TileMap::resize(const Size& new_size, const Vector& offset)
{
  m_tiles.resize(new_size, offset);
}

Size
TileMap::get_size() const
{
  return m_tiles.get_size();
}

bool
TileMap::get_autoresize() const
{
  return m_autoresize;
}

void
TileMap::set_autoresize(bool autoresize)
{
  m_autoresize = autoresize;
}

Vector
TileMap::get_position() const
{
  return m_position;
}

Size
TileMap::get_tile_size() const
{
  return m_tile_size;
}

const TileRange<TileMap::TileID>&
TileMap::get_tile_range() const
{
  return m_tiles;
}
