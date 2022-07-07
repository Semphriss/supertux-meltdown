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

#ifndef HEADER_STM_SCENES_TILEMAPEDITOR_HPP
#define HEADER_STM_SCENES_TILEMAPEDITOR_HPP

#include "scenes/scene.hpp"

#include <vector>

class TilemapEditor final :
  public Scene
{
public:
  TilemapEditor(SceneController& scene_controller);
  virtual ~TilemapEditor() override = default;

  virtual void event(const SDL_Event& event) override;
  virtual void update(float dt_sec) override;
  virtual void draw(DrawingContext& context) const override;

private:
  std::vector<std::vector<int>> m_tilemap;

private:
  TilemapEditor(const TilemapEditor&) = delete;
  TilemapEditor& operator=(const TilemapEditor&) = delete;
};

#endif
