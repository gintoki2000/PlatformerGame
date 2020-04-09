#include "Audio.h"
#include "SDL.h"
#include "SDL_mixer.h"
Audio::Audio()
{
    for (int i = 0; i < NUM_SOUNDS; ++i)
    {
        m_sounds[i] = nullptr;
    }
}

Audio::~Audio()
{
    for (int i = 0; i < NUM_SOUNDS; ++i)
    {
        Mix_FreeChunk(m_sounds[i]);
    }
}

bool Audio::load()
{
    struct Pair
    {
        const char* filename;
        int         id;
    };
    Pair pairs[NUM_SOUNDS] = {{"asserts/sounds/fireball.wav", FIREBALL},
                              {"asserts/sounds/landing.wav", LANDING}};

    for (int i = 0; i < NUM_SOUNDS; ++i)
    {
        if ((m_sounds[i] = Mix_LoadWAV(pairs[i].filename)) == nullptr)
        {
            SDL_Log("Failed to load sound: %s", Mix_GetError());
            return false;
        }
    }

    int allocatedChannels = Mix_AllocateChannels(4);
    if (allocatedChannels < 0)
    {
        SDL_Log("Failed to allocate mix channels\n");
        return false;
    }
    return true;
}

void Audio::play(int soundID) { Mix_PlayChannel(-1, m_sounds[soundID], 0); }
