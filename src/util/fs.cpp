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

#include <cstring>
#include <iostream>
#include <stdexcept>

#include "physfs.h"

SDL_RWops*
FS::get_rwops(const std::string& file, OP operation)
{
  auto ops = std::make_unique<SDL_RWops>();

  ops->size = [] (SDL_RWops* ops) -> Sint64
  {
    auto* file = static_cast<PHYSFS_file*>(ops->hidden.unknown.data1);
    return static_cast<Sint64>(PHYSFS_fileLength(file));
  };

  ops->seek = [] (SDL_RWops* ops, Sint64 offset, int whence) -> Sint64
  {
    auto* file = static_cast<PHYSFS_file*>(ops->hidden.unknown.data1);

    int success = 0;
    switch (whence) {
      case SEEK_SET:
        success = PHYSFS_seek(file, offset);
        break;

      case SEEK_CUR:
        success = PHYSFS_seek(file, offset + PHYSFS_tell(file));
        break;

      case SEEK_END:
        success = PHYSFS_seek(file, offset + PHYSFS_fileLength(file) + offset);
        break;

      default:
        return static_cast<Sint64>(-1);
        break;
    }

    if (!success)
    {
      std::cerr << "Problem when seeking in file '" << ops->hidden.unknown.data2
                << "': " << get_physfs_err() << std::endl;
      return static_cast<Sint64>(-1);
    }

    return static_cast<Sint64>(PHYSFS_tell(file));
  };

  ops->read = [] (SDL_RWops *ops, void *data, size_t size, size_t num) -> size_t
  {
    auto* file = static_cast<PHYSFS_file*>(ops->hidden.unknown.data1);

#if PHYSFS_VER_MAJOR > 2 || PHYSFS_VER_MAJOR == 2 && PHYSFS_VER_MINOR >= 1
    auto read = PHYSFS_readBytes(file, data, size * num);

    if (read < 0)
    {
      std::cerr << "Problem when reading from file '"
                << ops->hidden.unknown.data2 << "': " << get_physfs_err()
                << std::endl;
      return static_cast<size_t>(0);
    }

    return static_cast<size_t>(read / size);
#else
    auto read = PHYSFS_read(file, data, size, num);

    if (read < 0)
    {
      std::cerr << "Problem when reading from file '"
                << ops->hidden.unknown.data2 << "': " << get_physfs_err()
                << std::endl;
      return static_cast<size_t>(0);
    }

    return static_cast<size_t>(read);
#endif
  };

  ops->write = [] (SDL_RWops *ops, const void *data, size_t size, size_t num)
                   -> size_t
  {
    auto* file = static_cast<PHYSFS_file*>(ops->hidden.unknown.data1);

#if PHYSFS_VER_MAJOR > 2 || PHYSFS_VER_MAJOR == 2 && PHYSFS_VER_MINOR >= 1
    auto written = PHYSFS_writeBytes(file, data, size * num);

    if (written < 0)
    {
      std::cerr << "Problem when writing to file: " << get_physfs_err()
                << std::endl;
      return static_cast<size_t>(0);
    }

    return static_cast<size_t>(written / size);
#else
    auto written = PHYSFS_write(file, data, size, num);

    if (written < 0)
    {
      std::cerr << "Problem when writing to file: " << get_physfs_err()
                << std::endl;
      return static_cast<size_t>(0);
    }

    return static_cast<size_t>(written);
#endif
  };

  ops->close = [] (SDL_RWops *ops) -> int
  {
    auto* file = static_cast<PHYSFS_file*>(ops->hidden.unknown.data1);

    int success = PHYSFS_close(file);

    if (success)
    {
      free(ops->hidden.unknown.data2);
      delete ops;
      return 0;
    }
    else
    {
      std::cerr << "Problem when writing to file: " << get_physfs_err()
                << std::endl;
      return -1;
    }
  };

  ops->type = SDL_RWOPS_UNKNOWN;

  switch(operation)
  {
    case OP::APPEND:
      ops->hidden.unknown.data1 = PHYSFS_openAppend(file.c_str());
      break;

    case OP::READ:
      ops->hidden.unknown.data1 = PHYSFS_openRead(file.c_str());
      break;

    case OP::WRITE:
      if (!PHYSFS_mkdir(file.substr(0, file.find_last_of('/')).c_str()))
      {
        throw std::runtime_error("Could not mkdir for '" + file + "': "
                            + get_physfs_err());
      }

      ops->hidden.unknown.data1 = PHYSFS_openWrite(file.c_str());
      break;
  }

  if (!ops->hidden.unknown.data1)
  {
    throw std::runtime_error("Could not open file '" + file + "': "
                            + get_physfs_err());
  }

  // Need to do it the C way, can't just grab file.c_str directly because the
  // C++ string will probably be destroyed before the file is closed.
  // Keep at the very end to avoid having early returns cause a memory leak.
  ops->hidden.unknown.data2 = strdup(file.c_str());

  return ops.release();
}

std::string
FS::get_physfs_err()
{
#if PHYSFS_VER_MAJOR > 2 || PHYSFS_VER_MAJOR == 2 && PHYSFS_VER_MINOR >= 1
  return PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
#else
  return PHYSFS_getLastError();
#endif
}
