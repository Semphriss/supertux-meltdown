//  SuperTux Meltdown - Semphris' take on the popular Linux platformer
//  Copyright (C) 2021-2022 Semphris <semphris@semphris.com>
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

#include <ctime>
#include <memory>
#include <vector>

#include "make_unique.hpp"

#ifdef EMSCRIPTEN
#include "emscripten.h"
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

#include "fs/file.hpp"
#include "game/resource_manager.hpp"
#include "scenes/error_panel.hpp"
#include "scenes/level_editor.hpp"
#include "transitions/transition.hpp"
#include "util/color.hpp"
#include "util/log.hpp"
#include "video/drawing_context.hpp"
#include "video/font.hpp"

#ifndef STM_VER
#define STM_VER "(unknown version)"
#endif

static const std::vector<Window::VideoSystem> SUPPORTED_VIDEO_SYSTEMS = {
  Window::VideoSystem::SDL,
  Window::VideoSystem::GL
};

std::unique_ptr<GameManager> GameManager::s_main_game_manager = nullptr;

/**
 * Runs the game like a normal program.
 *
 * This function takes care of loading resources, parsing arguments, and running
 * the main loop. It is meant to be easily callable from a main() function with:
 *
 * ```cpp
 * int main(int argc, char** argv)
 * {
 *   return GameManager::run(argc, argv);
 * }
 * ```
 *
 * @note This function might return before the game ran! On some platforms (like
 *       WASM, but also potentially others), the main loop is handled by the
 *       environment rather than by the program itself, which requires the
 *       survival of the resources (notably, the GameManager object) after
 *       returning. Internally, the code already handles this by using a
 *       static class variable that survives the whole execution. If you need to
 *       add code after calling GameManager::run(), do not assume that the game
 *       finished running upon the function's return!
 *
 * @param argc The number of arguments passed.
 * @param argv The value of the arguments passed, as an array of null-terminated
 *             C strings. Although the standart says the array itself must also
 *             be null-terminated, this function relies only on argc and the
 *             last NULL argument can be omitted if necessary.
 * @returns 0 if the program ran successfully; 1 if a fatal error happened.
 * @author Semphris <semphris@semphris.com>, 2022
 */
int
GameManager::run(int argc, const char* const* argv)
{
  s_main_game_manager = std::make_unique<GameManager>();
  auto& gm = *s_main_game_manager;

  log_debug << "Initializing the game" << std::endl;
  ResourceManager::get_resource_manager(argv[0]);

  log_debug << "Setting up filesystem" << std::endl;
  gm.setup_filesystem();

  log_debug << "Starting video system" << std::endl;
  gm.change_video_system(Window::VideoSystem::SDL);

  log_debug << "Emplacing main menu" << std::endl;
  gm.push_scene(std::make_unique<LevelEditor>(gm), Transition::Type::DISSOLVE);

  log_debug << "Launching main loop" << std::endl;
  int return_code = gm.run_loops();

  // It is normal to see this message at the initialisation step on certain
  // platforms which handle the main loop separately, like Emscripten
  log_debug << "Main loop exited with code " << return_code << std::endl;
  return return_code;
}

/**
 * Creates a generic GameManager.
 *
 * @author Semphris <semphris@semphris.com>, 2022
 */
GameManager::GameManager() :
  m_window(nullptr),
  m_scenes(),
  m_transition(nullptr),
  m_popping_scene(nullptr),
  m_last_frame(std::chrono::steady_clock::now()),
  m_delay(0.01f)
{
}

/**
 * Pushes a new scene on the scene stack, optionally with a transition.
 *
 * If there is already a currently running transition, the call has no effect.
 *
 * Internally, all changes are effective immediately: Upon returning, if
 * there wasn't an active transition, it is guaranteed that the entering scene
 * will be at the top of the stack and, if the value of @p transition is
 * Transition::Type::NONE, no transition will be created and the scene becomes
 * the active scene, effective immediately.
 *
 * If @p transition is NOT Transition::Type::NONE but @p time is 0, a transition
 * will be created and will finish on the next update call.
 *
 * @param scene The scene to be pushed on top of the stack.
 * @param transition The scene to be pushed on top of the stack.
 * @param time The time the transition should take.
 * @author Semphris <semphris@semphris.com>, 2022
 */
