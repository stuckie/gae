#ifndef _GAE_AUDIO_SYSTEM_H_
#define _GAE_AUDIO_SYSTEM_H_

enum gae_audio_format {
    gae_audio_u8
,   gae_audio_s8
,   gae_audio_u16lsb
,   gae_audio_s16lsb
,   gae_audio_u16msb
,   gae_audio_s16msb
,   gae_audio_u16
,   gae_audio_s16
,   gae_audio_u16sys
,   gae_audio_s16sys
};

enum gae_audio_sound_type {
	gae_audio_sound_streaming
,	gae_audio_sound_sample
,	gae_audio_sound_buffer
,	gae_audio_sound_invalid
};

typedef struct gae_audio_system_s {
	unsigned int frequencyHz;
	unsigned int channels;
	unsigned int bufferSize;
	enum gae_audio_format format;

	void* platform;
} gae_audio_system_t;

typedef struct gae_audio_sound_s {
	enum gae_audio_sound_type soundType;
	void* platformSound;

	int channel;	/* current channel sound is playing on */
	int volume;		/* current channel volume of sound */
} gae_audio_sound_t;

#define GAE_AUDIO_DEFAULT_FREQUENCY 22050
#define GAE_AUDIO_DEFAULT_FORMAT gae_audio_s16lsb
#define GAE_AUDIO_DEFAULT_CHANNELS 2
#define GAE_AUDIO_DEFAULT_BUFFERSIZE 512

#define GAE_AUDIO_CHANNEL_UNASSIGNED -1

struct gae_buffer_s;
struct gae_file_s;

gae_audio_system_t* gae_audio_system_init(gae_audio_system_t* system, int frequencyHz, enum gae_audio_format format, int channels, int bufferSize);

/* Load a sound from the given path */
gae_audio_sound_t* gae_audio_sound_load_from_path(gae_audio_sound_t* sound, enum gae_audio_sound_type soundType, const char* const path);

/* Load a sound from the given file */
gae_audio_sound_t* gae_audio_sound_load_from_file(gae_audio_sound_t* sound, enum gae_audio_sound_type soundType, struct gae_file_s* file);

/* Load a sound from the given buffer */
gae_audio_sound_t* gae_audio_sound_load_from_buffer(gae_audio_sound_t* sound, struct gae_buffer_s* buffer);

/* Play the given sound with the requested loops and channel. Set to -1 for infinite loops, and any free channel */
gae_audio_sound_t* gae_audio_sound_play(gae_audio_sound_t* sound, int loops, int channel);

/* Set the volume on the given sound on the channel it is playing on. Set to -1 to put the current volume into sound. This will effect the entire channel. */
gae_audio_sound_t* gae_audio_sound_volume(gae_audio_sound_t* sound, int volume);

/* Set the volume on the given channel. Set to -1 to return the current volume */
int gae_audio_channel_volume(int channel, int volume);

/* Destroy the given audio system */
gae_audio_system_t* gae_audio_system_destroy(gae_audio_system_t* system);

#endif
