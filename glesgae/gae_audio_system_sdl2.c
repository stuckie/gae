#include "gae_audio_system.h"

#include <SDL2/SDL_mixer.h>

int gae_audio_system_init(int frequencyHz, enum gae_audio_format format, int channels, int chunkSize)
{
    Mix_Init(MIX_INIT_OGG);

    Mix_OpenAudio(frequencyHz, format, channels, chunkSize);
    return 0;
}

int gae_audio_system_destroy()
{
    Mix_CloseAudio();
    
    return 0;
}
