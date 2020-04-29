#ifndef FIRECASTPARTICLE_H
#define FIRECASTPARTICLE_H

#include "Animation.h"
#include "GameObject.h"
#include "SpriteSheet.h"
#include "Vec.h"


class FireCastParticle : public GameObject
{
  public:
    FireCastParticle();
    void init(const Vec2& position);

    void tick(float deltaTime) override;
    void paint() override;
    void cleanup() override;


  private:
    Animation*  m_animation;
    SpriteSheet m_spriteSheet;
    float       m_timer;
    bool        m_inUse;
};

#endif // FIRECASTPARTICLE_H
