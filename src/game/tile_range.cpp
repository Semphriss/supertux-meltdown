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

#ifndef _HEADER_STMELTDOWN_GAME_TILERANGE_HPP

#include "game/tile_range.hpp"

#else

#include "util/rect.hpp"

template<typename ID>
TileRange<ID>::TileRange(const Size& size) :
  m_tiles()
{
  m_tiles.resize(size.h);

  for (auto& row : m_tiles)
  {
    row.resize(size.w);
  }
}

template<typename ID>
void
TileRange<ID>::set_tile(const Vector& pos, const ID& tile)
{
  try
  {
    m_tiles.at(pos.y).at(pos.x) = tile;
  }
  catch (const std::out_of_range&)
  {
  }
}

template<typename ID>
void
TileRange<ID>::set_tiles(const Vector& pos, const TileRange& tiles)
{
  Size size = tiles.get_size();

  for (int y = 0; y < size.h; y++)
  {
    for (int x = 0; x < size.w; x++)
    {
      Vector offset(x, y);
      set_tile(pos + offset, tiles.get_tile(offset));
    }
  }
}

template<typename ID>
ID
TileRange<ID>::get_tile(const Vector& pos) const
{
  try
  {
    return m_tiles.at(pos.y).at(pos.x);
  }
  catch (const std::out_of_range&)
  {
    return {};
  }
}

template<typename ID>
TileRange<ID>
TileRange<ID>::get_tiles(const Rect& rect) const
{
  TileRange<ID> selection(rect.size());

  selection.set_tiles(Vector(), *this);

  return selection;
}

template<typename ID>
void
TileRange<ID>::resize(const Size& new_size, const Vector& offset)
{
  TileRange<ID> temp = std::move(*this);

  *this = TileRange<ID>(new_size);

  set_tiles(offset, temp);
}

template<typename ID>
Vector
TileRange<ID>::autoresize(const Vector& pos)
{
  Size size = get_size();
  Size half_size = (size - Size(1.0f, 1.0f)) / 2.0f;

  Vector dist(std::max(0.0f, std::abs(pos.x - half_size.w) - half_size.w),
              std::max(0.0f, std::abs(pos.y - half_size.h) - half_size.h));

  Vector sign((pos.x > 0.0f) - (pos.x < 0.0f), (pos.y > 0.0f) - (pos.y < 0.0f));
  Vector offset = dist * sign;

  resize(size + dist, offset);

  return Vector(std::max(0.0f, -offset.x), std::max(0.0f, -offset.y));
}

template<typename ID>
Vector
TileRange<ID>::autoresize(const Rect& rect)
{
  Vector offset = autoresize(rect.top_lft());
  autoresize(rect.bot_rgt() + offset);
  return offset;
}

template<typename ID>
Size
TileRange<ID>::get_size() const
{
  return !m_tiles.size() ? Size() : Size(m_tiles.at(0).size(), m_tiles.size());
}

#endif
