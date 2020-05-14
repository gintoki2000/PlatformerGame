#include "Audio.h"
#include "SDL_log.h"
#include "SDL_mixer.h"

Mix_Chunk* Audio::sounds[NUM_SOUNDS] = {};

bool Audio::init()
{
    struct Pair
    {
        const char* file;
        SoundID     id;
    };

    Pair pairs[] = {
        {"asserts/sounds/fireball.wav", SOUND_FIREBALL},
        {"asserts/sounds/impact.wav", SOUND_STAB},
        {"asserts/sounds/bomb-explosion.wav", SOUND_EXPLOSION}};
    Mix_Chunk* sound;
    for (int i = 0; i < NUM_SOUNDS; ++i)
    {
        if ((sound = Mix_LoadWAV(pairs[i].file)) == nullptr)
        {
            SDL_Log("Failed to load sound: %s", Mix_GetError());
            return false;
        }
        sounds[i] = sound;
    }
	return true;
}

void Audio::terminate() 
{
	for (int i = 0; i < NUM_SOUNDS; ++i)
	{
		Mix_FreeChunk(sounds[i]);
		sounds[i] = nullptr;
	}
}

void Audio::play(SoundID soundID)
{
	Mix_Chunk* sound = sounds[soundID];
	Mix_PlayChannel(-1, sound, 0);
}

void Audio::volumeSound(SoundID soundID, int volume)
{
	Mix_Chunk* sound = sounds[soundID];
	Mix_VolumeChunk(sound, volume);
}
