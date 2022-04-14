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

#ifndef _HEADER_STMELTDOWN_GAME_GAMEOBJECTMANAGER_HPP
#define _HEADER_STMELTDOWN_GAME_GAMEOBJECTMANAGER_HPP

#include <memory>
#include <unordered_map>
#include <vector>

#include "objects/object.hpp"

class DrawingContext;

class GameObjectManager final
{
public:
  // Only safe (future-proof) operations: `=`, `==`, `!=`, default ctor, copy
  // ctor, and `== 0` (means null if true)
  typedef int UID;

public:
  GameObjectManager() = default;

  void update(float dt_sec);
  void draw(DrawingContext& context) const;

  template<typename T, typename... Args> std::pair<UID, T*>
    add_object(Args&&... args);
  template<typename F, typename T = Object> std::vector<std::pair<UID, T*>>
    get_objects(const F& func, int limit = -1) const;
  template<typename T = Object> T* get_object(const UID& uid) const;
  UID get_uid(const Object& object) const;
  void remove_object(const UID& uid);

private:
  std::unordered_map<UID, std::unique_ptr<Object>> m_objects;
  UID m_last_uid;

private:
  GameObjectManager(const GameObjectManager&) = delete;
  GameObjectManager& operator=(const GameObjectManager&) = delete;
};

#include "game/game_object_manager.cpp"

#endif
