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

#include "video/font.hpp"

#include <memory>

#include "util/fs.hpp"

Font::Font(const std::string& file, bool physfs, int size) :
  m_font()
{
  if (physfs)
  {
    m_font = TTF_OpenFontRW(FS::get_rwops(file, FS::OP::READ), true, size);
  }
  else
  {
    m_font = TTF_OpenFont(file.c_str(), size);
  }

  if (!m_font)
  {
    throw std::runtime_error("Can't load font '" + file + "': "
                             + std::string(SDL_GetError()));
  }
}

Font::~Font()
{
  TTF_CloseFont(m_font);
}

std::unique_ptr<Texture>
Font::draw_text(Renderer& renderer, const std::string& text, float width) const
{
  SDL_Color white;
  white.r = 255;
  white.g = 255;
  white.b = 255;
  white.a = 255;

  SDL_Surface* s = TTF_RenderText_Blended_Wrapped(m_font, text.c_str(), white,
                                                  static_cast<int>(width));
  auto t = std::make_unique<Texture>(renderer, s, true);

  return std::move(t);
}
