#ifndef INPUT_H
#define INPUT_H
#include "SDL.h"
#include "SDL_scancode.h"
enum Button
{
    BUTTON_A,
    BUTTON_B,
    BUTTON_X,
    BUTTON_Y,
    BUTTON_START,
    BUTTON_SELECT,
    BUTTON_PAD_LEFT,
    BUTTON_PAD_RIGHT,
    BUTTON_PAD_UP,
    BUTTON_PAD_DOWN,
    BUTTON_L,
    BUTTON_R,
    NUM_BUTTONS,
};
class Input
{
  public:
    static int  getHorizontalInputDirection();
    //static int  getVerticalInputDirection();

    static bool isPressed(Button button);
    static bool isReleased(Button button);
    static bool isJustPressed(Button button);
    static bool isJustReleased(Button button);

	static void init();

    static void update();

  private:
    static int  mappingToScancode[NUM_BUTTONS];
    static bool pButtonState[NUM_BUTTONS];
    static bool cButtonState[NUM_BUTTONS];
};
#endif // INPUT_H
