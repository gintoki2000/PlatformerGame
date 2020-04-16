#include "Tilesets.h"
#include "tmxlite/Tileset.hpp"

Tilesets::Tilesets(const std::vector<tmx::Tileset>& tilesets,
                   SDL_Renderer*                    renderer)
{
    m_numTilesets = tilesets.size();
    m_items       = new Tileset*[tilesets.size()];
    int i         = 0;
    for (const auto& tileset : tilesets)
    {
        m_items[i++] = new Tileset(tileset, renderer);
    }
}

Tilesets::~Tilesets()
{
    for (int i = 0; i < m_numTilesets; ++i)
    {
        delete m_items[i];
        m_items[i] = nullptr;
    }
    delete[] m_items;
}

Tile* Tilesets::getTile(int tileID) const
{
    for (int i = 0; i < m_numTilesets; ++i)
    {
        Tile* tile = nullptr;
        if ((tile = m_items[i]->getTile(tileID)) != nullptr)
        {
            return tile;
        }
    }
    return nullptr;
}

bool Tilesets::hasTile(int tileID) const
{
    for (int i = 0; i < m_numTilesets; ++i)
    {
        if (m_items[i]->hasTile(tileID))
        {
            return true;
        }
    }
    return false;
}
