#ifndef PLAYER_SKILL_H
#define PLAYER_SKILL_H
#include "ListItem.h"
#include "SpriteSheet.h"
class Adventurer;
class Technique : public IListItem
{
  public:
    Technique(int MPCost, const Sprite& sprite);
    virtual ~Technique();

    virtual void enter(Adventurer& adventurer);
    virtual bool tick(Adventurer& adventurer, float deltaTime) = 0;
    virtual void exit(Adventurer& adventurer);

    int           getMPCost() const { return m_MPCost; }
    const Sprite* getIcon() const override { return &m_icon; }

  private:
    int    m_MPCost;
    Sprite m_icon;
};

#endif // PLAYER_SKILL_H
