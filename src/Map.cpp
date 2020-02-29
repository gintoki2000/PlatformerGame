#include "Map.h"
#include "TileLayer.h"
#include "Game.h"
#include "tmxlite/Map.hpp"
#include "tmxlite/TileLayer.hpp"
#include "NTTiledMapAnimatedTile.h"
#include <algorithm>
Map::Map():
    m_frontLayer(nullptr), m_mainLayer(nullptr), m_backgroundLayer(nullptr)
{
		NTTiledMapAnimatedTile::initBaseTime();
}

Map::~Map()
{
    delete m_frontLayer;
    delete m_mainLayer;
    delete m_backgroundLayer;
}

bool Map::load(const std::string& filename)
{
    tmx::Map tmxMap;
    if(!tmxMap.load(filename))
    {
        return false;
    }

	const int width = static_cast<int>(tmxMap.getTileCount().x);
	const int height = static_cast<int>(tmxMap.getTileCount().y);
	const int tileWidth = static_cast<int>(tmxMap.getTileSize().x);
	const int tileHeight = static_cast<int>(tmxMap.getTileSize().y);

    const std::vector<tmx::Layer::Ptr>& layers = tmxMap.getLayers();
    tmx::TileLayer* tmxMainLayer = (tmx::TileLayer*) std::find_if(std::begin(layers), std::end(layers), [=](const tmx::Layer::Ptr& layer)->bool{
        return layer->getName() == "main-layer";
    })->get();
	m_mainLayer = new TileLayer(width, height, tileWidth, tileHeight, tmxMainLayer->getTiles(), nullptr);
	tmx::TileLayer* tmxFrontLayer = (tmx::TileLayer*) std::find_if(std::begin(layers), std::end(layers), [=](const tmx::Layer::Ptr& layer)->bool{
        return layer->getName() == "main-layer";
	})->get();
	m_frontLayer = new TileLayer(width, height, tileWidth, tileHeight, tmxFrontLayer->getTiles(), nullptr);	
	tmx::TileLayer* tmxBackgroundLayer = (tmx::TileLayer*) std::find_if(std::begin(layers), std::end(layers), [=](const tmx::Layer::Ptr& layer)->bool{
        return layer->getName() == "main-layer";
	})->get();
	m_backgroundLayer = new TileLayer(width, height, tileWidth, tileHeight, tmxBackgroundLayer->getTiles(), nullptr);	


	m_viewPort.w = Game::WIDTH;
   	m_viewPort.h = Game::HEIGHT;	
    return true;
}

void Map::tick(float dt)
{
	NTTiledMapAnimatedTile::updateBaseTime();
}

void Map::paint(SDL_Renderer *renderer)
{
	m_backgroundLayer->draw(renderer, m_viewPort);
	m_mainLayer->draw(renderer, m_viewPort);
	m_frontLayer->draw(renderer, m_viewPort);
}
