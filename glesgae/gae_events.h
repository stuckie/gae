#ifndef _GAE_EVENTS_H_
#define _GAE_EVENTS_H_

#include "gae_point.h"

typedef enum gae_event_type_e
{	GAE_EVENT_MOUSE_MOVE
,	GAE_EVENT_MOUSE_BUTTON
,	GAE_EVENT_KEYBOARD
,	GAE_EVENT_QUIT
,	GAE_EVENT_WINDOW_FULLSCREEN
} gae_event_type;

typedef enum gae_mouse_button_e
{	GAE_MOUSE_BUTTON_ANY	= 0
,	GAE_MOUSE_BUTTON_LEFT 	= 1
,	GAE_MOUSE_BUTTON_MIDDLE	= 2
,	GAE_MOUSE_BUTTON_RIGHT	= 3
,	GAE_MOUSE_BUTTON_UP		= 4
,	GAE_MOUSE_BUTTON_DOWN	= 5
,	GAE_MOUSE_BUTTON_COUNT	= 6
} gae_mouse_button;

typedef enum gae_keyboard_key_e
{	GAE_KEYBOARD_KEY_UNKNOWN = 0
,	GAE_KEYBOARD_KEY_A = 4
,	GAE_KEYBOARD_KEY_B = 5
,	GAE_KEYBOARD_KEY_C = 6
,	GAE_KEYBOARD_KEY_D = 7
,	GAE_KEYBOARD_KEY_E = 8
,	GAE_KEYBOARD_KEY_F = 9
,	GAE_KEYBOARD_KEY_G = 10
,	GAE_KEYBOARD_KEY_H = 11
,	GAE_KEYBOARD_KEY_I = 12
,	GAE_KEYBOARD_KEY_J = 13
,	GAE_KEYBOARD_KEY_K = 14
,	GAE_KEYBOARD_KEY_L = 15
,	GAE_KEYBOARD_KEY_M = 16
,	GAE_KEYBOARD_KEY_N = 17
,	GAE_KEYBOARD_KEY_O = 18
,	GAE_KEYBOARD_KEY_P = 19
,	GAE_KEYBOARD_KEY_Q = 20
,	GAE_KEYBOARD_KEY_R = 21
,	GAE_KEYBOARD_KEY_S = 22
,	GAE_KEYBOARD_KEY_T = 23
,	GAE_KEYBOARD_KEY_U = 24
,	GAE_KEYBOARD_KEY_V = 25
,	GAE_KEYBOARD_KEY_W = 26
,	GAE_KEYBOARD_KEY_X = 27
,	GAE_KEYBOARD_KEY_Y = 28
,	GAE_KEYBOARD_KEY_Z = 29

,	GAE_KEYBOARD_KEY_1 = 30
,	GAE_KEYBOARD_KEY_2 = 31
,	GAE_KEYBOARD_KEY_3 = 32
,	GAE_KEYBOARD_KEY_4 = 33
,	GAE_KEYBOARD_KEY_5 = 34
,	GAE_KEYBOARD_KEY_6 = 35
,	GAE_KEYBOARD_KEY_7 = 36
,	GAE_KEYBOARD_KEY_8 = 37
,	GAE_KEYBOARD_KEY_9 = 38
,	GAE_KEYBOARD_KEY_0 = 39

,	GAE_KEYBOARD_KEY_RETURN = 40
,	GAE_KEYBOARD_KEY_ESCAPE = 41
,	GAE_KEYBOARD_KEY_BACKSPACE = 42
,	GAE_KEYBOARD_KEY_TAB = 43
,	GAE_KEYBOARD_KEY_SPACE = 44

,	GAE_KEYBOARD_KEY_MINUS = 45
,	GAE_KEYBOARD_KEY_EQUALS = 46
,	GAE_KEYBOARD_KEY_LEFTBRACKET = 47
,	GAE_KEYBOARD_KEY_RIGHTBRACKET = 48
,	GAE_KEYBOARD_KEY_BACKSLASH = 49

,	GAE_KEYBOARD_KEY_F1 = 58
,	GAE_KEYBOARD_KEY_F2 = 59
,	GAE_KEYBOARD_KEY_F3 = 60
,	GAE_KEYBOARD_KEY_F4 = 61
,	GAE_KEYBOARD_KEY_F5 = 62
,	GAE_KEYBOARD_KEY_F6 = 63
,	GAE_KEYBOARD_KEY_F7 = 64
,	GAE_KEYBOARD_KEY_F8 = 65
,	GAE_KEYBOARD_KEY_F9 = 66
,	GAE_KEYBOARD_KEY_F10 = 67
,	GAE_KEYBOARD_KEY_F11 = 68
,	GAE_KEYBOARD_KEY_F12 = 69

,	GAE_KEYBOARD_KEY_HOME = 74
,	GAE_KEYBOARD_KEY_PAGEUP = 75
,	GAE_KEYBOARD_KEY_DELETE = 76
,	GAE_KEYBOARD_KEY_END = 77
,	GAE_KEYBOARD_KEY_PAGEDOWN = 78
,	GAE_KEYBOARD_KEY_RIGHT = 79
,	GAE_KEYBOARD_KEY_LEFT = 80
,	GAE_KEYBOARD_KEY_DOWN = 81
,	GAE_KEYBOARD_KEY_UP = 82
} gae_keyboard_key;

typedef struct gae_event_s {
	gae_event_type type;
	void* event;
} gae_event_t;

typedef void (*gae_event_callback_t)(void* userDatum, gae_event_t* const event);
typedef struct gae_event_system_s {
	gae_event_callback_t onQuit;
	gae_event_callback_t onWindowEvent;
	gae_event_callback_t onMouseEvent;
	gae_event_callback_t onKeyboardEvent;
	gae_event_callback_t onJoystickEvent;
	gae_event_callback_t onControllerEvent;
	gae_event_callback_t onTouchEvent;
	void* userDatum;
} gae_event_system_t;

typedef struct gae_pointer_s {
	gae_point_2d_t pos;
	unsigned int isDown[GAE_MOUSE_BUTTON_COUNT];
} gae_pointer_t;

typedef struct gae_pointer_move_event_s {
	unsigned int x;
	unsigned int y;
} gae_pointer_move_event_t;

typedef struct gae_pointer_button_event_s {
	unsigned int x;
	unsigned int y;
	unsigned int buttonId;
	unsigned int isDown;
} gae_pointer_button_event_t;

typedef struct gae_keyboard_s {
	unsigned int down[512]; /* surely no-one has more than 512 keys... */
} gae_keyboard_t;

typedef struct gae_keyboard_event_s {
	unsigned int key;
	unsigned int isDown;
} gae_keyboard_event_t;

/* Initialise a Pointer */
gae_pointer_t* gae_pointer_init(gae_pointer_t* pointer);

/* Initialise a Keyboard */
gae_keyboard_t* gae_keyboard_init(gae_keyboard_t* keyboard);

/* Initialise an event system with the given user data */
gae_event_system_t* gae_event_system_init(gae_event_system_t* system, void* userDatum);

/* Update the event system */
gae_event_system_t* gae_event_system_update(gae_event_system_t* const system);

/* Destroy an event system */
gae_event_system_t* gae_event_system_destroy(gae_event_system_t* system);

#endif

