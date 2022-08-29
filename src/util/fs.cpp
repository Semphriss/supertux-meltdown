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

SDL_RWops*
FS::get_rwops(const std::string& file, OP operation)
{
  SDL_RWops* ops = new SDL_RWops;

  ops->size = [] (SDL_RWops* ops)
  {
    auto* file = static_cast<PHYSFS_file*>(ops->hidden.unknown.data1);
    return static_cast<Sint64>(PHYSFS_fileLength(file));
  };

  ops->seek = [] (SDL_RWops* ops, Sint64 offset, int whence)
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
      // TODO: Get the PHYSFS error and log it somewhere
      return static_cast<Sint64>(-1);

    return static_cast<Sint64>(PHYSFS_tell(file));
  };

  ops->read = [] (SDL_RWops *ops, void *data, size_t size, size_t num)
  {
    auto* file = static_cast<PHYSFS_file*>(ops->hidden.unknown.data1);

    auto read = PHYSFS_readBytes(file, data, size * num);

    if (read < 0)
      // TODO: Get the PHYSFS error and log it somewhere
      return static_cast<size_t>(0);

    return static_cast<size_t>(read / size);
  };

  ops->write = [] (SDL_RWops *ops, const void *data, size_t size, size_t num)
  {
    auto* file = static_cast<PHYSFS_file*>(ops->hidden.unknown.data1);

    auto written = PHYSFS_writeBytes(file, data, size * num);

    return static_cast<size_t>(written / size);
  };

  ops->close = [] (SDL_RWops *ops)
  {
    auto* file = static_cast<PHYSFS_file*>(ops->hidden.unknown.data1);

    return PHYSFS_close(file);
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
                            + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
      }

      ops->hidden.unknown.data1 = PHYSFS_openWrite(file.c_str());
      break;
  }

  if (!ops->hidden.unknown.data1)
  {
    throw std::runtime_error("Could not open file '" + file + "': "
                            + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
  }

  return ops;
}
