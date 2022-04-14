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

#ifndef _HEADER_STMELTDOWN_OBJECTS_TILEMAP_HPP
#define _HEADER_STMELTDOWN_OBJECTS_TILEMAP_HPP

#include "objects/object.hpp"

#include "game/tile_range.hpp"

class TileMap final :
  public Object
{
public:
  typedef int TileID;

public:
  static bool is(const Object& object);
  static constexpr const char* classname();

public:
  TileMap(GameObjectManager& gom);
  virtual ~TileMap() = default;

  virtual void update(float dt_sec) override;
  virtual void draw(DrawingContext& context) const override;

  /** \addtogroup GameObjectIdentifiers @{ */

  virtual std::string get_classname() const override;

  /** } */

  void set_tile(const Vector& pos, const TileID& tile);
  void set_tiles(const Vector& pos, const TileRange<TileID>& tiles);
  TileID get_tile(const Vector& pos) const;
  TileRange<TileID> get_tiles(const Rect& rect) const;

  void resize(const Size& new_size, const Vector& offset = Vector());
  Size get_size() const;

  bool get_autoresize() const;
  void set_autoresize(bool autoresize);

  Vector get_position() const;
  Size get_tile_size() const;

  const TileRange<TileID>& get_tile_range() const;

private:
  TileRange<TileID> m_tiles;
  Vector m_position;
  bool m_autoresize;
  Size m_tile_size;

private:
  TileMap(const TileMap&) = delete;
  TileMap& operator=(const TileMap&) = delete;
};

#endif
