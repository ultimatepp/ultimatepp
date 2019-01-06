#ifndef _VirtualGuiLinuxFrameBuffer_LinuxFrameBuffer_h_
#define _VirtualGuiLinuxFrameBuffer_LinuxFrameBuffer_h_

#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>
#include <libinput.h>

namespace Upp {
	
class LinuxFrameBuffer: public VirtualGui {
	virtual Size        GetSize()							{ return screen.resolution; }
	virtual dword       GetMouseButtons()					{ return input.mouse.buttons; }
	virtual dword       GetModKeys()						{ return input.keyboard.modkeys; }
	virtual bool        IsMouseIn()							{ return true; }
	virtual bool        ProcessEvent(bool *quit);
	virtual void        WaitEvent(int ms);
	virtual bool        IsWaitingEvent();
	virtual void        WakeUpGuiThread()					{}
	virtual void        SetMouseCursor(const Image& image)	{}
	virtual SystemDraw& BeginDraw();
	virtual void        CommitDraw();

	virtual void        Quit()								{}

private:
	struct screen_t{
		int				handle;
		uint32			*buffer;
		size_t			buff_len;
		Size			resolution;
		int				bits_per_pixel;
		SystemDraw		sysdraw;
		ImageBuffer		image;
		BufferPainter	painter;
		
		screen_t(): handle(-1), buffer(nullptr), buff_len(0), resolution(), bits_per_pixel(0) {}
	}screen;
	
	struct input_t{
		struct udev		*udev;
		struct libinput	*lib_inp;
		
		struct mouse_t{
			uint32		buttons;
			Point		pos;
			
			mouse_t(): buttons(0) {}
		}mouse;
		
		struct keyboard_t{
			uint32		modkeys;
			
			keyboard_t(): modkeys(0) {}
		}keyboard;
		
		input_t(): udev(nullptr), lib_inp(nullptr) {}
	}input;
	
	uint32 TranslateKeyEvent(uint32 chr);
	
public:
	bool Create(const char * device);
	void Destroy();

	~LinuxFrameBuffer();
};

}


#endif
