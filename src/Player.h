#ifndef PLAYER_H
#define PLAYER_H
#include "Animation.h"
#include "Constances.h"
#include "Enums.h"
#include "GameObject.h"
#include "NTLayer.h"
#include "NTTextureRegion.h"
#include "box2d/box2d.h"
class Level;
class Player : public GameObject
{
  private:
    enum State
    {
        STATE_IDLE_1,
        STATE_IDLE_2,
        STATE_RUN,
        STATE_JUMP,
        STATE_SMRSLT,
        STATE_GRAB,
        STATE_CLIMB,
        STATE_HURT,
        STATE_FALL,
        STATE_ATTACK_1,
        STATE_ATTACK_2,
        STATE_ATTACK_3,
        STATE_AIR_ATTACK_1,
        STATE_AIR_ATTACK_2,
        STATE_AIR_ATTACK_3_LOOP,
        STATE_AIR_ATTACK_3_RDY,
        STATE_AIR_ATTACK_3_END,
        STATE_WALL_SLIDE,
        STATE_SWD_DRAW,
        STATE_SWD_WITH,
        NUM_STATES
    };

    struct AttackInfo
    {
        float dx;
        float dy;
        float width;
        float height;
        int damage;
    };
    struct AttackingAction : public b2QueryCallback
    {

        Player* player;
        Level* level;
        AttackInfo attackInfo;

		AttackingAction();

        AttackingAction(Player* _player, Level* _level,
                        const AttackInfo& _attackInfo);
        void perform();

      private:
        bool ReportFixture(b2Fixture* fixture);
    };

  public:
    static constexpr float WIDTH = 16.f;
    static constexpr float HEIGHT = 30.f;
    static constexpr int SPRITE_WIDTH = 50;
    static constexpr int SPRITE_HEIGHT = 37;
    static constexpr float JUMP_VEL = 13.f;
    static constexpr float MAX_HVEL = 10.f;
    static constexpr float RUN_ACC = 0.8f;
    static constexpr float WIDTH_IN_METER = WIDTH / Constances::PPM;
    static constexpr float HEIGHT_IN_METER = HEIGHT / Constances::PPM;
    enum FixtureType
    {
        FIXTURE_TYPE_MAIN_BODY,
        FIXTURE_TYPE_FOOT_SENSOR,
        FIXTURE_TYPE_WALL_SENSOR
    };

  public:
    static Player* create(Level* level);

    ~Player();

    void update(float dt);

    void draw(SDL_Renderer* renderer, const NTRect& viewPort);

    void touchGround();

    void untouchGround();

    void touchWall();

    void untouchWall();

    bool isOnGround() const;

    bool isTouchingWall() const;

    const b2Vec2& getPosition() const { return m_body->GetPosition(); }

  private:
    Player();

    bool initialize(Level* level);

    bool initialize(Level* level, const b2Vec2& position);

    void changeState(State newState);

    void createBody(const b2Vec2& position);

    void createAnimations();

    void jump();

    bool isCurrentAnimationComplete();

    void move(float vx);

    void stopHorizontalMovement();

    void stopVerticalMovement();

    void attackArea(float x, float y, float width, float height);

    bool m_isOnGround;

    float m_timer;

    int m_direction;

    State m_state;

    b2Body* m_body;

    b2Fixture* m_fixture;

    Level* m_level;

    Animation<NTTextureRegion> m_animations[NUM_STATES];

    SDL_Texture* m_texture;

    bool m_continueAttack;

    bool m_isWallSliding;

    int m_touchingGroundCount;

    int m_touchingWallCount;

	AttackingAction m_attackAction1;
	AttackingAction m_attackAction2;
	AttackingAction m_attackAction3;

};
#endif // PLAYER_H
