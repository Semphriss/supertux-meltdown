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

#ifndef HEADER_STM_GAME_SCENEMANAGER_HPP
#define HEADER_STM_GAME_SCENEMANAGER_HPP

#include <memory>
#include <vector>

#include "game/scene_controller.hpp"
#include "scenes/scene.hpp"

class GameManager;

class SceneManager final
{
public:
  SceneManager(GameManager* game_manager);
  ~SceneManager() = default;

  void push_scene(std::unique_ptr<Scene> scene);
  void pop_scene();

  SceneController& get_controller();
  const std::vector<std::unique_ptr<Scene>>& get_scene_stack() const;
  bool empty() const;
  void quit();

  void event(const SDL_Event& event);
  void update(float dt_sec);
  void draw(DrawingContext& context) const;

  std::string fetch_data_folder() const;

private:
  DefaultSceneController m_controller;
  GameManager* const m_game_manager;
  std::vector<std::unique_ptr<Scene>> m_scenes;

private:
  SceneManager(const SceneManager&) = delete;
  SceneManager& operator=(const SceneManager&) = delete;
};

#endif
