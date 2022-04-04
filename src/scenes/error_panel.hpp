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

#ifndef _HEADER_STMELTDOWN_SCENES_ERRORPANEL_HPP
#define _HEADER_STMELTDOWN_SCENES_ERRORPANEL_HPP

#include "scenes/scene.hpp"

#include <memory>

class Texture;

class ErrorPanel final :
  public Scene
{
public:
  ErrorPanel() = delete;
  ErrorPanel(IGameManager& game_manager, const Scene& target_scene);
  virtual ~ErrorPanel() = default;

  virtual void event(const SDL_Event& event) override;
  virtual void update(float dt_sec) override;
  virtual void draw(DrawingContext& context) const override;

  virtual void enter() override;
  virtual void leave(bool to_parent) override;
  virtual bool quit(bool can_abort) override;

  virtual void reset_caches() override;
  virtual bool recover_from_error(const std::exception& err) override;

private:
  const Scene& m_scene;
  std::shared_ptr<Texture> m_image;
  std::string m_font_path;

private:
  ErrorPanel(const ErrorPanel&) = delete;
  ErrorPanel& operator=(const ErrorPanel&) = delete;
};

#endif
