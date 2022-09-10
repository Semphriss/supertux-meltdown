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

#include "game/game_manager.hpp"

#include <chrono>
#include <iostream>
#include <memory>

#include "physfs.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "tests.hpp"
#include "scenes/level_editor.hpp"
#include "util/fs.hpp"
#include "video/drawing_context.hpp"
#include "video/window.hpp"

#ifndef STM_VERSION
#define STM_VERSION "unknown"
#endif

GameManager::GameManager() :
  m_scene_manager(this),
  m_return_code(1),
  m_arg_data_folder()
{
}

int
GameManager::run(int argc, char** argv)
{
  if (!init(argv[0]))
    return m_return_code;

  if (!parse_cli_args(argc, argv))
  {
    deinit();
    return m_return_code;
  }

  if (!finish_setup())
  {
    deinit();
    return m_return_code;
  }

  m_return_code = launch_game();

  if (!deinit() && m_return_code == 0)
    m_return_code = 1;

  return m_return_code;
}

bool
GameManager::init(const char* arg0)
{
  if (SDL_Init(SDL_INIT_VIDEO))
  {
    std::cerr << "Could not init SDL: " << SDL_GetError() << std::endl;
    return false;
  }

  if (!IMG_Init(IMG_INIT_PNG))
  {
    std::cerr << "Could not init SDL_image: " << IMG_GetError() << std::endl;
    SDL_Quit();
    return false;
  }

  if (TTF_Init())
  {
    std::cerr << "Could not init SDL_ttf: " << TTF_GetError() << std::endl;
    IMG_Quit();
    SDL_Quit();
    return false;
  }

  if (!PHYSFS_init(arg0))
  {
    std::cerr << "Could not init PhysFS: " << FS::get_physfs_err() << std::endl;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return false;
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

  return true;
}

bool
GameManager::parse_cli_args(int argc, char** argv)
{
  m_arg_data_folder = std::string(PHYSFS_getBaseDir()) + ".."
                    + PHYSFS_getDirSeparator() + "data";

  for (int i = 1; i < argc; i++)
  {
    std::string arg(argv[i]);

    if (arg == "--data")
    {
      if (++i >= argc)
      {
        std::cerr << "Missing path after '--data'" << std::endl;
        m_return_code = 1;
        return false;
      }

      m_arg_data_folder = argv[i];
    }
    else if (arg == "--help")
    {
      std::cout << "Usage: stmeltdown [OPTIONS...]\n\n"
                << "Options:\n"
                << "  --data [PATH]   Change the data folder\n"
                << "  --help          Show this help text and exit\n"
                << "  --test          Run the test suite\n"
                << "  --version       Show version info and exit\n"
                << std::flush;
      m_return_code = 0;
      return false;
    }
    else if (arg == "--test")
    {
      m_return_code = run_tests(argc, argv);
      return false;
    }
    else if (arg == "--version")
    {
      std::cout << "stmeltdown " STM_VERSION << std::endl;
      m_return_code = 0;
      return false;
    }
    else
    {
      std::cerr << "Unknown option '" << arg << "'" << std::endl;
      m_return_code = 1;
      return false;
    }
  }

  return true;
}

bool
GameManager::finish_setup()
{
  auto base_dir = m_arg_data_folder.c_str();

#if PHYSFS_VER_MAJOR > 2 || PHYSFS_VER_MAJOR == 2 && PHYSFS_VER_MINOR >= 1
  const char* user_dir = PHYSFS_getPrefDir("SuperTux", "stmeltdown");
#else
  std::cerr << "SuperTux Meltdown was compiled against an old version of PhysFS"
               " and will use `~/.stmeltdown` as default userdir." << std::endl;

  if (!PHYSFS_setWriteDir(PHYSFS_getUserDir()) || !PHYSFS_mkdir(".stmeltdown"))
  {
    std::cerr << "Couldn't create user directory: " << FS::get_physfs_err()
              << " (continuing anyways)" << std::endl;
  }

  std::string udir = (std::string(PHYSFS_getUserDir()) + ".stmeltdown");
  const char* user_dir = udir.c_str();
#endif

  if (!user_dir)
  {
    std::cerr << "Cannot get user dir, PhysFS error: " << FS::get_physfs_err()
              << "\nThe game will most likely crash if this isn't expected - "
              << "continuing anyways." << std::endl;
  }

  if (!PHYSFS_mount(user_dir, NULL, true))
  {
    std::cerr << "Couldn't mount user directory: " << FS::get_physfs_err()
              << "\nThe game will most likely crash if this isn't expected - "
              << "continuing anyways." << std::endl;
  }

  if (!PHYSFS_mount(base_dir, NULL, true))
  {
    std::cerr << "Couldn't mount base directory: " << FS::get_physfs_err()
              << "\nThe game will most likely crash if this isn't expected - "
              << "continuing anyways." << std::endl;
  }

  if (!PHYSFS_setWriteDir(user_dir))
  {
    std::cerr << "Couldn't set write directory: " << FS::get_physfs_err()
              << "\nThe game will continue in read-only mode." << std::endl;
  }

  return true;
}

int
GameManager::launch_game()
{
  try
  {
    main_loop();
  }
  catch(const std::exception& e)
  {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return 1;
  }
  catch(const std::string& s)
  {
    std::cerr << "Fatal error: " << s << std::endl;
    return 1;
  }
  catch(const char* s)
  {
    std::cerr << "Fatal error: " << s << std::endl;
    return 1;
  }
  catch(...)
  {
    std::cerr << "Fatal error (unknown type)" << std::endl;
    return 1;
  }

  return 0;
}

void
GameManager::main_loop()
{
  Window w;
  Renderer& r = w.get_renderer();
  DrawingContext dc;
  auto last_time = std::chrono::steady_clock::now();

  w.set_title("SuperTux Meltdown " STM_VERSION);

  auto& controller = m_scene_manager.get_controller();
  m_scene_manager.push_scene(std::make_unique<LevelEditor>(controller));

  while(!m_scene_manager.empty())
  {
    SDL_Event e;
    while (SDL_PollEvent(&e) && !m_scene_manager.empty())
    {
      m_scene_manager.event(e);

      if (e.type == SDL_QUIT)
      {
        m_scene_manager.quit();
        break;
      }
    }

    if (m_scene_manager.empty())
      break;

    // https://en.cppreference.com/w/cpp/chrono/steady_clock says:
    //   "This clock [...] is most suitable for measuring intervals."
    // std::chrono::high_resolution_clock is merely an alias to another clock,
    // which may or may not be steady_clock, and which can have huge jumps if
    // the user changes their computer time, including back in time. See the
    // notes: https://en.cppreference.com/w/cpp/chrono/high_resolution_clock
    auto time = std::chrono::steady_clock::now();
    float diff = static_cast<float>((time - last_time).count())
                  * static_cast<float>(decltype(time)::period::num)
                  / static_cast<float>(decltype(time)::period::den);
    last_time = time;

    m_scene_manager.update(0.01f);

    if (m_scene_manager.empty())
      break;

    dc.target_size = w.get_size();

    m_scene_manager.draw(dc);

    dc.render(r);
    dc.clear();

    SDL_Delay(10);
  }
}

bool
GameManager::deinit()
{
  bool success = true;

  if (!PHYSFS_deinit())
  {
    std::cerr << "Problem when closing PhysFS: " << FS::get_physfs_err()
              << std::endl;
    m_return_code = 1;

    success = false;
    // No early return here, must deinit the other libraries no matter what.
  }

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return success;
}
