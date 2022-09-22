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

#ifndef HEADER_STM_EDITOR_EDITORCAMERA_HPP
#define HEADER_STM_EDITOR_EDITORCAMERA_HPP

#include "video/drawing_context.hpp"

class EditorCamera final
{
public:
  EditorCamera();

  void event(const SDL_Event& event);
  void update(float dt_sec);

  void apply_transform(DrawingContext& context) const;
  Vector apply_transform(const Vector& vector) const;

  const Vector& get_pos() const;
  float get_zoom() const;

private:
  Vector m_current_pos;
  float m_current_zoom;
  Vector m_pos;
  float m_zoom;
  bool m_moving;
  Vector m_mouse_pos;

private:
  EditorCamera(const EditorCamera&) = delete;
  EditorCamera& operator=(const EditorCamera&) = delete;
};

#endif
