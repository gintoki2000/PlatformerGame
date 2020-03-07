#ifndef INPUT_H
#define INPUT_H
#include "SDL.h"
class Input
{
  public:
    static bool isButtonAPressed();
    static bool isButtonBPressed();
    static bool isButtonUpPressed();
    static bool isButtonDownPressed();
    static bool isButtonLeftPressed();
    static bool isButtonRightPressed();
    static bool isButtonAJustPressed();
    static bool isButtonBJustPressed();
    static bool isButtonUpJustPressed();

    static bool isButtonDownJustPressed();

    static bool isButtonLeftJustPressed();

    static bool isButtonRightJustPressed();

    static int getInputDirectionX();

    static void update();

  private:
    static struct ButtonState
    {
        bool buttonA = false;
        bool buttonB = false;
        bool buttonUp = false;
        bool buttonDown = false;
        bool buttonLeft = false;
        bool buttonRight = false;
    } prevButtonState, currentButtonState;
};
#endif // INPUT_H
