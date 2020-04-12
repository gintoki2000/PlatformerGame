#include "WorldRenderer.h"
#include "SDL_render.h"

WorldRenderer::WorldRenderer(SDL_Renderer* renderer, float ppm) :
    m_renderer(renderer), m_ppm(ppm)
{
}

void WorldRenderer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount,
                                const b2Color& color)
{
    SDL_SetRenderDrawColor(m_renderer, color.r * 0xff, color.g * 0xff,
                           color.b * 0xff, color.a * 0xff);
    int x1, y1, x2, y2;
    for (int i = 0; i < vertexCount - 1; ++i)
    {
        x1 = vertices[i].x * m_ppm - m_viewport.x;
        y1 = vertices[i].y * m_ppm - m_viewport.y;
        x2 = vertices[i + 1].x * m_ppm - m_viewport.x;
        y2 = vertices[i + 1].y * m_ppm - m_viewport.y;
        SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
    }
    x1 = vertices[vertexCount - 1].x * m_ppm - m_viewport.x;
    y1 = vertices[vertexCount - 1].y * m_ppm - m_viewport.y;
    x2 = vertices[0].x * m_ppm - m_viewport.x;
    y2 = vertices[0].y * m_ppm - m_viewport.y;
    SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
}

void WorldRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount,
                                     const b2Color& color)
{
    SDL_SetRenderDrawColor(m_renderer, color.r * 0xff, color.g * 0xff,
                           color.b * 0xff, color.a * 0xff);
    int x1, y1, x2, y2;
    for (int i = 0; i < vertexCount - 1; ++i)
    {
        x1 = vertices[i].x * m_ppm - m_viewport.x;
        y1 = vertices[i].y * m_ppm - m_viewport.y;
        x2 = vertices[i + 1].x * m_ppm - m_viewport.x;
        y2 = vertices[i + 1].y * m_ppm - m_viewport.y;
        SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
    }
    x1 = vertices[vertexCount - 1].x * m_ppm - m_viewport.x;
    y1 = vertices[vertexCount - 1].y * m_ppm - m_viewport.y;
    x2 = vertices[0].x * m_ppm - m_viewport.x;
    y2 = vertices[0].y * m_ppm - m_viewport.y;
    SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
}

void WorldRenderer::DrawCircle(const b2Vec2& p, float r, const b2Color& c)
{
    SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0x00, 0xff);
    SDL_RenderDrawPoint(m_renderer, p.x * m_ppm, p.y * m_ppm);
    SDL_Rect rect;
    rect.x = (p.x - r) * m_ppm;
    rect.y = (p.y - r) * m_ppm;
    rect.w = r * 2.f * m_ppm;
    rect.h = r * 2.f * m_ppm;
    SDL_SetRenderDrawColor(m_renderer, c.r * 255, c.g * 255, c.b * 255,
                           c.a * 255);
    SDL_RenderDrawRect(m_renderer, &rect);
}

void WorldRenderer::DrawSolidCircle(const b2Vec2& p, float r, const b2Vec2&,
                                    const b2Color& c)
{
    SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0x00, 0xff);
    SDL_RenderDrawPoint(m_renderer, p.x * m_ppm - m_viewport.x, p.y * m_ppm - m_viewport.y);
    SDL_Rect rect;
    rect.x = (p.x - r) * m_ppm - m_viewport.x;
    rect.y = (p.y - r) * m_ppm - m_viewport.y;
    rect.w = r * 2.f * m_ppm;
    rect.h = r * 2.f * m_ppm;
    SDL_SetRenderDrawColor(m_renderer, c.r * 255, c.g * 255, c.b * 255,
                           c.a * 255);
    SDL_RenderDrawRect(m_renderer, &rect);
}

void WorldRenderer::DrawSegment(const b2Vec2&, const b2Vec2&, const b2Color&) {}

void WorldRenderer::DrawTransform(const b2Transform&) {}

void WorldRenderer::DrawPoint(const b2Vec2&, float, const b2Color&) {}

void WorldRenderer::setViewport(const Rect& viewport) { m_viewport = viewport; }
