#include "NTTiledMap.h"
#include "NTRect.h"
#include "NTTextureRegion.h"
#include "NTTiledMapCell.h"
#include "NTTiledMapTile.h"
#include "SDL_render.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
NTTiledMap::NTTiledMap(int width, int height, int tileWidth, int tileHeight)
{
    init(width, height, tileWidth, tileHeight);
}

NTTiledMap::NTTiledMap() :
    m_width(0), m_height(0), m_tileWidth(0), m_tileHeight(0), m_positionX(0),
    m_positionY(0), m_cells(nullptr)
{
}

NTTiledMap::~NTTiledMap()
{
    int count = m_width * m_height;
    for (int i = 0; i < count; ++i)
    {
        delete m_cells[i];
    }
    delete[] m_cells;
}

bool NTTiledMap::init(int width, int height, int tileWidth, int tileHeight)
{
    m_width = width;
    m_height = height;
    m_tileWidth = tileWidth;
    m_tileHeight = tileHeight;
    int count = m_width * m_height;
    m_cells = new NTTiledMapCell*[count];
    for (int i = 0; i < count; ++i)
    {
        m_cells[i] = nullptr;
    }
    m_positionX = 0;
    m_positionY = 0;

    return true;
}

void NTTiledMap::draw(SDL_Renderer* renderer, const NTRect& viewport)
{
    int startX = viewport.left() / m_tileWidth - 1;
    int startY = viewport.top() / m_tileHeight - 1;
    int endX = viewport.right() / m_tileWidth + 1;
    int endY = viewport.bottom() / m_tileHeight + 1;

    startX = std::max(0, startX);
    startY = std::max(0, startY);
    endX = std::min(m_width - 1, endX);
    endY = std::min(m_height - 1, endY);

    NTRect dstrect;
    dstrect.x = startX * m_tileWidth + m_positionX;
    dstrect.y = startY * m_tileHeight + m_positionY;
    dstrect.w = m_tileWidth;
    dstrect.h = m_tileHeight;
    for (int y = startY; y <= endY; ++y)
    {
        for (int x = startX; x <= endX; ++x)
        {
            NTTiledMapCell* cell = m_cells[x + y * m_width];
            if (cell == nullptr)
            {
                continue;
            }
            NTTiledMapTile* tile = cell->getTile();
            if (tile == nullptr)
            {
                continue;
            }
			dstrect.x = x * m_tileWidth + m_positionX - viewport.x;
			dstrect.y = y * m_tileHeight + m_positionY - viewport.y;
            const NTTextureRegion& textureRegion = tile->getTextureRegion();
            textureRegion.draw(renderer, &dstrect);
        }
    }
}

NTTiledMapCell* NTTiledMap::getCellAt(int x, int y)
{
    if (x < 0 || x > m_width)
        return nullptr;
    if (y < 0 || y > m_height)
        return nullptr;
    return m_cells[x + y * m_width];
}

void NTTiledMap::setCellAt(int x, int y, NTTiledMapCell* cell)
{
    if (x < 0 || x > m_width)
        return;
    if (y < 0 || y > m_height)
        return;
    int index = x + y * m_width;
    delete m_cells[index];
    m_cells[index] = cell;
}
