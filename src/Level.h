#ifndef LEVEL_H
#define LEVEL_H
#include "CameraShaker.h"
#include "LayerManager.h"
#include "SDL_mixer.h"
#include "WorldRenderer.h"
#include "box2d/box2d.h"
#include "tmxlite/ObjectGroup.hpp"
#include "tmxlite/TileLayer.hpp"
class Player;
class TileLayer;
class HUD;
class Tilesets;
class Background;
class ObjectLayer;
class ParticleSystem;
class Level : public LayerManager, public b2ContactListener
{
  public:
    ~Level();

    static Level* loadFromFile(const char* filename);

    /// getters
    Player*       getPlayer() const;
    Tilesets*     getTilesets() const;
    ObjectLayer*  getParticleLayer() const;
    ObjectLayer*  getSpriteLayer() const;
    CameraShaker* getCameraShaker();
	ParticleSystem* getParticleSystem() const;

    // stuff
    void setIsPaused(bool paused);
    void start();

    // LayerManager interface
  public:
    virtual void update(float deltaTime) override;
    virtual void render() override;

    // b2ContactListener interface
  public:
    virtual void BeginContact(b2Contact* contact) override;
    virtual void EndContact(b2Contact* contact) override;
    virtual void PreSolve(b2Contact*        contact,
                          const b2Manifold* oldManifold) override;
    virtual void PostSolve(b2Contact*              contact,
                           const b2ContactImpulse* impulse) override;

  private:
    Level();
    bool init(const char* filename);

    /// data fields
    bool            m_isPaused;
    Player*         m_player;
    Tilesets*       m_tilesets;
    WorldRenderer*  m_worldRenderer;
    bool            m_drawDebugData;
    ObjectLayer*    m_spriteLayer;
    ObjectLayer*    m_particleLayer;
    Mix_Music*      m_music;
    HUD*            m_hud;
    CameraShaker*   m_cameraShaker;
    ParticleSystem* m_particleSystem;

    friend class LevelParser;
};
#endif // LEVEL_H
