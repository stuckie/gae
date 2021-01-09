#include "gae_audio_system.h"
#include "gae_file_sdl2.h"
#include "gae_buffer.h"

#include <SDL2/SDL_mixer.h>

gae_audio_system_t* gae_audio_system_init(gae_audio_system_t* system, int frequencyHz, enum gae_audio_format format, int channels, int bufferSize)
{
    Uint16 sdlFormat = 0;
    Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3); /* only really want OGG*/

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

    if (0 == Mix_OpenAudio(frequencyHz, sdlFormat, channels, bufferSize)) {
		system->bufferSize = bufferSize;
		system->channels = channels;
		system->format = format;
	}
    
    return system;
}

gae_audio_sound_t* gae_audio_sound_load_from_path(gae_audio_sound_t* sound, enum gae_audio_sound_type soundType, const char* const path)
{
	switch (soundType) {
		case gae_audio_sound_streaming: {
			sound->platformSound = Mix_LoadMUS(path);
			sound->soundType = (0 != sound->platformSound) ? gae_audio_sound_streaming : gae_audio_sound_invalid;
		};
		break;

		case gae_audio_sound_sample: {
			sound->platformSound = Mix_LoadWAV(path);
			sound->soundType = (0 != sound->platformSound) ? gae_audio_sound_sample : gae_audio_sound_invalid;
		};
		break;

		default: {
			sound->soundType = gae_audio_sound_invalid;
		};
		break;
	}

	return sound;
}

gae_audio_sound_t* gae_audio_sound_load_from_file(gae_audio_sound_t* sound, enum gae_audio_sound_type soundType, gae_file_t* file)
{
	switch (soundType) {
		case gae_audio_sound_streaming: {
			gae_file_sdl2_t* sdlFile = file->platform;
			sound->platformSound = Mix_LoadMUS_RW(sdlFile->file, 0);
			sound->soundType = (0 != sound->platformSound) ? gae_audio_sound_streaming : gae_audio_sound_invalid;
		};
		break;

		case gae_audio_sound_sample: {
			gae_file_sdl2_t* sdlFile = file->platform;
			sound->platformSound = Mix_LoadWAV_RW(sdlFile->file, 0);
			sound->soundType = (0 != sound->platformSound) ? gae_audio_sound_sample : gae_audio_sound_invalid;
		};
		break;

		default: {
			sound->soundType = gae_audio_sound_invalid;
		};
		break;
	}

	return sound;
}

gae_audio_sound_t* gae_audio_sound_load_from_buffer(gae_audio_sound_t* sound, gae_buffer_t* buffer)
{
	sound->platformSound = Mix_QuickLoad_RAW(buffer->data, buffer->length);
	sound->soundType = (0 != sound->platformSound) ? gae_audio_sound_buffer : gae_audio_sound_invalid;
	return sound;
}

gae_audio_sound_t* gae_audio_sound_play(gae_audio_sound_t* sound, int loops, int channel)
{
	switch (sound->soundType) {
		case gae_audio_sound_streaming: {
			Mix_PlayMusic(sound->platformSound, loops);
		};
		break;

		case gae_audio_sound_sample:
		case gae_audio_sound_buffer: {
			Mix_PlayChannel(channel, sound->platformSound, loops);
		};
		break;

		default:
		break;
	};

	return sound;
}

gae_audio_system_t* gae_audio_system_destroy(gae_audio_system_t* system)
{
    Mix_CloseAudio();
    
    return system;
}
