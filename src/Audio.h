#ifndef SOUND_H
#define SOUND_H
#include "SDL_mixer.h"
class Audio
{
  public:
    enum ID
    {
        FIREBALL,
        NUM_SOUNDS
    };
    Audio();
    ~Audio();

    bool load();
    void play(int soundID);

  private:
    Mix_Chunk* m_sounds[NUM_SOUNDS];
};
#endif // SOUND_H
