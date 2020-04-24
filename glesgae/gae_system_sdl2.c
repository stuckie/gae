#include "gae_system.h"
#include "SDL2/SDL.h"

#define DELAY_ERROR 10

void gae_system_delay(unsigned int ms)
{
	/* Need to try and be a little bit clever here as most delays are ~10ms out */
	Uint32 current = SDL_GetTicks();
	Uint32 target = current + ms;

	while (((int64_t)target - (int64_t)current) > DELAY_ERROR) {
		SDL_Delay(DELAY_ERROR);
		current = SDL_GetTicks();
	}

	/* Busy loop the remaining */
	while (target > current) current = SDL_GetTicks();
}
