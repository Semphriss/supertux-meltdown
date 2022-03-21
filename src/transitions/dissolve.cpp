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

#include "transitions/dissolve.hpp"

#include <limits>

#include "scenes/scene.hpp"
#include "video/drawing_context.hpp"
#include "video/renderer.hpp"
#include "video/window.hpp"

Dissolve::Dissolve(Scene* from, Scene* to, float time) :
  Transition(from, to, time)
{
}

bool
Dissolve::update(float dt_sec)
{
  m_time_elapsed += dt_sec;

  if (m_time_elapsed >= m_total_time)
    return true;

  if (m_from)
  {
    m_from->update(dt_sec);
  }

  if (m_to)
  {
    m_to->update(dt_sec);
  }

  return false;
}

void
Dissolve::draw(DrawingContext& context) const
{
  float progress = m_time_elapsed / m_total_time;

  Size window_size = context.get_renderer().get_window().get_size();
  Rect area(window_size);
  Color color = Color(0.0f, 0.0f, 0.0f);
  Renderer::Blend blend = Renderer::Blend::BLEND;
  int layer = std::numeric_limits<int>::max();

  if (m_from && m_to)
  {
    auto& window = context.get_renderer().get_window();
    auto from = window.create_texture(window.get_size()),
         to   = window.create_texture(window.get_size());

    DrawingContext dc(context.get_renderer());
    m_from->draw(dc);
    dc.render(from.get());
    dc.clear();

    m_to->draw(dc);
    dc.render(to.get());
    dc.clear();

    Color white(1.0f, 1.0f, 1.0f);
    Color transparent(Color(white, progress));
    context.draw_texture(from, Rect(from->get_size()), Rect(window.get_size()),
                         0.0f, white, Renderer::Blend::BLEND, 0);
    context.draw_texture(to, Rect(to->get_size()), Rect(window.get_size()),
                         0.0f, transparent, Renderer::Blend::BLEND, 1);
  }
  else if (m_from)
  {
    m_from->draw(context);
    context.draw_filled_rect(area, Color(color, progress), blend, layer);
  }
  else if (m_to)
  {
    m_to->draw(context);
    context.draw_filled_rect(area, Color(color, 1.0f - progress), blend, layer);
  }
  else
  {
    context.draw_filled_rect(area, Color(color, 1.0f), blend, layer);
  }
}
