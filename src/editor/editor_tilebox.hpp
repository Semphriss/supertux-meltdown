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

#ifndef HEADER_STM_SCENES_EDITORTILEBOX_HPP
#define HEADER_STM_SCENES_EDITORTILEBOX_HPP

#include <string>
#include <vector>

#include "video/drawing_context.hpp"

class EditorTilemap;

class EditorTilebox final
{
public:
  EditorTilebox(EditorTilemap& parent,
                const std::vector<std::string>& tileset,
                const std::string& data_folder);

  bool event(const SDL_Event& event);
  void update(float dt_sec);
  void draw(DrawingContext& context) const;

private:
  EditorTilemap& m_parent;
  const std::vector<std::string>& m_tileset;
  std::string m_data_folder;
  Vector m_mouse_pos;
  bool m_mouse_down;

private:
  EditorTilebox(const EditorTilebox&) = delete;
  EditorTilebox& operator=(const EditorTilebox&) = delete;
};

#endif
