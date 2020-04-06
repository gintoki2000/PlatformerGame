#ifndef FIREBALL_H
#define FIREBALL_H
#include "Animation.h"
#include "GameObject.h"
#include "Level.h"
#include "SpriteSheet.h"
class Fireball : public GameObject
{
  public:
    Fireball(Level* level, const b2Vec2& initialPosition, const b2Vec2& speed);
    ~Fireball();

    void tick(float deltaTime) override;
    void paint() override;


  private:
    SpriteSheet* m_spriteSheet;
    Animation*   m_animation;
    b2Body*      m_body;
    SDL_Point    m_origin;
    float        m_animElapsedTime;

    static constexpr int SPRITE_WIDTH  = 64;
    static constexpr int SPRITE_HEIGHT = 64;
};
#endif // FIREBALL_H
