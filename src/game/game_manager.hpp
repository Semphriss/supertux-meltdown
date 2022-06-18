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

#ifndef HEADER_STM_GAME_GAMEMANAGER_HPP
#define HEADER_STM_GAME_GAMEMANAGER_HPP

#include "game/scene_manager.hpp"

class GameManager final
{
public:
  GameManager();
  ~GameManager() = default;

  int run(int argc, char** argv);

private:
  SceneManager m_scene_manager;

private:
  GameManager(const GameManager&) = delete;
  GameManager& operator=(const GameManager&) = delete;
};

#endif
