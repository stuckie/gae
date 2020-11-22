#include "gae_audio_system.h"

#include <SDL2/SDL_mixer.h>

int gae_audio_system_init(int frequencyHz, enum gae_audio_format format, int channels, int chunkSize)
{
    Uint16 sdlFormat;
    Mix_Init(MIX_INIT_OGG);

    switch (format) {
        case gae_audio_u8: sdlFormat = AUDIO_U8; break;
        case gae_audio_s8: sdlFormat = AUDIO_S8; break;
        case gae_audio_u16lsb: sdlFormat = AUDIO_U16LSB; break;
        case gae_audio_s16lsb: sdlFormat = AUDIO_S16LSB; break;
        case gae_audio_u16msb: sdlFormat = AUDIO_U16MSB; break;
        case gae_audio_s16msb: sdlFormat = AUDIO_S16LSB; break;
        case gae_audio_u16: sdlFormat = AUDIO_U16; break;
        case gae_audio_s16: sdlFormat = AUDIO_S16; break;
        case gae_audio_u16sys: sdlFormat = AUDIO_U16SYS; break;
        case gae_audio_s16sys: sdlFormat = AUDIO_S16SYS; break;
    };

    Mix_OpenAudio(frequencyHz, sdlFormat, channels, chunkSize);
    
    return 0;
}

int gae_audio_system_destroy()
{
    Mix_CloseAudio();
    
    return 0;
}
