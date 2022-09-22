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

#include "util/vector.hpp"

#include <cmath>
#include <sstream>

#include "util/size.hpp"

TEST(UNIT__Vector__ctor)
{
  Vector v1;
  EXPECT_FLT_EQ(v1.x, 0.0f);
  EXPECT_FLT_EQ(v1.y, 0.0f);

  Vector v2(1234.0f, -0.5f);
  EXPECT_FLT_EQ(v2.x, 1234.0f);
  EXPECT_FLT_EQ(v2.y, -0.5f);

  Vector v3(Size(0.3125e-1f, -8.75e2f));
  EXPECT_FLT_EQ(v3.x, 0.3125e-1f);
  EXPECT_FLT_EQ(v3.y, -8.75e2f);
}

TEST(UNIT__Vector__size)
{
  EXPECT_FLT_EQ(Vector().size().w, Size().w);
  EXPECT_FLT_EQ(Vector().size().h, Size().h);
  EXPECT_FLT_EQ(Vector(532.92f, 7537.765e-4f).size().w,
                Size(532.92f, 7537.765e-4f).w);
  EXPECT_FLT_EQ(Vector(532.92f, 7537.765e-4f).size().h,
                Size(532.92f, 7537.765e-4f).h);
}

TEST(UNIT__Vector__length)
{
  EXPECT_FLT_EQ(Vector().length(), 0.0f);
  EXPECT_FLT_EQ(Vector(532.92f, 34.1f).length(), 534.0098654519f);
}

TEST(UNIT__Vector__floor)
{
  EXPECT_FLT_EQ(Vector().floor().x, Vector().x);
  EXPECT_FLT_EQ(Vector().floor().y, Vector().y);
  EXPECT_FLT_EQ(Vector(5.1f, 3.9f).floor().x, Vector(5.0f, 3.0f).x);
  EXPECT_FLT_EQ(Vector(5.1f, 3.9f).floor().y, Vector(5.0f, 3.0f).y);
  EXPECT_FLT_EQ(Vector(-5.1f, -3.9f).floor().x, Vector(-6.0f, -4.0f).x);
  EXPECT_FLT_EQ(Vector(-5.1f, -3.9f).floor().y, Vector(-6.0f, -4.0f).y);
  EXPECT_FLT_EQ(Vector(-5.0f, 3.0f).floor().x, Vector(-5.0f, 3.0f).x);
  EXPECT_FLT_EQ(Vector(-5.0f, 3.0f).floor().y, Vector(-5.0f, 3.0f).y);
}

TEST(UNIT__Vector__round)
{
  EXPECT_FLT_EQ(Vector().round().x, Vector().x);
  EXPECT_FLT_EQ(Vector().round().y, Vector().y);
  EXPECT_FLT_EQ(Vector(5.1f, 3.9f).round().x, Vector(5.0f, 4.0f).x);
  EXPECT_FLT_EQ(Vector(5.1f, 3.9f).round().y, Vector(5.0f, 4.0f).y);
  EXPECT_FLT_EQ(Vector(5.5f, -5.5f).round().x, Vector(6.0f, -6.0f).x);
  EXPECT_FLT_EQ(Vector(5.5f, -5.5f).round().y, Vector(6.0f, -6.0f).y);
  EXPECT_FLT_EQ(Vector(-5.0f, 3.0f).floor().x, Vector(-5.0f, 3.0f).x);
  EXPECT_FLT_EQ(Vector(-5.0f, 3.0f).floor().y, Vector(-5.0f, 3.0f).y);
}

TEST(UNIT__Vector__ceil)
{
  EXPECT_FLT_EQ(Vector().ceil().x, Vector().x);
  EXPECT_FLT_EQ(Vector().ceil().y, Vector().y);
  EXPECT_FLT_EQ(Vector(5.1f, 3.9f).ceil().x, Vector(6.0f, 4.0f).x);
  EXPECT_FLT_EQ(Vector(5.1f, 3.9f).ceil().y, Vector(6.0f, 4.0f).y);
  EXPECT_FLT_EQ(Vector(-5.1f, -3.9f).ceil().x, Vector(-5.0f, -3.0f).x);
  EXPECT_FLT_EQ(Vector(-5.1f, -3.9f).ceil().y, Vector(-5.0f, -3.0f).y);
  EXPECT_FLT_EQ(Vector(-5.0f, 3.0f).ceil().x, Vector(-5.0f, 3.0f).x);
  EXPECT_FLT_EQ(Vector(-5.0f, 3.0f).ceil().y, Vector(-5.0f, 3.0f).y);
}

