#include "WorldRenderer.h"
#include "SDL2_gfxPrimitives.h"
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
    /*
circleRGBA(m_renderer, p.x * m_ppm - m_viewport.x,
   p.y * m_ppm - m_viewport.y, r * m_ppm, c.r * 255, c.g * 255,
   c.b * 255, c.a * 255);
               */
}

void WorldRenderer::DrawSolidCircle(const b2Vec2& p, float r, const b2Vec2&,
                                    const b2Color& c)
{
    /*
circleRGBA(m_renderer, p.x * m_ppm - m_viewport.x,
   p.y * m_ppm - m_viewport.y, r * m_ppm, c.r * 255, c.g * 255,
   c.b * 255, c.a * 255);
               */
}

void WorldRenderer::DrawSegment(const b2Vec2&, const b2Vec2&, const b2Color&) {}

void WorldRenderer::DrawTransform(const b2Transform&) {}

void WorldRenderer::DrawPoint(const b2Vec2&, float, const b2Color&) {}

void WorldRenderer::SetViewport(const Rect& viewport) { m_viewport = viewport; }
