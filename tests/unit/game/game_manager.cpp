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

#include "game/game_manager.hpp"
#include "gtest/gtest.h"

#include "make_unique.hpp"

#include "mock/scenes/scene.hpp"

#include "util/log.hpp"

TEST(GameManager, push_scene)
{
  // No transition, no other scene (must by default be without transition)
  {
    GameManager gm;

    auto scene = std::make_unique<testing::StrictMock<MockScene>>(gm);
    auto* mock_scene = scene.get();
    EXPECT_CALL(*mock_scene, enter()).Times(1);

    gm.push_scene(std::move(scene));

    ASSERT_EQ(gm.get_scene_stack().size(), 1);
    EXPECT_EQ(gm.get_scene_stack().back().get(), mock_scene);

    // Ensure there's no transition, future tests rely on it
    ASSERT_EQ(nullptr, gm.get_current_transition());
  }

  // Transition, no other scene (must by default have time > 0)
  {
    GameManager gm;

    auto scene = std::make_unique<testing::StrictMock<MockScene>>(gm);
    auto* mock_scene = scene.get();

    gm.push_scene(std::move(scene), static_cast<Transition::Type>(1));

    ASSERT_EQ(gm.get_scene_stack().size(), 1);
    EXPECT_EQ(gm.get_scene_stack().back().get(), mock_scene);

    ASSERT_NE(gm.get_current_transition(), nullptr);
    EXPECT_GT(gm.get_current_transition()->get_total_time(), 0.0f);
    EXPECT_EQ(gm.get_current_transition()->get_elapsed_time(), 0.0f);
    EXPECT_EQ(gm.get_current_transition()->get_origin_scene(), nullptr);
    EXPECT_EQ(gm.get_current_transition()->get_target_scene(), mock_scene);
  }

  // Transition, with other scene
  {
    GameManager gm;

    auto scene_1 = std::make_unique<testing::StrictMock<MockScene>>(gm);
    auto* mock_scene_1 = scene_1.get();
    EXPECT_CALL(*mock_scene_1, enter()).Times(1);
    EXPECT_CALL(*mock_scene_1, leave(false)).Times(1);

    auto scene_2 = std::make_unique<testing::StrictMock<MockScene>>(gm);
    auto* mock_scene_2 = scene_2.get();

    gm.push_scene(std::move(scene_1));
    gm.push_scene(std::move(scene_2), static_cast<Transition::Type>(1));

    ASSERT_EQ(gm.get_scene_stack().size(), 2);
    EXPECT_EQ(gm.get_scene_stack().back().get(), mock_scene_2);

    ASSERT_NE(gm.get_current_transition(), nullptr);
    EXPECT_GT(gm.get_current_transition()->get_total_time(), 0.0f);
    EXPECT_EQ(gm.get_current_transition()->get_elapsed_time(), 0.0f);
    EXPECT_EQ(gm.get_current_transition()->get_origin_scene(), mock_scene_1);
    EXPECT_EQ(gm.get_current_transition()->get_target_scene(), mock_scene_2);
  }

  // Overlapping transitions (deny the second one)
  {
    auto* out = Log::s_log;
    Log::s_log = nullptr;

    GameManager gm;

    auto scene_1 = std::make_unique<testing::StrictMock<MockScene>>(gm);
    auto* mock_scene_1 = scene_1.get();

    auto scene_2 = std::make_unique<testing::StrictMock<MockScene>>(gm);
    auto* mock_scene_2 = scene_1.get();

    gm.push_scene(std::move(scene_1), static_cast<Transition::Type>(1));
    gm.push_scene(std::move(scene_2), static_cast<Transition::Type>(1));

    ASSERT_EQ(gm.get_scene_stack().size(), 1);
    EXPECT_EQ(gm.get_scene_stack().back().get(), mock_scene_1);

    ASSERT_NE(gm.get_current_transition(), nullptr);
    EXPECT_GT(gm.get_current_transition()->get_total_time(), 0.0f);
    EXPECT_EQ(gm.get_current_transition()->get_elapsed_time(), 0.0f);
    EXPECT_EQ(gm.get_current_transition()->get_origin_scene(), nullptr);
    EXPECT_EQ(gm.get_current_transition()->get_target_scene(), mock_scene_1);

    Log::s_log = out;
  }
}

