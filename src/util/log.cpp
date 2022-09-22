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

#include "util/log.hpp"

#include <iostream>

Log::NullStream::NullStream() :
  std::ios(0),
  std::ostream(0)
{
}

Log::NullStream Log::s_null_stream;

std::ostream* Log::s_out = &std::cout;

std::ostream* Log::s_err = &std::cerr;

Log::Level Log::s_level = Log::Level::WARNING;

std::ostream&
Log::log(Level lv, const char* l, const char* file, int line)
{
  if (!s_err || lv > s_level)
    return s_null_stream;

  *s_err << "[" << l << "] " << file << ":" << line << ": ";

  return *s_err;
}

std::ostream&
Log::print()
{
  return Log::s_out ? *Log::s_out : s_null_stream;
}