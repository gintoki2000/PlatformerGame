#ifndef LEVEL_H
#define LEVEL_H
#include "NTRect.h"
#include "TileSet.h"
#include "WorldRenderer.h"
#include "box2d/box2d.h"
#include <vector>
class Monster;
class Player;
class ObjectList;
class TiledMap;
class Level : public b2ContactListener
{
  public:
	static Level* create(const std::string& filename);
    ~Level();

    /// getters
    const NTRect&                getViewport() const;
    b2World*                     getWorld() const;
    Player*                      getPlayer() const;
    const std::vector<TileSet*>& getTilesets() const;

    // stuff
    void render(float deltaTime);
    void addMonster(Monster* monster);
    void removeMonster(Monster* monster);
    void setPaused(bool paused);

  private:
    Level();
    bool init(const std::string& filename);
	void tick(float deltaTime);
	void updateViewport(float deltaTime);

	void BeginContact(b2Contact *contact) override;
	void EndContact(b2Contact *contact) override;

    /// data fields
    bool                  m_isPaused;
    Player*               m_player;
    ObjectList*           m_monsters;
    std::vector<TileSet*> m_tileSets;
    b2World*              m_world;
    TiledMap*             m_tiledMap;
    NTRect                m_viewport;
    WorldRenderer*        m_worldRenderer;

    static const int MAX_SIZE = 20;
    Monster*         m_monstersToBeRemoved[MAX_SIZE];
    int              m_monstersToBeRemovedCount;
};
#endif // LEVEL_H
