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

#ifndef HEADER_STM_TESTS_MOCK_SCENES_MOCKSCENE_HPP
#define HEADER_STM_TESTS_MOCK_SCENES_MOCKSCENE_HPP

#include "scenes/scene.hpp"

/**
 * @todo Make macros for those
 * @todo Add the possibility to validate received arguments
 * @todo Add the possibility to return some predetermined values
 */
class MockScene final :
  public Scene
{
public:
  using Scene::Scene;

private:
  mutable int m_mock_calls_event = 0;
public:
  int mock_get_calls_event() const { return m_mock_calls_event; }
  virtual void event(const SDL_Event& event) override
  {
    m_mock_calls_event++;
  }

private:
  mutable int m_mock_calls_update = 0;
public:
  int mock_get_calls_update() const { return m_mock_calls_update; }
  virtual void update(float dt_sec) override
  {
    m_mock_calls_update++;
  }

private:
  mutable int m_mock_calls_draw = 0;
public:
  int mock_get_calls_draw() const { return m_mock_calls_draw; }
  virtual void draw(DrawingContext& context) const override
  {
    m_mock_calls_draw++;
  }
};

#endif
