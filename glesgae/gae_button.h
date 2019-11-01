#ifndef _GAE_BUTTON_H_
#define _GAE_BUTTON_H_

#include "gae_point.h"
#include "gae_rect.h"

enum gae_button_state
{	gae_button_active = 0
,	gae_button_inactive
,	gae_button_hover
,	gae_button_pressed
,	gae_button_state_count
};

typedef void (*gae_button_callback_t)(void* data);

typedef struct gae_button_renderable_s {
	void* data;
	
	gae_button_callback_t onRender;
} gae_button_renderable_t;

typedef struct gae_button_s {
	gae_button_renderable_t graphics[gae_button_state_count];
	void* userData;
	
	gae_button_callback_t onEnter;
	gae_button_callback_t onExit;
	gae_button_callback_t onPressed;
	gae_button_callback_t onReleased;
	
	gae_rect_t dims;
	
	enum gae_button_state state;
} gae_button_t;

gae_button_t* gae_button_init(gae_button_t* button, gae_button_renderable_t* const graphics, gae_rect_t* const dims, void* userData);
gae_button_t* gae_button_update(gae_button_t* button, gae_point_2d_t* const cursor, int isClicked);
gae_button_t* gae_button_destroy(gae_button_t* button);

gae_button_renderable_t* gae_button_create_sprite(gae_button_t* const button, gae_button_renderable_t* renderable, struct gae_frame_s* const frame);
gae_button_renderable_t* gae_button_create_nineslice(gae_button_t* const button, gae_button_renderable_t* renderable, struct gae_nineslice_s* const nineslice);

#endif
