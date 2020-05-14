#include "PauseMenu.h"
#include "Constances.h"
#include "Game.h"
#include "Input.h"
#include "Level.h"
#include "SDL_render.h"
#include "Scene.h"
#include "ListMenu.h"
PauseMenu::PauseMenu() {}

PauseMenu* PauseMenu::create()
{
    PauseMenu* ret = new PauseMenu;
    if (ret->init())
    {
        return ret;
    }
    delete ret;
    return nullptr;
}

bool PauseMenu::init()
{
    deactivate();
    return true;
}

void PauseMenu::update(float) {
	if (Input::isJustPressed(BUTTON_B))
	{
		Level* level = static_cast<Level*>(getScene());
		level->hidePauseMenu();
	}
}
void PauseMenu::render()
{
	
}

