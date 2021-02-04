#include <gae.h>

#include <stdio.h>
#include <stdlib.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#include <emscripten/html5.h>
#include "SDL2/SDL.h"

void emscripten_fullscreen()
{
        emscripten_request_fullscreen(NULL, EM_FALSE);
        SDL_SetWindowFullscreen(gae_system.graphics.window->data, SDL_WINDOW_FULLSCREEN);
        printf("Fullscreening...\n");
}
#endif


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
    gae_sprite_t singleFrameSprite;
    gae_sprite_anim_t animatedSprite;
    gae_graphics_texture_t spriteTexture;
} global_t;

global_t GLOBAL;

static void OnQuit(void* userDatum, gae_event_t* const event)
{
	(void)(userDatum);
	(void)(event);
	
	isRunning = 0;
}

static void onOpenWindow()
{	
	/* See: pacoup.com/2011/06/12/list-of-true-169-resolutions */
	gae_system.graphics.window = gae_graphics_window_init(gae_alloc(sizeof(gae_graphics_window_t)), "Sprite Test", 0x2FFF0000U, 0x2FFF0000U, 640, 360, GAE_WINDOW_WINDOWED);
	gae_system.graphics.context = gae_graphics_context_init(gae_alloc(sizeof(gae_graphics_context_t)), gae_system.graphics.window, 0);
	
	gae_graphics_context_init_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer, 640, 360, 32);
}

static void onUpdateWindow()
{
	gae_graphics_context_set_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer);
	gae_graphics_context_clear(gae_system.graphics.context);

    gae_sprite_draw(&GLOBAL.singleFrameSprite);
    gae_sprite_anim_draw(&GLOBAL.animatedSprite);
	
	gae_graphics_context_set_render_target(gae_system.graphics.context, 0);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &GLOBAL.frameBuffer, 0, 0);
	
	gae_graphics_context_present(gae_system.graphics.context);
}

static void onCloseWindow()
{
	gae_graphics_window_destroy(gae_system.graphics.window);
	gae_graphics_context_destroy(gae_system.graphics.context);
}

static void setupSpriteTexture()
{
    gae_graphics_texture_init(&GLOBAL.spriteTexture);
    gae_graphics_context_texture_load_from_file(gae_system.graphics.context, "data/grubs.bmp", &GLOBAL.spriteTexture);
}

static void setupSingleFrameSprite()
{
    gae_rect_t rect = {0, 0, 32, 32};

	gae_sprite_init(&GLOBAL.singleFrameSprite, &GLOBAL.spriteTexture, &rect);
}

static void setupAnimatedSprite()
{
    gae_rect_t rects[] = {
        {0, 0, 32, 32}
    ,   {32, 0, 32, 32}
    ,   {64, 0, 32, 32}
    ,   {32, 0, 32, 32}
    };

    gae_sprite_anim_init(&GLOBAL.animatedSprite, &GLOBAL.spriteTexture, rects, 4);
    GLOBAL.animatedSprite.dst.x = 32;
    GLOBAL.animatedSprite.dst.w = 32;
    GLOBAL.animatedSprite.dst.h = 32;
    GLOBAL.animatedSprite.animSpeed = 250;
}

int gae_main(int argc, char** argv)
{
    (void)(argc);
	(void)(argv);

	gae_system.main_clock = gae_clock_init(gae_alloc(sizeof(gae_clock_t)));
	gae_timer_init(&GLOBAL.framerate.cap, gae_system.main_clock);
	GLOBAL.framerate.fps = 1.0F / 60.0F;
	GLOBAL.framerate.ticksPerFrame = GLOBAL.framerate.fps * 1000.0F;
	
	gae_system.event_system = gae_event_system_init(gae_alloc(sizeof(gae_event_system_t)), &GLOBAL);
	gae_system.event_system->onQuit = OnQuit;
	
	onOpenWindow();

    setupSpriteTexture();
    setupSingleFrameSprite();
    setupAnimatedSprite();

#if !defined(__EMSCRIPTEN__)
	while (isRunning)
		main_loop();
	
    onCloseWindow();

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
    gae_sprite_anim_update(&GLOBAL.animatedSprite, GLOBAL.framerate.ticksPerFrame);
	
	gae_event_system_update(gae_system.event_system);
	
	gae_timer_update(&GLOBAL.framerate.cap, gae_system.main_clock);
	
#ifndef __EMSCRIPTEN__
	gae_system_delay(gae_max(0, GLOBAL.framerate.ticksPerFrame - GLOBAL.framerate.cap.currentTime));
#endif

    onUpdateWindow();
}
