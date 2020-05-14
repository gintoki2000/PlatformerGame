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
		static bool init();
		static void terminate();
		static void play(SoundID soundID);
		static void volumeSound(SoundID soundID, int volume);
	
	private:
		Audio() {}

		static Mix_Chunk* sounds[NUM_SOUNDS]; 
};
#endif //AUDIO_H
