#include "Kobold.h"
/*
Koblod::Koblod(Level* level) : Monster(level, MONSTER_TYPE_KOBLOD, 20)
{
    m_width  = WIDTH;
    m_height = HEIGHT;
    SDL_Texture* texture =
        level->GetTextureManager()->Get("asserts/spritesheets/kobold.png");
    m_spriteSheet = new SpriteSheet(texture, SPRITE_WIDTH, SPRITE_HEIGHT);

    Animation* anims[NUM_ANIMS];

    anims[ANIM_IDLE]   = new Animation(m_spriteSheet, 0, 8, 1.f / 8.f);
    anims[ANIM_RUN]    = new Animation(m_spriteSheet, 8, 12, 1.f / 10.f);
    anims[ANIM_ATTACK] = new Animation(m_spriteSheet, 20, 5, 1.f / 10.f);
    anims[ANIM_HURT]   = new Animation(m_spriteSheet, 25, 2, 1.f / 8.f);
    anims[ANIM_DIE]    = new Animation(m_spriteSheet, 27, 8, 1.f / 8.f);

    anims[ANIM_IDLE]->SetPlayMode(Animation::PLAY_MODE_LOOP);
    anims[ANIM_RUN]->SetPlayMode(Animation::PLAY_MODE_LOOP);

    m_animator = new Animator(anims, NUM_ANIMS);
    m_animator->SetOriginX(SPRITE_WIDTH / 2);
    m_animator->SetOriginY(SPRITE_WIDTH / 2);

    b2BodyDef bdef;
    bdef.fixedRotation = true;
    bdef.userData      = this;
    bdef.type          = b2_dynamicBody;

    m_body = m_level->GetWorld()->CreateBody(&bdef);

    b2PolygonShape box;
    box.SetAsBox(WIDTH / 2.f / Constances::PPM, HEIGHT / 2.f / Constances::PPM);

    b2FixtureDef fdef;
    fdef.filter.categoryBits = CATEGORY_BIT_MONSTER;
    fdef.filter.maskBits =
        CATEGORY_BIT_BLOCK | CATEGORY_BIT_PLAYER | CATEGORY_BIT_SPELL;
    fdef.shape = &box;
    m_body->CreateFixture(&fdef);
    ResetMembers();
}

Koblod::~Koblod()
{
    delete m_spriteSheet;
    m_spriteSheet = nullptr;
}

void Koblod::ResetMembers()
{
    m_direction = DIRECTION_RIGHT;
    Idle();
    SetPosition(0.f, 0.f);
}

class KoblodAttackCallback : public b2QueryCallback
{
    bool ReportFixture(b2Fixture* fixture) override
    {
        auto userData = fixture->GetBody()->GetUserData();
        if (userData != nullptr &&
            ((GameObject*)userData)->GetGameObjectType() ==
                GAME_OBJECT_TYPE_PLAYER)
        {
            auto adventurer = (Adventurer*)userData;
            adventurer->GetHit(2);
        }
        return true;
    }
};
void Koblod::UpdateLogic(float deltaTime)
{
    m_timer += deltaTime;
    m_direction = (Direction)GetFacingAdventurerDirection();
    switch (m_state)
    {
    case STATE_IDLE:
    {
        if (GetDistanceToAdventurer() <= ACTIVATE_RUN_DIS)
        {
            Run();
        }
    }
    break;
    case STATE_RUN:
    {
        auto distanceToAdventurer = GetDistanceToAdventurer();
        if (distanceToAdventurer > ACTIVATE_RUN_DIS)
        {
            Idle();
        }
        else if (distanceToAdventurer <= ACTIVATE_ATK_DIS)
        {
            WaitToAttack();
        }
        else
        {
            int sign = m_direction == DIRECTION_LEFT ? -1 : 1;
            SetHorizontalSpeed(sign * 3.f);
        }
    }
    break;
    case STATE_ATTACK:
    {
        if (m_animator->IsCurrentAnimationFinshed())
        {
            b2AABB               area;
            KoblodAttackCallback callback;
            if (m_direction == DIRECTION_LEFT)
            {
                area.lowerBound.x = m_body->GetPosition().x - 2.f;
                area.lowerBound.y =
                    m_body->GetPosition().y - 4.f / Constances::PPM;
                area.upperBound.x = m_body->GetPosition().x;
                area.upperBound.y =
                    m_body->GetPosition().y + 4.f / Constances::PPM;
            }
            else
            {
                area.lowerBound.x = m_body->GetPosition().x;
                area.lowerBound.y =
                    m_body->GetPosition().y - 4.f / Constances::PPM;
                area.upperBound.x = m_body->GetPosition().x + 2.f;
                area.upperBound.y =
                    m_body->GetPosition().y + 4.f / Constances::PPM;
            }
            m_level->GetWorld()->QueryAABB(&callback, area);
            Wait();
        }
    }
    break;
    case STATE_WAIT:
    {
        if (m_animator->GetElapsedTime() > 2.f)
        {
            m_state = STATE_IDLE;
        }
    }
    break;
    case STATE_WAIT_TO_ATTACK:
    {
        if (m_timer > 3.f)
        {
            Attack();
        }
    }
    break;
    case STATE_HURT:
    {
        if (m_animator->IsCurrentAnimationFinshed())
        {
            Wait();
        }
    }
    break;
    case STATE_DIE:
    {
        if (m_animator->IsCurrentAnimationFinshed())
        {
            m_level->RemoveMonster(this);
        }
    }
    break;
    }
}

void Koblod::Idle()
{
    m_state = STATE_IDLE;
    m_animator->Play(ANIM_IDLE, 0.f);
    StopHorizontalMovement();
}

void Koblod::Run()
{
    m_state = STATE_RUN;
    m_animator->Play(ANIM_RUN, 0.f);
}

void Koblod::Attack()
{
    m_state = STATE_ATTACK;
    m_animator->Play(ANIM_ATTACK, 0.f);
}

void Koblod::Wait()
{
    m_state = STATE_WAIT;
    m_animator->Play(ANIM_IDLE, 0.f);
}

void Koblod::Hurt()
{
    m_state = STATE_HURT;
    m_animator->Play(ANIM_HURT, 0.f);
    StopHorizontalMovement();
}

void Koblod::Die()
{
    m_state = STATE_DIE;
    m_animator->Play(ANIM_DIE, 0.f);
    StopHorizontalMovement();
}

void Koblod::WaitToAttack()
{
    m_state = STATE_WAIT_TO_ATTACK;
    m_animator->Play(ANIM_IDLE, 0.f);
    StopHorizontalMovement();
}

void Koblod::GetHit(int damage)
{
    if (m_state != STATE_DIE && m_state != STATE_HURT)
    {
        Monster::GetHit(damage);
        if (m_hitPoints == 0)
        {
            Die();
        }
        else
        {
            Hurt();
        }
    }
}
*/
