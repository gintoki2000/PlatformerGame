#ifndef ANIMATOR_H
#define ANIMATOR_H
#include "Animation.h"
#include "NTTextureRegion.h"
#include "SDL_render.h"
class Animator
{
  public:
    Animator(Animation* animations[], int numAnimations);

    ~Animator();

    void play(int index, float initialTime);

    void tick(float deltaTime);

    void paint(SDL_Renderer* renderer);

    void pause() { m_isPaused = true; }

    void resume() { m_isPaused = false; }

    bool isCurrentAnimationFinshed();

    /// setter & getter
    int getAnimationIndex() const { return m_animationIndex; }

    float getElapsedTime() const { return m_elapsedTime; }

    bool isPaused() const { return m_isPaused; }
    void setPaused(bool paused) { m_isPaused = paused; }

    int getPositionX() const { return m_position.x; }
    void setPositionX(int x) { m_position.x = x; }

    int getPositionY() const { return m_position.y; }
    void setPositionY(int y) { m_position.y = y; }

    int getOriginX() const { return m_origin.x; }
    void setOriginX(int originX) { m_origin.x = originX; }

    int getOriginY() const { return m_origin.y; }
    void setOriginY(int originY) { m_origin.y = originY; }

    SDL_RendererFlip getFlip() const { return m_flip; }
    void setFlip(SDL_RendererFlip flip) { m_flip = flip; }

    double getRotation() const { return m_rotation; }
    void setRotation(double rotation) { m_rotation = rotation; }

  private:
    Animation** m_animations;
    int m_numAnimations;
    int m_animationIndex;
    float m_elapsedTime;
    bool m_isPaused;
    ///
    const Sprite* m_currentSprite;
    SDL_Rect m_dstrect;
    SDL_Point m_position;
    SDL_Point m_origin;
    double m_rotation;
    SDL_RendererFlip m_flip;
};
#endif // ANIMATOR_H
