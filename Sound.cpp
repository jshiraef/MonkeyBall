#include "Sound.h"

Sound::Sound(void)
{
	FMOD::System_Create(&system);// create an instance of the game engine
	system->init(32, FMOD_INIT_NORMAL, 0);// initialize the game engine with 32 channels (number of simultaneous sounds that can be played)
}

Sound::~Sound(void)
{
	for (int i = 0; i<NUM_SOUNDS; i++) sounds[i]->release();
	system->release();
}

bool Sound::Load()
{

	system->createSound("Sounds/swish.wav", FMOD_HARDWARE, 0, &sounds[SOUND_SWISH]);
	system->createSound("Sounds/warp.wav", FMOD_HARDWARE, 0, &sounds[SOUND_WARP]);
	system->createSound("Sounds/energyflow.wav", FMOD_HARDWARE, 0, &sounds[SOUND_ENERGYFLOW]);
	system->createSound("Sounds/bounce.wav", FMOD_HARDWARE, 0, &sounds[SOUND_BOUNCE]);
	return true;
}

void Sound::Play(int sound_id)
{
	
	system->playSound(FMOD_CHANNEL_FREE, sounds[sound_id], false, 0);
}

void Sound::PlayBounce(float vol)
{
	system->playSound(FMOD_CHANNEL_FREE, sounds[SOUND_BOUNCE], true, &bounceChannel);
	bounceChannel->setVolume(vol);
	bounceChannel->setPaused(false);
}

void Sound::StopAll()
{
	ambient1Channel->stop();
}

void Sound::Update()
{
	system->update();
}