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

	bool isActive() const { return m_isActive; }

  protected:
    Weapon();

    Player* m_player;
	bool m_isActive;

	friend class Player;
};
#endif // WEAPON_H
