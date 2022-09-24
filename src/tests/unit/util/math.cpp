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

#include "util/math.hpp"

TEST(UNIT__Math__clamp)
{
  EXPECT_EQ(Math::clamp(0, -1, 1), 0);
  EXPECT_EQ(Math::clamp(-1, -1, 1), -1);
  EXPECT_EQ(Math::clamp(1, -1, 1), 1);
  EXPECT_EQ(Math::clamp(-2, -1, 1), -1);
  EXPECT_EQ(Math::clamp(2, -1, 1), 1);
  EXPECT_EQ(Math::clamp(2.0f, -0.1f, 10.0f), 2.0f);
  EXPECT_EQ(Math::clamp(-0.1f, -0.1f, 10.0f), -0.1f);
  EXPECT_EQ(Math::clamp(10.0f, -0.1f, 10.0f), 10.0f);
  EXPECT_EQ(Math::clamp(-2.0e7f, -0.1f, 10.0f), -0.1f);
  EXPECT_EQ(Math::clamp(10.0001f, -0.1f, 10.0f), 10.0f);
}
