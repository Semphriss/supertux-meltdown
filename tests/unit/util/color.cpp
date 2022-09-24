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

#include "tests.hpp"

#include "util/color.hpp"

#include <sstream>

TEST(UNIT__Color__ctor__default)
{
  Color c;
  EXPECT_FLT_EQ(c.r, 0.0f);
  EXPECT_FLT_EQ(c.g, 0.0f);
  EXPECT_FLT_EQ(c.b, 0.0f);
  EXPECT_FLT_EQ(c.a, 1.0f);
}

TEST(UNIT__Color__ctor__floats)
{
  Color c(0.125f, 0.25f, 0.5f, 0.375f);
  EXPECT_FLT_EQ(c.r, 0.125f);
  EXPECT_FLT_EQ(c.g, 0.25f);
  EXPECT_FLT_EQ(c.b, 0.5f);
  EXPECT_FLT_EQ(c.a, 0.375f);
}

TEST(UNIT__Color__ctor__copy_alpha)
{
  Color c1(0.125f, 0.25f, 0.5f, 0.375f);
  Color c(c1, 0.875f);
  EXPECT_FLT_EQ(c.r, 0.125f);
  EXPECT_FLT_EQ(c.g, 0.25f);
  EXPECT_FLT_EQ(c.b, 0.5f);
  EXPECT_FLT_EQ(c.a, 0.875f);
}

TEST(UNIT__Color__ctor__copy)
{
  Color c1(0.125f, 0.25f, 0.5f, 0.375f);
  Color c(c1);
  EXPECT_FLT_EQ(c.r, 0.125f);
  EXPECT_FLT_EQ(c.g, 0.25f);
  EXPECT_FLT_EQ(c.b, 0.5f);
  EXPECT_FLT_EQ(c.a, 0.375f);
}

TEST(UNIT__Color__operator_plus)
{
  Color c1(143.0f, -756.0f, 19.53f, 1e6f);
  Color c2(12.0f, 54.0f, 43.8f, -1e5f);

  EXPECT_FLT_EQ((c1 + c2).r, 143.0f + 12.0f);
  EXPECT_FLT_EQ((c1 + c2).g, -756.0f + 54.0f);
  EXPECT_FLT_EQ((c1 + c2).b, 19.53f + 43.8f);
  EXPECT_FLT_EQ((c1 + c2).a, 1e6f + -1e5f);
}

TEST(UNIT__Color__operator_plus_equals)
{
  Color c1(143.0f, -756.0f, 19.53f, 1e6f);
  c1 += Color(12.0f, 54.0f, 43.8f, -1e5f);

  EXPECT_FLT_EQ(c1.r, 143.0f + 12.0f);
  EXPECT_FLT_EQ(c1.g, -756.0f + 54.0f);
  EXPECT_FLT_EQ(c1.b, 19.53f + 43.8f);
  EXPECT_FLT_EQ(c1.a, 1e6f + -1e5f);
}

TEST(UNIT__Color__operator_minus)
{
  Color c1(143.0f, -756.0f, 19.53f, 1e6f);
  Color c2(12.0f, 54.0f, 43.8f, -1e5f);

  EXPECT_FLT_EQ((c1 - c2).r, 143.0f - 12.0f);
  EXPECT_FLT_EQ((c1 - c2).g, -756.0f - 54.0f);
  EXPECT_FLT_EQ((c1 - c2).b, 19.53f - 43.8f);
  EXPECT_FLT_EQ((c1 - c2).a, 1e6f - -1e5f);
}

TEST(UNIT__Color__operator_minus_equals)
{
  Color c1(143.0f, -756.0f, 19.53f, 1e6f);
  c1 -= Color(12.0f, 54.0f, 43.8f, -1e5f);

  EXPECT_FLT_EQ(c1.r, 143.0f - 12.0f);
  EXPECT_FLT_EQ(c1.g, -756.0f - 54.0f);
  EXPECT_FLT_EQ(c1.b, 19.53f - 43.8f);
  EXPECT_FLT_EQ(c1.a, 1e6f - -1e5f);
}

TEST(UNIT__Color__operator_times)
{
  Color c1(143.0f, -756.0f, 19.53f, 1e6f);
  Color c2(12.0f, 54.0f, 43.8f, -1e5f);

  EXPECT_FLT_EQ((c1 * c2).r, 143.0f * 12.0f);
  EXPECT_FLT_EQ((c1 * c2).g, -756.0f * 54.0f);
  EXPECT_FLT_EQ((c1 * c2).b, 19.53f * 43.8f);
  EXPECT_FLT_EQ((c1 * c2).a, 1e6f * -1e5f);
}

TEST(UNIT__Color__operator_times_equals)
{
  Color c1(143.0f, -756.0f, 19.53f, 1e6f);
  c1 *= Color(12.0f, 54.0f, 43.8f, -1e5f);

  EXPECT_FLT_EQ(c1.r, 143.0f * 12.0f);
  EXPECT_FLT_EQ(c1.g, -756.0f * 54.0f);
  EXPECT_FLT_EQ(c1.b, 19.53f * 43.8f);
  EXPECT_FLT_EQ(c1.a, 1e6f * -1e5f);
}

TEST(UNIT__Color__operator_divide)
{
  Color c1(143.0f, -756.0f, 19.53f, 1e6f);
  Color c2(12.0f, 54.0f, 43.8f, -1e5f);

  EXPECT_FLT_EQ((c1 / c2).r, 143.0f / 12.0f);
  EXPECT_FLT_EQ((c1 / c2).g, -756.0f / 54.0f);
  EXPECT_FLT_EQ((c1 / c2).b, 19.53f / 43.8f);
  EXPECT_FLT_EQ((c1 / c2).a, 1e6f / -1e5f);
}

TEST(UNIT__Color__operator_divide_equals)
{
  Color c1(143.0f, -756.0f, 19.53f, 1e6f);
  c1 /= Color(12.0f, 54.0f, 43.8f, -1e5f);

  EXPECT_FLT_EQ(c1.r, 143.0f / 12.0f);
  EXPECT_FLT_EQ(c1.g, -756.0f / 54.0f);
  EXPECT_FLT_EQ(c1.b, 19.53f / 43.8f);
  EXPECT_FLT_EQ(c1.a, 1e6f / -1e5f);
}

TEST(UNIT__Color__operator_printstream)
{
  // There is no guarantee regarding what it will print, only that it won't
  // throw.
  std::stringstream s;
  s << Color();
}
