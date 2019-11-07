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

typedef struct global_s {
	framerate_t framerate;
	gae_graphics_texture_t frameBuffer;
	gae_font_t font;
	gae_button_t button[4];
	gae_pointer_t pointer;
} global_t;

global_t GLOBAL;

static gae_frame_t SetFrame(gae_graphics_texture_t* _texture, int _x, int _y, int _w, int _h)
{
	gae_frame_t frame;
	
	gae_rect_t rect;
	rect.x = _x;
	rect.y = _y;
	rect.w = _w;
	rect.h = _h;
	
	frame.texture = _texture;
	frame.src = rect;

	return frame;
}

static void SetupButton(gae_button_t* _button, int _x, int _y, int _w, int _h, gae_graphics_texture_t* const _texture)
{
	gae_rect_t dims;
	gae_button_renderable_t graphics[gae_button_state_count];
	gae_frame_t frame;
	
	dims.x = _x;
	dims.y = _y;
	dims.w = _w;
	dims.h = _h;
	
	frame = SetFrame(_texture, 0, 0, 16, 8); gae_button_create_sprite(_button, &graphics[gae_button_inactive], &frame);
	frame = SetFrame(_texture, 0, 8, 16, 8); gae_button_create_sprite(_button, &graphics[gae_button_active], &frame);
	frame = SetFrame(_texture, 0, 16, 16, 8); gae_button_create_sprite(_button, &graphics[gae_button_hover], &frame);
	frame = SetFrame(_texture, 0, 24, 26, 8); gae_button_create_sprite(_button, &graphics[gae_button_pressed], &frame);
	
	gae_button_init(_button, graphics, &dims, 0);
}

static void SetupTextButton(gae_button_t* _button, int _x, int _y, int _w, int _h, gae_graphics_texture_t* const _texture, gae_font_t* const _font, const char* _text)
{
	gae_rect_t dims;
	gae_button_renderable_t graphics[gae_button_state_count];
	gae_frame_t frame;
	
	dims.x = _x;
	dims.y = _y;
	dims.w = _w;
	dims.h = _h;
	
	frame = SetFrame(_texture, 0, 0, 16, 8); gae_button_create_sprite_with_text(_button, &graphics[gae_button_inactive], &frame, _font, _text);
	frame = SetFrame(_texture, 0, 8, 16, 8); gae_button_create_sprite_with_text(_button, &graphics[gae_button_active], &frame, _font, _text);
	frame = SetFrame(_texture, 0, 16, 16, 8); gae_button_create_sprite_with_text(_button, &graphics[gae_button_hover], &frame, _font, _text);
	frame = SetFrame(_texture, 0, 24, 26, 8); gae_button_create_sprite_with_text(_button, &graphics[gae_button_pressed], &frame, _font, _text);
	
	gae_button_init(_button, graphics, &dims, 0);
}

static void SetupNineslice(int _x, int _y, int _w, int _h, gae_graphics_texture_t* const _texture)
{
	gae_rect_t dims;
	gae_button_renderable_t graphics[gae_button_state_count];
	unsigned int i = 0;
	
	dims.x = _x;
	dims.y = _y;
	dims.w = _w;
	dims.h = _h;
	
	for (i = 0; i < gae_button_state_count; ++i) {
		gae_nineslice_t nineslice;
		gae_nineslice_dims_t dims;
		dims.x[0] = 0;	dims.y[0] = 0 + (i * 48);
		dims.x[1] = 16;	dims.y[1] = 16 + (i * 48);
		dims.x[2] = 32;	dims.y[2] = 32 + (i * 48);
		dims.x[3] = 48;	dims.y[3] = 48 + (i * 48);
	
		gae_nineslice_init(&nineslice, &dims, _texture);
		gae_button_create_nineslice(&GLOBAL.button[2], &graphics[i], &nineslice);
		gae_nineslice_destroy(&nineslice);
	}
	
	gae_button_init(&GLOBAL.button[2], graphics, &dims, 0);
}

