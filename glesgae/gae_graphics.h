#ifndef _GAE_GRAPHICS_H_
#define _GAE_GRAPHICS_H_

#include "gae_types.h"

struct gae_point_2d_s;
struct gae_rect_s;
struct gae_buffer_s;
struct gae_colour_rgba_s;

typedef enum gae_graphics_window_flags_e
{	GAE_WINDOW_WINDOWED				= 0
,	GAE_WINDOW_FULLSCREEN			= 1 << 0
,	GAE_WINDOW_OPENGL				= 1 << 1
,	GAE_WINDOW_SHOWN				= 1 << 2
,	GAE_WINDOW_HIDDEN				= 1 << 3
,	GAE_WINDOW_BORDERLESS			= 1 << 4
,	GAE_WINDOW_RESIZABLE			= 1 << 5
,	GAE_WINDOW_MINIMISED			= 1 << 6
,	GAE_WINDOW_MAXIMISED			= 1 << 7
,	GAE_WINDOW_INPUT_GRABBED		= 1 << 8
,	GAE_WINDOW_INPUT_FOCUS			= 1 << 9
,	GAE_WINDOW_MOUSE_FOCUS			= 1 << 10
,	GAE_WINDOW_FOREIGN				= 1 << 11
,	GAE_WINDOW_FULLSCREEN_DESKTOP	= ( GAE_WINDOW_FULLSCREEN | 1 << 12 )
,	GAE_WINDOW_ALLOW_HIGHDPI		= 1 << 13
,	GAE_WINDOW_MOUSE_CAPTURE		= 1 << 14
,	GAE_WINDOW_ALWAYS_ON_TOP		= 1 << 15
,	GAE_WINDOW_SKIP_TASKBAR			= 1 << 16
,	GAE_WINDOW_UTILITY				= 1 << 17
,	GAE_WINDOW_TOOLTIP				= 1 << 18
,	GAE_WINDOW_POPUP_MENU			= 1 << 19
,	GAE_WINDOW_VULKAN				= 1 << 28
} gae_graphics_window_flags;

typedef enum gae_texture_flip_e
{	GAE_TEXTURE_FLIP_NONE			= 1 << 0
,	GAE_TEXTURE_FLIP_HORIZONTAL		= 1 << 1
,	GAE_TEXTURE_FLIP_VERTICAL		= 1 << 2
} gae_texture_flip;

typedef struct gae_graphics_window_s {
	void* data;
	
	const char* name;
	int x, y, w, h;
	gae_graphics_window_flags flags;
} gae_graphics_window_t;

typedef struct gae_graphics_context_s {
	void* data;
} gae_graphics_context_t;

typedef struct gae_graphics_texture_s {
	void* data;
	
	int w, h;
} gae_graphics_texture_t;

typedef struct gae_graphics_context_blit_params_s {
	struct gae_graphics_texture_s* texture;
	struct gae_rect_s* srcRect;
	struct gae_rect_s* dstRect;
	struct gae_point_2d_s* origin;
	double degrees;
	gae_texture_flip flip;
} gae_graphics_context_blit_params_t;

/* Initialise a new window with the given name at the specified position, with the given width and height, and flags */
gae_graphics_window_t* gae_graphics_window_init(gae_graphics_window_t* window, const char* const name, int x, int y, int w, int h, unsigned int flags);

/* Set Window Fullscreen status */
gae_graphics_window_t* gae_graphics_window_set_fullscreen(gae_graphics_window_t* window, gae_graphics_window_flags flag);

/* Set Window Size */
gae_graphics_window_t* gae_graphics_window_set_size(gae_graphics_window_t* window, int w, int h);

/* Update window */
gae_graphics_window_t* gae_graphics_window_update(gae_graphics_window_t* window);

/* Destroy the given window */
gae_graphics_window_t* gae_graphics_window_destroy(gae_graphics_window_t* window);

/* Initialise a new context bound to the given window, with the relevant platform data */
gae_graphics_context_t* gae_graphics_context_init(gae_graphics_context_t* context, gae_graphics_window_t* window, void* platformData);

/* Clear the given context */
gae_graphics_context_t* gae_graphics_context_clear(gae_graphics_context_t* const context);

/* Present the given context */
gae_graphics_context_t* gae_graphics_context_present(gae_graphics_context_t* const context);

/* Load a texture into the given context fro the specified file */
gae_graphics_context_t* gae_graphics_context_texture_load_from_file(gae_graphics_context_t* const context, const char* const filepath, gae_graphics_texture_t* texture);

/* Blit the given texture to the context using the specified source and destination rects */
gae_graphics_context_t* gae_graphics_context_blit_texture(gae_graphics_context_t* const context, gae_graphics_texture_t* const texture, struct gae_rect_s* const src, struct gae_rect_s* const dest);

/* Blit a texture with the given params structure */
gae_graphics_context_t* gae_graphics_context_blit_texture_params(gae_graphics_context_t* const context, gae_graphics_context_blit_params_t* const params);

/* Modulate texture colour */
gae_graphics_context_t* gae_graphics_context_texture_colour(gae_graphics_context_t* const context, gae_graphics_texture_t* texture, struct gae_colour_rgba_s* const colour);

/* Set Draw Colour */
gae_graphics_context_t* gae_graphics_context_set_draw_colour(gae_graphics_context_t* const context, struct gae_colour_rgba_s* const colour);

/* Get Draw Colour */
gae_graphics_context_t* gae_graphics_context_get_draw_colour(gae_graphics_context_t* const context, struct gae_colour_rgba_s* colour);

/* Draw a point */
gae_graphics_context_t* gae_graphics_context_draw_point(gae_graphics_context_t* const context, struct gae_point_2d_s* const p);

/* Draw a line between points */
gae_graphics_context_t* gae_graphics_context_draw_line(gae_graphics_context_t* const context, struct gae_point_2d_s* const a, struct gae_point_2d_s* const b);

/* Draw a rectangle */
gae_graphics_context_t* gae_graphics_context_draw_rect(gae_graphics_context_t* const context, struct gae_rect_s* const rect);

/* Draw a filled rectangle */
gae_graphics_context_t* gae_graphics_context_draw_filled_rect(gae_graphics_context_t* const context, struct gae_rect_s* const rect);

/* Set context render size */
gae_graphics_context_t* gae_graphics_context_set_render_size(gae_graphics_context_t* const context, int w, int h);

/* Initialise a texture as a render target */
gae_graphics_context_t* gae_graphics_context_init_render_target(gae_graphics_context_t* context, gae_graphics_texture_t* texture, int width, int height, int depth);

/* Set current render target */
gae_graphics_context_t* gae_graphics_context_set_render_target(gae_graphics_context_t* context, gae_graphics_texture_t* const texture);

/* Destroy the given context */
gae_graphics_context_t* gae_graphics_context_destroy(gae_graphics_context_t* context);

/* Initialise a new texture object */
gae_graphics_texture_t* gae_graphics_texture_init(gae_graphics_texture_t* texture);

/* Fill texture with the given buffer */
gae_graphics_texture_t* gae_graphics_texture_fill_from_buffer(gae_graphics_texture_t* texture, gae_graphics_context_t* const context, struct gae_buffer_s* const buffer, int width, int height, int depth);

/* Destroy the given texture */
gae_graphics_texture_t* gae_graphics_texture_destroy(gae_graphics_texture_t* texture);

#endif
