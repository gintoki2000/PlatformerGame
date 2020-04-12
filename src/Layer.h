#ifndef LAYER_H
#define LAYER_H
#include "Rect.h"
#include "Vec.h"
#include "SDL.h"
class Layer
{
  public:
    Layer();

    virtual ~Layer() = 0;

    virtual void render(SDL_Renderer* renderer, const Rect& viewPort) = 0;

    bool isVisible() const { return m_isVisible; }

    void setVisible(bool v) { m_isVisible = v; }

    void show() { setVisible(true); }

    void hide() { setVisible(false); }

  protected:
    bool m_isVisible;
};
#endif // LAYER_H
