#ifndef _GAE_SPRITE_SHEET_H_
#define _GAE_SPRITE_SHEET_H_

#include "gae_graphics.h"
#include "gae_map.h"
#include "gae_hashstring.h"

struct gae_array_s;
struct gae_json_document_s;
struct gae_rect_s;

typedef struct gae_sprite_sheet_s {
	gae_graphics_texture_t texture;
	gae_map_t sprites;
    gae_map_t animations;
} gae_sprite_sheet_t;

/* Initialise a sprite sheet from the given json document */
gae_sprite_sheet_t* gae_sprite_sheet_init(gae_sprite_sheet_t* sheet, struct gae_json_document_s* jsDoc);

/* Helper function to draw a single sprite */
gae_sprite_sheet_t* gae_sprite_sheet_draw(gae_sprite_sheet_t* const sheet, gae_hashstring sprite, struct gae_rect_s* const pos);

/* Get the rect description of a sprite */
struct gae_rect_s* gae_sprite_sheet_get_rect(gae_sprite_sheet_t* const sheet, gae_hashstring sprite);

/* Get an array of sprites from this sprite sheet that make up an animation */
struct gae_array_s* gae_sprite_sheet_get_animation_frames(gae_sprite_sheet_t* const sheet, gae_hashstring animation);

/* Destroy the sprite sheet and clean up any memory created by it */
gae_sprite_sheet_t* gae_sprite_sheet_destroy(gae_sprite_sheet_t* sheet);

#endif
