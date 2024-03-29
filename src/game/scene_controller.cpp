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

#include "game/scene_controller.hpp"

#include "game/game_manager.hpp"
#include "game/scene_manager.hpp"

DefaultSceneController::DefaultSceneController(SceneManager* scene_manager,
                                               GameManager* game_manager) :
  m_scene_manager(scene_manager),
  m_game_manager(game_manager)
{
}

bool
DefaultSceneController::push_scene(std::unique_ptr<Scene> scene)
{
  if (m_scene_manager)
    m_scene_manager->push_scene(std::move(scene));

  // Technically, the double negation here is not needed, but only
  // `return m_scene_manager;` could be confusing.
  return !!m_scene_manager;
}

bool
DefaultSceneController::pop_scene()
{
  if (m_scene_manager)
    m_scene_manager->pop_scene();

  return !!m_scene_manager;
}

bool
DefaultSceneController::set_delay(float delay)
{
  if (m_game_manager)
    m_game_manager->set_delay(delay);

  return !!m_game_manager;
}
