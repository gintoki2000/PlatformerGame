#ifndef LEVEL_H
#define LEVEL_H
#include "AssertManager.h"
#include "Rect.h"
#include "SDL_mixer.h"
#include "WorldRenderer.h"
#include "box2d/box2d.h"
#include <vector>
class Monster;
class Player;
class ObjectList;
class TiledMap;
class Fireball;
class HUD;
class Tilesets;
class Level : public b2ContactListener
{
  public:
    static Level* create(const std::string& filename);
    ~Level();

    /// getters
    const Rect&     getViewport() const;
    b2World*        getWorld() const;
    Player*         getPlayer() const;
    Tilesets*       getTilesets() const;
    TextureManager* getTextureManager() const;

    // stuff
    void render(float deltaTime);
    void addMonster(Monster* monster);
    void removeMonster(Monster* monster);
    void addFireball(Fireball* fireball);
    void removeFireball(Fireball* fireball);
    void setPaused(bool paused);

  private:
    Level();
    bool init(const std::string& filename);
    void tick(float deltaTime);
    void updateViewport(float deltaTime);

    // Box2d callback functions
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;

    static const int MAX_SIZE = 20;
	static const int NUM_BACKGROUNDS = 5;
    /// data fields
    bool             m_isPaused;
    Player*          m_player;

    Tilesets*        m_tileSets;
    b2World*         m_world;
    TiledMap*        m_tiledMap;
    Rect             m_viewport;
    WorldRenderer*   m_worldRenderer;
    TextureManager*  m_textureManager;
    float            m_viewportX;
    Mix_Music*       m_music;

    HUD*             m_HUD;
    SDL_Texture*     m_backgrounds[NUM_BACKGROUNDS];
    /*
    Monster*         m_monstersToBeRemoved[MAX_SIZE];
    int              m_monstersToBeRemovedCount;
    Fireball*        m_fireballsToBeRemoved[MAX_SIZE];
    int              m_fireballsToBeRemovedCount;
    */
};
#endif // LEVEL_H
