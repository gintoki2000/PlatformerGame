#ifndef HUD_H
#define HUD_H
#include "Layer.h"
#include "SpriteSheet.h"
class Level;
class HUD : public Layer
{
  public:
    static HUD* Create();
    void        Update(float deltaTime) override;
    void        Render() override;

  private:
    HUD();
    bool   Init();
    Sprite m_circle;
    Sprite m_mpBorder;
    Sprite m_hpBorder;

    SpriteSheet m_hp;
    SpriteSheet m_mp;

    Level* m_level;
};

#endif // HUD_H
