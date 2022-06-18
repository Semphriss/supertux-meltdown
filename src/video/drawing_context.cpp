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

#include "video/drawing_context.hpp"
#include <memory>
#include <string>

DrawingContext::RenderCache::RenderCache(Renderer& renderer) :
  m_renderer(renderer)
{
}

Texture&
DrawingContext::RenderCache::get_texture(const std::string& texture)
{
  auto it = m_textures.find(texture);

  if (it == m_textures.end())
  {
    auto t = std::make_unique<Texture>(m_renderer, texture);
    it = m_textures.emplace(texture, std::move(t)).first;
  }

  return *(it->second);
}

DrawingContext::RectRequest::RectRequest(const Rect& rect, const Color& color,
                                         Blend blend) :
  m_rect(rect),
  m_color(color),
  m_blend(blend)
{
}

void
DrawingContext::RectRequest::draw(Renderer& renderer) const
{
  renderer.draw_filled_rect(m_rect, m_color, m_blend);
}

DrawingContext::LineRequest::LineRequest(const Vector& p1, const Vector& p2,
                                         const Color& color, Blend blend) :
  m_p1(p1),
  m_p2(p2),
  m_color(color),
  m_blend(blend)
{
}

void
DrawingContext::LineRequest::draw(Renderer& renderer) const
{
  renderer.draw_line(m_p1, m_p2, m_color, m_blend);
}

DrawingContext::TextureRequest::TextureRequest(const std::string& texture,
                                               const Rect& src, const Rect& dst,
                                               const Color& color, Blend blend,
                                               DrawingContext& context) :
  m_texture(texture),
  m_src(src),
  m_dst(dst),
  m_color(color),
  m_blend(blend),
  m_context(context)
{
}

void
DrawingContext::TextureRequest::draw(Renderer& renderer) const
{
  auto& texture = m_context.get_render_cache(&renderer).get_texture(m_texture);
  Rect src = m_src.is_null() ? texture.get_size() : m_src;

  renderer.draw_texture(texture, src, m_dst, m_color, m_blend);
}

DrawingContext::TextRequest::TextRequest(const std::string& text,
                                         const Font& font, const Rect& dst,
                                         TextAlign align, const Color& color,
                                         Blend blend) :
  m_text(text),
  m_font(font),
  m_dst(dst),
  m_align(align),
  m_color(color),
  m_blend(blend)
{
}

void
DrawingContext::TextRequest::draw(Renderer& renderer) const
{
  auto texture = m_font.draw_text(renderer, m_text, m_dst.width());
  auto texture_size = texture->get_size();

  Rect dst(m_dst.top_lft(), texture_size);
  Vector move;

  switch (m_align)
  {
    case TextAlign::MID_LEFT:
    case TextAlign::CENTER:
    case TextAlign::MID_RIGHT:
      move.y = m_dst.height() / 2.0f - texture_size.h / 2.0f;
      break;

    case TextAlign::BOT_LEFT:
    case TextAlign::BOT_MID:
    case TextAlign::BOT_RIGHT:
      move.y = m_dst.height() - texture_size.h;
      break;

    default:
      break;
  }

  switch (m_align)
  {
    case TextAlign::TOP_MID:
    case TextAlign::CENTER:
    case TextAlign::BOT_MID:
      move.x = m_dst.width() / 2.0f - texture_size.w / 2.0f;
      break;

    case TextAlign::TOP_RIGHT:
    case TextAlign::MID_RIGHT:
    case TextAlign::BOT_RIGHT:
      move.x = m_dst.width() - texture_size.w;
      break;

    default:
      break;
  }

  dst.move(move);

  renderer.draw_texture(*texture, texture_size, dst, m_color, m_blend);
}

void
DrawingContext::render(Renderer& renderer)
{
  for (const auto& request : m_requests)
  {
    request->draw(renderer);
  }
  renderer.flush();
}

void
DrawingContext::clear()
{
  m_requests.clear();
}

void
DrawingContext::draw_filled_rect(const Rect& rect, const Color& color,
                                 Blend blend)
{
  auto req = std::make_unique<RectRequest>(rect, color, blend);

  m_requests.push_back(std::move(req));
}

void 
DrawingContext::draw_line(const Vector& p1, const Vector& p2,
                          const Color& color, Blend blend)
{
  auto req = std::make_unique<LineRequest>(p1, p2, color, blend);

  m_requests.push_back(std::move(req));
}

void
DrawingContext::draw_texture(const std::string& texture, const Rect& src,
                             const Rect& dst, const Color& color, Blend blend)
{
  auto req = std::make_unique<TextureRequest>(texture, src, dst, color, blend,
                                              *this);

  m_requests.push_back(std::move(req));
}

void
DrawingContext::draw_text(const std::string& text, const std::string& font,
                          int size, TextAlign align, const Rect& dst,
                          const Color& color, Blend blend)
{
  std::string key = font + " (" + std::to_string(size) + ")";

  auto it = m_font_cache.find(key);

  if (it == m_font_cache.end())
  {
    it = m_font_cache.emplace(key, std::make_unique<Font>(font, size)).first;
  }

  auto& font_cache = *(it->second);

  auto req = std::make_unique<TextRequest>(text, font_cache, dst, align, color,
                                           blend);

  m_requests.push_back(std::move(req));
}

void
DrawingContext::unbind(Renderer* renderer)
{
  m_renderer_caches.erase(renderer);
}

DrawingContext::RenderCache&
DrawingContext::get_render_cache(Renderer* renderer)
{
  auto it = m_renderer_caches.find(renderer);

  if (it == m_renderer_caches.end())
  {
    auto cache = std::make_unique<RenderCache>(*renderer);
    it = m_renderer_caches.emplace(renderer, std::move(cache)).first;
  }

  return *(it->second);
}