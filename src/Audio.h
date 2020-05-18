#ifndef AUDIO_H
#define AUDIO_H
#include "SDL_mixer.h"
enum SoundID
{
	SOUND_FIREBALL,
	SOUND_STAB,
	SOUND_EXPLOSION,
	NUM_SOUNDS
};
class Audio 
{
	public:
		static bool Init();
		static void Terminate();
		static void Play(SoundID soundID);
		static void VolumeSound(SoundID soundID, int volume);
	
	private:
		Audio() {}

		static Mix_Chunk* sounds[NUM_SOUNDS]; 
};
#endif //AUDIO_H