void
GameManager::push_scene(std::unique_ptr<Scene> scene,
                        Transition::Type transition, float time)
{
  log_debug << "Request to push scene" << std::endl;

  if (m_transition)
  {
    log_warn << "Attempt to push scene during a transition" << std::endl;
    return;
  }

  auto* from = m_scenes.empty() ? nullptr : m_scenes.back().get();
  auto* to = scene.get();

  m_scenes.push_back(std::move(scene));
  m_transition = Transition::create_transition(from, to, time, transition);

  if (from)
    from->leave(false);

  // Skip transition if no transition
  if (!m_transition)
  {
    if (to)
      to->enter();
  }
}

/**
 * Pops the topmost scene from the scene stack, optionally with a transition.
 *
 * If there is already a currently running transition, the call has no effect.
 *
 * Internally, all changes are effective immediately: Upon returning, if
 * there wasn't an active transition, it is guaranteed that the leaving scene
 * will be out of the stack and, if the value of @p transition is
 * Transition::Type::NONE, no transition will be created and the scene is
 * destroyed.
 *
 * If @p transition is NOT Transition::Type::NONE but @p time is 0, a transition
 * will be created and will finish on the next update call.
 *
 * @param transition The scene to be pushed on top of the stack.
 * @param time The time the transition should take.
 * @author Semphris <semphris@semphris.com>, 2022
 */
void
GameManager::pop_scene(Transition::Type transition, float time)
{
  log_debug << "Request to pop scene" << std::endl;

  if (m_transition)
  {
    log_warn << "Attempt to pop scene during a transition" << std::endl;
    return;
  }

  if (m_scenes.size() < 1)
  {
    log_warn << "Cannot pop empty scene stack" << std::endl;
    return;
  }

  m_popping_scene = std::move(m_scenes.back());
  m_scenes.pop_back();

  auto* from = m_popping_scene.get();
  auto* to = m_scenes.empty() ? nullptr : m_scenes.back().get();

  m_transition = Transition::create_transition(from, to, time, transition);

  from->leave(true);

  // Skip transition if no transition
  if (!m_transition)
  {
    m_popping_scene = nullptr;

    if (to)
      to->enter();
  }
}

/**
 * Changes the window, using the specified video system.
 *
 * Calling this function invalidates all textures loaded from file or created
 * dynamically through the window; this function takes care of calling
 * Scene::reset_caches() on scenes, but if some resources are located outside
 * of scenes, they must be taken care of manually.
 *
 * @param video_system The type of video system to use when creating the window.
 * @param keep_status If true, copies the basic status of the old window to the
 *                    new window (size, position, etc). This parameter has no
 *                    effect if there is no old window (m_window is null).
 * @author Semphris <semphris@semphris.com>, 2022
 */
void
GameManager::change_video_system(Window::VideoSystem video_system,
                                 bool keep_status)
{
  log_debug << "Request to change video system to "
            << Window::get_video_system_tag(video_system) << std::endl;

  auto window = Window::create_window(video_system);
  window->set_title("SuperTux Meltdown " STM_VER);
  window->set_resizable(true);

  if (m_window && keep_status)
  {
    window->set_pos(m_window->get_pos());
    window->set_size(m_window->get_size());
    window->set_status(m_window->get_status());
  }

  m_window = std::move(window);

  // Textures are broken now that the video changed
  for (auto& scene : m_scenes)
  {
    scene->reset_caches();
  }

  // Clearing events is necessary, else events will be duplicated
  SDL_Event e;
  while (SDL_PollEvent(&e));
}

