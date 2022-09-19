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

#include <memory>

#include "game/game_manager.hpp"

#ifdef EMSCRIPTEN
// unique_ptr allows lazy loading
static std::unique_ptr<GameManager> g_game_manager = nullptr;
#endif

int main(int argc, char** argv)
{
#ifdef EMSCRIPTEN
  // The GameManager must survive after main() returned
  g_game_manager = std::make_unique<GameManager>();
  return g_game_manager->run(argc, argv);
#else
  return GameManager().run(argc, argv);
#endif
}
