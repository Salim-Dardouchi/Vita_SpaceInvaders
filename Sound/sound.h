/*
 * source.h
 *
 *  Created on: 26 avr. 2021
 *      Author: Salim
 */

typedef struct s_sound t_sound;

t_sound*SoundNew();
t_sound*SoundDel(t_sound*pSound);
t_sound*SoundPlayMusic(t_sound*pSound);
t_sound*SoundHaltMusic(t_sound*pSound);
t_sound*SoundPlayAlienKilledSound(t_sound*pSound);
t_sound*SoundPlayShipKilledSound(t_sound*pSound);
t_sound*SoundPlayShipShootingSound(t_sound*pSound);