/**
 * Changes the delay between frames (inverse of the FPS).
 *
 * This function will probably receive severe improvements in the future; see
 * the todo's below.
 *
 * @todo Add a function to set time multiplier for update()
 * @todo Change the function to take an FPS value instead of a delay value?
 * @todo Add a way to sync the main loop with the refresh rate of the screen.
 *
 * @param delay The new delay between frames. This may differ from the values
 *              sent to Scene::update(), since it uses a timer to calculate the
 *              actual delays accurately.
 * @author Semphris <semphris@semphris.com>, 2022
 */
void
GameManager::set_delay(float delay)
{
  log_debug << "Request to change delay to " << delay << std::endl;

  m_delay = delay;
}

/**
 * Returns the window used by the GameManager.
 *
 * @returns The window object.
 * @author Semphris <semphris@semphris.com>, 2022
 */
Window&
GameManager::get_window()
{
  return *m_window;
}

/**
 * Returns the scene stack used by the GameManager.
 *
 * @returns The scene stack.
 * @author Semphris <semphris@semphris.com>, 2022
 */
const IGameManager::SceneStack&
GameManager::get_scene_stack() const
{
  return m_scenes;
}

/**
 * Returns the currently popping scene, if any.
 *
 * @returns The currently popping scene, or null if there isn't any.
 * @author Semphris <semphris@semphris.com>, 2022
 */
const Scene*
GameManager::get_popping_scene() const
{
  return m_popping_scene.get();
}

/**
 * Returns the current transition, if any.
 *
 * @returns The current transition, or null if there isn't any. A transition is
 *          popping if GameManager::get_popping_scene() is non-null or pushing
 *          if GameManager::get_popping_scene() is null.
 * @author Semphris <semphris@semphris.com>, 2022
 */
const Transition*
GameManager::get_current_transition() const
{
  return m_transition.get();
}

/**
 * Sets up the filesystem to be used by the game.
 *
 * @todo Move that code into some module dedicated to cross-platform support.
 *
 * @author Semphris <semphris@semphris.com>, 2022
 */
void
GameManager::setup_filesystem() const
{
  auto local_path = File::get_pref_dir("Semphris", "SuperTux Meltdown");
#if defined(UBUNTU_TOUCH)
  auto src_path = File::build_os_path({File::get_base_dir(), "data"});
#elif defined(EMSCRIPTEN)
  auto src_path = "/data";
#elif defined(_MSC_VER)
  auto src_path = File::build_os_path({File::get_base_dir(), "..\\..\\data"});
#else
  auto src_path = File::build_os_path({File::get_base_dir(), "../data"});
#endif

  File::mount(local_path);
  File::mount(src_path);

  File::set_write_dir(local_path);
}

/**
 * Changes the video system to the next one from a hardcoded list of video
 * systems.
 *
 * Internally, it uses SUPPORTED_VIDEO_SYSTEMS as a list and
 * GameManager::change_video_system() to update the video system.
 *
 * @author Semphris <semphris@semphris.com>, 2022
 */
void
GameManager::advance_video_system()
{
  log_debug << "Request to advance video system (current: "
            << Window::get_video_system_tag(m_window->get_type()) << std::endl;

  Window::VideoSystem target_video_system = SUPPORTED_VIDEO_SYSTEMS.front();

  bool is_next = false;
  for (const auto& vs : SUPPORTED_VIDEO_SYSTEMS)
  {
    if (vs == m_window->get_type())
    {
      is_next = true;
    }
    else if (is_next)
    {
      target_video_system = vs;
      break;
    }
  }

  change_video_system(target_video_system);
}

