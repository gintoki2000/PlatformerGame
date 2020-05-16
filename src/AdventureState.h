#ifndef ADVENTURE_STATE_H
#define ADVENTURE_STATE_H

class Adventurer;
class AdventurerState
{
  public:
    virtual ~AdventurerState();
    virtual void             Enter(Adventurer& adventurer);
    virtual AdventurerState* HandleInput(Adventurer& adventurer);
    virtual AdventurerState* Tick(Adventurer& adventurer, float deltaTime);
    virtual void             Exit(Adventurer& adventurer);
};

class AdventurerOnGroundState : public AdventurerState
{
  public:
    AdventurerState* HandleInput(Adventurer& adventurer) override;
};

class AdventurerIdle1State : public AdventurerOnGroundState
{
    void Enter(Adventurer& adventurer) override;
};

class AdventurerIdle2State : public AdventurerOnGroundState
{
    void             Enter(Adventurer& adventurer) override;
    AdventurerState* HandleInput(Adventurer& adventurer) override;
	AdventurerState* Tick(Adventurer& adventurer, float deltaTime) override;

    float m_timer;
};

class AdventurerRunState : public AdventurerState
{
    void             Enter(Adventurer& adventurer) override;
    AdventurerState* HandleInput(Adventurer& adventurer) override;
};

class AdventurerJumpState : public AdventurerState
{
    void             Enter(Adventurer& adventurer) override;
    AdventurerState* HandleInput(Adventurer& adventurer) override;
};

class AdventurerSomersaultState : public AdventurerState
{
    void             Enter(Adventurer& adventurer) override;
    AdventurerState* HandleInput(Adventurer& adventurer) override;
    void             Exit(Adventurer& adventurer) override;
};

class AdventurerFallState : public AdventurerState
{
    void             Enter(Adventurer& adventurer) override;
    AdventurerState* HandleInput(Adventurer& adventurer) override;
};

class AdventurerHurtState : public AdventurerState
{
    void             Enter(Adventurer& adventurer) override;
    AdventurerState* HandleInput(Adventurer& adventurer) override;
    void             Exit(Adventurer& adventurer) override;
};

class AdventurerDieState : public AdventurerState
{
    void             Enter(Adventurer& adventurer) override;
    AdventurerState* HandleInput(Adventurer& adventurer) override;
    void             Exit(Adventurer& adventurer) override;
};

class AdventurerCrouchState : public AdventurerState
{
    void             Enter(Adventurer& adventurer) override;
    AdventurerState* HandleInput(Adventurer& adventurer) override;
};

class AdventurerAirJumpState : public AdventurerState
{
    void             Enter(Adventurer& adventurer) override;
    AdventurerState* HandleInput(Adventurer& adventurer) override;
};
#endif //ADVENTURE_STATE_H
