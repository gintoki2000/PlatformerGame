#include "TileLayer.h"
#include "Game.h"
#include "Level.h"
#include "Math.h"
#include "Rect.h"
#include "SDL_render.h"
#include "TextureRegion.h"
#include "TileLayerCell.h"
#include "TileLayerTile.h"
TileLayer::TileLayer(int width, int height, int tileWidth, int tileHeight)
{
    m_width      = width;
    m_height     = height;
    m_tileWidth  = tileWidth;
    m_tileHeight = tileHeight;
    int count    = m_width * m_height;
    m_cells      = new TileLayerCell*[(unsigned)count];
    for (int i = 0; i < count; ++i)
    {
        m_cells[i] = nullptr;
    }
}

TileLayer::~TileLayer()
{
    int count = m_width * m_height;
    for (int i = 0; i < count; ++i)
    {
        delete m_cells[i];
    }
    delete[] m_cells;
}

void TileLayer::update(float) {}

void TileLayer::render()
{
    const Rect&   viewport = getManager()->getCamera().getViewport();
    SDL_Renderer* renderer = Game::getInstance()->renderer();
    int           startX   = viewport.left() / m_tileWidth - 1;
    int           startY   = viewport.top() / m_tileHeight - 1;
    int           endX     = viewport.right() / m_tileWidth + 1;
    int           endY     = viewport.bottom() / m_tileHeight + 1;

    startX = Math::max(0, startX);
    startY = Math::max(0, startY);
    endX   = Math::min(m_width - 1, endX);
    endY   = Math::min(m_height - 1, endY);

    Rect dstrect;
    dstrect.x = startX * m_tileWidth + getPositionX();
    dstrect.y = startY * m_tileHeight + getPositionY();
    dstrect.w = m_tileWidth;
    dstrect.h = m_tileHeight;
    for (int y = startY; y <= endY; ++y)
    {
        for (int x = startX; x <= endX; ++x)
        {
            TileLayerCell* cell = m_cells[x + y * m_width];
            if (cell == nullptr)
            {
                continue;
            }
            TileLayerTile* tile = cell->getTile();
            if (tile == nullptr)
            {
                continue;
            }
            dstrect.x = x * m_tileWidth + getPositionX() - viewport.x;
            dstrect.y = y * m_tileHeight + getPositionY() - viewport.y;
            const TextureRegion& textureRegion = tile->getTextureRegion();
            textureRegion.draw(renderer, &dstrect);
        }
    }
}

TileLayerCell* TileLayer::getCellAt(int x, int y)
{
    if (x < 0 || x > m_width)
        return nullptr;
    if (y < 0 || y > m_height)
        return nullptr;
    return m_cells[x + y * m_width];
}

void TileLayer::setCellAt(int x, int y, TileLayerCell* cell)
{
    if (x < 0 || x > m_width)
        return;
    if (y < 0 || y > m_height)
        return;
    int index = x + y * m_width;
    delete m_cells[index];
    m_cells[index] = cell;
}