TEST(UNIT__Vector__operator_negate)
{
  Vector v1(0.0f, 0.0f);
  EXPECT_FLT_EQ((-v1).x, -0.0f);
  EXPECT_FLT_EQ((-v1).y, -0.0f);

  Vector v2(1.0f, 1.0f);
  EXPECT_FLT_EQ((-v2).x, -1.0f);
  EXPECT_FLT_EQ((-v2).y, -1.0f);

  Vector v3(-15643.65e7f, -1e-10f);
  EXPECT_FLT_EQ((-v3).x, 15643.65e7f);
  EXPECT_FLT_EQ((-v3).y, 1e-10f);
}

TEST(UNIT__Vector__operator_plus)
{
  Vector v1(143.0f, -756.0f);
  Vector v2(12.0f, 54.0f);

  EXPECT_FLT_EQ((v1 + v2).x, 143.0f + 12.0f);
  EXPECT_FLT_EQ((v1 + v2).y, -756.0f + 54.0f);
}

TEST(UNIT__Vector__operator_plus_equals)
{
  Vector v1(143.0f, -756.0f);
  v1 += Vector(12.0f, 54.0f);

  EXPECT_FLT_EQ(v1.x, 143.0f + 12.0f);
  EXPECT_FLT_EQ(v1.y, -756.0f + 54.0f);
}

TEST(UNIT__Vector__operator_minus)
{
  Vector v1(143.0f, -756.0f);
  Vector v2(12.0f, 54.0f);

  EXPECT_FLT_EQ((v1 - v2).x, 143.0f - 12.0f);
  EXPECT_FLT_EQ((v1 - v2).y, -756.0f - 54.0f);
}

TEST(UNIT__Vector__operator_minus_equals)
{
  Vector v1(143.0f, -756.0f);
  v1 -= Vector(12.0f, 54.0f);

  EXPECT_FLT_EQ(v1.x, 143.0f - 12.0f);
  EXPECT_FLT_EQ(v1.y, -756.0f - 54.0f);
}

TEST(UNIT__Vector__operator_times__vector)
{
  Vector v1(143.0f, -756.0f);
  Vector v2(12.0f, 54.0f);

  EXPECT_FLT_EQ((v1 * v2).x, 143.0f * 12.0f);
  EXPECT_FLT_EQ((v1 * v2).y, -756.0f * 54.0f);
}

TEST(UNIT__Vector__operator_times__float)
{
  Vector v1(143.0f, -756.0f);

  EXPECT_FLT_EQ((v1 * -27.0f).x, 143.0f * -27.0f);
  EXPECT_FLT_EQ((v1 * -27.0f).y, -756.0f * -27.0f);
}

TEST(UNIT__Vector__operator_times_equals__vector)
{
  Vector v1(143.0f, -756.0f);
  v1 *= Vector(12.0f, 54.0f);

  EXPECT_FLT_EQ(v1.x, 143.0f * 12.0f);
  EXPECT_FLT_EQ(v1.y, -756.0f * 54.0f);
}

TEST(UNIT__Vector__operator_times_equals__float)
{
  Vector v1(143.0f, -756.0f);
  v1 *= -27.0f;

  EXPECT_FLT_EQ(v1.x, 143.0f * -27.0f);
  EXPECT_FLT_EQ(v1.y, -756.0f * -27.0f);
}

TEST(UNIT__Vector__operator_divide__vector)
{
  Vector v1(143.0f, -756.0f);
  Vector v2(12.0f, 54.0f);

  EXPECT_FLT_EQ((v1 / v2).x, 143.0f / 12.0f);
  EXPECT_FLT_EQ((v1 / v2).y, -756.0f / 54.0f);
}

