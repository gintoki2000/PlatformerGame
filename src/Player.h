#ifndef PLAYER_H
#define PLAYER_H
#include "Animation.h"
#include "NTLayer.h"
#include "NTTextureRegion.h"
#include "box2d/box2d.h"
class Level;
class Player : public b2QueryCallback
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
        NUM_STATES
    };

    enum Direction
    {
        DIRECTION_NONE,
        DIRECTION_LEFT,
        DIRECTION_RIGHT
    };

  public:
    static constexpr float WIDTH = 20.f;
    static constexpr float HEIGHT = 37.f;
    static constexpr int SPRITE_WIDTH = 50;
    static constexpr int SPRITE_HEIGHT = 37;

  public:
    static Player* create(Level* level);

    ~Player();

    void update(float dt);

    void draw(SDL_Renderer* renderer, const NTRect& viewPort);

  private:
    Player();

    bool initialize(Level* level);

    bool initialize(Level* level, const b2Vec2& position);

    void changeState(State newState);

    void createBody(const b2Vec2& position);

    void createAnimations();

    void checkOnGround();

	void checkWallSliding();

    bool ReportFixture(b2Fixture* f);

    void jump();

    bool isCurrentAnimationComplete();

    void move(float vx);

    void setZeroVelX();

    void updateKeyState(float dt);

    bool isButtonAPressed() { return m_currentKeyState.buttonA; }
    bool isButtonBPressed() { return m_currentKeyState.buttonB; }
    bool isButtonUpPressed() { return m_currentKeyState.buttonUp; }
    bool isButtonDownPressed() { return m_currentKeyState.buttonDown; }
    bool isButtonLeftPressed() { return m_currentKeyState.buttonLeft; }
    bool isButtonRightPressed() { return m_currentKeyState.buttonRight; }
    bool isButtonAJustPressed()
    {
        return m_currentKeyState.buttonA && !m_prevKeyState.buttonA;
    }
    bool isButtonBJustPressed()
    {
        return m_currentKeyState.buttonB && !m_prevKeyState.buttonB;
    }
    bool isButtonUpJustPressed()
    {
        return m_currentKeyState.buttonUp && !m_prevKeyState.buttonUp;
    }

    bool isButtonDownJustPressed()
    {
        return m_currentKeyState.buttonDown && !m_prevKeyState.buttonDown;
    }

    bool isButtonLeftJustPressed()
    {
        return m_currentKeyState.buttonLeft && !m_prevKeyState.buttonLeft;
    }

    bool isButtonRightJustPressed()
    {
        return m_currentKeyState.buttonRight && !m_prevKeyState.buttonRight;
    }

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

    struct KeyState
    {
        bool buttonA = false;
        bool buttonB = false;
        bool buttonUp = false;
        bool buttonDown = false;
        bool buttonLeft = false;
        bool buttonRight = false;
    } m_prevKeyState, m_currentKeyState;

    enum
    {
        CHECK_GROUND,
        CHECK_SLIDE
    } m_checkingStatus;

	bool m_isWallSliding;
};
#endif // PLAYER_H
