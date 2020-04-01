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

bool Input::isButtonAReleased()
{
    return !currentButtonState.buttonA;
}

bool Input::isButtonBReleased()
{
    return !currentButtonState.buttonB;
}

bool Input::isButtonUpReleased()
{
    return !currentButtonState.buttonUp;
}
bool Input::isButtonDownReleased()
{
    return !currentButtonState.buttonDown;
}
bool Input::isButtonRightReleased()
{
    return !currentButtonState.buttonRight;
}
bool Input::isButtonLeftReleased()
{
    return !currentButtonState.buttonLeft;
}

bool Input::isButtonAJustReleased()
{
    return !currentButtonState.buttonA && prevButtonState.buttonA;
}

bool Input::isButtonBJustReleased()
{
    return !currentButtonState.buttonB && prevButtonState.buttonB;
}

bool Input::isButtonUpJustReleased()
{
    return !currentButtonState.buttonUp && prevButtonState.buttonUp;
}
bool Input::isButtonDownJustReleased()
{
    return !currentButtonState.buttonDown && prevButtonState.buttonDown;
}
bool Input::isButtonRightJustReleased()
{
    return !currentButtonState.buttonRight && prevButtonState.buttonRight;
}
bool Input::isButtonLeftJustReleased()
{
    return !currentButtonState.buttonLeft && prevButtonState.buttonLeft;
}


int Input::getHorizontalInputDirection()
{
    int horizontalDirection = 0;
    if (isButtonLeftPressed())
        horizontalDirection -= 1;
    if (isButtonRightPressed())
        horizontalDirection += 1;
    return horizontalDirection;
}

int Input::getVerticalInputDirection()
{
    int verticalDirection = 0;
    if (isButtonUpPressed())
        verticalDirection -= 1;
    if (isButtonDownPressed())
        verticalDirection += 1;
    return verticalDirection;
}

void Input::update()
{

    const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    prevButtonState = currentButtonState;
    currentButtonState.buttonA = keyState[SDL_SCANCODE_Z];
    currentButtonState.buttonB = keyState[SDL_SCANCODE_X];
    currentButtonState.buttonUp = keyState[SDL_SCANCODE_UP];
    currentButtonState.buttonDown = keyState[SDL_SCANCODE_DOWN];
    currentButtonState.buttonLeft = keyState[SDL_SCANCODE_LEFT];
    currentButtonState.buttonRight = keyState[SDL_SCANCODE_RIGHT];
}
