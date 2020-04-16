#ifndef LEVEL_PARSER_H
#define LEVEL_PARSER_H
#include "AssertManager.h"
#include "Background.h"
#include "TileLayer.h"
#include "Tilesets.h"
#include "box2d/b2_world.h"
#include "tmxlite/Layer.hpp"
#include "tmxlite/Map.hpp"
#include "tmxlite/TileLayer.hpp"
class Level;
class Layer;
class LevelParser
{
  public:
    LevelParser();

    Level* load(const char* filename);

  private:
    bool parseLevel(Level& level, const tmx::Map& levelData);
    void parseTileLayer(TileLayer& tileLayer, const Tilesets& tilesets,
                          const tmx::TileLayer& tileLayerData);
};
#endif // LEVEL_PARSER_H
