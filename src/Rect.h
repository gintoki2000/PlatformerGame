#ifndef NT_RECT_H
#define NT_RECT_H
#include "SDL2/SDL.h"
#include "Vec.h"
struct Rect : public SDL_Rect {
  Rect() : SDL_Rect({0, 0, 0, 0}) {}
  Rect(const SDL_Rect &_rect) : SDL_Rect(_rect) {}
  Rect(int _x, int _y, int _w, int _h) : SDL_Rect({_x, _y, _w, _h}) {}
  int left() const { return x; }
  int right() const { return x + w; }
  int bottom() const { return y + h; }
  int top() const { return y; }
  int width() const { return w; }
  int height() const { return h; }
};

struct FloatRect
{
    float x, y,  width, height;

    FloatRect():x(0), y(0), width(0), height(0){}

    FloatRect(float _x, float _y, float _w, float _h):
        x(_x),y(_y),width(_w), height(_h) {}
    FloatRect(const Vec2& xy, const Vec2& size):
        x(xy.x), y(xy.y), width(size.x), height(size.y){}

    float right() const { return x + width; }
    float bottom() const { return y + height; }
};
#endif // RECT_H
