#include <gae.h>

#include <stdio.h>
#include <stdlib.h>

static void main_loop();
static int isRunning = 1;

typedef struct framerate_s {
	gae_timer_t cap;
	float fps;
	float ticksPerFrame;
} framerate_t;

enum test_e 
{	test_window_start			= 0
,	test_fullscreen
,	test_fullscreen_to_window
,	test_resizable_window
,	test_frameless_window
,	test_resize_existing_window
,	test_window_icon
,	test_update_title
,	test_two_windows
,	test_count
};

struct test_s;
typedef void (*OnStart)(struct test_s* test);
typedef void (*OnUpdate)(struct test_s* test);
typedef void (*OnStop)(struct test_s* test);

typedef struct test_s {
	gae_timer_t timer;
	void* data;
	
	OnStart onStart;
	OnUpdate onUpdate;
	OnStop onStop;
} test_t;

typedef struct global_s {
	framerate_t framerate;
	gae_graphics_texture_t frameBuffer;
	test_t tests[test_count];
	unsigned int testIndex;
} global_t;

global_t GLOBAL;

static void OnQuit(void* userDatum, gae_event_t* const event)
{
	(void)(userDatum);
	(void)(event);
	
	isRunning = 0;
}

static void CommonStart(test_t* test)
{
	gae_timer_init(&test->timer, gae_system.main_clock);
	test->timer.currentTime = 10;
	test->timer.scale = -1;
}

static void OnStartFirstWindow(test_t* test)
{
	CommonStart(test);
	
	/* See: pacoup.com/2011/06/12/list-of-true-169-resolutions */
	gae_system.graphics.window = gae_graphics_window_init(gae_alloc(sizeof(gae_graphics_window_t)), "Window Test #1", 0x2FFF0000U, 0x2FFF0000U, 640, 360, GAE_WINDOW_WINDOWED);
	gae_system.graphics.context = gae_graphics_context_init(gae_alloc(sizeof(gae_graphics_context_t)), gae_system.graphics.window, 0);
	
	gae_graphics_context_init_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer, 640, 360, 32);
}

static void OnUpdateFirstWindow(test_t* test)
{
	(void)(test);
	gae_graphics_context_set_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer);
	gae_graphics_context_clear(gae_system.graphics.context);
	
	gae_graphics_context_set_render_target(gae_system.graphics.context, 0);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &GLOBAL.frameBuffer, 0, 0);
	
	gae_graphics_context_present(gae_system.graphics.context);
}

static void OnStopFirstWindow(test_t* test)
{
	(void)(test);
	gae_graphics_window_destroy(gae_system.graphics.window);
	gae_graphics_context_destroy(gae_system.graphics.context);
}

static void OnStartFullscreenWindow(test_t* test)
{
	CommonStart(test);
	
	/* See: pacoup.com/2011/06/12/list-of-true-169-resolutions */
	gae_system.graphics.window = gae_graphics_window_init(gae_alloc(sizeof(gae_graphics_window_t)), "Window Test #2", 0x2FFF0000U, 0x2FFF0000U, 640, 360, GAE_WINDOW_FULLSCREEN);
	gae_system.graphics.context = gae_graphics_context_init(gae_alloc(sizeof(gae_graphics_context_t)), gae_system.graphics.window, 0);
	
	gae_graphics_context_init_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer, 640, 360, 32);
}

static void OnUpdateFullscreenWindow(test_t* test)
{
	(void)(test);
	gae_graphics_context_set_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer);
	gae_graphics_context_clear(gae_system.graphics.context);
	
	gae_graphics_context_set_render_target(gae_system.graphics.context, 0);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &GLOBAL.frameBuffer, 0, 0);
	
	gae_graphics_context_present(gae_system.graphics.context);
}

static void OnStopFullscreenWindow(test_t* test)
{
	(void)(test);
	gae_graphics_window_destroy(gae_system.graphics.window);
	gae_graphics_context_destroy(gae_system.graphics.context);
}

static void OnStartFullscreenToWindowed(test_t* test)
{
	CommonStart(test);
	
	/* See: pacoup.com/2011/06/12/list-of-true-169-resolutions */
	gae_system.graphics.window = gae_graphics_window_init(gae_alloc(sizeof(gae_graphics_window_t)), "Window Test #3", 0x2FFF0000U, 0x2FFF0000U, 640, 360, GAE_WINDOW_FULLSCREEN);
	gae_system.graphics.context = gae_graphics_context_init(gae_alloc(sizeof(gae_graphics_context_t)), gae_system.graphics.window, 0);
	
	gae_graphics_context_init_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer, 640, 360, 32);
}

