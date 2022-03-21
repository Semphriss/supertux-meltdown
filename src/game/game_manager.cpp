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

#include <memory>
#include <vector>

#include "make_unique.hpp"

#include "SDL.h"

#include "fs/file.hpp"
#include "game/resource_manager.hpp"
#include "scenes/main_menu.hpp"
#include "transitions/transition.hpp"
#include "util/color.hpp"
#include "util/log.hpp"
#include "video/drawing_context.hpp"
#include "video/font.hpp"
static std::vector<Window::VideoSystem> SUPPORTED_VIDEO_SYSTEMS = {
  Window::VideoSystem::SDL,
  Window::VideoSystem::GL
};

int
GameManager::run(int argc, char** argv)
{
  ResourceManager::get_resource_manager(argv[0]);

  setup_filesystem();

  change_video_system(Window::VideoSystem::SDL);

  push_scene(std::make_unique<MainMenu>(*this), Transition::Type::DISSOLVE);

  m_last_frame = std::chrono::steady_clock::now();

  run_loops();

  return 0;
}

void
GameManager::change_video_system(Window::VideoSystem video_system)
{
  auto window = Window::create_window(video_system);
  window->set_title("SuperTux Meltdown");
  window->set_resizable(true);

  if (m_window)
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
}

Window&
GameManager::get_window()
{
  return *m_window;
}

void
GameManager::push_scene(std::unique_ptr<Scene> scene,
                        Transition::Type transition, float time)
{
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
}

void
GameManager::pop_scene(Transition::Type transition, float time)
{
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
}

void
GameManager::setup_filesystem() const
{
  auto local_path = File::get_pref_dir("Semphris", "SuperTux Meltdown");
#ifdef UBUNTU_TOUCH
  auto src_path = File::build_os_path({File::get_base_dir(), "data"});
#else
  auto src_path = File::build_os_path({File::get_base_dir(), "../data"});
#endif

  File::mount(local_path);
  File::mount(src_path);

  File::set_write_dir(local_path);
}

void
GameManager::advance_video_system()
{
  Window::VideoSystem target_video_system = SUPPORTED_VIDEO_SYSTEMS.back();

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

int
GameManager::run_loops()
{
  while(!empty())
  {
    handle_events();

    if (empty())
      break;

    handle_update();

    if (empty())
      break;

    handle_draw();

    SDL_Delay(10);
  }

  return 0;
}

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

      case SDL_KEYDOWN:
        switch(e.key.keysym.sym)
        {
          case SDLK_F1:
            // TODO: Open the help menu
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

            // TODO: Save screenshot to file
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
}

void
GameManager::handle_update()
{
  auto t = std::chrono::steady_clock::now();
  auto diff = t - m_last_frame;
  float dt_sec = static_cast<float>(diff.count()) / 1000000000.f;
  m_last_frame = t;

  if(m_transition && m_transition->update(dt_sec))
  {
    m_transition = nullptr;

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

bool
GameManager::empty() const
{
  return m_scenes.empty() && !m_transition;
}
