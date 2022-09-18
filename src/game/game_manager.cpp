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

#ifdef EMSCRIPTEN
#include "emscripten.h"
#endif
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

#define STARTING_SCENE LevelEditor

#ifdef EMSCRIPTEN
GameManager* GameManager::s_game_manager = nullptr;

void
GameManager::emscripten_loop()
{
  s_game_manager->single_loop();
}
#endif

GameManager::GameManager() :
  m_scene_manager(this),
  m_return_code(1),
  m_arg_data_folder(""),
  m_window(),
  m_context(),
  m_last_time()
{
}

int
GameManager::run(int argc, char** argv)
{
  if (!parse_cli_args(argc, argv))
    return m_return_code;

  if (!init(argv[0]))
    return m_return_code;

  m_return_code = 0;

  while (!main_loop())
  {
    if (!recover())
    {
      m_return_code = 1;
      break;
    }
  }

  if (!deinit() && m_return_code == 0)
    m_return_code = 1;

  return m_return_code;
}

bool
GameManager::parse_cli_args(int argc, char** argv)
{
  for (int i = 1; i < argc; i++)
  {
    std::string arg(argv[i]);

    if (arg == "-d" || arg == "--data")
    {
      if (++i >= argc)
      {
        std::cerr << "Missing path after '--data'" << std::endl;
        m_return_code = 1;
        return false;
      }

      m_arg_data_folder = argv[i];
    }
    else if (arg == "-h" || arg == "--help")
    {
      std::cout << "Usage: stmeltdown [OPTIONS...]\n\n"
                << "Options:\n"
                << "  -d, --data [PATH]   Change the data folder\n"
                << "  -h, --help          Show this help text and exit\n"
                << "  -t, --test          Run the test suite\n"
                << "  -v, --version       Show version info and exit\n"
                << std::flush;
      m_return_code = 0;
      return false;
    }
    else if (arg == "-t" || arg == "--test")
    {
      m_return_code = run_tests(argc, argv);
      return false;
    }
    else if (arg == "-v" || arg == "--version")
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

  if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2") == SDL_FALSE)
  {
    std::cerr << "Couldn't set SDL graphic quality: " << SDL_GetError()
              << std::endl;
  }

  if (m_arg_data_folder.empty())
  {
    // None of the PhysFS functions below may error and/or return null
    m_arg_data_folder = std::string(PHYSFS_getBaseDir()) + ".."
                      + PHYSFS_getDirSeparator() + "data";
  }

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

  if (!PHYSFS_mount(base_dir, NULL, true))
  {
    std::cerr << "Couldn't mount base directory: " << FS::get_physfs_err()
              << "\nThe game will most likely crash if this isn't expected - "
              << "continuing anyways." << std::endl;
  }

  if (!user_dir)
  {
    std::cerr << "Cannot get user data directory (Continuing read-only): "
                << FS::get_physfs_err() << std::endl;
  }
  else if (!PHYSFS_mount(user_dir, NULL, true))
  {
    std::cerr << "Couldn't mount user directory '" << user_dir
              << "' (continuing in read-only mode): " << FS::get_physfs_err()
              << std::endl;
  }
  else if (!PHYSFS_setWriteDir(user_dir))
  {
    std::cerr << "Couldn't set write directory '" << user_dir
              << "' (continuing in read-only mode): " << FS::get_physfs_err()
              << FS::get_physfs_err() << std::endl;
  }

  bool inited = generic_try([this] {
    this->m_window = std::make_unique<Window>();
    this->m_last_time = std::chrono::steady_clock::now();

    this->m_window->set_title("SuperTux Meltdown " STM_VERSION);

    auto& controller = this->m_scene_manager.get_controller();
    this->m_scene_manager.push_scene(std::make_unique<STARTING_SCENE>(controller));
  });

  if (!inited)
  {
    deinit();
    return false;
  }

  return true;
}

bool
GameManager::main_loop()
{
#ifdef EMSCRIPTEN
  s_game_manager = this;
  emscripten_set_main_loop([] { GameManager::emscripten_loop(); }, 0, true);
  return true;
#else
  return generic_try([this] {
    while(!this->m_scene_manager.empty())
      this->single_loop();
  });
#endif
}

void
GameManager::single_loop()
{
  SDL_Event e;
  while (SDL_PollEvent(&e) && !m_scene_manager.empty())
  {
    m_scene_manager.event(e);

    if (e.type == SDL_QUIT)
    {
      m_scene_manager.quit();
      return;
    }
  }

  if (m_scene_manager.empty())
    return;

  auto time = std::chrono::steady_clock::now();
  float diff = static_cast<float>((time - m_last_time).count())
                * static_cast<float>(decltype(time)::period::num)
                / static_cast<float>(decltype(time)::period::den);
  m_last_time = time;

  m_scene_manager.update(0.01f);

  if (m_scene_manager.empty())
    return;

  m_context.target_size = m_window->get_size();

  m_scene_manager.draw(m_context);


  m_context.render(m_window->get_renderer());
  m_context.clear();

  SDL_Delay(10);
}

bool
GameManager::deinit()
{
  bool success = true;

  m_window = nullptr;
  m_context.reset();

  if (!PHYSFS_deinit())
  {
    std::cerr << "Problem when closing PhysFS: " << FS::get_physfs_err()
              << std::endl;
    m_return_code = 1;

    // No early return here, must deinit the other libraries no matter what.
    success = false;
  }

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return success;
}

bool
GameManager::recover()
{
  /** @todo Tell the user a crash just happened and that the game may be
      unstable; advise closing and re-opening the game ASAP */

  std::cerr << "Recovering..." << std::endl;

  std::cerr << "Attempting to run another loop for unique bugs" << std::endl;

  if (generic_try([this] { this->single_loop(); }))
  {
    std::cerr << "Recover seems successful." << std::endl;
    return true;
  }

  std::cerr << "Attempting to reset data..." << std::endl;

  m_window = std::make_unique<Window>();
  m_window->set_title("SuperTux Meltdown " STM_VERSION);
  m_context.reset();

  if (generic_try([this] { this->single_loop(); }))
  {
    std::cerr << "Recover seems successful." << std::endl;
    return true;
  }

  /** @todo Try to recover by asking all scenes to backup their data and create
      new scenes from that backup data. */

  // Keep all non-destructive methods first, then try to recover by destructive
  // means from the least destructive to the most.

  if (m_scene_manager.get_scene_stack().size() > 1)
  {
    std::cerr << "Attempting to pop topmost scene..." << std::endl;

    m_scene_manager.pop_scene();

    if (generic_try([this] { this->single_loop(); }))
    {
      std::cerr << "Recover seems successful." << std::endl;
      return true;
    }
  }
  else
  {
    std::cerr << "Not trying to pop topmost scene, there is only 1 scene"
              << std::endl;
  }

  std::cerr << "Attempting to remove all scenes..." << std::endl;

  m_scene_manager.quit();
  auto& ctrl = m_scene_manager.get_controller();
  m_scene_manager.push_scene(std::make_unique<STARTING_SCENE>(ctrl));

  if (generic_try([this] { this->single_loop(); }))
  {
    std::cerr << "Recover seems successful." << std::endl;
    return true;
  }

  std::cerr << "Failed to recover." << std::endl;
  return false;
}

// Since all calls to this function are within this compilation unit, there is
// no need to expose the body to the header file.
template<typename F>
bool
GameManager::generic_try(F func)
{
  try
  {
    func();
  }
  catch(const std::exception& e)
  {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return false;
  }
  catch(const std::string& s)
  {
    std::cerr << "Fatal error: " << s << std::endl;
    return false;
  }
  catch(const char* s)
  {
    std::cerr << "Fatal error: " << s << std::endl;
    return false;
  }
  catch(...)
  {
    std::cerr << "Fatal error (unknown type)" << std::endl;
    return false;
  }

  return true;
}