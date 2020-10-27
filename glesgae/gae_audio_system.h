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

int gae_audio_system_init(int frequencyHz, enum gae_audio_format format, int channels, int chunkSize);
int gae_audio_system_destroy();

#endif
