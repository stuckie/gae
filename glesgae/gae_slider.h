#ifndef _GAE_SLIDER_H_
#define _GAE_SLIDER_H_

typedef struct gae_slider_s {
	float value, min, max, increment;
	
	gae_sprite_sheet_t frames;
	gae_rect_t dims;
} gae_slider_t;

gae_slider_t* gae_slider_init(gae_slider_t* slider, const char* sliderDef, gae_point_2d_t* const position, void* userData);
gae_slider_t* gae_slider_update(gae_slider_t* slider, gae_point_2d_t* const cursor, int isClicked);
gae_slider_t* gae_slider_setValue(gae_slider_t* slider, const float value);
gae_slider_t* gae_slider_destroy(gae_slider_t* slider);

#endif
