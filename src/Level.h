#ifndef LEVEL_H
#define LEVEL_H
#include "WorldRenderer.h"
#include "box2d/box2d.h"
#include "LayerManager.h"
class Player;
class TiledMap;
class HUD;
class Tilesets;
class Background;
class ObjectLayer;
class Level : public LayerManager,  public b2ContactListener
{
  public:
    ~Level();

    /// getters
    Player*     getPlayer() const;
    Tilesets*   getTilesets() const;

    // stuff
    void setIsPaused(bool paused);

    // LayerManager interface
public:
    virtual void update(float deltaTime) override;
    virtual void render() override;

    // b2ContactListener interface
public:
    virtual void BeginContact(b2Contact *contact) override;
    virtual void EndContact(b2Contact *contact) override;
    virtual void PreSolve(b2Contact *contact, const b2Manifold *oldManifold) override;
    virtual void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) override;

  private:
    Level();

    /// data fields
    bool           m_isPaused;
    Player*        m_player;
    Tilesets*      m_tileSets;
    Rect           m_viewport;
    WorldRenderer* m_worldRenderer;
    float          m_viewportX;

    ObjectLayer* m_enemyLayer;
    ObjectLayer* m_itemLayer;


    int m_rows;
    int m_cols;
    int m_tileWidth;
    int m_tileHeight;

    friend class LevelParser;

};
#endif // LEVEL_H
