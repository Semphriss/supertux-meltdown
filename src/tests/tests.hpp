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

#ifndef HEADER_STM_TESTS_TESTS_HPP
#define HEADER_STM_TESTS_TESTS_HPP

#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

extern const char* arg0;

std::map<std::string, void(*)()>&
g_tests(const std::string& name = "", void (*func)() = nullptr);

int run_tests(int argc, const char* const* argv);

#define TEST(func)                                                             \
  void func();                                                                 \
  static auto& g_test_##func = g_tests(#func, func);                           \
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

/**
 * Eases the task of scanning the output by grabbing the stream.
 */
class LogScanner final
{
public:
  LogScanner();
  ~LogScanner();

  const std::stringstream& get_out() const;
  const std::stringstream& get_err() const;

private:
  std::stringstream m_stream_out;
  std::stringstream m_stream_err;
  std::ostream* m_out;
  std::ostream* m_err;
};

#define ASSERT_FAIL(msg) throw AssertFail(msg, __FILE__, __LINE__)

#define EXPECT(condition)                                                      \
  {                                                                            \
    auto stm_condition = condition;                                            \
    if (!stm_condition)                                                        \
    {                                                                          \
      ASSERT_FAIL("Expected " #condition);                                     \
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
      ASSERT_FAIL(s.str());                                                    \
    }                                                                          \
  }


#define EXPECT_NEQ(have, need)                                                 \
  {                                                                            \
    auto stm_have = have;                                                      \
    auto stm_need = need;                                                      \
    if (stm_have == stm_need)                                                  \
    {                                                                          \
      std::stringstream s;                                                     \
      s << "Expected " #have " to not be " << stm_need << ", got " << stm_have;\
      ASSERT_FAIL(s.str());                                                    \
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
      ASSERT_FAIL(s.str());                                                    \
    }                                                                          \
  }
#define EXPECT_FLT_EQ(have, need) EXPECT_FLT_EQ_PRECISION(have, need, 0.001f)

#define EXPECT_FLT_NEQ_PRECISION(have, need, prec)                             \
  {                                                                            \
    auto stm_have = have;                                                      \
    auto stm_need = need;                                                      \
    if (std::abs(stm_have - stm_need) <=                                       \
        (std::abs(stm_have) + std::abs(stm_need)) * prec)                      \
    {                                                                          \
      std::stringstream s;                                                     \
      s << "Expected " #have " to not be " << stm_need << ", got " << stm_have;\
      ASSERT_FAIL(s.str());                                                    \
    }                                                                          \
  }
#define EXPECT_FLT_NEQ(have, need) EXPECT_FLT_NEQ_PRECISION(have, need, 0.001f)

#define EXPECT_THROW(expression)                                               \
  {                                                                            \
    try                                                                        \
    {                                                                          \
      {                                                                        \
        expression;                                                            \
      }                                                                        \
      ASSERT_FAIL("Expected '" #expression "' to throw");                      \
    }                                                                          \
    catch(...)                                                                 \
    {                                                                          \
    }                                                                          \
  }

#endif
