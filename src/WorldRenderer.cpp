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
    for (int i = 0; i < vertexCount - 1; ++i)
    {
        SDL_RenderDrawLine(m_renderer, vertices[i].x * m_ppm,
                           vertices[i].y * m_ppm, vertices[i + 1].x * m_ppm,
                           vertices[i + 1].y * m_ppm);
    }

    SDL_RenderDrawLine(m_renderer, vertices[vertexCount - 1].x * m_ppm,
                       vertices[vertexCount - 1].y * m_ppm,
                       vertices[0].x * m_ppm, vertices[0].y * m_ppm);
}

void WorldRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount,
                                     const b2Color& color)
{

    SDL_SetRenderDrawColor(m_renderer, color.r * 0xff, color.g * 0xff,
                           color.b * 0xff, color.a * 0xff);
    for (int i = 0; i < vertexCount - 1; ++i)
    {
        SDL_RenderDrawLine(m_renderer, vertices[i].x * m_ppm,
                           vertices[i].y * m_ppm, vertices[i + 1].x * m_ppm,
                           vertices[i + 1].y * m_ppm);
    }

    SDL_RenderDrawLine(m_renderer, vertices[vertexCount - 1].x * m_ppm,
                       vertices[vertexCount - 1].y * m_ppm,
                       vertices[0].x * m_ppm, vertices[0].y * m_ppm);
}

void WorldRenderer::DrawCircle(const b2Vec2&, float, const b2Color&) {}

void WorldRenderer::DrawSolidCircle(const b2Vec2&, float, const b2Vec2&,
                                    const b2Color&)
{
}

void WorldRenderer::DrawSegment(const b2Vec2&, const b2Vec2&, const b2Color&) {}

void WorldRenderer::DrawTransform(const b2Transform&) {}

void WorldRenderer::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
}
