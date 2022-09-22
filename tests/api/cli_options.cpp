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

#include "tests.hpp"

#include <stdexcept>

#include "SDL.h"

#include "game/game_manager.hpp"

#ifndef STM_VERSION
#define STM_VERSION "unknown"
#endif

TEST(API__cli_options__none)
{
  LogScanner log;

  SDL_Event e;
  e.type = SDL_QUIT;
  int push = SDL_PushEvent(&e);

  switch (push)
  {
    case -1:
      throw std::runtime_error("Couldn't push SDL_QUIT: "
                               + std::string(SDL_GetError()));
      break;

    case 0:
      throw std::runtime_error("SDL_QUIT was filtered");
      break;

    case 1:
      break;

    default:
      throw std::runtime_error("Invalid return code from SDL_PushEvent:"
                               + std::to_string(push));
      break;
  }

  const char* const args[] = {
    arg0,
    nullptr
  };

  int code = GameManager().run(sizeof(args) / sizeof(const char*) - 1, args);

  EXPECT_EQ(code, 0);
  EXPECT(log.get_out().str().empty());
  EXPECT(log.get_err().str().empty());
}

TEST(API__cli_options__help)
{
  LogScanner log;

  // GNU: "The standard --help option should output brief documentation for how
  // to invoke the program, on standard output, then exit successfully"
  // https://www.gnu.org/prep/standards/standards.html#g_t_002d_002dhelp
  const char* const args[] = {
    arg0,
    "--help",
    nullptr
  };

  int code = GameManager().run(sizeof(args) / sizeof(const char*) - 1, args);

  EXPECT_EQ(code, 0);
  EXPECT(!log.get_out().str().empty());
  EXPECT(log.get_err().str().empty());
  EXPECT(log.get_out().str().find("--test") != std::string::npos);
  // Other options are already checked by mk/tools/code_quality/check_args.sh
  // This merely tests whether the output of `--help` ended up in stdout.

  /** @todo Deprecation tests to automatically detect when files referenced like
            above are moved, renamed, or removed, so the comment can be updated
            accordingly. */
}

TEST(API__cli_options__version)
{
  LogScanner log;

  // GNU: "The standard --version option should direct the program to print
  // information about its name, version, origin and legal status, all on
  // standard output, and then exit successfully."
  // https://www.gnu.org/prep/standards/standards.html#g_t_002d_002dversion
  const char* const args[] = {
    arg0,
    "--version",
    nullptr
  };

  int code = GameManager().run(sizeof(args) / sizeof(const char*) - 1, args);

  EXPECT_EQ(code, 0);
  EXPECT(!log.get_out().str().empty());
  EXPECT(log.get_err().str().empty());
  EXPECT(log.get_out().str().find(STM_VERSION) != std::string::npos);
}

TEST(API__cli_options__incorrect_arg)
{
  LogScanner log;

  const char* const args[] = {
    arg0,
    "--this-is-not-an-arg",
    nullptr
  };

  int code = GameManager().run(sizeof(args) / sizeof(const char*) - 1, args);

  // No particular code is guaranteed in case of incorrect argument, only
  // nonzero; test only to check it isn't 0.
  EXPECT_NEQ(code, 0);
  EXPECT(log.get_out().str().empty());
  EXPECT(!log.get_err().str().empty());
  EXPECT(log.get_err().str().find("--this-is-not-an-arg") != std::string::npos);
}
