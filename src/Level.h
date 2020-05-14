#ifndef LEVEL_H
#define LEVEL_H
#include "CameraShaker.h"
#include "SDL_mixer.h"
#include "Scene.h"
#include "WorldRenderer.h"
#include "box2d/box2d.h"
#include "tmxlite/ObjectGroup.hpp"
#include "tmxlite/TileLayer.hpp"
class Adventurer;
class TileLayer;
class HUD;
class Tilesets;
class Background;
class ObjectLayer;
class ParticleSystem;
class PauseMenu;
class Level : public Scene, public b2ContactListener
{
  public:
    ~Level();

    static Level* loadFromFile(const char* filename);

    /// getters
    Adventurer*     getAdventurer() const;
    Tilesets*       getTilesets() const;
    ObjectLayer*    getParticleLayer() const;
    ObjectLayer*    getSpriteLayer() const;
    CameraShaker*   getCameraShaker();
    ParticleSystem* getParticleSystem() const;

    // stuff
    void setIsPaused(bool paused);
    void displayPauseMenu();
    void hidePauseMenu();

    // Scene interface
  public:
    void update(float deltaTime) override;
    void render() override;
    void start() override;

    // b2ContactListener interface
  public:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
    void PostSolve(b2Contact*              contact,
                   const b2ContactImpulse* impulse) override;

  private:
    Level();
    bool init(const char* filename);

    /// data fields
    bool            m_isPaused;
    Adventurer*     m_adventurer;
    Tilesets*       m_tilesets;
    WorldRenderer*  m_worldRenderer;
    bool            m_drawDebugData;
    ObjectLayer*    m_spriteLayer;
    ObjectLayer*    m_particleLayer;
    Mix_Music*      m_music;
    HUD*            m_hud;
    CameraShaker*   m_cameraShaker;
    ParticleSystem* m_particleSystem;
    PauseMenu*      m_pauseMenu;

    friend class LevelParser;
};
#endif // LEVEL_H
