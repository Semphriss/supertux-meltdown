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

#ifndef _HEADER_STMELTDOWN_EDITOR_EDITORCAMERA_HPP
#define _HEADER_STMELTDOWN_EDITOR_EDITORCAMERA_HPP

#include "scenes/scene.hpp"

#include <memory>

#include "game/game_object_manager.hpp"
#include "util/vector.hpp"

union SDL_Event;

class EditorCamera final
{
public:
  EditorCamera();

  bool event(const SDL_Event& event);
  void update(float dt_sec);

  const Vector& get_position() const;
  float get_zoom() const;
  bool has_focus() const;

private:
  Vector m_position;
  float m_zoom;

  Vector m_target_position;
  float m_target_zoom;

  Vector m_mouse_pos;

  bool m_go_up;
  bool m_go_right;
  bool m_go_left;
  bool m_go_down;

  bool m_pin_move;

private:
  EditorCamera(const EditorCamera&) = delete;
  EditorCamera& operator=(const EditorCamera&) = delete;
};

#endif
