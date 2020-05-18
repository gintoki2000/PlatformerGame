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
class EquipmentMenu;
class Level : public Scene, public b2ContactListener
{
  public:
    ~Level();

    static Level* LoadFromFile(const char* filename);

    /// getters
    Adventurer*     GetAdventurer() const;
    Tilesets*       GetTilesets() const;
    ObjectLayer*    GetParticleLayer() const;
    ObjectLayer*    GetSpriteLayer() const;
    CameraShaker*   GetCameraShaker();
    ParticleSystem* GetParticleSystem() const;

    // stuff
    void SetIsPaused(bool paused);
    void DisplayEquipmentMenu();
    void HideEquipmentMenu();

    // Scene interface
  public:
    void Tick(float deltaTime) override;
    void Paint() override;
    void Start() override;

    // b2ContactListener interface
  public:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
    void PostSolve(b2Contact*              contact,
                   const b2ContactImpulse* impulse) override;

  private:
    Level();
    bool Init(const char* filename);
    void UpdateCamera(float deltaTime);
    enum State
    {
        STATE_EQUIPMENT,
        STATE_PLAYING
    };

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
    EquipmentMenu*  m_pauseMenu;
    State           m_state;
};
#endif // LEVEL_H
