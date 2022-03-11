//  SuperTux Meltdown - Semphris' take on the popular Linux platformer
//  Copyright (C) 2021-2022 Semphris <semphris@semphris.com>
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

#ifndef _HEADER_STMELTDOWN_GAME_RESOURCEMANAGER_HPP
#define _HEADER_STMELTDOWN_GAME_RESOURCEMANAGER_HPP

#include <memory>

class ResourceManager final
{
public:
  static ResourceManager& get_resource_manager(const char* arg0);

public:
  ~ResourceManager();

private:
  ResourceManager(const char* arg0);

private:
  ResourceManager(const ResourceManager&) = delete;
  ResourceManager& operator=(const ResourceManager&) = delete;
};

#endif
