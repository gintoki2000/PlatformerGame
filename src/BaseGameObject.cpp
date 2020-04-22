#include "BaseGameObject.h"
#include "AssertManager.h"
#include "Game.h"

Game& BaseGameObject::gameMGR() { return *Game::getInstance(); }

SoundManager& BaseGameObject::soundMGR()
{
    return gameMGR().soundMGR();
}
TextureManager& BaseGameObject::textureMGR()
{
    return gameMGR().textureMGR();
}
