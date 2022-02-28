//  SuperTux Meltdown - Semphris' take on the popular Linux platformer
//  Copyright (C) 2021-2022 Semphris <semphris@protonmail.com>
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

#ifndef _HEADER_STMELTDOWN_TRANSITIONS_TRANSITION_HPP
#define _HEADER_STMELTDOWN_TRANSITIONS_TRANSITION_HPP

#include <memory>

class Scene;
class DrawingContext;

class Transition
{
public:
  enum class Type {
    NONE,
    FADE_BLACK,
    DISSOLVE
  };

public:
  static std::unique_ptr<Transition> create_transition(Scene* from, Scene* to,
                                                       float time, Type type);

public:
  Transition(Scene* from, Scene* to, float time);
  virtual ~Transition() = default;

  virtual bool update(float dt_sec) = 0;
  virtual void draw(DrawingContext& context) const = 0;

protected:
  Scene* m_from;
  Scene* m_to;
  float m_total_time;
  float m_time_elapsed;

private:
  Transition(const Transition&) = delete;
  Transition& operator=(const Transition&) = delete;
};

#endif
