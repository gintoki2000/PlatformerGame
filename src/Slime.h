#ifndef SLIME_H
#define SLIME_H
#include "Animation.h"
#include "Enums.h"
#include "GameObject.h"
#include "GameObjectPool.h"
#include "Monster.h"
#include "NTRect.h"
#include "SpriteSheet.h"
#include "box2d/box2d.h"
class Slime;
class Level;

class Slime : public Monster
{
  private:
    enum State
    {
        STATE_IDLE,
        STATE_MOVE,
        STATE_ATTACK,
        STATE_HURT,
        STATE_DIE,
        NUM_STATES
    };

  public:
    struct Asserts
    {
        Asserts();
        ~Asserts();
        bool load();
        SDL_Texture* texture;
        SpriteSheet* spriteSheet;
        static constexpr int SPRITE_WIDTH = 32;
        static constexpr int SPRITE_HEIGHT = 25;
    };
    static constexpr int WIDTH = 32;
    static constexpr int HEIGHT = 17;
    using Pool = GameObjectPool<Slime, Asserts>;
    Slime* initialize(Level* level, const b2Vec2& position);
    void update(float dt) override;
    void draw(SDL_Renderer* renderer, const NTRect& viewPort) override;
    void cleanup() override;

  private:
    /// private methods
    Slime();
    ~Slime();
    bool create(Pool* pool, const Asserts& asserts);
    void createBody(b2World* world, const b2Vec2& position);
	float getDistanceToPlayer();
	Direction getFacingPlayerDirection();
	void setHorizontalVel(float v);
	void getDamage(int damage) override; 

    /// private fields
    friend class GameObjectPool<Slime, Asserts>;
    Pool* m_pool;
    Animation<int> m_animations[NUM_STATES];
    const Asserts* m_asserts;

    float m_timer;
    State m_state;
    b2Body* m_body;
    Level* m_level;
    Direction m_direction;
	float m_directionTimer;
	int m_hitPoints;
};
#endif // SLIME_H
