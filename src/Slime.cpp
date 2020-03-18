#include "Slime.h"
#include "Animator.h"
#include "Constances.h"
#include "Enums.h"
#include "Level.h"
#include "Locator.h"
#include "Monster.h"
#include "NTRect.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SpriteSheet.h"
Slime::Slime(Level* level) :
    Monster(MONSTER_TYPE_SLIME, 10),
    m_body(nullptr),
    m_spriteSheet(nullptr),
    m_animator(nullptr)
{
    int textureWidth, textureHeight;
    SDL_Texture* texture =
        level->getAssertManager()->getAssert<SDL_Texture>("asserts/slime.png");
    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);
    m_spriteSheet = new SpriteSheet(texture, textureWidth / SPRITE_WIDTH,
                                    textureHeight / SPRITE_HEIGHT);
}

void Slime::resetMembers()
{
    m_animator->play(ANIM_IDLE, 0.f);
    setPosition(0, 0);
    m_direction = DIRECTION_LEFT;
    m_state = STATE_IDLE;
    m_timer = 0.f;
}
void Slime::updateGraphics(float deltaTime)
{
    const NTRect& viewport = m_level->getViewport();
    NTRect boundingBox;
    boundingBox.x = m_positionX - WIDTH / 2.f;
    boundingBox.y = m_positionY - HEIGHT / 2.f;
    boundingBox.w = WIDTH;
    boundingBox.h = HEIGHT;
    if (SDL_HasIntersection(&viewport, &boundingBox))
    {
        setVisible(true);
        m_animator->tick(deltaTime);
        m_animator->setPositionX(getPositionX() - viewport.x);
        m_animator->setPositionY(getPositionY() - viewport.y);
        m_animator->setFlip(m_direction == DIRECTION_LEFT
                                ? SDL_FLIP_NONE
                                : SDL_FLIP_HORIZONTAL);
    }
    else
    {
        setVisible(false);
    }
}

void Slime::updatePhysics()
{
    const auto& position = m_body->GetPosition();
    m_positionX = position.x * Constances::PPM;
    m_positionY = position.y * Constances::PPM;
    m_rotation = (double)m_body->GetAngle();
}

void Slime::updateLogic(float deltaTime)
{
    m_timer += deltaTime;
    switch (m_state)
    {
    case STATE_IDLE:
    {
        if (getDistanceToPlayer() <= ACTIVATE_DISTANCE)
        {
            m_state = STATE_MOVE;
            m_timer = 0.f;
            m_animator->play(ANIM_MOVE, 0.f);
        }
    }
    break;
    case STATE_WAIT:
    {
        if (m_timer > 1.f)
        {
            m_state = STATE_IDLE;
            m_timer = 0.f;
        }
    }
    break;
    case STATE_MOVE:
    {
        if (getDistanceToPlayer() <= ATTACK_DISTANCE)
        {
            m_state = STATE_ATTACK;
            m_timer = 0.f;
            m_animator->play(ANIM_ATTACK, 0.f);
        }
        else
        {
            int sign = m_direction == DIRECTION_LEFT ? -1 : 1;
            setHorizontalSpeed(MOVE_SPEED * sign);
        }
    }
    break;
    case STATE_ATTACK:
    {
		attackPlayer();
    }
    break;
    case STATE_HURT:
    {
        if (m_animator->isCurrentAnimFinshed())
        {
            m_state = STATE_IDLE;
            m_timer = 0.f;
            m_animator->play(ANIM_IDLE, 0.f);
        }
    }
    break;
    case STATE_DIE:
    {
        if (m_animator->isCurrentAnimFinshed())
        {
            m_level->removeMonster(this);
        }
    }
    break;
    }
}

void Slime::onPositionChanged() { synchronizeBodyPosition(); }

void Slime::synchronizeBodyPosition()
{
    b2Vec2 pos(m_positionX / Constances::PPM, m_positionY / Constances::PPM);
    m_body->SetTransform(pos, (float)m_rotation);
}

void Slime::render(float deltaTime)
{
    updatePhysics();
    updateGraphics(deltaTime);
    updateLogic(deltaTime);
    if (isVisible())
    {
        m_animator->render(Locator::getRenderer());
    }
}

void Slime::getHit(int damage)
{
    if (m_state != STATE_HURT && m_state != STATE_DIE)
    {
        Slime::getHit(damage);
        if (m_hitPoints == 0)
        {
            m_state = STATE_DIE;
            m_animator->play(ANIM_DIE, 0.f);
        }
        else
        {
            m_state = STATE_HURT;
            m_animator->play(ANIM_HURT, 0.f);
        }
    }
}
