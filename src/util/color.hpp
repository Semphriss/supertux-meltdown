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

#ifndef HEADER_STM_UTIL_COLOR_HPP
#define HEADER_STM_UTIL_COLOR_HPP

#include <ostream>

class Color final
{
public:
  Color();
  Color(const Color& c) = default;
  Color(float _r, float _g, float _b, float _a = 1.0f);
  Color(const Color& c, float _a);

  Color operator+(const Color& color) const;
  Color& operator+=(const Color& color);
  Color operator-(const Color& color) const;
  Color& operator-=(const Color& color);
  Color operator*(const Color& color) const;
  Color& operator*=(const Color& color);
  Color operator/(const Color& color) const;
  Color& operator/=(const Color& color);

public:
  float r, g, b, a;
};

std::ostream& operator<<(std::ostream& stream, const Color& color);

#endif
