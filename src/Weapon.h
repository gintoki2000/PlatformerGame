#ifndef WEAPON_H
#define WEAPON_H
class Player;
class Weapon
{
  public:
    virtual ~Weapon();

    virtual bool tick(float deltaTime) = 0;
	virtual void start() = 0;
	virtual void cancel() = 0;

  protected:
    Weapon();

    Player* m_player;

	friend class Player;
};
#endif // WEAPON_H
