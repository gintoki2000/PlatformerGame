#ifndef SWORD_H
#define SWORD_H
#include "Weapon.h"
class Sword : public Weapon
{
  public:
    Sword();
    ~Sword();

    bool tick(float deltaTime) override;
    void start() override;
	void cancel() override;

  private:
    void checkCollision();
	bool m_chain;
    int  m_phrase;
	int  m_lastAnim;

    friend class Weapon;
};
#endif // SWORD_H
