#include <linux/input.h>
#include "LinuxFrameBuffer.h"

using namespace Upp;

static const VectorMap<uint32_t, uint32_t> libinput_key_map = {
    {KEY_BACKSPACE,     K_BACKSPACE},
    {KEY_TAB,           K_TAB},
    {KEY_SPACE,         K_SPACE},
    {KEY_ENTER,         K_ENTER},
    
    {KEY_GRAVE,         K_GRAVE},
    {KEY_MINUS,         K_MINUS},
    {KEY_EQUAL,         K_PLUS},
    {KEY_LEFTBRACE,     K_LBRACKET},
    {KEY_RIGHTBRACE,    K_RBRACKET},
    {KEY_BACKSLASH,     K_BACKSLASH},
    {KEY_SEMICOLON,     K_SEMICOLON},
    {KEY_APOSTROPHE,    K_QUOTEDBL},
    {KEY_COMMA,         K_COMMA},
    {KEY_DOT,           K_PERIOD},
    {KEY_SLASH,         K_SLASH},
    
    {KEY_LEFTSHIFT,     K_SHIFT_LEFT},
    {KEY_LEFTCTRL,      K_CTRL_LEFT},
    {KEY_LEFTALT,       K_ALT_LEFT},
    
    {KEY_RIGHTSHIFT,    K_SHIFT_RIGHT},
    {KEY_RIGHTCTRL,     K_CTRL_RIGHT},
    {KEY_RIGHTALT,      K_ALT_RIGHT},
    
    {KEY_CAPSLOCK,      K_CAPSLOCK},
    {KEY_ESC,           K_ESCAPE},
    {KEY_PAGEUP,        K_PAGEUP},
    {KEY_PAGEDOWN,      K_PAGEDOWN},
    {KEY_END,           K_END},
    {KEY_HOME,          K_HOME},
    {KEY_LEFT,          K_LEFT},
    {KEY_UP,            K_UP},
    {KEY_RIGHT,         K_RIGHT},
    {KEY_DOWN,          K_DOWN},
    {KEY_INSERT,        K_INSERT},
    {KEY_DELETE,        K_DELETE},
    
    {KEY_KPSLASH,       K_DIVIDE},
    {KEY_KPASTERISK,    K_MULTIPLY},
    {KEY_KPMINUS,       K_SUBTRACT},
    {KEY_KPPLUS,        K_ADD},
    {KEY_KPENTER,       K_ENTER},
    {KEY_KPDOT,         K_DECIMAL},
    {KEY_KP0,           K_NUMPAD0},
    {KEY_KP1,           K_NUMPAD1},
    {KEY_KP2,           K_NUMPAD2},
    {KEY_KP3,           K_NUMPAD3},
    {KEY_KP4,           K_NUMPAD4},
    {KEY_KP5,           K_NUMPAD5},
    {KEY_KP6,           K_NUMPAD6},
    {KEY_KP7,           K_NUMPAD7},
    {KEY_KP8,           K_NUMPAD8},
    {KEY_KP9,           K_NUMPAD9},
    
    {KEY_A,             K_A},
    {KEY_B,             K_B},
    {KEY_C,             K_C},
    {KEY_D,             K_D},
    {KEY_E,             K_E},
    {KEY_F,             K_F},
    {KEY_G,             K_G},
    {KEY_H,             K_H},
    {KEY_I,             K_I},
    {KEY_J,             K_J},
    {KEY_K,             K_K},
    {KEY_L,             K_L},
    {KEY_M,             K_M},
    {KEY_N,             K_N},
    {KEY_O,             K_O},
    {KEY_P,             K_P},
    {KEY_Q,             K_Q},
    {KEY_R,             K_R},
    {KEY_S,             K_S},
    {KEY_T,             K_T},
    {KEY_U,             K_U},
    {KEY_V,             K_V},
    {KEY_W,             K_W},
    {KEY_X,             K_X},
    {KEY_Y,             K_Y},
    {KEY_Z,             K_Z},
    {KEY_0,             K_0},
    {KEY_1,             K_1},
    {KEY_2,             K_2},
    {KEY_3,             K_3},
    {KEY_4,             K_4},
    {KEY_5,             K_5},
    {KEY_6,             K_6},
    {KEY_7,             K_7},
    {KEY_8,             K_8},
    {KEY_9,             K_9},
    
    {KEY_F1,            K_F1},
    {KEY_F2,            K_F2},
    {KEY_F3,            K_F3},
    {KEY_F4,            K_F4},
    {KEY_F5,            K_F5},
    {KEY_F6,            K_F6},
    {KEY_F7,            K_F7},
    {KEY_F8,            K_F8},
    {KEY_F9,            K_F9},
    {KEY_F10,           K_F10},
    {KEY_F11,           K_F11},
    {KEY_F12,           K_F12},
};


uint32 LinuxFrameBuffer::TranslateScanCode(uint32 chr)
{
    uint32 out = 0;
    
    auto idx = libinput_key_map.Find(chr);
    if (idx >= 0) out = libinput_key_map[idx];

    if(out == K_ALT_KEY || out == K_CTRL_KEY || out == K_SHIFT_KEY) return out;

    if(GetCtrl())   out |= K_CTRL;
    if(GetAlt())    out |= K_ALT;
    if(GetShift())  out |= K_SHIFT;
    
    return out;
}


String LinuxFrameBuffer::ScanCodeToUtf8(uint32 chr)
{
    char buff[256];
    auto size = xkb_state_key_get_utf8(input.kb_state, chr + 8, buff, sizeof(buff));
    return String(buff, size);
}

