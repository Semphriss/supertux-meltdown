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

#ifndef HEADER_STM_TESTS
#define HEADER_STM_TESTS

#include <functional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

std::vector<std::pair<std::string, void(*)()>>&
g_tests(const std::string& name = "", void (*func)() = nullptr);

#define TEST(func)                                                             \
  void func();                                                                 \
  static auto g_test_##func = g_tests(#func, func);                            \
  void func()

class AssertFail :
  public std::runtime_error
{
public:
  AssertFail(const std::string& msg, const char* file, int line) :
    std::runtime_error(msg),
    m_file(file),
    m_line(line)
  {
  }

  std::string get_path() const
  {
    return std::string(m_file) + ":" + std::to_string(m_line);
  }

private:
  const char* const m_file;
  const int m_line;
};

#define EXPECT(condition)                                                      \
  {                                                                            \
    auto stm_condition = condition;                                            \
    if (!stm_condition)                                                        \
    {                                                                          \
      throw AssertFail("Expected " #condition, __FILE__, __LINE__);            \
    }                                                                          \
  }

#define EXPECT_EQ(have, need)                                                  \
  {                                                                            \
    auto stm_have = have;                                                      \
    auto stm_need = need;                                                      \
    if (stm_have != stm_need)                                                  \
    {                                                                          \
      std::stringstream s;                                                     \
      s << "Expected " #have " to be " << stm_need << ", got " << stm_have;    \
      throw AssertFail(s.str(), __FILE__, __LINE__);                           \
    }                                                                          \
  }

#define EXPECT_FLT_EQ_PRECISION(have, need, prec)                              \
  {                                                                            \
    auto stm_have = have;                                                      \
    auto stm_need = need;                                                      \
    if (std::abs(stm_have - stm_need) >                                        \
        (std::abs(stm_have) + std::abs(stm_need)) * prec)                      \
    {                                                                          \
      std::stringstream s;                                                     \
      s << "Expected " #have " to be " << stm_need << ", got " << stm_have;    \
      throw AssertFail(s.str(), __FILE__, __LINE__);                           \
    }                                                                          \
  }
#define EXPECT_FLT_EQ(have, need) EXPECT_FLT_EQ_PRECISION(have, need, 0.001f)

#endif
