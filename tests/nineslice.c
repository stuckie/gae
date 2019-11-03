#include <gae.h>

#include <stdio.h>
#include <stdlib.h>

static void main_loop();
static int isRunning = 1;
static int y = 0;

typedef struct framerate_s {
	gae_timer_t cap;
	float fps;
	float ticksPerFrame;
} framerate_t;

typedef struct global_s {
	framerate_t framerate;
	gae_graphics_texture_t frameBuffer;
	gae_nineslice_t nineslice;
	gae_pointer_t pointer;
} global_t;

global_t GLOBAL;

static void SetupNineslice(gae_nineslice_t* _nineslice, gae_graphics_texture_t* const _texture)
{
	gae_nineslice_dims_t dims;
	dims.x[0] = 0;	dims.y[0] = 0;
	dims.x[1] = 16;	dims.y[1] = 16;
	dims.x[2] = 32;	dims.y[2] = 32;
	dims.x[3] = 48;	dims.y[3] = 48;
	
	gae_nineslice_init(_nineslice, &dims, _texture);
	
	_nineslice->dst.x = 64;
	_nineslice->dst.y = y;
}

static void DestroyNineslice(gae_nineslice_t* _nineslice)
{
	gae_nineslice_destroy(_nineslice);
}

static void OnQuit(void* userDatum, gae_event_t* const event)
{
	(void)(userDatum);
	(void)(event);
	
	isRunning = 0;
}

static void OnMouseEvent(void* userDatum, gae_event_t* event)
{
	global_t* data = userDatum;
	
	switch (event->type) {
		case GAE_EVENT_MOUSE_MOVE: {
			gae_pointer_move_event_t* motion = event->event;
			data->pointer.pos.x = motion->x;
			data->pointer.pos.y = motion->y;
		};
		break;
		case GAE_EVENT_MOUSE_BUTTON: {
			gae_pointer_button_event_t* button = event->event;
			data->pointer.isDown[button->buttonId] = button->isDown;
			data->pointer.pos.x = button->x;
			data->pointer.pos.y = button->y;
		};
		break;
		default:
		break;
	}
}

int main(int argc, char** argv)
{
	gae_graphics_texture_t* ninesliceTexture;
	
	gae_system.main_clock = gae_clock_init(gae_alloc(sizeof(gae_clock_t)));
	gae_timer_init(&GLOBAL.framerate.cap, gae_system.main_clock);
	GLOBAL.framerate.fps = 1.0F / 60.0F;
	GLOBAL.framerate.ticksPerFrame = GLOBAL.framerate.fps * 1000.0F;
	
	gae_system.event_system = gae_event_system_init(gae_alloc(sizeof(gae_event_system_t)), &GLOBAL);
	
	/* See: pacoup.com/2011/06/12/list-of-true-169-resolutions */
	gae_system.graphics.window = gae_graphics_window_init(gae_alloc(sizeof(gae_graphics_window_t)), "Nineslice Test", 0x2FFF0000U, 0x2FFF0000U, 640, 360, 0);
	gae_system.graphics.context = gae_graphics_context_init(gae_alloc(sizeof(gae_graphics_context_t)), gae_system.graphics.window, 0);
	
	gae_graphics_context_init_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer, 640, 360, 32);
	
	ninesliceTexture = gae_alloc(sizeof(gae_graphics_texture_t));
	gae_graphics_texture_init(ninesliceTexture);
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, "data/window.bmp", ninesliceTexture);
	SetupNineslice(&GLOBAL.nineslice, ninesliceTexture);
	
	gae_system.event_system->onMouseEvent = OnMouseEvent;
	gae_system.event_system->onQuit = OnQuit;
	
	(void)(argc);
	(void)(argv);
	
#if !defined(__EMSCRIPTEN__)
	while (isRunning)
		main_loop();
	
	DestroyNineslice(&GLOBAL.nineslice);

	gae_graphics_texture_destroy(ninesliceTexture);
	gae_graphics_window_destroy(gae_system.graphics.window);
	gae_graphics_context_destroy(gae_system.graphics.context);
	gae_event_system_destroy(gae_system.event_system);
	gae_clock_destroy(gae_system.main_clock);

	gae_free(gae_system.event_system);
	gae_free(gae_system.graphics.window);
	gae_free(gae_system.graphics.context);
	gae_free(gae_system.main_clock);
	gae_free(ninesliceTexture);
#else
	emscripten_set_main_loop(main_loop, 60, 1);
#endif
	
	return 0;
}

/* main loop separated for Emscripten to work */
static void main_loop()
{
	gae_timer_update(&GLOBAL.framerate.cap, gae_system.main_clock);
	gae_timer_reset(&GLOBAL.framerate.cap);
	
	gae_clock_update(gae_system.main_clock);
	
	gae_event_system_update(gae_system.event_system);
	
	gae_graphics_context_set_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer);
	
	gae_graphics_context_clear(gae_system.graphics.context);

	/* render at 0,0 with original size of 64x64 */
	GLOBAL.nineslice.dst.y = y;
	GLOBAL.nineslice.dst.x = y;
	gae_nineslice_resize(&GLOBAL.nineslice, 64, 64);
	gae_nineslice_rescale(&GLOBAL.nineslice, 64, 64);
	gae_nineslice_render(&GLOBAL.nineslice);
	
	/* render at underneath first box on the left, but resized to 256x128 */
	GLOBAL.nineslice.dst.y += (GLOBAL.nineslice.dst.h * GLOBAL.nineslice.scale.y);
	gae_nineslice_resize(&GLOBAL.nineslice, 256, 128);
	gae_nineslice_render(&GLOBAL.nineslice);
	
	/* render at underneath second box on the left, resized to the original 64x64, but scaled to 256x128 */
	GLOBAL.nineslice.dst.y += (GLOBAL.nineslice.dst.h * GLOBAL.nineslice.scale.y);
	gae_nineslice_resize(&GLOBAL.nineslice, 64, 64);
	gae_nineslice_rescale(&GLOBAL.nineslice, 256, 128);
	gae_nineslice_render(&GLOBAL.nineslice);
	
	/* render to the right of the second box on the left, resized and scaled to 256x128 - so 1:1 mapping to prove it works */
	GLOBAL.nineslice.dst.x += (GLOBAL.nineslice.dst.w * GLOBAL.nineslice.scale.x);
	GLOBAL.nineslice.dst.y = 64;
	gae_nineslice_resize(&GLOBAL.nineslice, 256, 128);
	gae_nineslice_rescale(&GLOBAL.nineslice, 256, 128);
	gae_nineslice_render(&GLOBAL.nineslice);
	
	gae_graphics_context_set_render_target(gae_system.graphics.context, 0);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &GLOBAL.frameBuffer, 0, 0);
	
	gae_graphics_context_present(gae_system.graphics.context);
	
	gae_timer_update(&GLOBAL.framerate.cap, gae_system.main_clock);
	
#ifndef __EMSCRIPTEN__
	gae_system_delay(gae_max(0, GLOBAL.framerate.ticksPerFrame - GLOBAL.framerate.cap.currentTime));
#endif
}
