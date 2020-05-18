#include "AdventureState.h"
#include "Adventurer.h"
#include "Animator.h"
#include "Input.h"
#include "Layer.h"
#include "Utils.h"
#include "box2d/box2d.h"

AdventurerState::~AdventurerState() {}

void AdventurerState::Enter(Adventurer&) {}

void AdventurerState::Exit(Adventurer&) {}

AdventurerState* AdventurerState::HandleInput(Adventurer&) { return nullptr; }

AdventurerState* AdventurerState::Tick(Adventurer&, float) { return nullptr; }

AdventurerState* AdventurerOnGroundState::HandleInput(Adventurer& adventurer)
{

    int inputDirection = Input::GetHorizontalInputDirection();
    if (adventurer.m_jumpPressedRemember > 0.f)
    {
        return &Adventurer::jumpState;
    }
    if (Input::IsPressed(BUTTON_PAD_DOWN))
    {
        return &Adventurer::crouchState;
    }
    else if (inputDirection != 0)
    {
        return &Adventurer::runState;
    }
    return nullptr;
}

AdventurerState* AdventurerOnGroundState::Tick(Adventurer& adventurer, float)
{
    if (!adventurer.IsGrounded())
    {
        return &Adventurer::fallState;
    }
    return nullptr;
}

void AdventurerIdle1State::Enter(Adventurer& adventurer)
{
    adventurer.m_horizontalAcceleration = 0.f;
    adventurer.GetAnimator()->Play(Adventurer::ANIM_IDLE_1, 0.f);
	adventurer.GetBody()->SetAwake(false);
}

void AdventurerIdle1State::Exit(Adventurer& adventurer)
{
	adventurer.GetBody()->SetAwake(true);
}

void AdventurerIdle2State::Enter(Adventurer& adventurer)
{
    m_timer                             = 0.f;
    adventurer.m_horizontalAcceleration = 0;
    adventurer.GetAnimator()->Play(Adventurer::ANIM_IDLE_2, 0.f);
}

AdventurerState* AdventurerIdle2State::HandleInput(Adventurer& adventurer)
{
    return AdventurerOnGroundState::HandleInput(adventurer);
}

AdventurerState* AdventurerIdle2State::Tick(Adventurer&, float deltaTime)
{
    m_timer += deltaTime;
    return m_timer > 3.f ? &Adventurer::idle1State : nullptr;
}

void AdventurerRunState::Enter(Adventurer& adventurer)
{
    adventurer.GetAnimator()->Play(Adventurer::ANIM_RUN, 0.f);
}

AdventurerState* AdventurerRunState::HandleInput(Adventurer& adventurer)
{
    int inputDirection = Input::GetHorizontalInputDirection();
    if (inputDirection < 0)
    {
        adventurer.SetDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        adventurer.SetDirection(DIRECTION_RIGHT);
    }
    if (adventurer.m_jumpPressedRemember > 0.f)
    {
        return &Adventurer::jumpState;
    }
    else if (inputDirection == 0)
    {
        return &Adventurer::idle1State;
    }
    else
    {
        adventurer.m_horizontalAcceleration =
            adventurer.m_runAcceleration * inputDirection;
    }
    return nullptr;
}

AdventurerState* AdventurerRunState::Tick(Adventurer& adventurer, float)
{
    if (!adventurer.IsGrounded())
    {
        return &Adventurer::fallState;
    }
    return nullptr;
}

void AdventurerJumpState::Enter(Adventurer& adventurer)
{
    b2Vec2 vel = adventurer.GetBody()->GetLinearVelocity();
    vel.y      = -Adventurer::JUMP_VEL;
    adventurer.GetBody()->SetLinearVelocity(vel);
    adventurer.GetAnimator()->Play(Adventurer::ANIM_JUMP, 0.f);
    adventurer.SetUnGrounded();
    adventurer.m_jumpPressedRemember = 0.f;
    adventurer.m_groundedRemember    = 0.f;
}

