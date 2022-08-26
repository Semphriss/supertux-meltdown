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

#ifndef HEADER_STM_SCENES_EDITORTILEMAP_HPP
#define HEADER_STM_SCENES_EDITORTILEMAP_HPP

#include <vector>

#include "editor/editor_camera.hpp"
#include "editor/editor_tilebox.hpp"
#include "util/vector.hpp"
#include "video/drawing_context.hpp"

class EditorTilemap final
{
public:
  /** @todo move the file paths to a module dedicated to FS ops */
  EditorTilemap(const std::string& data_folder);

  void event(const SDL_Event& event);
  void update(float dt_sec);
  void draw(DrawingContext& context) const;

  void load_tilemap(const std::string& file);
  void save_tilemap(const std::string& file) const;

  void set_tile_id(size_t id);
  void resize_tilemap_to(const Vector& tilemap_point);

  Vector screen_to_tilemap(const Vector& screen_point) const;
  Vector tilemap_to_screen(const Vector& tilemap_point) const;

private:
  std::vector<std::vector<int>> m_tilemap;
  EditorCamera m_camera;
  EditorTilebox m_tilebox;
  Vector m_tilemap_offset;
  std::string m_data_folder;
  size_t m_tile_id;
  Vector m_mouse_pos;

private:
  EditorTilemap(const EditorTilemap&) = delete;
  EditorTilemap& operator=(const EditorTilemap&) = delete;
};

#endif
