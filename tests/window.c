#include <gae.h>

#include <stdio.h>
#include <stdlib.h>

/*
 * Window Test
 * We want to run the following tests:
 * Create a Window
 * Create a Fullscreen window
 * Switch between Fullscreen to Windowed
 * Create a Resizable Window
 * Create a Window without a frame
 * Create two windows
 * Resize existing window
 * Window Icon
 * Update Window Title
 */

static void main_loop();
static int isRunning = 1;

typedef struct framerate_s {
	gae_timer_t cap;
	float fps;
	float ticksPerFrame;
} framerate_t;

typedef struct global_s {
	framerate_t framerate;
	gae_graphics_texture_t frameBuffer;
	gae_stack_t stateStack;
} global_t;

global_t GLOBAL;

static void OnQuit(void* userDatum, gae_event_t* const event)
{
	(void)(userDatum);
	(void)(event);
	
	isRunning = 0;
}

int main(int argc, char** argv)
{
	gae_system.main_clock = gae_clock_init(gae_alloc(sizeof(gae_clock_t)));
	gae_timer_init(&GLOBAL.framerate.cap, gae_system.main_clock);
	GLOBAL.framerate.fps = 1.0F / 60.0F;
	GLOBAL.framerate.ticksPerFrame = GLOBAL.framerate.fps * 1000.0F;
	
	gae_system.event_system = gae_event_system_init(gae_alloc(sizeof(gae_event_system_t)), &GLOBAL);
	
	/* See: pacoup.com/2011/06/12/list-of-true-169-resolutions */
	gae_system.graphics.window = gae_graphics_window_init(gae_alloc(sizeof(gae_graphics_window_t)), "Window Test", 0x2FFF0000U, 0x2FFF0000U, 640, 360, 0);
	gae_system.graphics.context = gae_graphics_context_init(gae_alloc(sizeof(gae_graphics_context_t)), gae_system.graphics.window, 0);
	
	gae_graphics_context_init_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer, 640, 360, 32);
	
	gae_system.event_system->onQuit = OnQuit;
	
	(void)(argc);
	(void)(argv);
	
#if !defined(__EMSCRIPTEN__)
	while (isRunning)
		main_loop();
	
	gae_graphics_window_destroy(gae_system.graphics.window);
	gae_graphics_context_destroy(gae_system.graphics.context);
	gae_event_system_destroy(gae_system.event_system);

	gae_free(gae_system.event_system);
	gae_free(gae_system.graphics.window);
	gae_free(gae_system.graphics.context);
	gae_free(gae_system.main_clock);
	
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
	
	gae_graphics_context_set_render_target(gae_system.graphics.context, 0);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &GLOBAL.frameBuffer, 0, 0);
	
	gae_graphics_context_present(gae_system.graphics.context);
	
	gae_timer_update(&GLOBAL.framerate.cap, gae_system.main_clock);
	
#ifndef __EMSCRIPTEN__
	gae_system_delay(gae_max(0, GLOBAL.framerate.ticksPerFrame - GLOBAL.framerate.cap.currentTime));
#endif
}
