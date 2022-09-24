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

#include "util/log.hpp"

TEST(UNIT__Log__console)
{
  LogScanner log;

  console << "Hello, world!" << std::endl;

  EXPECT_EQ(log.get_out().str(), "Hello, world!\n");
  EXPECT(log.get_err().str().empty());
}

TEST(UNIT__Log__log_fatal)
{
  LogScanner log;

  Log::s_level = Log::Level::ALL;
  log_fatal << "Hello, world!" << std::endl;

  EXPECT(log.get_out().str().empty());
  EXPECT(log.get_err().str().find("Hello, world!\n") != std::string::npos);
}

TEST(UNIT__Log__log_error)
{
  LogScanner log;

  Log::s_level = Log::Level::ALL;
  log_error << "Hello, world!" << std::endl;

  EXPECT(log.get_out().str().empty());
  EXPECT(log.get_err().str().find("Hello, world!\n") != std::string::npos);
}

TEST(UNIT__Log__log_warning)
{
  LogScanner log;

  Log::s_level = Log::Level::ALL;
  log_warning << "Hello, world!" << std::endl;

  EXPECT(log.get_out().str().empty());
  EXPECT(log.get_err().str().find("Hello, world!\n") != std::string::npos);
}

TEST(UNIT__Log__log_info)
{
  LogScanner log;

  Log::s_level = Log::Level::ALL;
  log_info << "Hello, world!" << std::endl;

  EXPECT(log.get_out().str().empty());
  EXPECT(log.get_err().str().find("Hello, world!\n") != std::string::npos);
}

TEST(UNIT__Log__log_debug)
{
  LogScanner log;

  Log::s_level = Log::Level::ALL;
  log_debug << "Hello, world!" << std::endl;

  EXPECT(log.get_out().str().empty());
  EXPECT(log.get_err().str().find("Hello, world!\n") != std::string::npos);
}

TEST(UNIT__Log__levels)
{
  LogScanner log;

  Log::s_level = Log::Level::NONE;
  log_fatal << "__a__";
  log_error << "__b__";
  log_warning << "__c__";
  log_info << "__d__";
  log_debug << "__e__";

  Log::s_level = Log::Level::FATAL;
  log_fatal << "__f__";
  log_error << "__g__";
  log_warning << "__h__";
  log_info << "__i__";
  log_debug << "__j__";

  Log::s_level = Log::Level::ERROR;
  log_fatal << "__k__";
  log_error << "__l__";
  log_warning << "__m__";
  log_info << "__n__";
  log_debug << "__o__";

  Log::s_level = Log::Level::WARNING;
  log_fatal << "__p__";
  log_error << "__q__";
  log_warning << "__r__";
  log_info << "__s__";
  log_debug << "__t__";

  Log::s_level = Log::Level::INFO;
  log_fatal << "__u__";
  log_error << "__v__";
  log_warning << "__w__";
  log_info << "__x__";
  log_debug << "__y__";

  Log::s_level = Log::Level::DEBUG;
  log_fatal << "__z__";
  log_error << "__A__";
  log_warning << "__B__";
  log_info << "__C__";
  log_debug << "__D__";

  Log::s_level = Log::Level::ALL;
  log_fatal << "__E__";
  log_error << "__F__";
  log_warning << "__G__";
  log_info << "__H__";
  log_debug << "__I__";

  EXPECT(log.get_out().str().empty());
  EXPECT(log.get_err().str().find("__a__") == std::string::npos);
  EXPECT(log.get_err().str().find("__b__") == std::string::npos);
  EXPECT(log.get_err().str().find("__c__") == std::string::npos);
  EXPECT(log.get_err().str().find("__d__") == std::string::npos);
  EXPECT(log.get_err().str().find("__e__") == std::string::npos);
  EXPECT(log.get_err().str().find("__f__") != std::string::npos);
  EXPECT(log.get_err().str().find("__g__") == std::string::npos);
  EXPECT(log.get_err().str().find("__h__") == std::string::npos);
  EXPECT(log.get_err().str().find("__i__") == std::string::npos);
  EXPECT(log.get_err().str().find("__j__") == std::string::npos);
  EXPECT(log.get_err().str().find("__k__") != std::string::npos);
  EXPECT(log.get_err().str().find("__l__") != std::string::npos);
  EXPECT(log.get_err().str().find("__m__") == std::string::npos);
  EXPECT(log.get_err().str().find("__n__") == std::string::npos);
  EXPECT(log.get_err().str().find("__o__") == std::string::npos);
  EXPECT(log.get_err().str().find("__p__") != std::string::npos);
  EXPECT(log.get_err().str().find("__q__") != std::string::npos);
  EXPECT(log.get_err().str().find("__r__") != std::string::npos);
  EXPECT(log.get_err().str().find("__s__") == std::string::npos);
  EXPECT(log.get_err().str().find("__t__") == std::string::npos);
  EXPECT(log.get_err().str().find("__u__") != std::string::npos);
  EXPECT(log.get_err().str().find("__v__") != std::string::npos);
  EXPECT(log.get_err().str().find("__w__") != std::string::npos);
  EXPECT(log.get_err().str().find("__x__") != std::string::npos);
  EXPECT(log.get_err().str().find("__y__") == std::string::npos);
  EXPECT(log.get_err().str().find("__z__") != std::string::npos);
  EXPECT(log.get_err().str().find("__A__") != std::string::npos);
  EXPECT(log.get_err().str().find("__B__") != std::string::npos);
  EXPECT(log.get_err().str().find("__C__") != std::string::npos);
  EXPECT(log.get_err().str().find("__D__") != std::string::npos);
  EXPECT(log.get_err().str().find("__E__") != std::string::npos);
  EXPECT(log.get_err().str().find("__F__") != std::string::npos);
  EXPECT(log.get_err().str().find("__G__") != std::string::npos);
  EXPECT(log.get_err().str().find("__H__") != std::string::npos);
  EXPECT(log.get_err().str().find("__I__") != std::string::npos);
}
