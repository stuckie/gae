#include "gae.h"

gae_pointer_t* gae_pointer_init(gae_pointer_t* pointer)
{
	unsigned int i = 0;
	for (; i < GAE_MOUSE_BUTTON_COUNT; ++i)
		pointer->isDown[i] = 0;
	
	pointer->pos.x = 0;
	pointer->pos.y = 0;
	
	return pointer;
}

gae_keyboard_t* gae_keyboard_init(gae_keyboard_t* keyboard)
{
	unsigned int i = 0;
	for (; i < 512; ++i)
		keyboard->down[i] = 0;
	
	return keyboard;
}
