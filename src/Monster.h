#ifndef MONSTER_H
#define MONSTER_H
#include "Animator.h"
#include "GameObject.h"
#include "box2d/box2d.h"
class Level;
class Monster : public GameObject
{
  public:
    Monster(Level* level, int monsterType, int hitPoints);
	~Monster();
    virtual void getHit(int damage);
    bool         isDead() const { return m_hitPoints == 0; }

    /// setters && getters
    int getMonsterType() const { return m_monsterType; }
    int getHitPoints() const { return m_hitPoints; }

	void tick(float deltaTime) override;
	void paint() override;

	void queryBoundingBox(NTRect& boundingBox) const;

  private:
    int m_monsterType;

  protected:
    virtual void updateGraphics(float deltaTime);
    virtual void updatePhysics();
    virtual void updateLogic(float deltaTime) = 0;

    void  synchronizeBodyTransform();
    void  synchronizeAnimatorTransform();
    void  onPositionChanged() override;
    void  setHorizontalSpeed(float speed);
    void  stopHorizontalMovement();
    float getDistanceToPlayer();
    int   getFacingPlayerDirection();
	void  changeDirectionToPlayer();

    int       m_hitPoints;
    int       m_direction;
    b2Body*   m_body;
    Animator* m_animator;
    int       m_width;
    int       m_height;
};
#endif // MONSTER_H
