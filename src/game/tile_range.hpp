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
#define _HEADER_STMELTDOWN_GAME_TILERANGE_HPP

#include <vector>

#include "util/size.hpp"
#include "util/vector.hpp"

class Rect;

template<typename ID>
class TileRange final
{
public:
  TileRange() = default;
  TileRange(const TileRange&) = default;
  TileRange(const Size& size);

  void set_tile(const Vector& pos, const ID& tile);
  void set_tiles(const Vector& pos, const TileRange& tiles);
  ID get_tile(const Vector& pos) const;
  TileRange<ID> get_tiles(const Rect& rect) const;

  void resize(const Size& new_size, const Vector& offset = Vector());
  Vector autoresize(const Vector& pos);
  Vector autoresize(const Rect& rect);
  Size get_size() const;

  TileRange& operator=(const TileRange& other) = default;

private:
  std::vector<std::vector<ID>> m_tiles;
};

#include "game/tile_range.cpp"

#endif
