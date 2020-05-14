#include "Input.h"
#include "SDL_scancode.h"
#include "SDL_stdinc.h"

bool Input::pButtonState[NUM_BUTTONS] = {};
bool Input::cButtonState[NUM_BUTTONS] = {};
int Input::mappingToScancode[NUM_BUTTONS] = {};

void Input::init()
{
    mappingToScancode[BUTTON_A]         = SDL_SCANCODE_X;
    mappingToScancode[BUTTON_B]         = SDL_SCANCODE_Z;
    mappingToScancode[BUTTON_X]         = SDL_SCANCODE_A;
    mappingToScancode[BUTTON_Y]         = SDL_SCANCODE_S;
    mappingToScancode[BUTTON_SELECT]    = SDL_SCANCODE_SPACE;
    mappingToScancode[BUTTON_START]     = SDL_SCANCODE_RETURN;
    mappingToScancode[BUTTON_L]         = SDL_SCANCODE_Q;
    mappingToScancode[BUTTON_R]         = SDL_SCANCODE_W;
    mappingToScancode[BUTTON_PAD_UP]    = SDL_SCANCODE_UP;
    mappingToScancode[BUTTON_PAD_DOWN]  = SDL_SCANCODE_DOWN;
    mappingToScancode[BUTTON_PAD_LEFT]  = SDL_SCANCODE_LEFT;
    mappingToScancode[BUTTON_PAD_RIGHT] = SDL_SCANCODE_RIGHT;
}

void Input::update()
{
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);

    SDL_memcpy(pButtonState, cButtonState, NUM_BUTTONS * sizeof(bool));

    for (int i = 0; i < NUM_BUTTONS; ++i)
    {
        cButtonState[i] = keyState[mappingToScancode[i]];
    }
}

bool Input::isPressed(Button button) { return cButtonState[button]; }

bool Input::isReleased(Button button) { return !cButtonState[button]; }

bool Input::isJustPressed(Button button)
{
    return !pButtonState[button] && cButtonState[button];
}

bool Input::isJustReleased(Button button)
{
    return pButtonState[button] && !cButtonState[button];
}

int Input::getHorizontalInputDirection()
{
	int direction = 0;
	if (isPressed(BUTTON_PAD_LEFT))
	{
		direction -= 1;
	}
	if (isPressed(BUTTON_PAD_RIGHT))
	{
		direction += 1;
	}
	return direction;
}
