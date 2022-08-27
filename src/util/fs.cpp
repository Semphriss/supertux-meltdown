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

#include "util/fs.hpp"

#include <stdexcept>

#include "physfs.h"

// Note: This won't work if the file is in an archive.
std::string
FS::path(const std::string& file)
{
  std::string path = file;
  std::string separator = PHYSFS_getDirSeparator();

  if (separator != "/")
  {
    size_t pos = 0;
    while ((pos = path.find("/", pos)) != std::string::npos)
    {
      path.replace(pos, 1, separator);
      pos += separator.length();
    }
  }

  const char* dir = PHYSFS_getRealDir(file.c_str());

  if (!dir)
  {
    std::string dirlist = "";
    char **dirs;

    for (dirs = PHYSFS_getSearchPath(); *dirs != NULL; dirs++)
    {
      dirlist += std::string(*dirs) + ";";
    }

    throw std::runtime_error("File '" + file + "' not found in: " + dirlist);
  }

  return std::string(dir) + separator + path;
}
