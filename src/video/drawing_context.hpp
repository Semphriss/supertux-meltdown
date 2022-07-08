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

#ifndef HEADER_STM_VIDEO_DRAWINGCONTEXT_HPP
#define HEADER_STM_VIDEO_DRAWINGCONTEXT_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "util/color.hpp"
#include "util/rect.hpp"
#include "util/vector.hpp"
#include "video/font.hpp"
#include "video/renderer.hpp"

enum class TextAlign
{
  TOP_LEFT,
  TOP_MID,
  TOP_RIGHT,
  MID_LEFT,
  CENTER,
  MID_RIGHT,
  BOT_LEFT,
  BOT_MID,
  BOT_RIGHT
};

// This class has three purposes:
// - Allow drawing out of order
// - Bufferise the draw requests to render to multiple render targets
// - Manage the cache for textures, fonts, and other cacheable assets
class DrawingContext final
{
private:
  class RenderCache final
  {
  public:
    RenderCache(Renderer& renderer);
    ~RenderCache() = default;

    Texture& get_texture(const std::string& texture);

  private:
    Renderer& m_renderer;
    std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;

  private:
    RenderCache(const RenderCache&) = delete;
    RenderCache& operator=(const RenderCache&) = delete;
  };

  class Request
  {
  public:
    Request() = default;
    virtual ~Request() = default;

    virtual void draw(Renderer& renderer) const = 0;

  private:
    Request(const Request&) = delete;
    Request& operator=(const Request&) = delete;
  };

  class RectRequest final :
    public Request
  {
  public:
    RectRequest(const Rect& rect, const Color& color, Blend blend);
    virtual ~RectRequest() = default;

    virtual void draw(Renderer& renderer) const override;

  private:
    const Rect m_rect;
    const Color m_color;
    const Blend m_blend;

  private:
    RectRequest(const RectRequest&) = delete;
    RectRequest& operator=(const RectRequest&) = delete;
  };

  class LineRequest final :
    public Request
  {
  public:
    LineRequest(const Vector& p1, const Vector& p2, const Color& color,
                Blend blend);
    virtual ~LineRequest() = default;

    virtual void draw(Renderer& renderer) const override;

  private:
    const Vector m_p1;
    const Vector m_p2;
    const Color m_color;
    const Blend m_blend;

  private:
    LineRequest(const LineRequest&) = delete;
    LineRequest& operator=(const LineRequest&) = delete;
  };

  class TextureRequest final :
    public Request
  {
  public:
    TextureRequest(const std::string& texture, const Rect& src, const Rect& dst,
                   const Color& color, Blend blend, DrawingContext& context);
    virtual ~TextureRequest() = default;

    virtual void draw(Renderer& renderer) const override;

  private:
    const std::string m_texture;
    const Rect m_src;
    const Rect m_dst;
    const Color m_color;
    const Blend m_blend;
    DrawingContext& m_context;

  private:
    TextureRequest(const TextureRequest&) = delete;
    TextureRequest& operator=(const TextureRequest&) = delete;
  };

  class TextRequest final :
    public Request
  {
  public:
    TextRequest(const std::string& text, const Font& font, const Rect& dst,
                TextAlign align, const Color& color, Blend blend, bool outline);
    virtual ~TextRequest() = default;

    virtual void draw(Renderer& renderer) const override;

  private:
    const std::string m_text;
    const Font& m_font;
    const Rect m_dst;
    const TextAlign m_align;
    const Color m_color;
    const Blend m_blend;
    const bool m_outline;

  private:
    TextRequest(const TextRequest&) = delete;
    TextRequest& operator=(const TextRequest&) = delete;
  };

  class Transform final
  {
    friend class DrawingContext;

  public:
    Transform();
    Transform(const Transform&) = default;

    void move(const Vector& offset);

    // Accessors are voluntarily omitted; any code may change those values at
    // any time for any reason, so nothing can be inferred from their values.

  private:
    Vector m_offset;
  };

public:
  DrawingContext();
  ~DrawingContext() = default;

  void push_transform();
  void pop_transform();
  Transform& get_transform();

  void render(Renderer& renderer);
  void clear();

  void draw_filled_rect(const Rect& rect, const Color& color, Blend blend);
  void draw_line(const Vector& p1, const Vector& p2, const Color& color,
                 Blend blend);
  void draw_texture(const std::string& texture, const Rect& src,
                    const Rect& dst, const Color& color, Blend blend);
  void draw_text(const std::string& text, const std::string& font, int size,
                 TextAlign align, const Rect& dst, const Color& color,
                 Blend blend, bool outline = true);

  // This function is usually called from the dtor of the Renderer, but can
  // safely be called at any time.
  void unbind(Renderer* renderer);

  RenderCache& get_render_cache(Renderer* renderer);

public:
  Size target_size;

private:
  std::vector<std::unique_ptr<Request>> m_requests;
  std::unordered_map<Renderer*, std::unique_ptr<RenderCache>> m_renderer_caches;
  std::unordered_map<std::string, std::unique_ptr<Font>> m_font_cache;
  std::vector<Transform> m_transforms;

private:
  DrawingContext(const DrawingContext&) = delete;
  DrawingContext& operator=(const DrawingContext&) = delete;
};

#endif
