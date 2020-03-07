#include "Input.h"

Input::ButtonState Input::currentButtonState = {};
Input::ButtonState Input::prevButtonState = {};

bool Input::isButtonAPressed() { return currentButtonState.buttonA; }

bool Input::isButtonBPressed() { return currentButtonState.buttonB; }

bool Input::isButtonUpPressed() { return currentButtonState.buttonUp; }

bool Input::isButtonDownPressed() { return currentButtonState.buttonDown; }

bool Input::isButtonLeftPressed() { return currentButtonState.buttonLeft; }

bool Input::isButtonRightPressed()
{
    return currentButtonState.buttonRight;
}

bool Input::isButtonAJustPressed()
{
    return currentButtonState.buttonA && !prevButtonState.buttonA;
}

bool Input::isButtonBJustPressed()
{
    return currentButtonState.buttonB && !prevButtonState.buttonB;
}

bool Input::isButtonUpJustPressed()
{
    return currentButtonState.buttonUp && !prevButtonState.buttonUp;
}

bool Input::isButtonDownJustPressed()
{
    return currentButtonState.buttonDown && !prevButtonState.buttonDown;
}

bool Input::isButtonLeftJustPressed()
{
    return currentButtonState.buttonLeft && !prevButtonState.buttonLeft;
}

bool Input::isButtonRightJustPressed()
{
    return currentButtonState.buttonRight && !prevButtonState.buttonRight;
}

int Input::getInputDirectionX()
{
    int dir = 0;
    if (isButtonLeftPressed())
        dir -= 1;
    if (isButtonRightPressed())
        dir += 1;
    return dir;
}

void Input::update()
{

    const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    prevButtonState = currentButtonState;
    currentButtonState.buttonA = keyState[SDL_SCANCODE_X];
    currentButtonState.buttonB = keyState[SDL_SCANCODE_Z];
    currentButtonState.buttonUp = keyState[SDL_SCANCODE_UP];
    currentButtonState.buttonDown = keyState[SDL_SCANCODE_DOWN];
    currentButtonState.buttonLeft = keyState[SDL_SCANCODE_LEFT];
    currentButtonState.buttonRight = keyState[SDL_SCANCODE_RIGHT];
}
