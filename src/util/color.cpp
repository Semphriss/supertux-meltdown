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

#include "util/color.hpp"

Color::Color() :
  r(),
  g(),
  b(),
  a(1.f)
{
}

Color::Color(float _r, float _g, float _b, float _a) :
  r(_r),
  g(_g),
  b(_b),
  a(_a)
{
}

Color::Color(const Color& c, float _a) :
  r(c.r),
  g(c.g),
  b(c.b),
  a(_a)
{
}

Color
Color::operator+(const Color& color) const
{
  return Color(r + color.r, g + color.g, b + color.b, a + color.a);
}

Color&
Color::operator+=(const Color& color)
{
  r += color.r;
  g += color.g;
  b += color.b;
  a += color.a;
  return *this;
}

Color
Color::operator-(const Color& color) const
{
  return Color(r - color.r, g - color.g, b - color.b, a - color.a);
}

Color&
Color::operator-=(const Color& color)
{
  r -= color.r;
  g -= color.g;
  b -= color.b;
  a -= color.a;
  return *this;
}

Color
Color::operator*(const Color& color) const
{
  return Color(r * color.r, g * color.g, b * color.b, a * color.a);
}

Color&
Color::operator*=(const Color& color)
{
  r *= color.r;
  g *= color.g;
  b *= color.b;
  a *= color.a;
  return *this;
}

Color
Color::operator/(const Color& color) const
{
  return Color(r / color.r, g / color.g, b / color.b, a / color.a);
}

Color&
Color::operator/=(const Color& color)
{
  r /= color.r;
  g /= color.g;
  b /= color.b;
  a /= color.a;
  return *this;
}

std::ostream& operator<<(std::ostream& stream, const Color& color)
{
  return stream << "Color(" << color.r << ", " << color.g << ", " << color.b
                << ", " << color.a << ")";
}
