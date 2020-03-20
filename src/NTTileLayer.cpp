#include "NTTileLayer.h"
#include "NTRect.h"
#include "NTTextureRegion.h"
#include "NTTileLayerCell.h"
#include "NTTileLayerTile.h"
#include "SDL_render.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
NTTileLayer::NTTileLayer(int width, int height, int tileWidth, int tileHeight)
{
    init(width, height, tileWidth, tileHeight);
}

NTTileLayer::NTTileLayer() :
    m_width(0), m_height(0), m_tileWidth(0), m_tileHeight(0), m_cells(nullptr)
{
}

NTTileLayer::~NTTileLayer()
{
    int count = m_width * m_height;
    for (int i = 0; i < count; ++i)
    {
        delete m_cells[i];
    }
    delete[] m_cells;
}

bool NTTileLayer::init(int width, int height, int tileWidth, int tileHeight)
{
    m_width = width;
    m_height = height;
    m_tileWidth = tileWidth;
    m_tileHeight = tileHeight;
    m_positionX = 0;
    m_positionY = 0;
    int count = m_width * m_height;
    m_cells = new NTTileLayerCell*[(unsigned)count];
    for (int i = 0; i < count; ++i)
    {
        m_cells[i] = nullptr;
    }

    return true;
}

void NTTileLayer::render(SDL_Renderer* renderer, const NTRect& viewport)
{
    if (!isVisible())
        return;
    int startX = viewport.left() / m_tileWidth - 1;
    int startY = viewport.top() / m_tileHeight - 1;
    int endX = viewport.right() / m_tileWidth + 1;
    int endY = viewport.bottom() / m_tileHeight + 1;

    startX = std::max(0, startX);
    startY = std::max(0, startY);
    endX = std::min(m_width - 1, endX);
    endY = std::min(m_height - 1, endY);

    NTRect dstrect;
    dstrect.x = startX * m_tileWidth + (int)getPositionX();
    dstrect.y = startY * m_tileHeight + (int)getPositionY();
    dstrect.w = m_tileWidth;
    dstrect.h = m_tileHeight;
    for (int y = startY; y <= endY; ++y)
    {
        for (int x = startX; x <= endX; ++x)
        {
            NTTileLayerCell* cell = m_cells[x + y * m_width];
            if (cell == nullptr)
            {
                continue;
            }
            NTTileLayerTile* tile = cell->getTile();
            if (tile == nullptr)
            {
                continue;
            }
            dstrect.x = x * m_tileWidth + getPositionX() - viewport.x;
            dstrect.y = y * m_tileHeight + getPositionY() - viewport.y;
            const NTTextureRegion& textureRegion = tile->getTextureRegion();
            textureRegion.draw(renderer, &dstrect);
        }
    }
}

NTTileLayerCell* NTTileLayer::getCellAt(int x, int y)
{
    if (x < 0 || x > m_width)
        return nullptr;
    if (y < 0 || y > m_height)
        return nullptr;
    return m_cells[x + y * m_width];
}

void NTTileLayer::setCellAt(int x, int y, NTTileLayerCell* cell)
{
    if (x < 0 || x > m_width)
        return;
    if (y < 0 || y > m_height)
        return;
    int index = x + y * m_width;
    delete m_cells[index];
    m_cells[index] = cell;
}