TEST(GameManager, pop_scene)
{
  // No transition, no other scene
  {
    GameManager gm;

    auto scene_1 = std::make_unique<testing::StrictMock<MockScene>>(gm);
    auto* mock_scene_1 = scene_1.get();
    EXPECT_CALL(*mock_scene_1, enter()).Times(2);
    EXPECT_CALL(*mock_scene_1, leave(false)).Times(1);
    EXPECT_CALL(*mock_scene_1, leave(true)).Times(1);

    auto scene_2 = std::make_unique<testing::StrictMock<MockScene>>(gm);
    auto* mock_scene_2 = scene_2.get();
    EXPECT_CALL(*mock_scene_2, enter()).Times(1);
    EXPECT_CALL(*mock_scene_2, leave(true)).Times(1);

    gm.push_scene(std::move(scene_1));
    gm.push_scene(std::move(scene_2));

    EXPECT_EQ(gm.get_scene_stack().size(), 2);
    EXPECT_EQ(gm.get_scene_stack().back().get(), mock_scene_2);
    EXPECT_EQ(gm.get_current_transition(), nullptr);
    EXPECT_EQ(gm.get_popping_scene(), nullptr);

    gm.pop_scene();

    EXPECT_EQ(gm.get_scene_stack().size(), 1);
    EXPECT_EQ(gm.get_scene_stack().back().get(), mock_scene_1);
    EXPECT_EQ(gm.get_current_transition(), nullptr);
    EXPECT_EQ(gm.get_popping_scene(), nullptr);

    gm.pop_scene();

    EXPECT_TRUE(gm.get_scene_stack().empty());
    EXPECT_EQ(gm.get_current_transition(), nullptr);
    EXPECT_EQ(gm.get_popping_scene(), nullptr);
  }

  // Transition, no other scene (must by default have time > 0)
  {
    GameManager gm;

    auto scene = std::make_unique<testing::StrictMock<MockScene>>(gm);
    auto* mock_scene = scene.get();
    EXPECT_CALL(*mock_scene, enter()).Times(1);
    EXPECT_CALL(*mock_scene, leave(true)).Times(1);

    gm.push_scene(std::move(scene));
    gm.pop_scene(static_cast<Transition::Type>(1));

    EXPECT_TRUE(gm.get_scene_stack().empty());
    EXPECT_EQ(gm.get_popping_scene(), mock_scene);

    ASSERT_NE(gm.get_current_transition(), nullptr);
    EXPECT_GT(gm.get_current_transition()->get_total_time(), 0.0f);
    EXPECT_EQ(gm.get_current_transition()->get_elapsed_time(), 0.0f);
    EXPECT_EQ(gm.get_current_transition()->get_origin_scene(), mock_scene);
    EXPECT_EQ(gm.get_current_transition()->get_target_scene(), nullptr);
  }

  // Transition, with other scene
  {
    GameManager gm;

    auto scene_1 = std::make_unique<testing::StrictMock<MockScene>>(gm);
    auto* mock_scene_1 = scene_1.get();
    EXPECT_CALL(*mock_scene_1, enter()).Times(1);
    EXPECT_CALL(*mock_scene_1, leave(false)).Times(1);

    auto scene_2 = std::make_unique<testing::StrictMock<MockScene>>(gm);
    auto* mock_scene_2 = scene_2.get();
    EXPECT_CALL(*mock_scene_2, enter()).Times(1);
    EXPECT_CALL(*mock_scene_2, leave(true)).Times(1);

    gm.push_scene(std::move(scene_1));
    gm.push_scene(std::move(scene_2));

    // Time MUST be > 0.0f by default
    gm.pop_scene(static_cast<Transition::Type>(1));

    EXPECT_EQ(gm.get_scene_stack().size(), 1);
    EXPECT_EQ(gm.get_scene_stack().back().get(), mock_scene_1);
    EXPECT_EQ(gm.get_popping_scene(), mock_scene_2);

    ASSERT_NE(gm.get_current_transition(), nullptr);
    EXPECT_GT(gm.get_current_transition()->get_total_time(), 0.0f);
    EXPECT_EQ(gm.get_current_transition()->get_elapsed_time(), 0.0f);
    EXPECT_EQ(gm.get_current_transition()->get_origin_scene(), mock_scene_2);
    EXPECT_EQ(gm.get_current_transition()->get_target_scene(), mock_scene_1);
  }
}
