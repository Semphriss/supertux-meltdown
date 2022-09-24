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

#include "util/size.hpp"

#include <cmath>
#include <sstream>

#include "util/vector.hpp"

TEST(UNIT__Size__ctor)
{
  Size s1;
  EXPECT_FLT_EQ(s1.w, 0.0f);
  EXPECT_FLT_EQ(s1.h, 0.0f);

  Size s2(1234.0f, -0.5f);
  EXPECT_FLT_EQ(s2.w, 1234.0f);
  EXPECT_FLT_EQ(s2.h, -0.5f);

  Size s3(Vector(0.3125e-1f, -8.75e2f));
  EXPECT_FLT_EQ(s3.w, 0.3125e-1f);
  EXPECT_FLT_EQ(s3.h, -8.75e2f);
}

TEST(UNIT__Size__vector)
{
  EXPECT_FLT_EQ(Size().vector().x, Vector().x);
  EXPECT_FLT_EQ(Size().vector().y, Vector().y);
  EXPECT_FLT_EQ(Size(532.92f, 7537.765e-4f).vector().x,
                Vector(532.92f, 7537.765e-4f).x);
  EXPECT_FLT_EQ(Size(532.92f, 7537.765e-4f).vector().y,
                Vector(532.92f, 7537.765e-4f).y);
}

TEST(UNIT__Size__length)
{
  EXPECT_FLT_EQ(Size().length(), 0.0f);
  EXPECT_FLT_EQ(Size(532.92f, 34.1f).length(), 534.0098654519f);
}

TEST(UNIT__Size__floor)
{
  EXPECT_FLT_EQ(Size().floor().w, Size().w);
  EXPECT_FLT_EQ(Size().floor().h, Size().h);
  EXPECT_FLT_EQ(Size(5.1f, 3.9f).floor().w, Size(5.0f, 3.0f).w);
  EXPECT_FLT_EQ(Size(5.1f, 3.9f).floor().h, Size(5.0f, 3.0f).h);
  EXPECT_FLT_EQ(Size(-5.1f, -3.9f).floor().w, Size(-6.0f, -4.0f).w);
  EXPECT_FLT_EQ(Size(-5.1f, -3.9f).floor().h, Size(-6.0f, -4.0f).h);
  EXPECT_FLT_EQ(Size(-5.0f, 3.0f).floor().w, Size(-5.0f, 3.0f).w);
  EXPECT_FLT_EQ(Size(-5.0f, 3.0f).floor().h, Size(-5.0f, 3.0f).h);
}

TEST(UNIT__Size__round)
{
  EXPECT_FLT_EQ(Size().round().w, Size().w);
  EXPECT_FLT_EQ(Size().round().h, Size().h);
  EXPECT_FLT_EQ(Size(5.1f, 3.9f).round().w, Size(5.0f, 4.0f).w);
  EXPECT_FLT_EQ(Size(5.1f, 3.9f).round().h, Size(5.0f, 4.0f).h);
  EXPECT_FLT_EQ(Size(5.5f, -5.5f).round().w, Size(6.0f, -6.0f).w);
  EXPECT_FLT_EQ(Size(5.5f, -5.5f).round().h, Size(6.0f, -6.0f).h);
  EXPECT_FLT_EQ(Size(-5.0f, 3.0f).floor().w, Size(-5.0f, 3.0f).w);
  EXPECT_FLT_EQ(Size(-5.0f, 3.0f).floor().h, Size(-5.0f, 3.0f).h);
}

TEST(UNIT__Size__ceil)
{
  EXPECT_FLT_EQ(Size().ceil().w, Size().w);
  EXPECT_FLT_EQ(Size().ceil().h, Size().h);
  EXPECT_FLT_EQ(Size(5.1f, 3.9f).ceil().w, Size(6.0f, 4.0f).w);
  EXPECT_FLT_EQ(Size(5.1f, 3.9f).ceil().h, Size(6.0f, 4.0f).h);
  EXPECT_FLT_EQ(Size(-5.1f, -3.9f).ceil().w, Size(-5.0f, -3.0f).w);
  EXPECT_FLT_EQ(Size(-5.1f, -3.9f).ceil().h, Size(-5.0f, -3.0f).h);
  EXPECT_FLT_EQ(Size(-5.0f, 3.0f).ceil().w, Size(-5.0f, 3.0f).w);
  EXPECT_FLT_EQ(Size(-5.0f, 3.0f).ceil().h, Size(-5.0f, 3.0f).h);
}

