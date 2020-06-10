#include <libinput.h>
#include <linux/input.h>
#include "LinuxFrameBuffer.h"

using namespace Upp;


void LinuxFrameBuffer::WaitEvent(int ms)
{
	if (input.lib_inp == nullptr) return;
	
	auto fd = libinput_get_fd(input.lib_inp);
	
	fd_set set[1];
	FD_ZERO(set);
	FD_SET(fd, set);
	timeval tval = { ms / 1000, ms % 1000 * 1000 };
	if (select(fd + 1, set, nullptr, nullptr, &tval) > 0){
		libinput_dispatch(input.lib_inp);
	}
}


bool LinuxFrameBuffer::IsWaitingEvent()
{
	return input.lib_inp != nullptr && libinput_next_event_type(input.lib_inp) != LIBINPUT_EVENT_NONE;
}

#define CHMOD(mod, mask, press)	do{ if (press) mod |= mask; else mod &= ~mask; }while(false)

bool LinuxFrameBuffer::ProcessEvent(bool *quit)
{
	if (input.lib_inp == nullptr) return false;
	struct libinput_event *event;
	bool ret = false;
	if((event = libinput_get_event(input.lib_inp)) != nullptr){
		
		switch(libinput_event_get_type(event)){

			case LIBINPUT_EVENT_KEYBOARD_KEY:
			{
				struct libinput_event_keyboard *key_event = libinput_event_get_keyboard_event(event);
				auto key = libinput_event_keyboard_get_key(key_event);
				bool pressed = (libinput_event_keyboard_get_key_state(key_event) == LIBINPUT_KEY_STATE_PRESSED);
				switch(key){
					case KEY_LEFTALT:		CHMOD(input.keyboard.modkeys, KM_LALT, pressed); break;
					case KEY_RIGHTALT:		CHMOD(input.keyboard.modkeys, KM_RALT, pressed); break;
					case KEY_LEFTCTRL:		CHMOD(input.keyboard.modkeys, KM_LCTRL, pressed); break;
					case KEY_RIGHTCTRL:		CHMOD(input.keyboard.modkeys, KM_RCTRL, pressed); break;
					case KEY_LEFTSHIFT:		CHMOD(input.keyboard.modkeys, KM_LSHIFT, pressed); break;
					case KEY_RIGHTSHIFT:	CHMOD(input.keyboard.modkeys, KM_RSHIFT, pressed); break;
				}

				auto upp_scan = TranslateScanCode(key);
				if (upp_scan) Ctrl::DoKeyFB(upp_scan | (!pressed ? K_KEYUP : 0), 1);
				else { LOG(Format("Scan code %d was not translated to K_*, see input-event-codes.h", (int)key)); }

				if (pressed){
					WString txt = ScanCodeToUtf8(key).ToWString();
					for (int i = 0; i < txt.GetCount(); i++) Ctrl::DoKeyFB(txt[i], 1);
				}
			}
			break;
				
			case LIBINPUT_EVENT_POINTER_MOTION:
			{
				struct libinput_event_pointer *motion_event = libinput_event_get_pointer_event(event);
				input.mouse.pos.x += libinput_event_pointer_get_dx(motion_event);
				input.mouse.pos.y += libinput_event_pointer_get_dy(motion_event);
				input.mouse.pos.x = minmax(input.mouse.pos.x, 0, screen.resolution.cx);
				input.mouse.pos.y = minmax(input.mouse.pos.y, 0, screen.resolution.cy);
				Ctrl::DoMouseFB(Ctrl::MOUSEMOVE, input.mouse.pos);
			}
			break;
			
			case LIBINPUT_EVENT_POINTER_BUTTON:
			{
				struct libinput_event_pointer *button_event = libinput_event_get_pointer_event(event);
				auto button = libinput_event_pointer_get_button(button_event);
				auto state = libinput_event_pointer_get_button_state(button_event);
				switch(state){
					
					case LIBINPUT_BUTTON_STATE_PRESSED:
						switch(button){
							case BTN_LEFT: input.mouse.buttons |= (1<<0); Ctrl::DoMouseFB(Ctrl::LEFTDOWN, input.mouse.pos); break;
							case BTN_RIGHT: input.mouse.buttons |= (1<<1); Ctrl::DoMouseFB(Ctrl::RIGHTDOWN, input.mouse.pos); break;
							case BTN_MIDDLE: input.mouse.buttons |= (1<<2); Ctrl::DoMouseFB(Ctrl::MIDDLEDOWN, input.mouse.pos); break;
						}
						break;
					
					case LIBINPUT_BUTTON_STATE_RELEASED:
						switch(button){
							case BTN_LEFT: input.mouse.buttons &= ~(1<<0); Ctrl::DoMouseFB(Ctrl::LEFTUP, input.mouse.pos); break;
							case BTN_RIGHT: input.mouse.buttons &= ~(1<<1); Ctrl::DoMouseFB(Ctrl::RIGHTUP, input.mouse.pos); break;
							case BTN_MIDDLE: input.mouse.buttons &= ~(1<<2); Ctrl::DoMouseFB(Ctrl::MIDDLEUP, input.mouse.pos); break;
						}
						break;
				}
			}
			break;
				
			case LIBINPUT_EVENT_POINTER_AXIS:
			{
				struct libinput_event_pointer *axis_event = libinput_event_get_pointer_event (event);
				auto source = libinput_event_pointer_get_axis_source(axis_event);
				const auto axis = LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL;
				if (libinput_event_pointer_has_axis(axis_event, axis) && source == LIBINPUT_POINTER_AXIS_SOURCE_WHEEL){
					auto delta = libinput_event_pointer_get_axis_value_discrete(axis_event, axis);
					Ctrl::DoMouseFB(Ctrl::MOUSEWHEEL, input.mouse.pos, -120*delta);
				}
			}
			break;

		}
		
		libinput_event_destroy(event);
		libinput_dispatch(input.lib_inp);
		ret = true;
	}
	
	return ret;
}
