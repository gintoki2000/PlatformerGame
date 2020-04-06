#ifndef NT_LAYER_H
#define NT_LAYER_H
#include "NTRect.h"
#include "NTVec.h"
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
#endif // NT_LAYER_H
