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

#include "scenes/level_editor.hpp"
#include "tests/tests.hpp"
#include "util/fs.hpp"
#include "util/log.hpp"
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
  m_last_time(),
  m_delay(0.01f)
{
}

int
GameManager::run(int argc, const char* const* argv)
{
  if (!parse_cli_args(argc, argv))
    return m_return_code;

  if (!init(argv[0]))
    return m_return_code;

  while (!main_loop())
    if (!recover())
      break;

  deinit();

  return m_return_code;
}

void
GameManager::set_delay(float delay)
{
  m_delay = delay;
}

/**
 * Parses a null-terminated list of null-terminated strings, and sets the
 * appropriate values in the GameManager.
 *
 * This function may set `m_return_code` to any value. If false is returned,
 * this is the error code that should be returned.
 *
 * @param argc The number of non-null args in argv. This must be included and
 *             valid even though @p argv is null-terminated.
 * @param argv A null-terminated list of null-terminated strings, representing
 *             the arguments. No arguments may be null except the last one,
 *             which must be null. Arguments may be empty, but empty arguments
 *             may caue an error.
 *
 * @returns true if the program may continue, false if it should quit. Since
 *          this function does not require `init()`, there is no need to call
 *          `deinit()` if this function returns false.
 */
bool
GameManager::parse_cli_args(int argc, const char* const* argv)
{
  for (int i = 1; i < argc; i++)
  {
    std::string arg(argv[i]);

    if (arg == "-d" || arg == "--data")
    {
      if (++i >= argc)
      {
        log_fatal << "Missing path after '--data'" << std::endl;
        m_return_code = 1;
        return false;
      }

      m_arg_data_folder = argv[i];
    }
    else if (arg == "-h" || arg == "--help")
    {
      console << "Usage: stmeltdown [OPTIONS...]\n\n"
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
      console << "stmeltdown " STM_VERSION << std::endl;
      m_return_code = 0;
      return false;
    }
    else
    {
      log_fatal << "Unknown option '" << arg << "'" << std::endl;
      m_return_code = 1;
      return false;
    }
  }

  return true;
}

/**
 * Loads the libraries, setup sane configuration and prepare resources. CLI
 * arguments should be parsed before calling this function. It may be unsafe to
 * use any part other class before this function is called or until `deinit()`
 * is called. It is unsafe to call this function again without first calling
 * `deinit()`.
 *
 * This function may set `m_return_code` to any value. If false is returned,
 * this is the error code that should be returned.
 *
 * @param arg0 The value of argv[0]. Necessary to load PhysFS.
 *
 * @returns true if initialization is successful, false if a problem happened.
 *          Since a problem may happen at any stage of initialization, this
 *          function takes care of deinitializing the libraries if any problem
 *          happens. If true is returned, deinit() should be called to close
 *          the libraries. If false is returned, the libraries that were open
 *          will already have been re-closed, if applicable.
 */
