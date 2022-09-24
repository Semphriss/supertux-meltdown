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

#include <cmath>

#include "util/math.hpp"

// Will move as 1/(n^t) (invert exponential). t is time in seconds, this is n.
// 1 is no movement at all. Infinity is instantaneous. <1 is bad.
//
// For easy setting, this is calculated as 10^(2^val) where val is:
// - 0: Heavy (super slow)
// - 1: Dragging
// - 2: Comfortable
// - 3: Fast
// - 4: Super fast
// - 5: Transition barely noticeable
// - 6: Instantaneous (same as no transition at all)
static const float CAM_EXPONENT = std::pow(10.0f, std::pow(2.0f, 2.0f));

EditorCamera::EditorCamera() :
  m_current_pos(0.0f, 0.0f),
  m_current_zoom(1.0f),
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
#if SDL_VERSION_ATLEAST(2, 0, 4)
          // Return code ignored; it errors only if the feature is unsupported
          SDL_CaptureMouse(SDL_TRUE);
#endif
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
#if SDL_VERSION_ATLEAST(2, 0, 4)
          // Return code ignored; it errors only if the feature is unsupported
          SDL_CaptureMouse(SDL_FALSE);
#endif
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
EditorCamera::update(float dt_sec)
{
  // The advantage of using an invert exponential equation is that the remaining
  // portion of the movement can be known without knowing the initial value.
  //
  // For example, if CAM_EXPONENT is 2, the time elapsed is 1 second, the
  // initial value is 1 and the target value is 0, no matter if the current time
  // is 0, 1, 2.5 or 392.58374, the new value can be obtained simply by dividing
  // the current value by two.
  //
  // More generally, the new value is the current value, divided by CAM_EXPONENT
  // to the power of the elapsed time in seconds.
  //
  // The code implements that concept, and adds the target point into the mix.

  m_current_pos = (m_current_pos - m_pos) / std::pow(CAM_EXPONENT, dt_sec)
                + m_pos;
  m_current_zoom = (m_current_zoom - m_zoom) / std::pow(CAM_EXPONENT, dt_sec)
                 + m_zoom;
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
  return Vector((vector.x - get_pos().x) / get_zoom(),
                (vector.y - get_pos().y) / get_zoom());
}

const Vector&
EditorCamera::get_pos() const
{
  return m_current_pos;
}

float
EditorCamera::get_zoom() const
{
  return m_current_zoom;
}
