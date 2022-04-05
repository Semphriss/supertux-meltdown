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

#ifndef _HEADER_STMELTDOWN_MOCK_SCENES_SCENE_HPP
#define _HEADER_STMELTDOWN_MOCK_SCENES_SCENE_HPP

#include "scenes/scene.hpp"

#include "gmock/gmock.h"

class MockScene :
  public Scene
{
public:
  MockScene() = delete;
  MockScene(IGameManager& game_manager);
  virtual ~MockScene() = default;

  MOCK_METHOD(void, event, (const SDL_Event& event), (override));
  MOCK_METHOD(void, update, (float dt_sec), (override));
  MOCK_METHOD(void, draw, (DrawingContext& context), (const, override));

  MOCK_METHOD(void, enter, (), (override));
  MOCK_METHOD(void, leave, (bool to_parent), (override));
  MOCK_METHOD(bool, quit, (bool can_abort), (override));

  MOCK_METHOD(void, reset_caches, (), (override));
  MOCK_METHOD(bool, recover_from_error, (const std::exception& err),
              (override));

private:
  MockScene(const MockScene&) = delete;
  MockScene& operator=(const MockScene&) = delete;
};

#endif
