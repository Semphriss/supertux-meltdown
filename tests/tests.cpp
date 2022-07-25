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

#include <chrono>
#include <iostream>

static std::string str_time(float t)
{
  std::stringstream oss;
  oss.precision(3);

  if (t < 1e-6f)
  {
    oss << (t * 1e9f) << " ns";
  }
  else if (t < 1e-3f)
  {
    oss << (t * 1e6f) << " us";
  }
  else if (t < 1.0f)
  {
    oss << (t * 1e3f) << " ms";
  }
  else
  {
    oss << t << " s";
  }

  return oss.str();
}

std::map<std::string, void(*)()>&
g_tests(const std::string& name, void (*func)())
{
  static std::map<std::string, void(*)()> tests;

  if (func)
    tests.emplace(std::make_pair(name, func));

  return tests;
}

int run_tests(int argc, char** argv)
{
  std::string color_pass = "\033[32m",
              color_fail = "\033[31m",
              color_warn = "\033[95m",
              color_file = "\033[36m",
              color_bold = "\033[1m",
              color_dark = "\033[2m",
              color_uline = "\033[4m",
              color_reset = "\033[0m";
  std::ostream* out = &std::cout;
  bool fail_fast = false;

  for (int i = 1; i < argc; i++)
  {
    std::string arg(argv[i]);

    if (arg == "--no-color")
    {
      color_pass = color_fail = color_warn = color_file = color_bold =
                   color_dark = color_uline = color_reset = "";
    }
    else if (arg == "--stderr")
    {
      out = &std::cerr;
    }
    else if (arg == "--fail-fast")
    {
      fail_fast = true;
    }
    else if (arg != "--test")
    {
      std::cerr << "Unrecognized option: " << arg << std::endl;
      return 2;
    }
  }

  std::string warn = color_warn + color_bold + "warning" + color_reset + ": ";

  auto& tests = g_tests();

  auto start = std::chrono::steady_clock::now();
  size_t passed = 0, failed = 0;

  for (const auto& test : tests)
  {
    *out << "[  Running  ] " << color_bold << test.first << color_reset << "\r"
         << std::flush;

    auto test_start = std::chrono::steady_clock::now();

    try
    {
      test.second();
      passed++;

      *out << color_pass << "[  SUCCESS  ] " << color_bold << test.first;
    }
    catch (const AssertFail& a)
    {
      failed++;

      *out << color_fail << "[: FAILURE :] " << color_bold << test.first << ": "
           << color_reset << color_file << a.get_path() << ": " << color_fail
           << a.what();
    }
    catch (const std::exception& e)
    {
      failed++;

      *out << color_fail << "[ ! ERROR ! ] " << color_bold << test.first
           << color_reset << color_fail << ": " << e.what();
    }
    catch (const std::string& s)
    {
      failed++;

      *out << color_fail << "[ ! ERROR ! ] " << color_bold << test.first
           << color_reset << color_fail << ": " << s;
    }
    catch (const char* c)
    {
      failed++;

      *out << color_fail << "[ ! ERROR ! ] " << color_bold << test.first
           << color_reset << color_fail << ": " << c;
    }
    catch (...)
    {
      failed++;

      *out << color_fail << "[ ! ERROR ! ] " << color_bold << test.first;
    }

    auto test_end = std::chrono::steady_clock::now();
    float diff = static_cast<float>((test_end - test_start).count())
                * static_cast<float>(decltype(test_end)::period::num)
                / static_cast<float>(decltype(test_end)::period::den);

    *out << color_reset << color_dark << " (" << str_time(diff) << ")\n"
         << color_reset;

    if (failed && fail_fast)
      break;
  }

  auto end = std::chrono::steady_clock::now();
  float diff = static_cast<float>((end - start).count())
              * static_cast<float>(decltype(end)::period::num)
              / static_cast<float>(decltype(end)::period::den);

  *out << color_reset << "\n" << color_bold << color_uline
       << (passed ? color_pass : "") << passed << color_reset  << " passed, "
       << color_bold << color_uline << (failed ? color_fail : "") << failed
       << color_reset << " failed." << color_dark << " (" << str_time(diff)
       << ")" << color_reset << std::endl;

  if (failed && fail_fast)
    *out << "A test failed and --fail-fast was passed; some tests may not have "
            "run." << std::endl;

  return failed != 0;
}
