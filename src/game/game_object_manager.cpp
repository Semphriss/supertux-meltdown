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

#include "game/game_object_manager.hpp"

#include <algorithm>

#include "objects/object.hpp"

void
GameObjectManager::update(float dt_sec)
{
  for (auto& object : m_objects)
    object.second->update(dt_sec);
}

void
GameObjectManager::draw(DrawingContext& context) const
{
  for (const auto& object : m_objects)
    object.second->draw(context);
}

GameObjectManager::UID
GameObjectManager::get_uid(const Object& object) const
{
  auto it = std::find_if(m_objects.begin(), m_objects.end(),
    [&object](decltype(m_objects)::const_reference p) {
    return p.second.get() == &object;
  });

  return (it == m_objects.end()) ? 0 : it->first;
}

void
GameObjectManager::remove_object(const UID& uid)
{
  m_objects.erase(uid);
}

#else

#include "make_unique.hpp"

#include "util/log.hpp"

template<typename T, typename... Args>
std::pair<GameObjectManager::UID, T*>
GameObjectManager::add_object(Args&&... args)
{
  m_last_uid++;

  auto object = std::make_unique<T>(*this, args...);
  T* object_ptr = object.get();

  m_objects.emplace(m_last_uid, std::move(object));

  return std::make_pair(m_last_uid, object_ptr);
}

template<typename F, typename T>
std::vector<std::pair<GameObjectManager::UID, T*>>
GameObjectManager::get_objects(const F& func, int limit) const
{
  std::vector<std::pair<UID, T*>> array;
  int size_cache = 0;

  for (const auto& pair : m_objects)
  {
    if (T::is(pair.second.get()) && func(pair.second.get()))
    {
      array.push_back(std::make_pair(pair.first,
                                     static_cast<T*>(pair.second.get())));
      size_cache++;

      if (limit > 0 && size_cache >= limit)
        break;
    }
  }

  return array;
}

template<typename T>
T*
GameObjectManager::get_object(const UID& uid) const
{
  try
  {
    auto& obj = m_objects.at(uid);

    if (!obj || !T::is(obj.get()))
      return nullptr;

    return static_cast<T*>(obj.get());
  }
  catch(const std::out_of_range&)
  {
    return nullptr;
  }
}

#endif
