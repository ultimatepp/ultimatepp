#include <linux/input.h>
#include "LinuxFrameBuffer.h"

using namespace Upp;

struct{
	uint32 a;
	uint32 b;
}static const libinput_key_map[] = {

	{ K_BACK,			KEY_BACK },
	{ K_BACKSPACE,		KEY_BACKSPACE },
	{ K_TAB,			KEY_TAB },
	{ K_SPACE,			KEY_SPACE },
	{ K_ENTER,			KEY_ENTER },

	{ K_SHIFT_KEY,		KEY_LEFTSHIFT },
	{ K_CTRL_KEY,		KEY_LEFTCTRL },
	{ K_ALT_KEY,		KEY_LEFTALT },
	
	{ K_CAPSLOCK,		KEY_CAPSLOCK },
	{ K_ESCAPE,			KEY_ESC },
	{ K_PAGEUP,			KEY_PAGEUP },
	{ K_PAGEDOWN,		KEY_PAGEDOWN },
	{ K_END,			KEY_END },
	{ K_HOME,			KEY_HOME },
	{ K_LEFT,			KEY_LEFT },
	{ K_UP,				KEY_UP },
	{ K_RIGHT,			KEY_RIGHT },
	{ K_DOWN,			KEY_DOWN },
	{ K_INSERT,			KEY_INSERT },
	{ K_DELETE,			KEY_DELETE },

	{ K_NUMPAD0,		KEY_NUMERIC_0 },
	{ K_NUMPAD1,		KEY_NUMERIC_1 },
	{ K_NUMPAD2,		KEY_NUMERIC_2 },
	{ K_NUMPAD3,		KEY_NUMERIC_3 },
	{ K_NUMPAD4,		KEY_NUMERIC_4 },
	{ K_NUMPAD5,		KEY_NUMERIC_5 },
	{ K_NUMPAD6,		KEY_NUMERIC_6 },
	{ K_NUMPAD7,		KEY_NUMERIC_7 },
	{ K_NUMPAD8,		KEY_NUMERIC_8 },
	{ K_NUMPAD9,		KEY_NUMERIC_9 },

	{ K_A,				KEY_A },
//	{ 'a',				KEY_A },
	{ K_B,				KEY_B },
	{ K_C,				KEY_C },
	{ K_D,				KEY_D },
	{ K_E,				KEY_E },
	{ K_F,				KEY_F },
	{ K_G,				KEY_G },
	{ K_H,				KEY_H },
	{ K_I,				KEY_I },
	{ K_J,				KEY_J },
	{ K_K,				KEY_K },
	{ K_L,				KEY_L },
	{ K_M,				KEY_M },
	{ K_N,				KEY_N },
	{ K_O,				KEY_O },
	{ K_P,				KEY_P },
	{ K_Q,				KEY_Q },
	{ K_R,				KEY_R },
	{ K_S,				KEY_S },
	{ K_T,				KEY_T },
	{ K_U,				KEY_U },
	{ K_V,				KEY_V },
	{ K_W,				KEY_W },
	{ K_X,				KEY_X },
	{ K_Y,				KEY_Y },
	{ K_Z,				KEY_Z },
	{ K_0,				KEY_0 },
	{ K_1,				KEY_1 },
	{ K_2,				KEY_2 },
	{ K_3,				KEY_3 },
	{ K_4,				KEY_4 },
	{ K_5,				KEY_5 },
	{ K_6,				KEY_6 },
	{ K_7,				KEY_7 },
	{ K_8,				KEY_8 },
	{ K_9,				KEY_9 },
};


uint32 LinuxFrameBuffer::TranslateKeyEvent(uint32 chr)
{
	for(auto& m : libinput_key_map){
		if(chr == m.b){
			chr = m.a;
			break;
		}
	}

	if(chr == K_ALT_KEY || chr == K_CTRL_KEY || chr == K_SHIFT_KEY) return chr;

	if(GetCtrl())	chr |= K_CTRL;
	if(GetAlt())	chr |= K_ALT;
	if(GetShift())	chr |= K_SHIFT;
	return chr;
}

