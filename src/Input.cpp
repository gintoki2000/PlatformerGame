#include "Input.h"
#include "SDL_scancode.h"
#include "SDL_stdinc.h"

bool Input::pButtonState[NUM_BUTTONS] = {};
bool Input::cButtonState[NUM_BUTTONS] = {};
int Input::mappingToScancode[NUM_BUTTONS] = {};

void Input::Init()
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

void Input::Update()
{
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);

    SDL_memcpy(pButtonState, cButtonState, NUM_BUTTONS * sizeof(bool));

    for (int i = 0; i < NUM_BUTTONS; ++i)
    {
        cButtonState[i] = keyState[mappingToScancode[i]];
    }
}

void Input::ResetState()
{
	SDL_memset(cButtonState, 0, NUM_BUTTONS * sizeof(bool));	
	SDL_memset(pButtonState, 0, NUM_BUTTONS * sizeof(bool));	
}

bool Input::IsPressed(Button button) { return cButtonState[button]; }

bool Input::IsReleased(Button button) { return !cButtonState[button]; }

bool Input::IsJustPressed(Button button)
{
    return !pButtonState[button] && cButtonState[button];
}

bool Input::IsJustReleased(Button button)
{
    return pButtonState[button] && !cButtonState[button];
}

int Input::GetHorizontalInputDirection()
{
	int direction = 0;
	if (IsPressed(BUTTON_PAD_LEFT))
	{
		direction -= 1;
	}
	if (IsPressed(BUTTON_PAD_RIGHT))
	{
		direction += 1;
	}
	return direction;
}