/**
 * Handle events for game-wide generic commands, such as flushing the cache or
 * changing the video system.
 *
 * This function SHOULD NOT be called externally; instead, use the wanted
 * functions directly.
 *
 * @todo Add some function for clearing caches...
 *
 * List of such events (please check the code to ensure this is up-to-date):
 * - F5: Flush all known caches
 * - F10: Change the video system to the next available one
 *   (see GameManager::advance_video_system)
 * - F11: Toggle fullscreen
 *
 * Also handles some internal controls:
 * - Quitting the game
 * - Flushing caches when windows are resized (for text rendering; see the code)
 * - Flushing caches when OS reports low memory
 *
 * @param e The event to be handled.
 * @author Semphris <semphris@semphris.com>, 2022
 */
void
GameManager::handle_internal_event(const SDL_Event& e)
{
  switch (e.type)
  {
    case SDL_QUIT:
      for (auto it = m_scenes.rbegin(); it != m_scenes.rend(); it++)
      {
        if (!(*it)->quit(true))
        {
          break;
        }
        else
        {
          m_scenes.pop_back();
        }
      }
      break;

    case SDL_WINDOWEVENT:
      switch(e.window.event)
      {
        case SDL_WINDOWEVENT_RESIZED:
          // Text rendering uses a cache that varies when the components are
          // resized; without this line, resizing the screen would grow the
          // memory by 10's or 100's of megabytes in mere seconds
          Font::flush_fonts();
          break;

        default:
          break;
      }
      break;

    case SDL_APP_LOWMEMORY:
      Font::flush_fonts();
      m_window->flush_texture_cache();
      for (auto& scene : m_scenes)
      {
        scene->reset_caches();
      }
      break;

    case SDL_KEYDOWN:
      switch(e.key.keysym.sym)
      {
        case SDLK_F1:
          /** @todo F1 should open some help menu */
          break;

        case SDLK_F5:
          Font::flush_fonts();
          m_window->flush_texture_cache();
          for (auto& scene : m_scenes)
          {
            scene->reset_caches();
          }
          break;

        case SDLK_F10:
          advance_video_system();
          break;

        case SDLK_F11:
          m_window->toggle_fullscreen();
          break;

        case SDLK_F12:
        {
          std::time_t t = std::time(0);
          std::tm* now = std::localtime(&t);
          std::string name = "screenshot_"
                            + std::to_string(now->tm_year + 1900) + "-"
                            + std::to_string(now->tm_mon + 1) + "-"
                            + std::to_string(now->tm_mday) + "_"
                            + std::to_string(now->tm_hour) + "-"
                            + std::to_string(now->tm_min) + "-"
                            + std::to_string(now->tm_sec) + ".png";

          /** @todo F12 should save a screenshot to file */
        }
          break;

        default:
          break;
      }
      break;

    default:
      break;
  }
}

/**
 * Try to recover from an uncaught exception.
 *
 * This function SHOULD NOT be called externally; it is public for testing
 * purposes only. Handle your exceptions yourself!
 *
 * This works by attempting to apply a series of common fixes and by saving as
 * much data as possible. The point is to avoid crashes or loss of data as much
 * as possible.
 *
 * Calling this function (which should always be done from GameManager!) allows
 * the program to take any action it sees fit to recover the program, including
 * destructive actions (if it can avoid a crash); do not rely on it, ever.
 *
 * @param err The event to be handled.
 * @returns true if recovery was successful and execution can resume; false is
 *          recovery was unsuccessful and program should crash.
 * @author Semphris <semphris@semphris.com>, 2022
 */
