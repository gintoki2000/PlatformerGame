#ifndef ANIMATOR_H
#define ANIMATOR_H
#include "Animation.h"
#include "NTTextureRegion.h"
class Animator
{
  public:
    Animator(Animation* animations[], int numAnimations);

    ~Animator();

    void play(int index, float initialTime);

    void update(float deltaTime);

    void render(SDL_Renderer* renderer, int x, int y);

	void pause() { m_isPaused = true; }

	void resume() { m_isPaused = false; }

	/// setter & getter
	int getAnimationIndex() const { return m_animationIndex; }

	float getElapsedTime() const { return m_elapsedTime; }

	bool isPaused() const { return m_isPaused; }
	void setPaused(bool paused) { m_isPaused = paused; }

  private:
    Animation** m_animations;
    int m_numAnimations;
	int m_animationIndex;
	float m_elapsedTime;
	bool m_isPaused;
	const Sprite* m_currentSprite;
};
#endif // ANIMATOR_H
