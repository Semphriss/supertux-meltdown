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

#include "util/math.hpp"

EditorCamera::EditorCamera() :
  m_pos(0.0f, 0.0f),
  m_zoom(1.0f),
  m_moving(false),
  m_mouse_pos()
{
}

void
EditorCamera::event(const SDL_Event& event)
{
  switch (event.type)
  {
    case SDL_MOUSEBUTTONDOWN:
      switch (event.button.button)
      {
        case SDL_BUTTON_RIGHT:
          m_moving = true;
          break;

        default:
          break;
      }
      break;

    case SDL_MOUSEBUTTONUP:
      switch (event.button.button)
      {
        case SDL_BUTTON_RIGHT:
          m_moving = false;
          break;

        default:
          break;
      }
      break;

    case SDL_MOUSEMOTION:
      m_mouse_pos = Vector(event.motion.x, event.motion.y);
      if (m_moving)
      {
        m_pos += Vector(event.motion.xrel, event.motion.yrel);
      }
      break;

    case SDL_MOUSEWHEEL:
      {
        float m_old_zoom = m_zoom;
        m_zoom += static_cast<float>(event.wheel.y) / 8.0f;
        m_zoom = Math::clamp(m_zoom, 0.5f, 2.0f);
        m_pos = m_mouse_pos - (m_mouse_pos - m_pos) * m_zoom / m_old_zoom;
      }
      break;

    default:
      break;
  }
}

void
EditorCamera::apply_transform(DrawingContext& context) const
{
  context.get_transform().move(get_pos());
  context.get_transform().scale(Size(1.0f, 1.0f) * get_zoom());
}

Vector
EditorCamera::apply_transform(const Vector& vector) const
{
  return Vector((vector.x - m_pos.x) / m_zoom, (vector.y - m_pos.y) / m_zoom);
}

const Vector&
EditorCamera::get_pos() const
{
  return m_pos;
}

float
EditorCamera::get_zoom() const
{
  return m_zoom;
}
