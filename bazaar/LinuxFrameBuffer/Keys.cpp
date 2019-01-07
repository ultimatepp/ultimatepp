#include <linux/input.h>
#include "LinuxFrameBuffer.h"

using namespace Upp;

struct{
	uint32 a;
	uint32 b;
}static const libinput_key_map[] = {

	{ K_BACKSPACE,		KEY_BACKSPACE },
	{ K_TAB,			KEY_TAB },
	{ K_SPACE,			KEY_SPACE },
	{ K_ENTER,			KEY_ENTER },
	
	{ K_GRAVE,			KEY_GRAVE },
	{ K_MINUS,			KEY_MINUS },
	{ K_PLUS,			KEY_EQUAL },
	{ K_LBRACKET,		KEY_LEFTBRACE },
	{ K_RBRACKET,		KEY_RIGHTBRACE },
	{ K_BACKSLASH,		KEY_BACKSLASH },
	{ K_SEMICOLON,		KEY_SEMICOLON },
	{ K_QUOTEDBL,		KEY_APOSTROPHE },
	{ K_COMMA,			KEY_COMMA },
	{ K_PERIOD,			KEY_DOT },
	{ K_SLASH,			KEY_SLASH },

	{ K_SHIFT_LEFT,		KEY_LEFTSHIFT },
	{ K_CTRL_LEFT,		KEY_LEFTCTRL },
	{ K_ALT_LEFT,		KEY_LEFTALT },

	{ K_SHIFT_RIGHT,	KEY_RIGHTSHIFT},
	{ K_CTRL_RIGHT,		KEY_RIGHTCTRL},
	{ K_ALT_RIGHT,		KEY_RIGHTALT},
	
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

	{ K_DIVIDE,			KEY_KPSLASH },
	{ K_MULTIPLY,		KEY_KPASTERISK },
	{ K_SUBTRACT,		KEY_KPMINUS },
	{ K_ADD,			KEY_KPPLUS },
	{ K_ENTER,			KEY_KPENTER },
	{ K_DECIMAL,		KEY_KPDOT },
	{ K_NUMPAD0,		KEY_KP0 },
	{ K_NUMPAD1,		KEY_KP1 },
	{ K_NUMPAD2,		KEY_KP2 },
	{ K_NUMPAD3,		KEY_KP3 },
	{ K_NUMPAD4,		KEY_KP4 },
	{ K_NUMPAD5,		KEY_KP5 },
	{ K_NUMPAD6,		KEY_KP6 },
	{ K_NUMPAD7,		KEY_KP7 },
	{ K_NUMPAD8,		KEY_KP8 },
	{ K_NUMPAD9,		KEY_KP9 },

	{ K_A,			    KEY_A },
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
	
	{ K_F1,				KEY_F1 },
	{ K_F2,				KEY_F2 },
	{ K_F3,				KEY_F3 },
	{ K_F4,				KEY_F4 },
	{ K_F5,				KEY_F5 },
	{ K_F6,				KEY_F6 },
	{ K_F7,				KEY_F7 },
	{ K_F8,				KEY_F8 },
	{ K_F9,				KEY_F9 },
	{ K_F10,			KEY_F10 },
	{ K_F11,			KEY_F11 },
	{ K_F12,			KEY_F12 },
};


uint32 LinuxFrameBuffer::TranslateScanCode(uint32 chr)
{
	uint32 out = 0;
	
	for(auto& m : libinput_key_map){
		if(chr == m.b){
			out = m.a;
			break;
		}
	}

	if(out == K_ALT_KEY || out == K_CTRL_KEY || out == K_SHIFT_KEY) return out;

	if(GetCtrl())	out |= K_CTRL;
	if(GetAlt())	out |= K_ALT;
	if(GetShift())	out |= K_SHIFT;
	
	return out;
}


String LinuxFrameBuffer::ScanCodeToUtf8(uint32 chr)
{
	char buff[256];
	auto size = xkb_state_key_get_utf8(input.kb_state, chr + 8, buff, sizeof(buff));
	return String(buff, size);
}

