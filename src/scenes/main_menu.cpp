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

#include "scenes/main_menu.hpp"

#include <memory>

#include "video/drawing_context.hpp"

MainMenu::MainMenu(SceneController& scene_controller) :
  Scene(scene_controller)
{
}

void
MainMenu::event(const SDL_Event& event)
{
}

void
MainMenu::update(float dt_sec)
{
}

void
MainMenu::draw(DrawingContext& context) const
{
  context.draw_texture("images/background.png", true, {}, context.target_size,
                       Color(1.0f, 1.0f, 1.0f), Blend::BLEND);

  Rect dst(context.target_size);

  context.draw_text("Copyright (c) 2022 Semphris <semphris@semphris.com>\n"
                    "This game comes with ABSOLUTELY NO WARRANTY. This is free "
                    "software, and you are welcome to redistribute it under "
                    "certain conditions; see the license file for details.",
                    "fonts/SuperTux-Medium.ttf", true, 12,
                    TextAlign::BOT_RIGHT, dst, Color(1.0f, 1.0f, 1.0f),
                    Blend::BLEND);
}
