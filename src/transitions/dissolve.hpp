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

#ifndef _HEADER_STMELTDOWN_TRANSITIONS_DISSOLVE_HPP
#define _HEADER_STMELTDOWN_TRANSITIONS_DISSOLVE_HPP

#include "transitions/transition.hpp"

class Dissolve final :
  public Transition
{
public:
  Dissolve(Scene* from, Scene* to, float time);
  virtual ~Dissolve() override = default;

  virtual bool update(float dt_sec) override;
  virtual void draw(DrawingContext& context) const override;

private:
  Dissolve(const Dissolve&) = delete;
  Dissolve& operator=(const Dissolve&) = delete;
};

#endif
