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

#include "util/rect.hpp"

#include "util/math.hpp"
#include "util/size.hpp"
#include "util/vector.hpp"

Rect::Rect() :
  x1(0.f),
  y1(0.f),
  x2(0.f),
  y2(0.f)
{
}

Rect::Rect(const Size& size) :
  x1(0.f),
  y1(0.f),
  x2(size.w),
  y2(size.h)
{
}

Rect::Rect(float _x1, float _y1, float _x2, float _y2) :
  x1(_x1),
  y1(_y1),
  x2(_x2),
  y2(_y2)
{
}

Rect::Rect(const Vector& top_lft, const Size& size) :
  x1(top_lft.x),
  y1(top_lft.y),
  x2(top_lft.x + size.w),
  y2(top_lft.y + size.h)
{
}

Rect::Rect(const Vector& p1, const Vector& p2) :
  x1(std::min(p1.x, p2.x)),
  y1(std::min(p1.y, p2.y)),
  x2(std::max(p1.x, p2.x)),
  y2(std::max(p1.y, p2.y))
{
}

float
Rect::width() const
{
  return x2 - x1;
}

float
Rect::height() const
{
  return y2 - y1;
}

Size
Rect::size() const
{
  return Size(width(), height());
}

Vector
Rect::top_lft() const
{
  return Vector(x1, y1);
}

Vector
Rect::top_rgt() const
{
  return Vector(x2, y1);
}

Vector
Rect::bot_lft() const
{
  return Vector(x1, y2);
}

Vector
Rect::bot_rgt() const
{
  return Vector(x2, y2);
}

Vector
Rect::mid() const
{
  return Vector((x1 + x2) / 2.f, (y1 + y2) / 2.f);
}

bool
Rect::is_null() const
{
  return x1 == 0 && y1 == 0 && x2 == 0 && y2 == 0;
}

bool
Rect::is_valid() const
{
  return x1 < x2 && y1 < y2;
}

Rect&
Rect::move(const Vector& v)
{
  x1 += v.x;
  x2 += v.x;
  y1 += v.y;
  y2 += v.y;
  return *this;
}

Rect&
Rect::clip(const Rect& rect)
{
  x1 = std::max(x1, rect.x1);
  y1 = std::max(y1, rect.y1);
  x2 = std::min(x2, rect.x2);
  y2 = std::min(y2, rect.y2);
  return *this;
}

Rect&
Rect::grow(float f)
{
  x1 -= f;
  y1 -= f;
  x2 += f;
  y2 += f;
  return *this;
}


Rect&
Rect::fix()
{
  if (x1 > x2)
  {
    std::swap(x1, x2);
  }

  if (y1 > y2)
  {
    std::swap(y1, y2);
  }

  return *this;
}

Rect&
Rect::set_x1(float x1)
{
  this->x1 = x1;
  return *this;
}

Rect&
Rect::set_x2(float x2)
{
  this->x2 = x2;
  return *this;
}

Rect&
Rect::set_y1(float y1)
{
  this->y1 = y1;
  return *this;
}

Rect&
Rect::set_y2(float y2)
{
  this->y2 = y2;
  return *this;
}

Rect&
Rect::resize(const Size& s)
{
  x2 = x1 + s.w;
  y2 = y1 + s.h;
  return *this;
}

Rect&
Rect::scale(const Size& s)
{
  x2 = (x2 - x1) * s.w + x1;
  y2 = (y2 - y1) * s.h + y1;
  return *this;
}

Rect&
Rect::scale(float f)
{
  x2 = (x2 - x1) * f + x1;
  y2 = (y2 - y1) * f + y1;
  return *this;
}

Rect&
Rect::flip(bool h, bool v)
{
  if (h)
  {
    std::swap(x1, x2);
  }

  if (v)
  {
    std::swap(y1, y2);
  }

  return *this;
}

Rect
Rect::moved(const Vector& v) const
{
  return Rect(Vector(x1, y1) - v, Size(width(), height()));
}

Rect
Rect::clipped(const Rect& rect) const
{
  return Rect(std::max(x1, rect.x1), std::max(y1, rect.y1),
              std::min(x2, rect.x2), std::min(y2, rect.y2));
}

Rect
Rect::grown(float f) const
{
  return Rect(x1 - f, y1 - f, x2 + f, y2 + f);
}

Rect
Rect::fixed() const
{
  Rect r = *this;
  return r.fix();
}

Rect
Rect::with_x1(float x1) const
{
  return Rect(*this).set_x1(x1);
}

Rect
Rect::with_x2(float x2) const
{
  return Rect(*this).set_x2(x2);
}

Rect
Rect::with_y1(float y1) const
{
  return Rect(*this).set_y1(y1);
}

Rect
Rect::with_y2(float y2) const
{
  return Rect(*this).set_y2(y2);
}

Rect
Rect::resized(const Size& s) const
{
  return Rect(top_lft(), s);
}

Rect
Rect::scaled(const Size& s) const
{
  return Rect(top_lft(), size() * s);
}

Rect
Rect::scaled(float f) const
{
  return Rect(top_lft(), size() * f);
}

Rect
Rect::flipped(bool h, bool v) const
{
  return Rect(top_lft() + Vector(size() * Size(h, v)),
              size() * Size(1 - h * 2, 1 - v * 2));
}

bool
Rect::contains(const Vector& point) const
{
  return point.x >= x1 && point.x < x2 && point.y >= y1 && point.y < y2;
}

