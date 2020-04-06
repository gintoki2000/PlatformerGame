#ifndef ANIMATOR_H
#define ANIMATOR_H
#include "Animation.h"
#include "NTTextureRegion.h"
#include "SDL_render.h"
struct AnimatorState
{
    AnimatorState()
    {
        index       = 0;
        elapsedTime = 0.f;
        isPaused    = false;
    }

    AnimatorState(int _index)
    {
        index       = _index;
        elapsedTime = 0.f;
        isPaused    = false;
    }

    AnimatorState(int _index, float _initialTime)
    {
        index       = _index;
        elapsedTime = _initialTime;
        isPaused    = false;
    }

    int   index;
    float elapsedTime;
    bool  isPaused;
};
class Animator
{
  public:
    Animator(Animation* animations[], int numAnimations);

    ~Animator();

    void play(int index, float initialTime);
    void tick(float deltaTime);
    void paint(SDL_Renderer* renderer);
    void pause();
    void resume(); 
    bool isCurrentAnimationFinshed();

    /// setter & getter
    int              getAnimationIndex() const;
    float            getElapsedTime() const; 
	void 			 setElapsedTime(float elapsedTime);
    bool             isPaused() const; 
    void             setPaused(bool paused); 
    int              getPositionX() const { return m_position.x; }
    void             setPositionX(int x) { m_position.x = x; }
    int              getPositionY() const { return m_position.y; }
    void             setPositionY(int y) { m_position.y = y; }
    int              getOriginX() const { return m_origin.x; }
    void             setOriginX(int originX) { m_origin.x = originX; }
    int              getOriginY() const { return m_origin.y; }
    void             setOriginY(int originY) { m_origin.y = originY; }
    SDL_RendererFlip getFlip() const { return m_flip; }
    void             setFlip(SDL_RendererFlip flip) { m_flip = flip; }
    double           getRotation() const { return m_rotation; }
    void             setRotation(double rotation) { m_rotation = rotation; }
    Animation*       getCurrentAnimation() const;

    void pushState(const AnimatorState& state);
    void popState();
    void changeState(const AnimatorState& state);

  private:
    AnimatorState&       getCurrentState() { return m_states[m_numStates - 1]; }
    const AnimatorState& getCurrentState() const
    {
        return m_states[m_numStates - 1];
    }
    Animation**      m_animations;
    int              m_numAnimations;
    SDL_Rect         m_dstrect;
    SDL_Point        m_position;
    SDL_Point        m_origin;
    double           m_rotation;
    SDL_RendererFlip m_flip;

    static constexpr int MAX_STATES = 5;
    AnimatorState        m_states[MAX_STATES];
    int                  m_numStates;
};
#endif // ANIMATOR_H
