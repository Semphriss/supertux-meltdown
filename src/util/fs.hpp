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

#ifndef HEADER_STM_UTIL_FS_HPP
#define HEADER_STM_UTIL_FS_HPP

#include <string>

#include <SDL.h>

class FS final
{
public:
  enum class OP
  {
    APPEND,
    READ,
    WRITE
  };

public:
  /** The pointer must be freed by the caller. */
  static SDL_RWops* get_rwops(const std::string& file, OP operation);
};

#endif
