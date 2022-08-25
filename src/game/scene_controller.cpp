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

#include "game/scene_manager.hpp"

DefaultSceneController::DefaultSceneController(SceneManager& scene_manager) :
  m_scene_manager(scene_manager)
{
}

void
DefaultSceneController::push_scene(std::unique_ptr<Scene> scene)
{
  m_scene_manager.push_scene(std::move(scene));
}

void
DefaultSceneController::pop_scene()
{
  m_scene_manager.pop_scene();
}

std::string
DefaultSceneController::get_data_folder()
{
  return m_scene_manager.fetch_data_folder();
}