bool
GameManager::init(const char* arg0)
{
  if (SDL_Init(SDL_INIT_VIDEO))
  {
    log_fatal << "Could not init SDL: " << SDL_GetError() << std::endl;
    return false;
  }

  if (!IMG_Init(IMG_INIT_PNG))
  {
    log_fatal << "Could not init SDL_image: " << IMG_GetError() << std::endl;
    SDL_Quit();
    return false;
  }

  if (TTF_Init())
  {
    log_fatal << "Could not init SDL_ttf: " << TTF_GetError() << std::endl;
    IMG_Quit();
    SDL_Quit();
    return false;
  }

  if (!PHYSFS_init(arg0))
  {
    log_fatal << "Could not init PhysFS: " << FS::get_physfs_err() << std::endl;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return false;
  }

  if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2") == SDL_FALSE)
  {
    log_fatal << "Couldn't set SDL graphic quality: " << SDL_GetError()
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
  log_warning << "SuperTux Meltdown was compiled against an old version of "
              << "PhysFS and will use `~/.stmeltdown` as default userdir."
              << std::endl;

  if (!PHYSFS_setWriteDir(PHYSFS_getUserDir()) || !PHYSFS_mkdir(".stmeltdown"))
  {
    log_error << "Couldn't create user directory: " << FS::get_physfs_err()
              << " (continuing anyways)" << std::endl;
  }

  std::string udir = (std::string(PHYSFS_getUserDir()) + ".stmeltdown");
  const char* user_dir = udir.c_str();
#endif

  if (!PHYSFS_mount(base_dir, NULL, true))
  {
    log_error << "Couldn't mount base directory: " << FS::get_physfs_err()
              << "\nThe game will most likely crash if this isn't expected - "
              << "continuing anyways." << std::endl;
  }

  if (!user_dir)
  {
    log_error << "Cannot get user data directory (Continuing read-only): "
                << FS::get_physfs_err() << std::endl;
  }
  else if (!PHYSFS_mount(user_dir, NULL, true))
  {
    log_error << "Couldn't mount user directory '" << user_dir
              << "' (continuing in read-only mode): " << FS::get_physfs_err()
              << std::endl;
  }
  else if (!PHYSFS_setWriteDir(user_dir))
  {
    log_error << "Couldn't set write directory '" << user_dir
              << "' (continuing in read-only mode): " << FS::get_physfs_err()
              << FS::get_physfs_err() << std::endl;
  }

  bool inited = generic_try([this] {
    this->m_window = std::make_unique<Window>();
    this->m_last_time = std::chrono::steady_clock::now();

    this->m_window->set_title("SuperTux Meltdown " STM_VERSION);

    auto& ctrl = this->m_scene_manager.get_controller();
    this->m_scene_manager.push_scene(std::make_unique<STARTING_SCENE>(ctrl));
  });

  if (!inited)
  {
    log_fatal << "Couldn't load resources" << std::endl;
    deinit();
    return false;
  }

  return true;
}

/**
 * Runs the main loop. Expects the resources to be available and the game
 * configuration to be in place.
 *
 * @note    This function may return early, or perform a dummy "throw". This is
 *          necessary with Emscripten. Currently, it uses only the latter, but
 *          a future update may use the former, or neither.
 * @returns true if the game finished running normally, false if a problem
 *          happened.
 */
bool
GameManager::main_loop()
{
  m_return_code = 0;

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

/**
 * Performs a single game loop.
 *
 * This takes care of handling the events, calculating the delta time, and
 * rendering on the window, by using the appropriate functions on the
 * `m_scene_manager`.
 *
 * This may be called independently from `main_loop()`.
 */
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

  SDL_Delay(static_cast<int>(m_delay * 1000.0f));
}

/**
 * Closes the libraries.
 *
 * This must be called if and only if `init()` has been called, when resources
 * have been deinitialized and the dependencies are no longer needed.
 *
 * This must be called before calling `init()` a second time.
 *
 * This function may set `m_return_code` to any value. If false is returned,
 * this is the error code that should be returned.
 *
 * @returns true if deinitialization was successful, false otherwise. If
 *          false, the program is probably in a bad state and should close as
 *          soon as possible.
 */
bool
GameManager::deinit()
{
  bool success = true;

  m_window = nullptr;
  m_context.reset();

  if (!PHYSFS_deinit())
  {
    log_error << "Problem when closing PhysFS: " << FS::get_physfs_err()
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

/**
 * Attempts to recover the game after a problem happened.
 *
 * This function is relevant only if `main_loop()` returned false.
 *
 * This function may set `m_return_code` to any value. If false is returned,
 * this is the error code that should be returned.
 *
 * @note This function is allowed to perform any operation on any part of the
 *       game. Although destructive means are kept as last resort, anything may
 *       change when this function is called, including changes to the scene
 *       stack.
 *
 * @returns true if recovery was successful and the game may be resumed.
 */
bool
GameManager::recover()
{
  /** @todo Tell the user a crash just happened and that the game may be
      unstable; advise closing and re-opening the game ASAP */

  log_info << "Recovering..." << std::endl;

  log_debug << "Attempting to run another loop for unique bugs" << std::endl;

  if (generic_try([this] { this->single_loop(); }))
  {
    log_info << "Recover seems successful." << std::endl;
    return true;
  }

  log_debug << "Attempting to reset data..." << std::endl;

  m_window = std::make_unique<Window>();
  m_window->set_title("SuperTux Meltdown " STM_VERSION);
  m_context.reset();

  if (generic_try([this] { this->single_loop(); }))
  {
    log_info << "Recover seems successful." << std::endl;
    return true;
  }

  /** @todo Try to recover by asking all scenes to backup their data and create
      new scenes from that backup data. */

  // Keep all non-destructive methods first, then try to recover by destructive
  // means from the least destructive to the most.

  if (m_scene_manager.get_scene_stack().size() > 1)
  {
    log_debug << "Attempting to pop topmost scene..." << std::endl;

    m_scene_manager.pop_scene();

    if (generic_try([this] { this->single_loop(); }))
    {
      log_info << "Recover seems successful." << std::endl;
      return true;
    }
  }
  else
  {
    log_debug << "Not trying to pop topmost scene, there is only 1 scene"
              << std::endl;
  }

  log_debug << "Attempting to remove all scenes..." << std::endl;

  m_scene_manager.quit();
  auto& ctrl = m_scene_manager.get_controller();
  m_scene_manager.push_scene(std::make_unique<STARTING_SCENE>(ctrl));

  if (generic_try([this] { this->single_loop(); }))
  {
    log_info << "Recover seems successful." << std::endl;
    return true;
  }

  log_fatal << "Failed to recover." << std::endl;
  m_return_code = 1;
  return false;
}

/**
 * Attempts to recover the game after a problem happened.
 *
 * This function is relevant only if `main_loop()` returned false.
 *
 * This function may set `m_return_code` to any value. If false is returned,
 * this is the error code that should be returned.
 *
 * @param func The function
 *
 * @returns true if @p func ran successfully; false if an exception was thrown.
 */
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
    log_error << "Uncaught error: " << e.what() << std::endl;
    return false;
  }
  catch(const std::string& s)
  {
    log_error << "Uncaught error: " << s << std::endl;
    return false;
  }
  catch(const char* s)
  {
    log_error << "Uncaught error: " << s << std::endl;
    return false;
  }
  catch(...)
  {
    log_error << "Uncaught error (unknown type)" << std::endl;
    return false;
  }

  return true;
}
