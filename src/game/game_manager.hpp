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

#include <chrono>
#include <string>

#include "game/scene_manager.hpp"
#include "video/drawing_context.hpp"
#include "video/window.hpp"

class GameManager final
{
public:
  GameManager();
  ~GameManager() = default;

  int run(int argc, char** argv);

private:
  bool init(const char* arg0);
  bool parse_cli_args(int argc, char** argv);
  bool finish_setup();
  int launch_game();
  void main_loop();
  void single_loop();
  bool deinit();

  template<typename F> bool generic_try(F func);

private:
  SceneManager m_scene_manager;
  int m_return_code;
  std::string m_arg_data_folder;
  std::unique_ptr<Window> m_window;
  DrawingContext m_context;
  // https://en.cppreference.com/w/cpp/chrono/steady_clock says:
  //   "This clock [...] is most suitable for measuring intervals."
  // std::chrono::high_resolution_clock is merely an alias to another clock,
  // which may or may not be steady_clock, and which can have huge jumps if
  // the user changes their computer time, including back in time. See the
  // notes: https://en.cppreference.com/w/cpp/chrono/high_resolution_clock
  std::chrono::steady_clock::time_point m_last_time;

private:
  GameManager(const GameManager&) = delete;
  GameManager& operator=(const GameManager&) = delete;
};

#endif
