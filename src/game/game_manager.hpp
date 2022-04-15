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

#ifndef _HEADER_STMELTDOWN_GAME_GAMEMANAGER_HPP
#define _HEADER_STMELTDOWN_GAME_GAMEMANAGER_HPP

#include "game/i_game_manager.hpp"

#include <chrono>
#include <memory>

class GameManager final :
  public IGameManager
{
public:
  // Required by Emscripten
  static std::unique_ptr<GameManager> s_main_game_manager;
  static int run(int argc, const char* const* argv);

public:
  GameManager();
  virtual ~GameManager() override = default;

  virtual void push_scene(std::unique_ptr<Scene> scene,
                          Transition::Type transition = Transition::Type::NONE,
                          float time = 0.5f) override;
  virtual void pop_scene(Transition::Type transition = Transition::Type::NONE,
                         float time = 0.5f) override;

  virtual void change_video_system(Window::VideoSystem video_system,
                                   bool keep_status = true) override;
  virtual void set_delay(float delay) override;

  virtual Window& get_window() override;
  virtual const SceneStack& get_scene_stack() const override;
  virtual const Scene* get_popping_scene() const override;
  virtual const Transition* get_current_transition() const override;

  void setup_filesystem() const;
  void advance_video_system();
  void handle_internal_event(const SDL_Event& e);
  bool try_recover(const std::exception& err);

  void run_single_loop();
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

  float m_delay;

private:
  GameManager(const GameManager&) = delete;
  GameManager& operator=(const GameManager&) = delete;
};

extern "C" {

// Helper functions for Emscripten
const char* getExceptionMessage(intptr_t address);

}

#endif