static void SetupTextNineslice(int _x, int _y, int _w, int _h, gae_graphics_texture_t* const _texture, gae_font_t* _font, const char* _text)
{
	gae_rect_t dims;
	gae_button_renderable_t graphics[gae_button_state_count];
	unsigned int i = 0;
	
	dims.x = _x;
	dims.y = _y;
	dims.w = _w;
	dims.h = _h;
	
	for (i = 0; i < gae_button_state_count; ++i) {
		gae_nineslice_t nineslice;
		gae_nineslice_dims_t dims;
		dims.x[0] = 0;	dims.y[0] = 0 + (i * 48);
		dims.x[1] = 16;	dims.y[1] = 16 + (i * 48);
		dims.x[2] = 32;	dims.y[2] = 32 + (i * 48);
		dims.x[3] = 48;	dims.y[3] = 48 + (i * 48);
	
		gae_nineslice_init(&nineslice, &dims, _texture);
		gae_button_create_nineslice_with_text(&GLOBAL.button[3], &graphics[i], &nineslice, _font, _text);
		gae_nineslice_destroy(&nineslice);
	}
	
	gae_button_init(&GLOBAL.button[3], graphics, &dims, 0);
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
	gae_graphics_texture_t* buttonTexture, *ninesliceTexture, *fontTexture;
	
	gae_system.main_clock = gae_clock_init(gae_alloc(sizeof(gae_clock_t)));
	gae_timer_init(&GLOBAL.framerate.cap, gae_system.main_clock);
	GLOBAL.framerate.fps = 1.0F / 60.0F;
	GLOBAL.framerate.ticksPerFrame = GLOBAL.framerate.fps * 1000.0F;
	
	gae_system.event_system = gae_event_system_init(gae_alloc(sizeof(gae_event_system_t)), &GLOBAL);
	
	/* See: pacoup.com/2011/06/12/list-of-true-169-resolutions */
	gae_system.graphics.window = gae_graphics_window_init(gae_alloc(sizeof(gae_graphics_window_t)), "Button Test", 0x2FFF0000U, 0x2FFF0000U, 640, 360, 0);
	gae_system.graphics.context = gae_graphics_context_init(gae_alloc(sizeof(gae_graphics_context_t)), gae_system.graphics.window, 0);
	
	gae_graphics_context_init_render_target(gae_system.graphics.context, &GLOBAL.frameBuffer, 640, 360, 32);
	
	fontTexture = gae_alloc(sizeof(gae_graphics_texture_t));
	gae_graphics_texture_init(fontTexture);
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, "data/8x8font.bmp", fontTexture);
	gae_font_init(&GLOBAL.font, 8, 8, fontTexture, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@");
	
	buttonTexture = gae_alloc(sizeof(gae_graphics_texture_t));
	gae_graphics_texture_init(buttonTexture);
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, "data/button.bmp", buttonTexture);
	SetupButton(&GLOBAL.button[0], 32, 24, 64, 48, buttonTexture);
	SetupTextButton(&GLOBAL.button[1], 32, 96, 64, 48, buttonTexture, &GLOBAL.font, "Right");
	
	ninesliceTexture = gae_alloc(sizeof(gae_graphics_texture_t));
	gae_graphics_texture_init(ninesliceTexture);
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, "data/button-nineslice.bmp", ninesliceTexture);
	SetupNineslice(32, 192, 128, 64, ninesliceTexture);
	SetupTextNineslice(320, 64, 128, 32, ninesliceTexture, &GLOBAL.font, "Nineslice");
	
	gae_system.event_system->onMouseEvent = OnMouseEvent;
	gae_system.event_system->onQuit = OnQuit;
	
	(void)(argc);
	(void)(argv);
	
#if !defined(__EMSCRIPTEN__)
	while (isRunning)
		main_loop();
	
	gae_button_destroy(&GLOBAL.button[0]);
	gae_button_destroy(&GLOBAL.button[1]);
	gae_button_destroy(&GLOBAL.button[2]);
	gae_button_destroy(&GLOBAL.button[3]);
	
	gae_font_destroy(&GLOBAL.font);

	gae_graphics_texture_destroy(buttonTexture);
	gae_graphics_texture_destroy(ninesliceTexture);
	gae_graphics_texture_destroy(fontTexture);
	gae_graphics_window_destroy(gae_system.graphics.window);
	gae_graphics_context_destroy(gae_system.graphics.context);
	gae_event_system_destroy(gae_system.event_system);
	gae_clock_destroy(gae_system.main_clock);

	gae_free(gae_system.event_system);
	gae_free(gae_system.graphics.window);
	gae_free(gae_system.graphics.context);
	gae_free(gae_system.main_clock);
	gae_free(buttonTexture);
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

	gae_button_update(&GLOBAL.button[0], &GLOBAL.pointer.pos, GLOBAL.pointer.isDown[GAE_MOUSE_BUTTON_LEFT]);
	gae_button_update(&GLOBAL.button[1], &GLOBAL.pointer.pos, GLOBAL.pointer.isDown[GAE_MOUSE_BUTTON_RIGHT]);
	gae_button_update(&GLOBAL.button[2], &GLOBAL.pointer.pos, GLOBAL.pointer.isDown[GAE_MOUSE_BUTTON_LEFT]);
	gae_button_update(&GLOBAL.button[3], &GLOBAL.pointer.pos, GLOBAL.pointer.isDown[GAE_MOUSE_BUTTON_LEFT]);
	
	gae_font_writeText(&GLOBAL.font, "Hello World!@", 16, 16, 1);
	
	gae_graphics_context_set_render_target(gae_system.graphics.context, 0);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &GLOBAL.frameBuffer, 0, 0);
	
	gae_graphics_context_present(gae_system.graphics.context);
	
	gae_timer_update(&GLOBAL.framerate.cap, gae_system.main_clock);
	
#ifndef __EMSCRIPTEN__
	gae_system_delay(gae_max(0, GLOBAL.framerate.ticksPerFrame - GLOBAL.framerate.cap.currentTime));
#endif
}
