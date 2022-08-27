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

#ifndef HEADER_STM_VIDEO_FONT_HPP
#define HEADER_STM_VIDEO_FONT_HPP

#include <memory>
#include <string>

#include "SDL2/SDL_ttf.h"

#include "video/texture.hpp"

class Font final
{
public:
  Font(const std::string& file, bool physfs, int size);
  ~Font();

  std::unique_ptr<Texture> draw_text(Renderer& renderer,
                                     const std::string& text,
                                     float width) const;

private:
  TTF_Font* m_font;

private:
  Font(const Font&) = delete;
  Font& operator=(const Font&) = delete;
};

#endif