AdventurerState* AdventurerJumpState::HandleInput(Adventurer& adventurer)
{
    int inputDirection = Input::GetHorizontalInputDirection();
    if (inputDirection < 0)
    {
        adventurer.SetDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        adventurer.SetDirection(DIRECTION_RIGHT);
    }
    if (Input::IsReleased(BUTTON_B))
    {
        b2Vec2 vel = adventurer.GetBody()->GetLinearVelocity();
        if (vel.y < 0.f)
        {
            vel.y *= adventurer.m_cutJumpHeight;
            adventurer.GetBody()->SetLinearVelocity(vel);
        }
    }
    if (Input::IsJustPressed(BUTTON_B) && adventurer.m_extraJumps > 0)
    {
        --adventurer.m_extraJumps;
        return &Adventurer::airJumpState;
    }
    else if (adventurer.GetBody()->GetLinearVelocity().y > 0.f)
    {
        return &Adventurer::fallState;
    }
    else if (inputDirection != 0)
    {
        adventurer.m_horizontalAcceleration =
            adventurer.m_runAcceleration * inputDirection;
    }
    else
    {
        adventurer.m_horizontalAcceleration = 0.f;
    }
    return nullptr;
}

AdventurerState* AdventurerJumpState::Tick(Adventurer& adventurer, float)
{
    if (adventurer.IsGrounded())
    {
        return &Adventurer::idle1State;
    }
    return nullptr;
}

void AdventurerSomersaultState::Enter(Adventurer& adventurer)
{
    adventurer.GetAnimator()->Play(Adventurer::ANIM_SOMERSULT, 0.f);
    adventurer.m_ableToUseTechnique = false;
}

AdventurerState* AdventurerSomersaultState::HandleInput(Adventurer& adventurer)
{

    int inputDirection = Input::GetHorizontalInputDirection();
    if (inputDirection < 0)
    {
        adventurer.m_direction = DIRECTION_LEFT;
    }
    if (inputDirection > 0)
    {
        adventurer.m_direction = DIRECTION_RIGHT;
    }
    if (adventurer.IsGrounded())
    {
        return &Adventurer::idle1State;
    }
    else if (adventurer.GetAnimator()->IsCurrentAnimationFinshed())
    {
        return &Adventurer::fallState;
    }

    if (Input::IsReleased(BUTTON_B))
    {
        b2Vec2 vel = adventurer.GetBody()->GetLinearVelocity();
        if (vel.y < 0.f)
        {
            vel.y *= adventurer.m_cutJumpHeight;
            adventurer.GetBody()->SetLinearVelocity(vel);
        }
    }
    if (inputDirection != 0)
    {
        adventurer.m_horizontalAcceleration =
            adventurer.m_runAcceleration * inputDirection;
    }
    else
    {
        adventurer.m_horizontalAcceleration = 0.f;
    }
    return nullptr;
}

void AdventurerSomersaultState::Exit(Adventurer& adventurer)
{
    adventurer.m_ableToUseTechnique = true;
}

void AdventurerFallState::Enter(Adventurer& adventurer)
{
    adventurer.GetAnimator()->Play(Adventurer::ANIM_FALL, 0.f);
}

AdventurerState* AdventurerFallState::HandleInput(Adventurer& adventurer)
{
    int inputDirection = Input::GetHorizontalInputDirection();
    if (inputDirection < 0)
    {
        adventurer.SetDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        adventurer.SetDirection(DIRECTION_RIGHT);
    }
    if (adventurer.m_jumpPressedRemember > 0.f &&
        adventurer.m_groundedRemember > 0.f)
    {
        return &Adventurer::jumpState;
    }
    else if (adventurer.IsGrounded())
    {
        if (inputDirection == 0)
        {
            return &Adventurer::idle1State;
        }
        else
        {
            return &Adventurer::runState;
        }
    }
    else if (Input::IsJustPressed(BUTTON_B) && adventurer.m_extraJumps > 0)
    {
        --adventurer.m_extraJumps;
        return &Adventurer::airJumpState;
    }
    else if (inputDirection != 0)
    {
        adventurer.m_horizontalAcceleration =
            adventurer.m_runAcceleration * inputDirection;
    }
    else
    {
        adventurer.m_horizontalAcceleration = 0.f;
    }
    return nullptr;
}
AdventurerState* AdventurerFallState::Tick(Adventurer& adventurer, float)
{
    if (adventurer.IsGrounded())
    {
        return &Adventurer::idle1State;
    }
    return nullptr;
}

