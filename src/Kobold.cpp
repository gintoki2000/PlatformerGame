#include "Kobold.h"
#include "Animation.h"
#include "Animator.h"
#include "Constances.h"
#include "Enums.h"
#include "Level.h"
#include "Monster.h"
#include "SpriteSheet.h"

Koblod::Koblod(Level* level) : Monster(level, MONSTER_TYPE_KOBLOD, 10)
{
    m_width  = WIDTH;
    m_height = HEIGHT;
    SDL_Texture* texture =
        level->getTextureManager()->get("asserts/kobold.png");
    m_spriteSheet = new SpriteSheet(texture, SPRITE_WIDTH, SPRITE_HEIGHT);

    Animation* anims[NUM_ANIMS];

    anims[ANIM_IDLE]   = new Animation(m_spriteSheet, 0, 8, 1.f);
    anims[ANIM_RUN]    = new Animation(m_spriteSheet, 8, 12, 1.f);
    anims[ANIM_ATTACK] = new Animation(m_spriteSheet, 20, 4, 1.f);
    anims[ANIM_HURT]   = new Animation(m_spriteSheet, 24, 3, 1.f);
    anims[ANIM_DIE]    = new Animation(m_spriteSheet, 27, 8, 1.f);

    m_animator = new Animator(anims, NUM_ANIMS);

    b2BodyDef bdef;
    bdef.fixedRotation = true;
    bdef.userData      = this;

    m_body = m_level->getWorld()->CreateBody(&bdef);

    b2PolygonShape box;
    box.SetAsBox(WIDTH / 2.f / Constances::PPM, HEIGHT / 2.f / Constances::PPM);

    b2FixtureDef fdef;
    fdef.filter.categoryBits = CATEGORY_BIT_MONSTER;
    fdef.filter.maskBits     = CATEGORY_BIT_BLOCK | CATEGORY_BIT_PLAYER;
    fdef.shape               = &box;
    m_body->CreateFixture(&fdef);
}

Koblod::~Koblod()
{
    delete m_spriteSheet;
    m_spriteSheet = nullptr;
}

void Koblod::updateLogic(float deltaTime)
{
    m_timer += deltaTime;
    switch (m_state)
    {
    case STATE_IDLE:
    {
		if (getDistanceToPlayer() < 100.f)
		{

		}
    }
    break;
    case STATE_RUN:
    {
    }
    break;
    case STATE_ATTACK:
    {
    }
    break;
    case STATE_WAIT:
    {
    }
    break;
    case STATE_HURT:
    {
    }
    break;
    case STATE_DIE:
    {
    }
    break;
    }
}
