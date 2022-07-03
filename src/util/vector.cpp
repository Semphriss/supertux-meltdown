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

#include "util/vector.hpp"

#include <cmath>

#include "util/size.hpp"

Vector::Vector() :
  x(),
  y()
{
}

Vector::Vector(float _x, float _y) :
  x(_x),
  y(_y)
{
}

Vector::Vector(const Size& s) :
  x(s.w),
  y(s.h)
{
}

Size
Vector::size() const
{
  return Size(*this);
}

float
Vector::length() const
{
  return std::sqrt(x * x + y * y);
}

Vector
Vector::floor() const
{
  return Vector(std::floor(x), std::floor(y));
}

Vector
Vector::round() const
{
  return Vector(std::round(x), std::round(y));
}

Vector
Vector::ceil() const
{
  return Vector(std::ceil(x), std::ceil(y));
}

Vector
Vector::operator-() const
{
  return Vector(-x, -y);
}

Vector
Vector::operator+(const Vector& v) const
{
  return Vector(x + v.x, y + v.y);
}

Vector&
Vector::operator+=(const Vector& v)
{
  x += v.x;
  y += v.y;
  return *this;
}

Vector
Vector::operator-(const Vector& v) const
{
  return Vector(x - v.x, y - v.y);
}

Vector&
Vector::operator-=(const Vector& v)
{
  x -= v.x;
  y -= v.y;
  return *this;
}

Vector
Vector::operator*(const Vector& v) const
{
  return Vector(x * v.x, y * v.y);
}

Vector
Vector::operator*(float f) const
{
  return Vector(x * f, y * f);
}

Vector&
Vector::operator*=(const Vector& v)
{
  x *= v.x;
  y *= v.y;
  return *this;
}

Vector&
Vector::operator*=(float f)
{
  x *= f;
  y *= f;
  return *this;
}

Vector
Vector::operator/(const Vector& v) const
{
  return Vector(x / v.x, y / v.y);
}

Vector
Vector::operator/(float f) const
{
  return Vector(x / f, y / f);
}

Vector&
Vector::operator/=(const Vector& v)
{
  x /= v.x;
  y /= v.y;
  return *this;
}

Vector&
Vector::operator/=(float f)
{
  x /= f;
  y /= f;
  return *this;
}

Vector
Vector::operator%(const Vector& v) const
{
  return Vector(std::fmod(x, v.x), std::fmod(y, v.y));
}

Vector
Vector::operator%(float f) const
{
  return Vector(std::fmod(x, f), std::fmod(y, f));
}

Vector&
Vector::operator%=(const Vector& v)
{
  x = std::fmod(x, v.x);
  y = std::fmod(y, v.y);
  return *this;
}

Vector&
Vector::operator%=(float f)
{
  x = std::fmod(x, f);
  y = std::fmod(y, f);
  return *this;
}

bool
Vector::operator==(const Vector& v) const
{
  return x == v.x && y == v.y;
}

bool
Vector::operator!=(const Vector& v) const
{
  return x != v.x || y != v.y;
}

std::ostream&
operator<<(std::ostream& out, const Vector& v)
{
  out << "Vector(" << v.x << ", " << v.y << ")";
  return out;
}