void AdventurerHurtState::Enter(Adventurer& adventurer)
{
    adventurer.m_horizontalAcceleration = 0.f;
    adventurer.GetAnimator()->Play(Adventurer::ANIM_HURT, 0.f);
    adventurer.m_ableToUseTechnique = false;
    adventurer.m_vulnerable         = false;
    adventurer.GetBody()->SetLinearVelocity(b2Vec2_zero);
}

AdventurerState* AdventurerHurtState::HandleInput(Adventurer& adventurer)
{
    if (adventurer.GetAnimator()->IsCurrentAnimationFinshed() &&
        adventurer.IsGrounded())
    {
        return &Adventurer::idle1State;
    }
    return nullptr;
}

void AdventurerHurtState::Exit(Adventurer& adventurer)
{
    adventurer.m_ableToUseTechnique = true;
    adventurer.m_vulnerable         = true;
}

void AdventurerDieState::Enter(Adventurer& adventurer)
{
    adventurer.GetAnimator()->Play(Adventurer::ANIM_DIE);
    adventurer.m_ableToUseTechnique     = false;
    adventurer.m_vulnerable             = false;
    adventurer.m_horizontalAcceleration = 0.f;
    adventurer.GetBody()->SetLinearVelocity(b2Vec2_zero);
}

AdventurerState* AdventurerDieState::HandleInput(Adventurer& adventurer)
{
    if (adventurer.GetAnimator()->IsCurrentAnimationFinshed())
    {
        adventurer.ResetMembers();
    }
    return nullptr;
}

void AdventurerDieState::Exit(Adventurer& adventurer)
{
    adventurer.m_ableToUseTechnique = true;
    adventurer.m_vulnerable         = true;
}

void AdventurerCrouchState::Enter(Adventurer& adventurer)
{
    adventurer.GetAnimator()->Play(Adventurer::ANIM_CROUCH);
}

AdventurerState* AdventurerCrouchState::HandleInput(Adventurer& adventurer)
{
    if (Input::IsReleased(BUTTON_PAD_DOWN))
    {
        return &Adventurer::idle1State;
    }
    int inputDirection = Input::GetHorizontalInputDirection();
    if (inputDirection < 0)
    {
        adventurer.SetDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        adventurer.SetDirection(DIRECTION_RIGHT);
    }
    return nullptr;
}

void AdventurerAirJumpState::Enter(Adventurer& adventurer)
{
    adventurer.GetAnimator()->Play(Adventurer::ANIM_JUMP);
    b2Vec2 vel = adventurer.GetBody()->GetLinearVelocity();
    vel.y      = -Adventurer::JUMP_VEL;
    adventurer.GetBody()->SetLinearVelocity(vel);
}

AdventurerState* AdventurerAirJumpState::HandleInput(Adventurer& adventurer)
{
    int inputDirection = Input::GetHorizontalInputDirection();
    if (inputDirection < 0)
    {
        adventurer.SetDirection(DIRECTION_LEFT);
    }
    if (inputDirection > 0)
    {
        adventurer.SetDirection(DIRECTION_RIGHT);
    }
    if (adventurer.GetAnimator()->IsCurrentAnimationFinshed())
    {
        return &Adventurer::somersaultState;
    }
    if (Input::IsReleased(BUTTON_B))
    {
        b2Vec2 vel = adventurer.GetBody()->GetLinearVelocity();
        if (vel.y < 0.f)
        {
            vel.y *= adventurer.m_cutJumpHeight;
            adventurer.GetBody()->SetLinearVelocity(vel);
        }
    }

    if (inputDirection != 0)
    {
        adventurer.m_horizontalAcceleration =
            adventurer.m_runAcceleration * inputDirection;
    }
    else
    {
        adventurer.m_horizontalAcceleration = 0.f;
    }
    return nullptr;
}
