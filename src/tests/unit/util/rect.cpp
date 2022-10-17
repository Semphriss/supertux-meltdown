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

#include "util/rect.hpp"

#include <sstream>

#include "util/size.hpp"
#include "util/vector.hpp"

TEST(UNIT__Rect__ctor)
{
  Rect r1;
  EXPECT_FLT_EQ(r1.x1, 0.0f);
  EXPECT_FLT_EQ(r1.x2, 0.0f);
  EXPECT_FLT_EQ(r1.y1, 0.0f);
  EXPECT_FLT_EQ(r1.y2, 0.0f);

  Rect r2(Size(14.0f, 2.1f));
  EXPECT_FLT_EQ(r2.x1, 0.0f);
  EXPECT_FLT_EQ(r2.y1, 0.0f);
  EXPECT_FLT_EQ(r2.x2, 14.0f);
  EXPECT_FLT_EQ(r2.y2, 2.1f);

  Rect r3(1e2f, 3e-4f, -5e6f, -7e-8f);
  EXPECT_FLT_EQ(r3.x1, 1e2f);
  EXPECT_FLT_EQ(r3.y1, 3e-4f);
  EXPECT_FLT_EQ(r3.x2, -5e6f);
  EXPECT_FLT_EQ(r3.y2, -7e-8f);

  Rect r4(Vector(7.4f, 8.2f), Size(14.0f, 2.1f));
  EXPECT_FLT_EQ(r4.x1, 7.4f);
  EXPECT_FLT_EQ(r4.y1, 8.2f);
  EXPECT_FLT_EQ(r4.x2, 14.0f + 7.4f);
  EXPECT_FLT_EQ(r4.y2, 2.1f + 8.2f);

  Rect r5(Vector(7.4f, 8.2f), Vector(14.0f, 52.1f));
  EXPECT_FLT_EQ(r5.x1, 7.4f);
  EXPECT_FLT_EQ(r5.y1, 8.2f);
  EXPECT_FLT_EQ(r5.x2, 14.0f);
  EXPECT_FLT_EQ(r5.y2, 52.1f);
}

TEST(UNIT__Rect__width)
{
  Rect r1;
  EXPECT_FLT_EQ(r1.width(), 0.0f);

  Rect r2(Size(14.0f, 2.1f));
  EXPECT_FLT_EQ(r2.width(), 14.0f);

  Rect r3(1.2f, 3.4f, 5.6f, 7.8f);
  EXPECT_FLT_EQ(r3.width(), 5.6f - 1.2f);

  Rect r4(Vector(7.4f, 8.2f), Size(14.0f, 2.1f));
  EXPECT_FLT_EQ(r4.width(), 14.0f);

  Rect r5(Vector(7.4f, 8.2f), Vector(54.0f, 52.1f));
  EXPECT_FLT_EQ(r5.width(), 54.0f - 7.4f);
}

TEST(UNIT__Rect__height)
{
  Rect r1;
  EXPECT_FLT_EQ(r1.height(), 0.0f);

  Rect r2(Size(14.0f, 2.1f));
  EXPECT_FLT_EQ(r2.height(), 2.1f);

  Rect r3(1.2f, 3.4f, 5.6f, 7.8f);
  EXPECT_FLT_EQ(r3.height(), 7.8f - 3.4f);

  Rect r4(Vector(7.4f, 8.2f), Size(14.0f, 2.1f));
  EXPECT_FLT_EQ(r4.height(), 2.1f);

  Rect r5(Vector(7.4f, 8.2f), Vector(54.0f, 52.1f));
  EXPECT_FLT_EQ(r5.height(), 52.1f - 8.2f);
}

TEST(UNIT__Rect__size)
{
  Rect r1;
  EXPECT_FLT_EQ(r1.size().w, 0.0f);
  EXPECT_FLT_EQ(r1.size().h, 0.0f);

  Rect r2(Size(14.0f, 2.1f));
  EXPECT_FLT_EQ(r2.size().w, 14.0f);
  EXPECT_FLT_EQ(r2.size().h, 2.1f);

  Rect r3(1.2f, 3.4f, 5.6f, 7.8f);
  EXPECT_FLT_EQ(r3.size().w, 5.6f - 1.2f);
  EXPECT_FLT_EQ(r3.size().h, 7.8f - 3.4f);

  Rect r4(Vector(7.4f, 8.2f), Size(14.0f, 2.1f));
  EXPECT_FLT_EQ(r4.size().w, 14.0f);
  EXPECT_FLT_EQ(r4.size().h, 2.1f);

  Rect r5(Vector(7.4f, 8.2f), Vector(54.0f, 52.1f));
  EXPECT_FLT_EQ(r5.size().w, 54.0f - 7.4f);
  EXPECT_FLT_EQ(r5.size().h, 52.1f - 8.2f);
}