bool
GameManager::try_recover(const std::exception& err)
{
  log_debug << "Attempting to recover from error" << std::endl;

  std::string message = "";

  try
  {
    log_debug << "Attempting to make scene recover" << std::endl;

    if (!m_scenes.back()->recover_from_error(err))
      throw "Scene failed to recover from error";

    log_debug << "Scene took action, attempting to run a frame" << std::endl;

    run_single_loop();

    log_debug << "Recover appears successful, resuming normal use" << std::endl;

    if (!dynamic_cast<ErrorPanel*>(m_scenes.back().get()))
      push_scene(std::make_unique<ErrorPanel>(*this, *m_scenes.back()));

    return true;
  }
  catch(const std::exception& err)
  {
    message = err.what();
  }
  catch(const std::string& msg)
  {
    message = msg;
  }
  catch(const char* msg)
  {
    message = msg;
  }
  catch(...)
  {
    message = "(unknown error type)";
  }

  log_debug << "Recover unsuccessful: " << message << std::endl;

  try
  {
    log_debug << "Attempting to reset all components" << std::endl;

    change_video_system(m_window->get_type(), false);
    Font::flush_fonts();
    for (auto& scene : m_scenes)
    {
      scene->reset_caches();
    }

    log_debug << "All components reset, attempting to run a frame" << std::endl;

    run_single_loop();

    log_debug << "Recover appears successful, resuming normal use" << std::endl;

    if (!dynamic_cast<ErrorPanel*>(m_scenes.back().get()))
      push_scene(std::make_unique<ErrorPanel>(*this, *m_scenes.back()));

    return true;
  }
  catch(const std::exception& err)
  {
    message = err.what();
  }
  catch(const std::string& msg)
  {
    message = msg;
  }
  catch(const char* msg)
  {
    message = msg;
  }
  catch(...)
  {
    message = "(unknown error type)";
  }

  log_debug << "Recover unsuccessful: " << message << std::endl;

  /** @todo Add a function in Scenes to allow them to emergency-save their
            essential data, also try reloading a scene */

  // This action is destructive, so must come in last place.
  try
  {
    log_debug << "Attempting to kill topmost scene" << std::endl;

    m_transition = nullptr;

    if (m_popping_scene)
    {
      m_popping_scene = nullptr;
    }
    else
    {
      m_scenes.pop_back();
    }

    if (m_scenes.empty())
      throw "No scene left to test, quitting";

    log_debug << "Scene wiped, attempting to run a frame" << std::endl;

    run_single_loop();

    log_debug << "Recover appears successful, resuming normal use" << std::endl;

    if (!dynamic_cast<ErrorPanel*>(m_scenes.back().get()))
      push_scene(std::make_unique<ErrorPanel>(*this, *m_scenes.back()));

    return true;
  }
  catch(const std::exception& err)
  {
    message = err.what();
  }
  catch(const std::string& msg)
  {
    message = msg;
  }
  catch(const char* msg)
  {
    message = msg;
  }
  catch(...)
  {
    message = "(unknown error type)";
  }

  log_debug << "Recover unsuccessful: " << message << std::endl;

  log_fatal << "Could not recover from error." << std::endl;
  return false;
}

/**
 * Runs a single frame from the main loop.
 *
 * This function SHOULD NOT be called externally; is it public for testing
 * purposes only. The main loop will take care of calling this by itself!
 *
 * This simply executes the event() - update() - draw() routine. Note that there
 * is no guarantee about the order in which they are called, or their frequency;
 * scenes can observe update() - event() - draw() - draw() - event() - draw()...
 *
 * @author Semphris <semphris@semphris.com>, 2022
 */
void
GameManager::run_single_loop()
{
  handle_events();

  if (empty())
    return;

  handle_update();

  if (empty())
    return;

  handle_draw();
}

#ifdef EMSCRIPTEN
#define HANDLE_ERR(err) if (!s_main_game_manager->try_recover(err)) return;
#else
#define HANDLE_ERR(err) if (!try_recover(err)) return 1
#endif

/**
 * Runs the main loop.
 *
 * This function SHOULD NOT be called externally; is it public for testing
 * purposes only. The main loop is already running!
 *
 * For most platforms, this is simply a `while` loop which repeatedly calls
 * GameManager::run_single_loop(), checking for quitting conditions and catching
 * uncaught errors. For certain platforms, like WASM, the main loop is not
 * handled by the C++ code; this function only establishes the main loop
 * through the external means relevant to the platform and may return
 * immediately.
 *
 * @returns 0 if the game finished executing OR, on some platforms, if the main
 *          loop was set; 1 if a fatal error occured and couldn't be recovered
 *          from.
 * @author Semphris <semphris@semphris.com>, 2022
 */
