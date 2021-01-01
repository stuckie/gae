#include "gae.h"

gae_button_t* gae_button_init(gae_button_t* button, gae_button_renderable_t* const graphics, gae_rect_t* const dims, void* userData)
{
	unsigned int i = 0;
	for (; i < gae_button_state_count; ++i)
		button->graphics[i] = graphics[i];
	
	button->onEnter = 0;
	button->onExit = 0;
	button->onPressed = 0;
	button->onReleased = 0;
	
	button->dims = *dims;
	button->userData = userData;
	
	button->state = gae_button_active;
	
	return button;
}

gae_button_t* gae_button_update(gae_button_t* button, gae_point_2d_t* const cursor, int isClicked)
{
	enum gae_button_state lastState = button->state;
	
	if (gae_button_inactive != button->state) {
		if (0 != gae_point2d_in_rectangle(*cursor, button->dims))
			button->state = 0 == isClicked ? gae_button_hover : gae_button_pressed;
		else
			button->state = gae_button_active;
		
		if (gae_button_hover == button->state && gae_button_hover != lastState) {
			if (0 != button->onEnter) (*button->onEnter)(button->userData);
		} else if (gae_button_hover != button->state && gae_button_hover == lastState) {
			if (0 != button->onExit) (*button->onExit)(button->userData);
		} 
		
		if (gae_button_pressed == button->state && gae_button_pressed != lastState) {
			if (0 != button->onPressed) (*button->onPressed)(button->userData);
		} else if (gae_button_pressed != button->state && gae_button_pressed == lastState && 0 == isClicked) {
			if (0 != button->onReleased) (*button->onReleased)(button->userData);
		}
	}
	
	return button;
}

gae_button_t* gae_button_render(gae_button_t* button)
{
	gae_button_renderable_t frame = button->graphics[button->state];

	(*frame.onRender)(frame.data);

	return button;
}

gae_button_t* gae_button_destroy(gae_button_t* button)
{
	unsigned int i = 0;
	for (; i < gae_button_state_count; ++i)
		gae_free(button->graphics[i].data);
	
	return button;
}

typedef struct sprite_button_s {
	gae_frame_t frame;
	gae_rect_t* dims;
} sprite_button_t;

static void OnButtonSpriteRender(void* data)
{
	sprite_button_t* button = data;
	gae_graphics_context_blit_texture(gae_system.graphics.context, button->frame.texture, &button->frame.src, button->dims);
}

gae_button_renderable_t* gae_button_create_sprite(gae_button_t* const button, gae_button_renderable_t* renderable, gae_frame_t* const frame)
{
	sprite_button_t* spriteButton = gae_alloc(sizeof(sprite_button_t));
	spriteButton->frame = *frame;
	spriteButton->dims = &button->dims;
	
	renderable->data = spriteButton;
	renderable->onRender = OnButtonSpriteRender;
	
	return renderable;
}

typedef struct sprite_button_with_text_s {
	sprite_button_t button;
	gae_font_t* font;
	
	const char* text;
	unsigned int offsetX;
	unsigned int offsetY;
	unsigned int spacing;
} sprite_button_with_text_t;

static void OnButtonSpriteWithTextRender(void* data)
{
	sprite_button_with_text_t* button = data;
	OnButtonSpriteRender(&button->button);
	gae_font_writeText(button->font, button->text, button->button.dims->x + button->offsetX, button->button.dims->y + button->offsetY, button->spacing);
}

gae_button_renderable_t* gae_button_create_sprite_with_text(gae_button_t* const button, gae_button_renderable_t* renderable, gae_frame_t* const frame, gae_font_t* font, const char* text)
{
	sprite_button_with_text_t* spriteButton = gae_alloc(sizeof(sprite_button_with_text_t));
	spriteButton->button.frame = *frame;
	spriteButton->button.dims = &button->dims;
	spriteButton->font = font;
	spriteButton->text = text;
	
	spriteButton->offsetX = 8;
	spriteButton->offsetY = 18;
	spriteButton->spacing = 1;
	
	renderable->data = spriteButton;
	renderable->onRender = OnButtonSpriteWithTextRender;

	return renderable;
}

typedef struct nineslice_button_s {
	gae_nineslice_t nineslice;
	gae_rect_t* dims;
} nineslice_button_t;

static void OnButtonNinesliceRender(void* data)
{
	nineslice_button_t* ninesliceButton = data;
	gae_nineslice_resize(&ninesliceButton->nineslice, ninesliceButton->dims->w, ninesliceButton->dims->h);
	
	ninesliceButton->nineslice.dst.x = ninesliceButton->dims->x;
	ninesliceButton->nineslice.dst.y = ninesliceButton->dims->y;
	
	gae_nineslice_render(&ninesliceButton->nineslice);
}

gae_button_renderable_t* gae_button_create_nineslice(gae_button_t* button, gae_button_renderable_t* renderable, gae_nineslice_t* const nineslice)
{
	nineslice_button_t* ninesliceButton = gae_alloc(sizeof(nineslice_button_t));
	ninesliceButton->nineslice = *nineslice;
	ninesliceButton->dims = &button->dims;
	gae_nineslice_resize(&ninesliceButton->nineslice, button->dims.w, button->dims.h);
	
	renderable->data = ninesliceButton;
	renderable->onRender = OnButtonNinesliceRender;
	
	return renderable;
}

typedef struct nineslice_button_with_text_s {
	nineslice_button_t button;
	gae_font_t* font;
	
	const char* text;
	unsigned int offsetX;
	unsigned int offsetY;
	unsigned int spacing;
} nineslice_button_with_text_t;

static void OnButtonNinesliceWithTextRender(void* data)
{
	nineslice_button_with_text_t* button = data;
	OnButtonNinesliceRender(&button->button);
	
	gae_font_writeText(	button->font
					, 	button->text
					,	button->button.nineslice.dst.x + button->button.nineslice.dimensions[gae_nineslice_centre].x + button->offsetX
					,	button->button.nineslice.dst.y + button->button.nineslice.dimensions[gae_nineslice_centre].y + button->offsetY
					,	button->spacing);
}

gae_button_renderable_t* gae_button_create_nineslice_with_text(gae_button_t* button, gae_button_renderable_t* renderable, gae_nineslice_t* const nineslice, gae_font_t* const font, const char* text)
{
	nineslice_button_with_text_t* ninesliceButton = gae_alloc(sizeof(nineslice_button_with_text_t));
	ninesliceButton->button.nineslice = *nineslice;
	ninesliceButton->button.dims = &button->dims;
	gae_nineslice_resize(&ninesliceButton->button.nineslice, button->dims.w, button->dims.h);
	
	ninesliceButton->font = font;
	ninesliceButton->text = text;
	
	ninesliceButton->offsetX = 0;
	ninesliceButton->offsetY = 0;
	ninesliceButton->spacing = 1;
	
	renderable->data = ninesliceButton;
	renderable->onRender = OnButtonNinesliceWithTextRender;
	
	return renderable;
}
