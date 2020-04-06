#include "Monster.h"
#include "Constances.h"
#include "Enums.h"
#include "GameObject.h"
#include "Level.h"
#include "Locator.h"
#include "Player.h"
#include "SDL_log.h"
#include <cmath>

Monster::Monster(Level* level, int monsterType, int hitPoints) :
    GameObject(GAME_OBJECT_TYPE_MONSTER, level), m_monsterType(monsterType),
    m_hitPoints(hitPoints), m_direction(DIRECTION_LEFT), m_body(nullptr),
    m_animator(nullptr), m_width(Constances::TILE_SIZE),
    m_height(Constances::TILE_SIZE)
{
}

Monster::~Monster()
{
    delete m_animator;
    m_body->GetWorld()->DestroyBody(m_body);
}
void Monster::getHit(int damage)
{
    if (!isDead())
    {
        m_hitPoints -= damage;
        if (m_hitPoints < 0)
        {
            m_hitPoints = 0;
        }
    }
}

float Monster::getDistanceToPlayer()
{
    float playerX = m_level->getPlayer()->getPositionX();
    float playerY = m_level->getPlayer()->getPositionY();
    float a       = playerX - m_positionX;
    float b       = playerY - m_positionY;
    return std::sqrt(a * a + b * b);
}

int Monster::getFacingPlayerDirection()
{
    float playerX = m_level->getPlayer()->getPositionX();
    if (playerX < m_positionX)
    {
        return DIRECTION_LEFT;
    }
    if (playerX > m_positionY)
    {
        return DIRECTION_RIGHT;
    }
    return DIRECTION_NONE;
}

void Monster::queryBoundingBox(Rect& boundingBox) const
{
    boundingBox.x = m_positionX - m_width / 2.f;
    boundingBox.y = m_positionY - m_height / 2.f;
    boundingBox.w = m_width;
    boundingBox.h = m_height;
}

void Monster::updateGraphics(float deltaTime)
{
    const Rect& viewport = m_level->getViewport();
    Rect        boundingBox;
    queryBoundingBox(boundingBox);
    if (SDL_HasIntersection(&viewport, &boundingBox))
    {
        setVisible(true);
        synchronizeAnimatorTransform();
        m_animator->tick(deltaTime);
        auto flip =
            m_direction == DIRECTION_LEFT ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        m_animator->setFlip(flip);
    }
    else
    {
        setVisible(false);
    }
}

void Monster::updatePhysics()
{
    const auto& position = m_body->GetPosition();
    m_positionX          = position.x * Constances::PPM;
    m_positionY          = position.y * Constances::PPM;
    m_rotation           = (double)m_body->GetAngle();
}

void Monster::onPositionChanged()
{
    synchronizeBodyTransform();
    synchronizeAnimatorTransform();
}

void Monster::synchronizeBodyTransform()
{
    b2Vec2 pos(m_positionX / Constances::PPM, m_positionY / Constances::PPM);
    m_body->SetTransform(pos, (float)m_rotation);
}

void Monster::synchronizeAnimatorTransform()
{
    const auto& viewport = m_level->getViewport();
    m_animator->setPositionX(m_positionX - viewport.x);
    m_animator->setPositionY(m_positionY - viewport.y);
    m_animator->setRotation(m_rotation);
}

void Monster::setHorizontalSpeed(float speed)
{
    b2Vec2 vel = m_body->GetLinearVelocity();
    vel.x      = speed;
    m_body->SetLinearVelocity(vel);
}

void Monster::changeDirectionToPlayer()
{
    m_direction = getFacingPlayerDirection();
}

void Monster::stopHorizontalMovement() { setHorizontalSpeed(0.f); }

void Monster::tick(float deltaTime)
{
    updatePhysics();
    updateLogic(deltaTime);
    updateGraphics(deltaTime);
}

void Monster::paint()
{
    if (m_isVisible)
        m_animator->paint(Locator::getRenderer());
}
