#ifndef FIREBALL_H
#define FIREBALL_H
#include "Animation.h"
#include "GameObject.h"
#include "Level.h"
#include "SpriteSheet.h"
class Fireball : public GameObject, public b2QueryCallback
{
  public:
    Fireball(Level* level, const b2Vec2& initialPosition, const b2Vec2& speed);
    ~Fireball();

    void tick(float deltaTime) override;
    void paint() override;

  private:
	void checkCollision();
	bool ReportFixture(b2Fixture *fixture) override;
    SpriteSheet* m_spriteSheet;
    Animation*   m_animation;
    b2Body*      m_body;
    SDL_Point    m_origin;
};
#endif // FIREBALL_H