TEST(UNIT__Size__operator_negate)
{
  Size s1(0.0f, 0.0f);
  EXPECT_FLT_EQ((-s1).w, -0.0f);
  EXPECT_FLT_EQ((-s1).h, -0.0f);

  Size s2(1.0f, 1.0f);
  EXPECT_FLT_EQ((-s2).w, -1.0f);
  EXPECT_FLT_EQ((-s2).h, -1.0f);

  Size s3(-15643.65e7f, -1e-10f);
  EXPECT_FLT_EQ((-s3).w, 15643.65e7f);
  EXPECT_FLT_EQ((-s3).h, 1e-10f);
}

TEST(UNIT__Size__operator_plus)
{
  Size s1(143.0f, -756.0f);
  Size s2(12.0f, 54.0f);

  EXPECT_FLT_EQ((s1 + s2).w, 143.0f + 12.0f);
  EXPECT_FLT_EQ((s1 + s2).h, -756.0f + 54.0f);
}

TEST(UNIT__Size__operator_plus_equals)
{
  Size s1(143.0f, -756.0f);
  s1 += Size(12.0f, 54.0f);

  EXPECT_FLT_EQ(s1.w, 143.0f + 12.0f);
  EXPECT_FLT_EQ(s1.h, -756.0f + 54.0f);
}

TEST(UNIT__Size__operator_minus)
{
  Size s1(143.0f, -756.0f);
  Size s2(12.0f, 54.0f);

  EXPECT_FLT_EQ((s1 - s2).w, 143.0f - 12.0f);
  EXPECT_FLT_EQ((s1 - s2).h, -756.0f - 54.0f);
}

TEST(UNIT__Size__operator_minus_equals)
{
  Size s1(143.0f, -756.0f);
  s1 -= Size(12.0f, 54.0f);

  EXPECT_FLT_EQ(s1.w, 143.0f - 12.0f);
  EXPECT_FLT_EQ(s1.h, -756.0f - 54.0f);
}

TEST(UNIT__Size__operator_times__size)
{
  Size s1(143.0f, -756.0f);
  Size s2(12.0f, 54.0f);

  EXPECT_FLT_EQ((s1 * s2).w, 143.0f * 12.0f);
  EXPECT_FLT_EQ((s1 * s2).h, -756.0f * 54.0f);
}

TEST(UNIT__Size__operator_times__float)
{
  Size s1(143.0f, -756.0f);

  EXPECT_FLT_EQ((s1 * -27.0f).w, 143.0f * -27.0f);
  EXPECT_FLT_EQ((s1 * -27.0f).h, -756.0f * -27.0f);
}

TEST(UNIT__Size__operator_times_equals__size)
{
  Size s1(143.0f, -756.0f);
  s1 *= Size(12.0f, 54.0f);

  EXPECT_FLT_EQ(s1.w, 143.0f * 12.0f);
  EXPECT_FLT_EQ(s1.h, -756.0f * 54.0f);
}

TEST(UNIT__Size__operator_times_equals__float)
{
  Size s1(143.0f, -756.0f);
  s1 *= -27.0f;

  EXPECT_FLT_EQ(s1.w, 143.0f * -27.0f);
  EXPECT_FLT_EQ(s1.h, -756.0f * -27.0f);
}

TEST(UNIT__Size__operator_divide__size)
{
  Size s1(143.0f, -756.0f);
  Size s2(12.0f, 54.0f);

  EXPECT_FLT_EQ((s1 / s2).w, 143.0f / 12.0f);
  EXPECT_FLT_EQ((s1 / s2).h, -756.0f / 54.0f);
}