static void OnUpdateFullscreenToWindowed(test_t* test)
{
	gae_graphics_context_set_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer);
	gae_graphics_context_clear(gae_system.graphics.context);
	
	gae_graphics_context_set_render_target(gae_system.graphics.context, 0);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &GLOBAL.frameBuffer, 0, 0);
	
	gae_graphics_context_present(gae_system.graphics.context);
	
	if (test->timer.currentTime < 5.0F) {
		if (0 != (gae_system.graphics.window->flags & GAE_WINDOW_FULLSCREEN))
			gae_graphics_window_set_fullscreen(gae_system.graphics.window, GAE_WINDOW_WINDOWED);
	}
}

static void OnStopFullscreenToWindowed(test_t* test)
{
	(void)(test);
	gae_graphics_window_destroy(gae_system.graphics.window);
	gae_graphics_context_destroy(gae_system.graphics.context);
}

static void OnStartResizableWindow(test_t* test)
{
	CommonStart(test);
	
	/* See: pacoup.com/2011/06/12/list-of-true-169-resolutions */
	gae_system.graphics.window = gae_graphics_window_init(gae_alloc(sizeof(gae_graphics_window_t)), "Resizable Window Test #4", 0x2FFF0000U, 0x2FFF0000U, 640, 360, GAE_WINDOW_WINDOWED | GAE_WINDOW_RESIZABLE);
	gae_system.graphics.context = gae_graphics_context_init(gae_alloc(sizeof(gae_graphics_context_t)), gae_system.graphics.window, 0);
	
	gae_graphics_context_init_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer, 640, 360, 32);
}

static void OnUpdateResizableWindow(test_t* test)
{
	(void)(test);
	gae_graphics_context_set_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer);
	gae_graphics_context_clear(gae_system.graphics.context);
	
	gae_graphics_context_set_render_target(gae_system.graphics.context, 0);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &GLOBAL.frameBuffer, 0, 0);
	
	gae_graphics_context_present(gae_system.graphics.context);
}

static void OnStopResizableWindow(test_t* test)
{
	(void)(test);
	gae_graphics_window_destroy(gae_system.graphics.window);
	gae_graphics_context_destroy(gae_system.graphics.context);
}

static void OnStartFramelessWindow(test_t* test)
{
	CommonStart(test);
	
	/* See: pacoup.com/2011/06/12/list-of-true-169-resolutions */
	gae_system.graphics.window = gae_graphics_window_init(gae_alloc(sizeof(gae_graphics_window_t)), "Frameless Window Test #5", 0x2FFF0000U, 0x2FFF0000U, 640, 360, GAE_WINDOW_WINDOWED | GAE_WINDOW_BORDERLESS);
	gae_system.graphics.context = gae_graphics_context_init(gae_alloc(sizeof(gae_graphics_context_t)), gae_system.graphics.window, 0);
	
	gae_graphics_context_init_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer, 640, 360, 32);
}

static void OnUpdateFramelessWindow(test_t* test)
{
	(void)(test);
	gae_graphics_context_set_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer);
	gae_graphics_context_clear(gae_system.graphics.context);
	
	gae_graphics_context_set_render_target(gae_system.graphics.context, 0);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &GLOBAL.frameBuffer, 0, 0);
	
	gae_graphics_context_present(gae_system.graphics.context);
}

static void OnStopFramelessWindow(test_t* test)
{
	(void)(test);
	gae_graphics_window_destroy(gae_system.graphics.window);
	gae_graphics_context_destroy(gae_system.graphics.context);
}

static void OnStartWindowResized(test_t* test)
{
	CommonStart(test);
	
	/* See: pacoup.com/2011/06/12/list-of-true-169-resolutions */
	gae_system.graphics.window = gae_graphics_window_init(gae_alloc(sizeof(gae_graphics_window_t)), "Window Test #6", 0x2FFF0000U, 0x2FFF0000U, 640, 360, GAE_WINDOW_WINDOWED);
	gae_system.graphics.context = gae_graphics_context_init(gae_alloc(sizeof(gae_graphics_context_t)), gae_system.graphics.window, 0);
	
	gae_graphics_context_init_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer, 640, 360, 32);
}

