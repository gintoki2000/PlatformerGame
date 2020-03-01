#ifndef NT_LAYER_H
#define NT_LAYER_H
#include "NTRect.h"
#include "NTVec.h"
#include "SDL.h"
class NTLayer
{
public:
    NTLayer();

    virtual ~NTLayer() = 0;

    virtual void update(float dt) = 0;

    virtual void draw(SDL_Renderer* renderer, const NTRect& viewPort) = 0;

    virtual void setPosition(const NTVec& newPos) { m_position = newPos; }

    const NTVec& getPosition() const { return m_position; }

    virtual void moveBy(const NTVec& v) { m_position += v; }

    virtual void setPositionX(float x) { m_position.x = x; }

    virtual void setPositionY(float y) { m_position.y = y; }

    bool isVisible() const { return m_isVisible; }

    virtual void setVisible(bool v) { m_isVisible = v; }

    virtual void show() { setVisible(true); }

    virtual void hide() { setVisible(false); }

  protected:
    NTVec m_position;
    bool m_isVisible;
};
#endif // NT_LAYER_H
