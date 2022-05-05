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

#include "util/size.hpp"

#include <cmath>

#include "util/vector.hpp"

Size::Size() :
  w(),
  h()
{
}

Size::Size(float _w, float _h) :
  w(_w),
  h(_h)
{
}

Size::Size(const Vector& v) :
  w(v.x),
  h(v.y)
{
}

Vector
Size::vector() const
{
  return Vector(*this);
}

float
Size::length() const
{
  return std::sqrt(w * w + h * h);
}

Size
Size::floor() const
{
  return Size(std::floor(w), std::floor(h));
}

Size
Size::round() const
{
  return Size(std::round(w), std::round(h));
}

Size
Size::ceil() const
{
  return Size(std::ceil(w), std::ceil(h));
}

Size
Size::operator*(float f) const
{
  return Size(w * f, h * f);
}

Size
Size::operator*(const Size& s) const
{
  return Size(w * s.w, h * s.h);
}

Size&
Size::operator*=(float f)
{
  w *= f;
  h *= f;
  return *this;
}

Size&
Size::operator*=(const Size& s)
{
  w *= s.w;
  h *= s.h;
  return *this;
}

Size
Size::operator/(float f) const
{
  return Size(w / f, h / f);
}

Size
Size::operator/(const Size& s) const
{
  return Size(w / s.w, h / s.h);
}

Size&
Size::operator/=(float f)
{
  w /= f;
  h /= f;
  return *this;
}

Size&
Size::operator/=(const Size& s)
{
  w /= s.w;
  h /= s.h;
  return *this;
}

Size
Size::operator+(const Size& s) const
{
  return Size(w + s.w, h + s.h);
}

Size&
Size::operator+=(const Size& s)
{
  w += s.w;
  h += s.h;
  return *this;
}

Size
Size::operator-(const Size& s) const
{
  return Size(w - s.w, h - s.h);
}

Size&
Size::operator-=(const Size& s)
{
  w -= s.w;
  h -= s.h;
  return *this;
}

bool
Size::operator==(const Size& s) const
{
  return w == s.w && h == s.h;
}

bool
Size::operator!=(const Size& s) const
{
  return w != s.w || h != s.h;
}

std::ostream&
operator<<(std::ostream& out, const Size& s)
{
  out << "Size(" << s.w << ", " << s.h << ")";
  return out;
}
