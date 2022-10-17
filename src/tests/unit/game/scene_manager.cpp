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

#include "tests/tests.hpp"

#include "game/scene_manager.hpp"

#include <memory>

#include "SDL2/SDL.h"

#include "tests/mock/scenes/mock_scene.hpp"
#include "video/drawing_context.hpp"

TEST(UNIT__SceneManager__scene_manipulation)
{
  SceneManager sm(nullptr);

  EXPECT_EQ(sm.get_scene_stack().size(), 0);
  EXPECT(sm.empty());

  auto scene = std::make_unique<MockScene>(sm.get_controller());
  auto& s = *scene;

  sm.push_scene(std::move(scene));

  EXPECT_EQ(sm.get_scene_stack().size(), 1);
  EXPECT_EQ(sm.get_scene_stack().back().get(), &s);
  EXPECT(!sm.empty());

  auto scene2 = std::make_unique<MockScene>(sm.get_controller());
  auto& s2 = *scene2;

  sm.push_scene(std::move(scene2));

  EXPECT_EQ(sm.get_scene_stack().size(), 2);
  EXPECT_EQ(sm.get_scene_stack().back().get(), &s2);
  EXPECT(!sm.empty());

  sm.pop_scene();

  EXPECT_EQ(sm.get_scene_stack().size(), 1);
  EXPECT_EQ(sm.get_scene_stack().back().get(), &s);
  EXPECT(!sm.empty());

  sm.pop_scene();

  EXPECT_EQ(sm.get_scene_stack().size(), 0);
  EXPECT(sm.empty());

  EXPECT_THROW(sm.pop_scene());

  sm.push_scene(std::make_unique<MockScene>(sm.get_controller()));
  sm.push_scene(std::make_unique<MockScene>(sm.get_controller()));
  sm.push_scene(std::make_unique<MockScene>(sm.get_controller()));

  EXPECT_EQ(sm.get_scene_stack().size(), 3);

  sm.quit();

  EXPECT_EQ(sm.get_scene_stack().size(), 0);
  EXPECT(sm.empty());

  EXPECT_THROW(sm.pop_scene());
}

TEST(UNIT__SceneManager__scene_calls)
{
  SceneManager sm(nullptr);

  auto scene = std::make_unique<MockScene>(sm.get_controller());
  auto& s = *scene;

  sm.push_scene(std::move(scene));

  auto scene2 = std::make_unique<MockScene>(sm.get_controller());
  auto& s2 = *scene2;

  sm.push_scene(std::move(scene2));

  SDL_Event e;
  DrawingContext dc;

  SDL_zero(e);
  e.type = SDL_FIRSTEVENT;

  sm.event(e);
  sm.update(0.0f);
  sm.draw(dc);

  EXPECT_EQ(s.mock_get_calls_event(), 0);
  EXPECT_EQ(s.mock_get_calls_update(), 0);
  EXPECT_EQ(s.mock_get_calls_draw(), 0);
  EXPECT_EQ(s2.mock_get_calls_event(), 1);
  EXPECT_EQ(s2.mock_get_calls_update(), 1);
  EXPECT_EQ(s2.mock_get_calls_draw(), 1);
}
