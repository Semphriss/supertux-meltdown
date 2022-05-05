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

#ifndef HEADER_STM_UTIL_SIZE_HPP
#define HEADER_STM_UTIL_SIZE_HPP

#include <ostream>

class Vector;

class Size final
{
public:
  Size();
  Size(float _w, float _h);
  explicit Size(const Vector& s);

  Vector vector() const;
  float length() const;

  Size floor() const;
  Size round() const;
  Size ceil() const;

  Size operator*(float f) const;
  Size operator*(const Size& s) const;
  Size& operator*=(float f);
  Size& operator*=(const Size& s);
  Size operator/(float f) const;
  Size operator/(const Size& s) const;
  Size& operator/=(float f);
  Size& operator/=(const Size& s);
  Size operator+(const Size& s) const;
  Size& operator+=(const Size& s);
  Size operator-(const Size& s) const;
  Size& operator-=(const Size& s);
  bool operator==(const Size& v) const;
  bool operator!=(const Size& v) const;

public:
  float w, h;
};

std::ostream& operator<<(std::ostream& out, const Size& s);

#endif
