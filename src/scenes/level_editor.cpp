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

#include "scenes/level_editor.hpp"

#include <iostream>

LevelEditor::LevelEditor(SceneController& scene_controller) :
  Scene(scene_controller),
  m_tilemap()
{
}

void
LevelEditor::event(const SDL_Event& event)
{
  m_tilemap.event(event);

  switch (event.type)
  {
    case SDL_KEYDOWN:
      if (event.key.keysym.mod & KMOD_CTRL)
      {
        switch (event.key.keysym.sym)
        {
          case SDLK_s:
            try
            {
              m_tilemap.save_tilemap("/levels/level.bmp");
            }
            catch (const std::exception& e)
            {
              std::cerr << e.what() << std::endl;
            }
            break;

          case SDLK_o:
            try
            {
              m_tilemap.load_tilemap("/levels/level.bmp");
            }
            catch (const std::exception& e)
            {
              std::cerr << e.what() << std::endl;
            }
            break;
        }
      }
      break;

    default:
      break;
  }
}

void
LevelEditor::update(float dt_sec)
{
  m_tilemap.update(dt_sec);
}

void
LevelEditor::draw(DrawingContext& context) const
{
  m_tilemap.draw(context);
}
