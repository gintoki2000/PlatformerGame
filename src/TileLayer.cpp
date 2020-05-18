#include "TileLayer.h"
#include "Game.h"
#include "Level.h"
#include "Math.h"
#include "Rect.h"
#include "SDL_render.h"
#include "TextureRegion.h"
#include "TileLayerCell.h"
#include "TileLayerTile.h"
TileLayer::TileLayer() : m_cells(nullptr) {}

TileLayer::~TileLayer()
{
    if (m_cells != nullptr)
    {
        int count = m_width * m_height;
        for (int i = 0; i < count; ++i)
        {
            delete m_cells[i];
        }
        delete[] m_cells;
        m_cells = nullptr;
    }
}

TileLayer* TileLayer::Create(int width, int height, int tileWidth,
                             int tileHeight)
{
    TileLayer* ret = new TileLayer;
    if (ret->Init(width, height, tileWidth, tileHeight))
    {
        return ret;
    }
    delete ret;
    return nullptr;
}

bool TileLayer::Init(int width, int height, int tileWidth, int tileHeight)
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
	return true;
}

void TileLayer::Tick(float) {}

void TileLayer::Paint()
{
    const Rect&   viewport = GetScene()->GetCamera().GetViewport();
    SDL_Renderer* renderer = GAME->GetRenderer();
    int           startX   = viewport.Left() / m_tileWidth - 1;
    int           startY   = viewport.Top() / m_tileHeight - 1;
    int           endX     = viewport.Right() / m_tileWidth + 1;
    int           endY     = viewport.Bottom() / m_tileHeight + 1;

    startX = Math::Max(0, startX);
    startY = Math::Max(0, startY);
    endX   = Math::Min(m_width - 1, endX);
    endY   = Math::Min(m_height - 1, endY);

    Rect dstrect;
    dstrect.x = startX * m_tileWidth + GetPositionX();
    dstrect.y = startY * m_tileHeight + GetPositionY();
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
            TileLayerTile* tile = cell->GetTile();
            if (tile == nullptr)
            {
                continue;
            }
            dstrect.x = x * m_tileWidth + GetPositionX() - viewport.x;
            dstrect.y = y * m_tileHeight + GetPositionY() - viewport.y;
            const TextureRegion& textureRegion = tile->GetTextureRegion();
            textureRegion.Draw(renderer, &dstrect);
        }
    }
}

TileLayerCell* TileLayer::GetCellAt(int x, int y)
{
    if (x < 0 || x > m_width)
        return nullptr;
    if (y < 0 || y > m_height)
        return nullptr;
    return m_cells[x + y * m_width];
}

void TileLayer::SetCellAt(int x, int y, TileLayerCell* cell)
{
    if (x < 0 || x > m_width)
        return;
    if (y < 0 || y > m_height)
        return;
    int index = x + y * m_width;
    delete m_cells[index];
    m_cells[index] = cell;
}