TEST(UNIT__Vector__operator_divide__float)
{
  Vector v1(143.0f, -756.0f);

  EXPECT_FLT_EQ((v1 / -27.0f).x, 143.0f / -27.0f);
  EXPECT_FLT_EQ((v1 / -27.0f).y, -756.0f / -27.0f);
}

TEST(UNIT__Vector__operator_divide_equals__vector)
{
  Vector v1(143.0f, -756.0f);
  v1 /= Vector(12.0f, 54.0f);

  EXPECT_FLT_EQ(v1.x, 143.0f / 12.0f);
  EXPECT_FLT_EQ(v1.y, -756.0f / 54.0f);
}

TEST(UNIT__Vector__operator_divide_equals__float)
{
  Vector v1(143.0f, -756.0f);
  v1 /= -27.0f;

  EXPECT_FLT_EQ(v1.x, 143.0f / -27.0f);
  EXPECT_FLT_EQ(v1.y, -756.0f / -27.0f);
}

TEST(UNIT__Vector__operator_modulo__vector)
{
  Vector v1(143.0f, -756.0f);
  Vector v2(12.0f, 54.0f);

  EXPECT_FLT_EQ((v1 % v2).x, fmodf(143.0f, 12.0f));
  EXPECT_FLT_EQ((v1 % v2).y, fmodf(-756.0f, 54.0f));
}

TEST(UNIT__Vector__operator_modulo__float)
{
  Vector v1(143.0f, -756.0f);

  EXPECT_FLT_EQ((v1 % -27.0f).x, fmodf(143.0f, -27.0f));
  EXPECT_FLT_EQ((v1 % -27.0f).y, fmodf(-756.0f, -27.0f));
}

TEST(UNIT__Vector__operator_modulo_equals__vector)
{
  // If anybody thinks this test suite is useless because the functions are
  // "too simple" to be prone to mistakes, know that at the moment of writing
  // this test, I had mistakenly written in Vector::operator%=() :
  //
  //     x = std::fmod(x, f);
  //     y = std::fmod(x, f);
  //     return *this;
  //
  // Notice the 'x' on the second line. Thanks to this test, I realised
  // something was wrong.

  Vector v1(143.0f, -756.0f);
  v1 %= Vector(12.0f, 54.0f);

  EXPECT_FLT_EQ(v1.x, fmodf(143.0f, 12.0f));
  EXPECT_FLT_EQ(v1.y, fmodf(-756.0f, 54.0f));
}

TEST(UNIT__Vector__operator_modulo_equals__float)
{
  Vector v1(143.0f, -756.0f);
  v1 %= -27.0f;

  EXPECT_FLT_EQ(v1.x, fmodf(143.0f, -27.0f));
  EXPECT_FLT_EQ(v1.y, fmodf(-756.0f, -27.0f));
}

TEST(UNIT__Vector__operator_equals)
{
  EXPECT(Vector() == Vector());
  EXPECT(Vector() == Vector(0.0f, 0.0f));
  EXPECT(Vector(8764.0e7f, -1e10f) == Vector(8764.0e7f, -1e10f));
  EXPECT(!(Vector(1.0f, 2.0f) == Vector(1.0f, 1.0f)));
  EXPECT(!(Vector(2.0f, 1.0f) == Vector(1.0f, 1.0f)));
}

TEST(UNIT__Vector__operator_not_equals)
{
  EXPECT(!(Vector() != Vector()));
  EXPECT(!(Vector() != Vector(0.0f, 0.0f)));
  EXPECT(!(Vector(8764.0e7f, -1e10f) != Vector(8764.0e7f, -1e10f)));
  EXPECT((Vector(1.0f, 2.0f) != Vector(1.0f, 1.0f)));
  EXPECT((Vector(2.0f, 1.0f) != Vector(1.0f, 1.0f)));
}

TEST(UNIT__Vector__operator_printstream)
{
  // There is no guarantee regarding what it will print, only that it won't
  // throw.
  std::stringstream s;
  s << Vector() << Vector(8764.0e-7f, -1e10f)
    << Vector(0.0f / 0.0f, -1e10f * 1e10f);
}
