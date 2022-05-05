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

#ifndef HEADER_STM_UTIL_RECT_HPP
#define HEADER_STM_UTIL_RECT_HPP

#include <utility>
#include <ostream>

class Size;
class Vector;

class Rect final
{
public:
  Rect();
  Rect(const Size& size);
  Rect(float _x1, float _y1, float _x2, float _y2);
  Rect(const Vector& top_lft, const Size& size);
  Rect(const Vector& p1, const Vector& p2);

  float width() const;
  float height() const;
  Size size() const;
  Vector top_lft() const;
  Vector top_rgt() const;
  Vector bot_lft() const;
  Vector bot_rgt() const;
  Vector mid() const;
  bool is_null() const;
  bool is_valid() const;

  Rect& move(const Vector& v);
  Rect& clip(const Rect& rect);
  Rect& grow(float f);
  Rect& fix();
  Rect& set_x1(float x1);
  Rect& set_x2(float x2);
  Rect& set_y1(float y1);
  Rect& set_y2(float y2);

  Rect moved(const Vector& v) const;
  Rect clipped(const Rect& rect) const;
  Rect grown(float f) const;
  Rect fixed() const;
  Rect with_x1(float x1) const;
  Rect with_x2(float x2) const;
  Rect with_y1(float y1) const;
  Rect with_y2(float y2) const;

  bool contains(const Vector& point) const;
  std::pair<Vector, Vector> clip_line(const Vector& p1, const Vector& p2) const;

  Rect operator+(const Size& s) const;
  Rect& operator+=(const Size& s);
  Rect operator-(const Size& s) const;
  Rect& operator-=(const Size& s);
  Rect operator*(const Size& s) const;
  Rect& operator*=(const Size& s);
  Rect operator/(const Size& s) const;
  Rect& operator/=(const Size& s);
  Rect operator+(const Vector& v) const;
  Rect& operator+=(const Vector& v);
  Rect operator-(const Vector& v) const;
  Rect& operator-=(const Vector& v);
  Rect operator*(const Vector& v) const;
  Rect& operator*=(const Vector& v);
  Rect operator/(const Vector& v) const;
  Rect& operator/=(const Vector& v);
  bool operator==(const Rect& rect) const;
  bool operator!=(const Rect& rect) const;

public:
  float x1, y1, x2, y2;
};

std::ostream& operator<<(std::ostream& stream, const Rect& rect);

#endif
