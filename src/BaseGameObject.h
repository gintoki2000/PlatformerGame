#ifndef BASE_GAME_OBJECT_H
#define BASE_GAME_OBJECT_H
#include "GameObject.h"
class Game;
class TextureManager;
class SoundManager;
class BaseGameObject : public GameObject
{
  protected:
    Game&           gameMGR();
    SoundManager&   soundMGR();
    TextureManager& textureMGR();
};
#endif // BASE_H
