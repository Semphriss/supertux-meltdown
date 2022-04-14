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

#ifndef _HEADER_STMELTDOWN_OBJECTS_OBJECT_HPP
#define _HEADER_STMELTDOWN_OBJECTS_OBJECT_HPP

#include <string>

class DrawingContext;
class GameObjectManager;

class Object
{
public:
  static constexpr bool is(const Object& object);
  static constexpr const char* classname();

public:
  Object(GameObjectManager& gom);
  virtual ~Object() = default;

  virtual void update(float dt_sec) = 0;
  virtual void draw(DrawingContext& context) const = 0;

  /**
   * \defgroup GameObjectIdentifiers GameObject Identifiers
   * Functions used to identify objects.
   *
   * \addtogroup GameObjectIdentifiers
   * @{
   */

  virtual std::string get_classname() const = 0;

  /** @} */

protected:
  GameObjectManager& m_gom;

private:
  Object(const Object&) = delete;
  Object& operator=(const Object&) = delete;
};

#endif
