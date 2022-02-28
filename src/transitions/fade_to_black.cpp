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

#include "transitions/fade_to_black.hpp"

#include <limits>

#include "scenes/scene.hpp"
#include "video/drawing_context.hpp"
#include "video/renderer.hpp"
#include "video/window.hpp"

FadeToBlack::FadeToBlack(Scene* from, Scene* to, float time) :
  Transition(from, to, time)
{
}

bool
FadeToBlack::update(float dt_sec)
{
  m_time_elapsed += dt_sec;

  if (m_time_elapsed >= m_total_time)
    return true;

  float progress = m_time_elapsed / m_total_time;

  if (progress < .5f)
  {
    if (m_from)
    {
      m_from->update(dt_sec);
    }
    else if (m_to)
    {
      m_to->update(dt_sec);
    }
  }
  else
  {
    if (m_to)
    {
      m_to->update(dt_sec);
    }
    else if (m_from)
    {
      m_from->update(dt_sec);
    }
  }

  return false;
}

void
FadeToBlack::draw(DrawingContext& context) const
{
  float progress = m_time_elapsed / m_total_time;

  Size window_size = context.get_renderer().get_window().get_size();
  Rect area(Vector(), window_size);
  Color color = Color(0.f, 0.f, 0.f);
  Renderer::Blend blend = Renderer::Blend::BLEND;
  int layer = std::numeric_limits<int>::max();
  float alpha;

  if (m_from && m_to)
  {
    if (progress < .5f)
    {
      m_from->draw(context);
      alpha = progress * 2.f;
    }
    else
    {
      m_to->draw(context);
      alpha = (1.f - progress) * 2.f;
    }
  }
  else if (m_from)
  {
    m_from->draw(context);
    alpha = progress;
  }
  else if (m_to)
  {
    m_to->draw(context);
    alpha = 1.f - progress;
  }
  else
  {
    alpha = 1.f;
  }

  context.draw_filled_rect(area, Color(color, alpha), blend, layer);
}
