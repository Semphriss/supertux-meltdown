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

#include "editor/editor_camera.hpp"

#include "SDL.h"

#include "util/math.hpp"

static const Vector CAM_SPEED = Vector(256.0f, 256.0f);

EditorCamera::EditorCamera() :
  m_position(),
  m_zoom(1.0f),
  m_target_position(),
  m_target_zoom(1.0f),
  m_mouse_pos(),
  m_go_up(false),
  m_go_right(false),
  m_go_left(false),
  m_go_down(false),
  m_pin_move(false)
{
}

bool
EditorCamera::event(const SDL_Event& event)
{
  switch (event.type)
  {
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym)
      {
        case SDLK_w:
        case SDLK_UP:
          m_go_up = true;
          break;

        case SDLK_a:
        case SDLK_LEFT:
          m_go_left = true;
          break;

        case SDLK_s:
        case SDLK_DOWN:
          m_go_down = true;
          break;

        case SDLK_d:
        case SDLK_RIGHT:
          m_go_right = true;
          break;

        default:
          break;
      }
      break;

    case SDL_KEYUP:
      switch (event.key.keysym.sym)
      {
        case SDLK_w:
        case SDLK_UP:
          m_go_up = false;
          break;

        case SDLK_a:
        case SDLK_LEFT:
          m_go_left = false;
          break;

        case SDLK_s:
        case SDLK_DOWN:
          m_go_down = false;
          break;

        case SDLK_d:
        case SDLK_RIGHT:
          m_go_right = false;
          break;

        default:
          break;
      }
      break;

    case SDL_MOUSEBUTTONDOWN:
      if (event.button.button == SDL_BUTTON_RIGHT)
      {
        m_pin_move = true;
      }
      break;

    case SDL_MOUSEBUTTONUP:
      if (event.button.button == SDL_BUTTON_RIGHT)
      {
        m_pin_move = false;
      }
      break;

    case SDL_MOUSEMOTION:
      m_mouse_pos = Vector(event.motion.x, event.motion.y);
      if (m_pin_move)
      {
        Vector delta(event.motion.xrel, event.motion.yrel);
        m_target_position -= delta;
      }
      break;

    case SDL_MOUSEWHEEL:
      m_target_zoom += static_cast<float>(event.wheel.y) / 8.f;
      m_target_zoom = Math::clamp(0.5f, 2.f, m_target_zoom);
      m_target_position = (m_mouse_pos + m_position) / m_zoom * m_target_zoom
                        - m_mouse_pos;
      break;

    default:
      break;
  }

  return false;
}

void
EditorCamera::update(float dt_sec)
{
  if (m_go_up)
    m_target_position.y -= CAM_SPEED.y * dt_sec;
  if (m_go_down)
    m_target_position.y += CAM_SPEED.y * dt_sec;
  if (m_go_left)
    m_target_position.x -= CAM_SPEED.x * dt_sec;
  if (m_go_right)
    m_target_position.x += CAM_SPEED.x * dt_sec;

  m_position = m_position * 0.9f + m_target_position * 0.1f;
  m_zoom = m_zoom * 0.9f + m_target_zoom * 0.1f;
}

const Vector&
EditorCamera::get_position() const
{
  return m_position;
}

float
EditorCamera::get_zoom() const
{
  return m_zoom;
}

bool
EditorCamera::has_focus() const
{
  return m_pin_move;
}
