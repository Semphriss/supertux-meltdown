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

#include "game/scene_manager.hpp"

#include "game/game_manager.hpp"

SceneManager::SceneManager(GameManager* game_manager) :
  m_controller(*this),
  m_game_manager(game_manager),
  m_scenes()
{
}

void
SceneManager::push_scene(std::unique_ptr<Scene> scene)
{
  m_scenes.push_back(std::move(scene));
}

void
SceneManager::pop_scene()
{
  m_scenes.pop_back();
}

SceneController&
SceneManager::get_controller()
{
  return m_controller;
}

const std::vector<std::unique_ptr<Scene>>&
SceneManager::get_scene_stack() const
{
  return m_scenes;
}

bool
SceneManager::empty() const
{
  return m_scenes.empty();
}

void
SceneManager::quit()
{
  m_scenes.clear();
}

void
SceneManager::event(const SDL_Event& event)
{
  if (!m_scenes.empty())
    m_scenes.back()->event(event);
}

void
SceneManager::update(float dt_sec)
{
  if (!m_scenes.empty())
    m_scenes.back()->update(dt_sec);
}

void
SceneManager::draw(DrawingContext& context) const
{
  if (!m_scenes.empty())
    m_scenes.back()->draw(context);
}

void
SceneManager::set_delay(float delay)
{
  if (m_game_manager)
    m_game_manager->set_delay(delay);
}