static void OnUpdateWindowResized(test_t* test)
{
	gae_graphics_context_set_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer);
	gae_graphics_context_clear(gae_system.graphics.context);
	
	gae_graphics_context_set_render_target(gae_system.graphics.context, 0);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &GLOBAL.frameBuffer, 0, 0);
	
	gae_graphics_context_present(gae_system.graphics.context);
	
	if (test->timer.currentTime < 5.0F) {
		if (320 != gae_system.graphics.window->w)
			gae_graphics_window_set_size(gae_system.graphics.window, 320, 240);
	}
}

static void OnStopWindowResized(test_t* test)
{
	(void)(test);
	gae_graphics_window_destroy(gae_system.graphics.window);
	gae_graphics_context_destroy(gae_system.graphics.context);
}

static void SetupTests(test_t* tests)
{
	unsigned int i = 0;
	OnStart startCallbacks[test_count] = 
	{	OnStartFirstWindow
	,	OnStartFullscreenWindow
	,	OnStartFullscreenToWindowed
	,	OnStartResizableWindow
	,	OnStartFramelessWindow
	,	OnStartWindowResized
	,	OnStartFirstWindow
	,	OnStartFirstWindow
	,	OnStartFirstWindow
	};
	
	OnUpdate updateCallbacks[test_count] =
	{	OnUpdateFirstWindow
	,	OnUpdateFullscreenWindow
	,	OnUpdateFullscreenToWindowed
	,	OnUpdateResizableWindow
	,	OnUpdateFramelessWindow
	,	OnUpdateWindowResized
	,	OnUpdateFirstWindow
	,	OnUpdateFirstWindow
	,	OnUpdateFirstWindow
	};
	
	OnStop stopCallbacks[test_count] =
	{	OnStopFirstWindow
	,	OnStopFullscreenWindow
	,	OnStopFullscreenToWindowed
	,	OnStopResizableWindow
	,	OnStopFramelessWindow
	,	OnStopWindowResized
	,	OnStopFirstWindow
	,	OnStopFirstWindow
	,	OnStopFirstWindow
	};
	
	for (; i < test_count; ++i) {
		tests[i].onStart = startCallbacks[i];
		tests[i].onUpdate = updateCallbacks[i];
		tests[i].onStop = stopCallbacks[i];
	}
}

int main(int argc, char** argv)
{
	gae_system.main_clock = gae_clock_init(gae_alloc(sizeof(gae_clock_t)));
	gae_timer_init(&GLOBAL.framerate.cap, gae_system.main_clock);
	GLOBAL.framerate.fps = 1.0F / 60.0F;
	GLOBAL.framerate.ticksPerFrame = GLOBAL.framerate.fps * 1000.0F;
	
	gae_system.event_system = gae_event_system_init(gae_alloc(sizeof(gae_event_system_t)), &GLOBAL);
	
	gae_system.event_system->onQuit = OnQuit;
	
	SetupTests(GLOBAL.tests);
	GLOBAL.testIndex = 0;
	GLOBAL.tests[GLOBAL.testIndex].onStart(&GLOBAL.tests[GLOBAL.testIndex]);
	
	(void)(argc);
	(void)(argv);
	
#if !defined(__EMSCRIPTEN__)
	while (isRunning)
		main_loop();
	
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
	test_t* test = &GLOBAL.tests[GLOBAL.testIndex];
	
	gae_timer_update(&GLOBAL.framerate.cap, gae_system.main_clock);
	gae_timer_reset(&GLOBAL.framerate.cap);
	
	gae_clock_update(gae_system.main_clock);
	
	gae_event_system_update(gae_system.event_system);
	
	gae_timer_update(&test->timer, gae_system.main_clock);
	test->onUpdate(test);
	if (0 >= test->timer.currentTime) {
		test->onStop(test);
		++GLOBAL.testIndex;
		if (GLOBAL.testIndex >= test_count)
			isRunning = 0;
		else
			GLOBAL.tests[GLOBAL.testIndex].onStart(&GLOBAL.tests[GLOBAL.testIndex]);
	}
	
	gae_timer_update(&GLOBAL.framerate.cap, gae_system.main_clock);
	
#ifndef __EMSCRIPTEN__
	gae_system_delay(gae_max(0, GLOBAL.framerate.ticksPerFrame - GLOBAL.framerate.cap.currentTime));
#endif
}
