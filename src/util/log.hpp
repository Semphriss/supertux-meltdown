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

#ifndef HEADER_STM_UTIL_LOG_HPP
#define HEADER_STM_UTIL_LOG_HPP

#include <ostream>

#define console     Log::print()
#define log_(level) Log::log(Log::Level::level, #level, __FILE__, __LINE__)
#define log_fatal   log_(FATAL)
#define log_error   log_(ERROR)
#define log_warning log_(WARNING)
#define log_info    log_(INFO)
#define log_debug   log_(DEBUG)

class Log final
{
private:
  class NullStream :
    public std::ostream
  {
  public:
    NullStream();
  };

public:
  enum class Level
  {
    NONE,

    /** FATAL messages indicate total failure. Those should be kept at the very
        last moment possible. (e. g. they shouldn't be used except by the
        recovery mechanism, if it fails to recover the game). */
    FATAL,

    // FATAL vs ERROR:
    // Is the application guaranteed to crash after the message gets printed?
    //
    // - Yes
    // FATAL
    //
    // - No
    // - Not sure
    // - No, but something else may/will throw an error later because of this
    // - Yes, because something else will throw an error later because of this
    // ERROR

    /** ERROR indicate operation failure. The requested operation cannot be
        executed, but the program can continue running anyways (unless the type
        of operation is the kind that would naturally close the game after its
        completion, successful or not, such as invoking --version). */
    ERROR,

    // ERROR vs WARNING:
    // Can the requested operaion complete anyways?
    //
    // - No, not at all
    // ERROR
    //
    // - Yes
    // - Probably
    // - Possibly
    // - Yes, but in a poor way
    // - Only partially
    // WARNING

    /** WARNING indicate a problem which does not prevent the execution of an
        operation. That is, something went wrong, and some stuff might look odd,
        but the task will be done anyways. */
    WARNING,

    // WARNING vs INFO:
    // Is this message expected to be printed as part of a normal operation?
    //
    // - No
    // WARNING
    //
    // - Yes
    // INFO

    /** INFO is for general information messages about what happened. It is
        useful to inform the user of certain things they should know (e. g.,
        where a certain file was saved). */
    INFO,

    // WARNING vs INFO:
    // Is the information more relevant to end users (players) or to other devs?
    //
    // - End users
    // - Maybe some end users fiddling with the game in basic ways (e. g.
    //   running with a different hidden settings)
    // INFO
    //
    // - Maintainers, modders, porters, packagers, sysadmins, etc.
    // DEBUG

    /** DEBUG is for developer information, such as the version of the libraries
        loaded at runtime */
    DEBUG,

    ALL = DEBUG
  };

private:
  static NullStream s_null_stream;

public:
  static std::ostream* s_out;
  static std::ostream* s_err;
  static Level s_level;

public:
  static std::ostream& log(Level lv, const char* l, const char* file, int line);
  static std::ostream& print();
};

#endif
