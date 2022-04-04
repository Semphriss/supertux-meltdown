//  SuperTux Meltdown - Semphris' take on the popular Linux platformer
//  Copyright (C) 2021-2022 Semphris <semphris@semphris.com>
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

#ifndef _HEADER_STMELTDOWN_SCENES_SCENE_HPP
#define _HEADER_STMELTDOWN_SCENES_SCENE_HPP

class DrawingContext;
class IGameManager;
union SDL_Event;
namespace std {
class exception;
}

class Scene
{
public:
  Scene() = delete;
  Scene(IGameManager& game_manager);
  virtual ~Scene() = default;

  virtual void event(const SDL_Event& event) = 0;
  virtual void update(float dt_sec) = 0;
  virtual void draw(DrawingContext& context) const = 0;

  virtual void enter() = 0;
  virtual void leave(bool to_parent) = 0;
  virtual bool quit(bool can_abort) = 0;

  virtual void reset_caches() = 0;
  virtual bool recover_from_error(const std::exception& err) = 0;

protected:
  IGameManager& m_game_manager;

private:
  Scene(const Scene&) = delete;
  Scene& operator=(const Scene&) = delete;
};

#endif
