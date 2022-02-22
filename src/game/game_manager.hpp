//  SuperTux Meltdown - Semphris' take on the popular Linux platformer
//  Copyright (C) 2021 Semphris <semphris@protonmail.com>
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

#ifndef _HEADER_STMELTDOWN_GAME_GAMEMANAGER_HPP
#define _HEADER_STMELTDOWN_GAME_GAMEMANAGER_HPP

#include <chrono>
#include <memory>
#include <vector>

#include "scenes/scene.hpp"
#include "transitions/transition.hpp"
#include "video/window.hpp"

class GameManager final
{
public:
  GameManager() = default;

  int run();

  void push_scene(std::unique_ptr<Scene> scene,
                  Transition::Type transition = Transition::Type::NONE,
                  float time = .5f);
  void pop_scene(Transition::Type transition = Transition::Type::NONE,
                 float time = .5f);

  const Window& get_window() const;

private:
  int run_loops();

  void handle_events();
  void handle_update();
  void handle_draw();

  bool empty() const;

private:
  std::unique_ptr<Window> m_window;
  std::vector<std::unique_ptr<Scene>> m_scenes;
  std::unique_ptr<Transition> m_transition;
  std::unique_ptr<Scene> m_popping_scene;

  // steady_clock, because high_resolution_clock is not recommended:
  // https://en.cppreference.com/w/cpp/chrono/high_resolution_clock
  std::chrono::steady_clock::time_point m_last_frame;

private:
  GameManager(const GameManager&) = delete;
  GameManager& operator=(const GameManager&) = delete;
};

#endif
