#ifndef HUD_H
#define HUD_H
#include "SpriteSheet.h"
class Level;
class HUD
{
  public:
    static HUD* create();
    void        tick(Level& level, float deltaTime);
    void        paint(Level& level);

  private:
    HUD();
    bool   init();
    Sprite m_circle;
    Sprite m_mpBorder;
    Sprite m_hpBorder;
    
	SpriteSheet m_hp;
	SpriteSheet m_mp;
};

#endif // HUD_H
