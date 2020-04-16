#ifndef PLAYER_ANIMATOR_CONTROLLER_H
#define PLAYER_ANIMATOR_CONTROLLER_H
#include "Animator.h"
class PlayerAnimatorController
{
  public:
    enum StateA
    {
        STATE_A_IDLE_1,
        STATE_A_IDLE_2,
        STATE_A_RUN,
        STATE_A_JUMP,
        STATE_A_SOMERSAULT,
        STATE_A_FALL,
		STATE_A_SLIDE
    };
    enum StateB
    {
        STATE_B_ATK,
        STATE_B_CAST_SPELL,
        STATE_B_CAST_LOOP
    };
    PlayerAnimatorController(Animator* animator);
    ~PlayerAnimatorController();

    void   setStateA(StateA newStateA);
    void   setStateB(StateB newStateB);
    StateA getStateA() const;
    StateB getStateB() const;

  private:
    Animator* m_animator;
    int       m_animation;
    StateA    m_stateA;
    StateB    m_stateB;
};
#endif // PLAYER_ANIMATOR_CONTROLLER_H
