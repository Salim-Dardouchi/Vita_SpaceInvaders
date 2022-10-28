/*
 * sound.c
 *
 *  Created on: 26 avr. 2021
 *      Author: Salim
 */

#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL_mixer.h>

#include <defines.h>

#include "sound.h"

#define SOUND_ALIEN_KILLED_PATH 	"app0:/Sound/Audio/alienKilled.wav"
#define SOUND_SHIP_KILLED_PATH		"app0:/Sound/Audio/shipKilled.wav"
#define SOUND_SHIP_SHOOTING_PATH	"app0:/Sound/Audio/shoot.wav"

#define SOUND_MUSIC_PATH			"app0:/Sound/Audio/music.mp3"

typedef enum e_SoundErrors{
	ST_EVERYTHING_IS_FINE = 0x0,
	ST_MIX_OPENAUDIO_FAIL = 0x1,
	ST_MIX_LOADMUSIC_FAIL = 0x2,
}t_SoundErrors;

struct s_sound {
	Mix_Music* 		pMusic;
	t_SoundErrors	uErrors;
	Mix_Chunk*		pSAlienKilled;
	Mix_Chunk*		pSShipKilled;
	Mix_Chunk*		pSShootingSnd;
};


t_sound*SoundNew(){
	t_sound* pNewSnd = (t_sound*)malloc(sizeof(t_sound));
	mBitsSet(pNewSnd->uErrors, ST_EVERYTHING_IS_FINE);

	if(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024)){
		mBitsSet(pNewSnd->uErrors, ST_MIX_OPENAUDIO_FAIL);
		return pNewSnd;
	}

	pNewSnd->pMusic=Mix_LoadMUS(SOUND_MUSIC_PATH);
	if(!pNewSnd->pMusic){
		mBitsSet(pNewSnd->uErrors, ST_MIX_LOADMUSIC_FAIL);
		return pNewSnd;
	}

	Mix_AllocateChannels(3); //I've allocated 3 channels for my 3 different sounds so they can be played at the same time if we need it.

	pNewSnd->pSAlienKilled=Mix_LoadWAV_RW(SDL_RWFromFile(SOUND_ALIEN_KILLED_PATH, "rb"), 1);
	pNewSnd->pSShipKilled=Mix_LoadWAV_RW(SDL_RWFromFile(SOUND_SHIP_KILLED_PATH, "rb"), 1);
	pNewSnd->pSShootingSnd=Mix_LoadWAV_RW(SDL_RWFromFile(SOUND_SHIP_SHOOTING_PATH, "rb"), 1);

	return pNewSnd;
}

t_sound*SoundDel(t_sound*pSound){
	Mix_FreeChunk(pSound->pSShootingSnd);
	Mix_FreeChunk(pSound->pSShipKilled);
	Mix_FreeChunk(pSound->pSAlienKilled);

	Mix_FreeMusic(pSound->pMusic);

	Mix_CloseAudio();

	free(pSound);
	return NULL;
}

t_sound*SoundPlayMusic(t_sound*pSound){
	Mix_PlayMusic(pSound->pMusic, -1);
	return NULL;
}

t_sound*SoundHaltMusic(t_sound*pSound){
	Mix_HaltMusic();
	return NULL;
}

t_sound*SoundPlayAlienKilledSound(t_sound*pSound){
	Mix_PlayChannelTimed(0, pSound->pSAlienKilled, 0, -1);
	return NULL;
}

t_sound*SoundPlayShipKilledSound(t_sound*pSound){
	Mix_PlayChannelTimed(1, pSound->pSShipKilled, 0, -1);
	return NULL;
}

t_sound*SoundPlayShipShootingSound(t_sound*pSound){
	Mix_PlayChannelTimed(2, pSound->pSShootingSnd, 0, -1);
	return NULL;
}

