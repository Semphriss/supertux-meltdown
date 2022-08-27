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

#ifndef HEADER_STM_GAME_SCENECONTROLLER_HPP
#define HEADER_STM_GAME_SCENECONTROLLER_HPP

#include <memory>

class Scene;

class SceneManager;

// Allows interactions between the scene and the controller. Helps de-coupling
// the Scene class from the SceneManager class.
class SceneController
{
public:
  SceneController() = default;
  virtual ~SceneController() = default;

  virtual void push_scene(std::unique_ptr<Scene> scene) = 0;
  virtual void pop_scene() = 0;

private:
  SceneController(const SceneController&) = delete;
  SceneController& operator=(const SceneController&) = delete;
};

class DefaultSceneController :
  public SceneController
{
public:
  DefaultSceneController(SceneManager& scene_manager);
  virtual ~DefaultSceneController() override = default;

  virtual void push_scene(std::unique_ptr<Scene> scene) override;
  virtual void pop_scene() override;

private:
  SceneManager& m_scene_manager;

private:
  DefaultSceneController(const DefaultSceneController&) = delete;
  DefaultSceneController& operator=(const DefaultSceneController&) = delete;
};

#endif