TEST(UNIT__Size__operator_divide__float)
{
  Size s1(143.0f, -756.0f);

  EXPECT_FLT_EQ((s1 / -27.0f).w, 143.0f / -27.0f);
  EXPECT_FLT_EQ((s1 / -27.0f).h, -756.0f / -27.0f);
}

TEST(UNIT__Size__operator_divide_equals__size)
{
  Size s1(143.0f, -756.0f);
  s1 /= Size(12.0f, 54.0f);

  EXPECT_FLT_EQ(s1.w, 143.0f / 12.0f);
  EXPECT_FLT_EQ(s1.h, -756.0f / 54.0f);
}

TEST(UNIT__Size__operator_divide_equals__float)
{
  Size s1(143.0f, -756.0f);
  s1 /= -27.0f;

  EXPECT_FLT_EQ(s1.w, 143.0f / -27.0f);
  EXPECT_FLT_EQ(s1.h, -756.0f / -27.0f);
}

TEST(UNIT__Size__operator_modulo__size)
{
  Size s1(143.0f, -756.0f);
  Size s2(12.0f, 54.0f);

  EXPECT_FLT_EQ((s1 % s2).w, fmodf(143.0f, 12.0f));
  EXPECT_FLT_EQ((s1 % s2).h, fmodf(-756.0f, 54.0f));
}

TEST(UNIT__Size__operator_modulo__float)
{
  Size s1(143.0f, -756.0f);

  EXPECT_FLT_EQ((s1 % -27.0f).w, fmodf(143.0f, -27.0f));
  EXPECT_FLT_EQ((s1 % -27.0f).h, fmodf(-756.0f, -27.0f));
}

TEST(UNIT__Size__operator_modulo_equals__size)
{
  // If anybody thinks this test suite is useless because the functions are
  // "too simple" to be prone to mistakes, know that at the moment of writing
  // this test, I had mistakenly written in Size::operator%=() :
  //
  //     x = std::fmod(x, f);
  //     y = std::fmod(x, f);
  //     return *this;
  //
  // Notice the 'x' on the second line. Thanks to this test, I realised
  // something was wrong, and I fixed the issue.

  Size s1(143.0f, -756.0f);
  s1 %= Size(12.0f, 54.0f);

  EXPECT_FLT_EQ(s1.w, fmodf(143.0f, 12.0f));
  EXPECT_FLT_EQ(s1.h, fmodf(-756.0f, 54.0f));
}

TEST(UNIT__Size__operator_modulo_equals__float)
{
  Size s1(143.0f, -756.0f);
  s1 %= -27.0f;

  EXPECT_FLT_EQ(s1.w, fmodf(143.0f, -27.0f));
  EXPECT_FLT_EQ(s1.h, fmodf(-756.0f, -27.0f));
}

TEST(UNIT__Size__operator_equals)
{
  EXPECT(Size() == Size());
  EXPECT(Size() == Size(0.0f, 0.0f));
  EXPECT(Size(8764.0e7f, -1e10f) == Size(8764.0e7f, -1e10f));
  EXPECT(!(Size(1.0f, 2.0f) == Size(1.0f, 1.0f)));
  EXPECT(!(Size(2.0f, 1.0f) == Size(1.0f, 1.0f)));
}

TEST(UNIT__Size__operator_not_equals)
{
  EXPECT(!(Size() != Size()));
  EXPECT(!(Size() != Size(0.0f, 0.0f)));
  EXPECT(!(Size(8764.0e7f, -1e10f) != Size(8764.0e7f, -1e10f)));
  EXPECT((Size(1.0f, 2.0f) != Size(1.0f, 1.0f)));
  EXPECT((Size(2.0f, 1.0f) != Size(1.0f, 1.0f)));
}

TEST(UNIT__Size__operator_printstream)
{
  // There is no guarantee regarding what it will print, only that it won't
  // throw.
  std::stringstream s;
  s << Size() << Size(8764.0e-7f, -1e10f)
    << Size(0.0f / 0.0f, -1e10f * 1e10f);
}
