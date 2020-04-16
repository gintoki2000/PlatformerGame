#include "LevelParser.h"
#include "AssertManager.h"
#include "Background.h"
#include "Cell.h"
#include "Constances.h"
#include "Enums.h"
#include "Layer.h"
#include "LayerManager.h"
#include "Level.h"
#include "Locator.h"
#include "Player.h"
#include "SDL_render.h"
#include "TileLayer.h"
#include "Tilesets.h"
#include "box2d/b2_world.h"
#include "tmxlite/ImageLayer.hpp"
#include "tmxlite/Layer.hpp"
#include "tmxlite/Map.hpp"
#include "tmxlite/TileLayer.hpp"

LevelParser::LevelParser() {}

Level* LevelParser::load(const char* filename)
{
    tmx::Map levelData;
    if (!levelData.load(filename))
    {
        SDL_Log("%s: Failed to load map \'%s\'", __PRETTY_FUNCTION__, filename);
        return nullptr;
    }

    Level* level = new Level;
    if (parseLevel(*level, levelData))
    {
        return level;
    }
    delete level;
    return nullptr;
}

bool LevelParser::parseLevel(Level& level, const tmx::Map& levelData)
{

    std::string textures[] = {
        "asserts/spritesheets/player.png",
        "asserts/spritesheets/slime.png",
        "asserts/spritesheets/kobold.png",
        "asserts/spritesheets/fire-ball.png",
    };
    TextureManager& tmgr     = Locator::getTextureManager();
    tmgr.unloadAll();
    for (auto& t : textures)
    {
        if (!tmgr.load(t))
        {
            SDL_Log("Failed to load texture \'%s\'", t.c_str());
            return false;
        }
    }

	b2World* currentWorld = Locator::getWorld();
	if (currentWorld != nullptr) 
	{
		delete currentWorld;
		Locator::setWorld(nullptr);
	}

	Locator::setWorld(new b2World(Constances::GRAVITY));

    Locator::getWorld()->SetDebugDraw(level.m_worldRenderer);
    Locator::getWorld()->SetContactListener(&level);


    level.m_tileSets = new Tilesets(levelData.getTilesets(), Locator::getRenderer());

    level.m_rows       = levelData.getTileCount().y;
    level.m_cols       = levelData.getTileCount().x;
    level.m_tileWidth  = levelData.getTileSize().x;
    level.m_tileHeight = levelData.getTileSize().y;
    Vec2 vs(Constances::GAME_WIDTH, Constances::GAME_HEIGHT);
    Vec2 vc;
    vc.x = vs.x / 2;
    vc.y = level.m_rows * level.m_tileHeight - vs.y / 2.f;
    level.getCamera().setSize(vs);
    level.getCamera().setCenter(vc);
    for (const auto& l : levelData.getLayers())
    {
        tmx::Layer& layerData = (tmx::Layer&)*l.get();
        if (layerData.getType() == tmx::Layer::Type::Tile)
        {
            TileLayer* tileLayer =
                new TileLayer(level.m_cols, level.m_rows, level.m_tileWidth,
                              level.m_tileHeight);
            parseTileLayer(*tileLayer, *level.m_tileSets,
                           (tmx::TileLayer&)layerData);
            level.addLayer(tileLayer);
        }
        else if (layerData.getType() == tmx::Layer::Type::Image)
        {
            Background* background = new Background();
            background->parse((tmx::ImageLayer&)layerData);
            level.addLayer(background);
        }
    }
    level.m_player = new Player();
    level.m_player->setLayerManager(&level);
    return true;
}

void LevelParser::parseTileLayer(TileLayer& tileLayer, const Tilesets& tilesets,
                                 const tmx::TileLayer& tileLayerData)
{
    int         tileLayerWidth  = tileLayer.getWidth();
    int         tileLayerHeight = tileLayer.getHeight();
    int         tileWidth       = tileLayer.getTileWidth();
    int         tileHeight      = tileLayer.getTileHeight();
    const auto& dataOfTiles     = tileLayerData.getTiles();
    for (int x = 0; x < tileLayerWidth; ++x)
    {
        for (int y = 0; y < tileLayerHeight; ++y)
        {

            const auto& tileData =
                dataOfTiles[(std::size_t)(x + y * tileLayerWidth)];
            Tile* tile = tilesets.getTile((int)tileData.ID);
            if (tile != nullptr)
            {
                Cell* cell = new Cell();
                Vec2  center;
                center.x = x * tileWidth;
                center.y = y * tileHeight;
                cell->init(tile, center);
                cell->setTile(tile);
                tileLayer.setCellAt(x, y, cell);
            }
        }
    }
}