int
GameManager::run_loops()
{
#ifdef EMSCRIPTEN
  emscripten_set_main_loop([] {
#else
  while(!empty())
  {
#endif
    try
    {
#ifdef EMSCRIPTEN
      s_main_game_manager->run_single_loop();
#else
      run_single_loop();
      SDL_Delay(static_cast<Uint32>(m_delay * 1000.0f));
#endif
    }
    catch(const std::exception& err)
    {
      log_error << "Unexpected exception: " << err.what() << std::endl;
      HANDLE_ERR(err);
    }
    catch(const std::string& msg)
    {
      log_error << "Unexpected error: " << msg << std::endl;
      HANDLE_ERR(std::runtime_error(msg));
    }
    catch(const char* msg)
    {
      log_error << "Unexpected error: " << msg << std::endl;
      HANDLE_ERR(std::runtime_error(msg));
    }
    catch(...)
    {
      log_error << "Unexpected unspecified error" << std::endl;
      HANDLE_ERR(std::runtime_error(""));
    }
#ifdef EMSCRIPTEN
  }, -1, false);
#else
  }
#endif

  return 0;
}

/**
 * Takes care of the event phase from the main loop.
 *
 * This function SHOULD NOT be called externally; is it public for testing
 * purposes only. This will be called automatically by the main loop.
 *
 * @author Semphris <semphris@semphris.com>, 2022
 */
void
GameManager::handle_events()
{
  SDL_Event e;
  while (SDL_PollEvent(&e) && !empty())
  {
    if (!m_transition)
    {
      m_scenes.back()->event(e);
    }
    else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
    {
      m_transition = nullptr;
    }

    handle_internal_event(e);
  }
}

/**
 * Takes care of the update phase from the main loop.
 *
 * This function SHOULD NOT be called externally; is it public for testing
 * purposes only. This will be called automatically by the main loop.
 *
 * @author Semphris <semphris@semphris.com>, 2022
 */
void
GameManager::handle_update()
{
  auto t = std::chrono::steady_clock::now();
  auto diff = t - m_last_frame;
  float dt_sec = static_cast<float>(diff.count()) / 1000000000.0f;
  m_last_frame = t;

  if (m_transition && m_transition->update(dt_sec))
  {
    m_transition = nullptr;
    m_popping_scene = nullptr;

    if (!m_scenes.empty())
    {
      m_scenes.back()->enter();
    }
  }

  if (!m_transition && !m_scenes.empty())
  {
    m_scenes.back()->update(dt_sec);
  }
}

/**
 * Takes care of the drawing phase from the main loop.
 *
 * This function SHOULD NOT be called externally; is it public for testing
 * purposes only. This will be called automatically by the main loop.
 *
 * @author Semphris <semphris@semphris.com>, 2022
 */
void
GameManager::handle_draw()
{
  DrawingContext context(m_window->get_renderer());

  if (m_transition)
  {
    m_transition->draw(context);
  }
  else
  {
    m_scenes.back()->draw(context);
  }

  context.render();
}

/**
 * Determines if the game has finished running.
 *
 * @returns true if the game finished running, e. g. the scene stack is empty
 *          and there are no active transitions.
 * @author Semphris <semphris@semphris.com>, 2022
 */
bool
GameManager::empty() const
{
  return m_scenes.empty() && !m_transition;
}

extern "C" {

/**
 * Helper function for Emscripten: Allows getting the message of an exception
 * at a specific address in memory.
 *
 * @note This is for use with the Emscripten int-based exception handler ONLY;
 *       passing other values to this function may result in undefined behavior.
 *
 * @returns The message from the exception.
 * @author Semphris <semphris@semphris.com>, 2022
 */
const char*
getExceptionMessage(intptr_t address)
{
  return reinterpret_cast<std::exception*>(address)->what();
}

}