std::pair<Vector, Vector>
Rect::clip_line(const Vector& p1, const Vector& p2) const
{
  if (contains(p1) && contains(p2))
    return { p1, p2 };

  if (p1.x == p2.x)
  {
    if (p1.x < x1 || p1.x > x2)
      return { Vector(), Vector() };

    return {
      Vector(p1.x, Math::clamp(y1, y2, p1.y)),
      Vector(p2.x, Math::clamp(y1, y2, p2.y))
    };
  }
  else if (p1.y == p2.y)
  {
    if (p1.y < y1 || p1.y > y2)
      return { Vector(), Vector() };

    return {
      Vector(Math::clamp(x1, x2, p1.x), p1.y),
      Vector(Math::clamp(x1, x2, p2.x), p2.y)
    };
  }
  else
  {
    float a = (p2.y - p1.y) / (p2.x - p1.x);
    float b = p1.y - a * p1.x;

    Vector r1 = p1, r2 = p2;
    if (a * x1 + b >= y1 && a * x1 + b <= y2)
    {
      if (p1.x < x1 && p2.x > x1)
        r1 = Vector(x1, a * x1 + b);
      if (p2.x < x1 && p1.x > x1)
        r2 = Vector(x1, a * x1 + b);
    }

    if (a * x2 + b >= y1 && a * x2 + b <= y2)
    {
      if (p1.x < x2 && p2.x > x2)
        r2 = Vector(x2, a * x2 + b);
      if (p2.x < x2 && p1.x > x2)
        r1 = Vector(x2, a * x2 + b);
    }

    if ((y1 - b) / a >= x1 && (y1 - b) / a <= x2)
    {
      if (p1.y < y1 && p2.y > y1)
        r1 = Vector((y1 - b) / a, y1);
      if (p2.y < y1 && p1.y > y1)
        r2 = Vector((y1 - b) / a, y1);
    }

    if ((y2 - b) / a >= x1 && (y2 - b) / a <= x2)
    {
      if (p1.y < y2 && p2.y > y2)
        r2 = Vector((y2 - b) / a, y2);
      if (p2.y < y2 && p1.y > y2)
        r1 = Vector((y2 - b) / a, y2);
    }

    if (r1 == p1 && r2 == p2)
      return { Vector(), Vector() };

    return { r1, r2 };
  }
}

Rect
Rect::operator*(float f) const
{
  return Rect(top_lft() * f, size() * f);
}

Rect&
Rect::operator*=(float f)
{
  x1 *= f;
  y1 *= f;
  x2 *= f;
  y2 *= f;
  return *this;
}

Rect
Rect::operator/(float f) const
{
  return Rect(top_lft() / f, size() / f);
}

Rect&
Rect::operator/=(float f)
{
  x1 /= f;
  y1 /= f;
  x2 /= f;
  y2 /= f;
  return *this;
}

Rect
Rect::operator+(const Size& s) const
{
  return Rect(Vector(x1, y1), size() + s);
}

Rect&
Rect::operator+=(const Size& s)
{
  x2 += s.w;
  y2 += s.h;
  return *this;
}

Rect
Rect::operator-(const Size& s) const
{
  return Rect(Vector(x1, y1), size() - s);
}

Rect&
Rect::operator-=(const Size& s)
{
  x2 -= s.w;
  y2 -= s.h;
  return *this;
}

Rect
Rect::operator*(const Size& s) const
{
  return Rect(Vector(x1, y1) * s.vector(), size() * s);
}

Rect&
Rect::operator*=(const Size& s)
{
  x1 *= s.w;
  y1 *= s.h;
  x2 *= s.w;
  y2 *= s.h;
  return *this;
}

Rect
Rect::operator/(const Size& s) const
{
  return Rect(Vector(x1, y1), size() / s);
}

Rect&
Rect::operator/=(const Size& s)
{
  x2 = (x2 - x1) / s.w + x1;
  y2 = (y2 - y1) / s.h + y1;
  return *this;
}

Rect
Rect::operator+(const Vector& v) const
{
  return Rect(Vector(x1, y1) + v, size());
}

Rect&
Rect::operator+=(const Vector& v)
{
  x1 += v.x;
  y1 += v.y;
  x2 += v.x;
  y2 += v.y;
  return *this;
}

Rect
Rect::operator-(const Vector& v) const
{
  return Rect(Vector(x1, y1) - v, size());
}

Rect&
Rect::operator-=(const Vector& v)
{
  x1 -= v.x;
  y1 -= v.y;
  x2 -= v.x;
  y2 -= v.y;
  return *this;
}

Rect
Rect::operator*(const Vector& v) const
{
  return Rect(Vector(x1, y1) * v, size() * v.size());
}

Rect&
Rect::operator*=(const Vector& v)
{
  x1 *= v.x;
  y1 *= v.y;
  x2 *= v.x;
  y2 *= v.y;
  return *this;
}

Rect
Rect::operator/(const Vector& v) const
{
  return Rect(Vector(x1, y1) / v, size() / v.size());
}

Rect&
Rect::operator/=(const Vector& v)
{
  x1 /= v.x;
  y1 /= v.y;
  x2 /= v.x;
  y2 /= v.y;
  return *this;
}

bool
Rect::operator==(const Rect& rect) const
{
  return x1 == rect.x1 && y1 == rect.y1 && x2 == rect.x2 && y2 == rect.y2;
}

bool
Rect::operator!=(const Rect& rect) const
{
  return x1 != rect.x1 || y1 != rect.y1 || x2 != rect.x2 || y2 != rect.y2;
}

std::ostream& operator<<(std::ostream& stream, const Rect& rect)
{
  return stream << "Rect(" << rect.x1 << ", " << rect.y1 << ", " << rect.x2
                << ", " << rect.y2 << ")";
}
