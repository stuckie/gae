#ifndef _GAE_FILE_SDL2_H_
#define _GAE_FILE_SDL2_H_

#include "gae_file.h"
#include <SDL2/SDL.h>

typedef struct gae_file_sdl2_s {
	SDL_RWops* file;
	
	Sint64 pos;
	Sint64 length;
} gae_file_sdl2_t;

#endif

