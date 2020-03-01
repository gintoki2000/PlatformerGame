#include "Map.h"
#include "Game.h"
#include "NTTileLayerAnimatedTile.h"
#include "SDL_keyboard.h"
#include "SDL_render.h"
#include "TileLayer.h"
#include "TileSet.h"
#include "tmxlite/Map.hpp"
#include "tmxlite/TileLayer.hpp"
#include <algorithm>
Map::Map() :
    m_frontLayer(nullptr), m_mainLayer(nullptr), m_backgroundLayer(nullptr)
{
    NTTileLayerAnimatedTile::initBaseTime();
}

Map::~Map()
{
    delete m_frontLayer;
    delete m_mainLayer;
    delete m_backgroundLayer;
    for (auto tileSet : m_tileSets)
        delete tileSet;
}

bool Map::load(const std::string& filename)
{
    tmx::Map tmxMap;
    if (!tmxMap.load(filename))
    {
        return false;
    }

    for (const auto& tmxTileSet : tmxMap.getTilesets())
    {
        m_tileSets.push_back(
            new TileSet(tmxTileSet, Game::getInstance()->getRenderer()));
    }

    const int width = static_cast<int>(tmxMap.getTileCount().x);
    const int height = static_cast<int>(tmxMap.getTileCount().y);
    const int tileWidth = static_cast<int>(tmxMap.getTileSize().x);
    const int tileHeight = static_cast<int>(tmxMap.getTileSize().y);

    const std::vector<tmx::Layer::Ptr>& layers = tmxMap.getLayers();
    tmx::TileLayer* tmxMainLayer =
        (tmx::TileLayer*)std::find_if(
            std::begin(layers), std::end(layers),
            [=](const tmx::Layer::Ptr& layer) -> bool {
                return layer->getName() == "main-layer";
            })
            ->get();
    m_mainLayer = new TileLayer(width, height, tileWidth, tileHeight,
                                *tmxMainLayer, m_tileSets);
    tmx::TileLayer* tmxFrontLayer =
        (tmx::TileLayer*)std::find_if(
            std::begin(layers), std::end(layers),
            [=](const tmx::Layer::Ptr& layer) -> bool {
                return layer->getName() == "front-layer";
            })
            ->get();
    m_frontLayer = new TileLayer(width, height, tileWidth, tileHeight,
                                 *tmxFrontLayer, m_tileSets);
    tmx::TileLayer* tmxBackgroundLayer =
        (tmx::TileLayer*)std::find_if(
            std::begin(layers), std::end(layers),
            [=](const tmx::Layer::Ptr& layer) -> bool {
                return layer->getName() == "background-layer";
            })
            ->get();
    m_backgroundLayer =
        new TileLayer(width, height, tileWidth, tileHeight,
                      *tmxBackgroundLayer, m_tileSets);

    m_viewPort.w = Game::WIDTH;
    m_viewPort.h = Game::HEIGHT;

    m_viewPort.y = 0;
    return true;
}

void Map::tick(float dt)
{
    NTTileLayerAnimatedTile::updateBaseTime();
    const Uint8* keyBroadState = SDL_GetKeyboardState(nullptr);
    if (keyBroadState[SDL_SCANCODE_UP])
    {
        m_viewPort.y -= 16;
    }
    if (keyBroadState[SDL_SCANCODE_DOWN])
    {
        m_viewPort.y += 16;
    }
	if (keyBroadState[SDL_SCANCODE_LEFT])
	{
		m_viewPort.x -= 16;
	}

	if (keyBroadState[SDL_SCANCODE_RIGHT])
	{
		m_viewPort.x += 16;
	}
}
void Map::paint()
{
    SDL_Renderer* renderer = Game::getInstance()->getRenderer();
    SDL_RenderClear(renderer);
    m_backgroundLayer->draw(renderer, m_viewPort);
    m_mainLayer->draw(renderer, m_viewPort);
    m_frontLayer->draw(renderer, m_viewPort);
}
