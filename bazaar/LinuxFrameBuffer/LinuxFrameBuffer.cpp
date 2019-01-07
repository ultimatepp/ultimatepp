#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "LinuxFrameBuffer.h"

using namespace Upp;


static int open_restricted(const char *path, int flags, void *user_data)
{
	int fd = open(path, flags);
	return fd < 0 ? -errno : fd;
}


static void close_restricted(int fd, void *user_data)
{
	close(fd);
}


static const struct libinput_interface input_interface =
{
	.open_restricted = open_restricted,
	.close_restricted = close_restricted,
};


LinuxFrameBuffer::~LinuxFrameBuffer()
{
	Destroy();
}


void LinuxFrameBuffer::Destroy()
{
	if (input.kb_state != nullptr){
		xkb_state_unref(input.kb_state);
		input.kb_state = nullptr;
	}

	if (input.kb_map != nullptr){
		xkb_keymap_unref(input.kb_map);
		input.kb_map = nullptr;
	}

	if (input.kb_ctx != nullptr){
		xkb_context_unref(input.kb_ctx);
		input.kb_ctx = nullptr;
	}
	
	if (input.lib_inp != nullptr){
		libinput_unref(input.lib_inp);
		input.lib_inp = nullptr;
	}
	
	if (input.udev != nullptr){
		udev_unref(input.udev);
		input.udev = nullptr;
	}
	
	if (screen.buffer != nullptr){
		munmap(screen.buffer, screen.buff_len);
		screen.buffer = nullptr;
	}
	
	if (screen.handle >= 0){
		close(screen.handle);
		screen.handle = -1;
	}
}


bool LinuxFrameBuffer::Create(const char * device)
{
	Destroy();
	screen.handle = open(device, O_RDWR);
	if (screen.handle < 0){
		RLOG("Error opening device: '" << device << "': " << GetLastErrorMessage());
		return false;
	}

	struct fb_var_screeninfo info;
	if (ioctl(screen.handle, FBIOGET_VSCREENINFO, &info) == 0){
		screen.resolution.cx = info.xres;
		screen.resolution.cy = info.yres;
		screen.bits_per_pixel = info.bits_per_pixel;
	}

	struct fb_fix_screeninfo finfo;
	if (ioctl(screen.handle, FBIOGET_FSCREENINFO, &finfo) == 0){
		screen.buff_len = finfo.smem_len;
	}
	
	screen.buffer = (uint32 *)mmap(nullptr, screen.buff_len, PROT_READ | PROT_WRITE, MAP_SHARED, screen.handle, 0);
	if (screen.buffer == MAP_FAILED){
		RLOG("Memory mapping error: " << GetLastErrorMessage());
		Destroy();
		return false;
	}

	input.udev = udev_new();
	if (input.udev == nullptr){
		RLOG("udev_new error");
		Destroy();
		return false;
	}
	
	input.lib_inp = libinput_udev_create_context(&input_interface, nullptr, input.udev);
	if (input.lib_inp == nullptr){
		RLOG("libinput create context error");
		Destroy();
		return false;
	}
	
	libinput_udev_assign_seat(input.lib_inp, "seat0");
	libinput_dispatch(input.lib_inp);
	 
	input.kb_ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
	if (input.kb_ctx == nullptr){
		RLOG("xkb context error");
		Destroy();
		return false;
	}
	
	input.kb_map = xkb_keymap_new_from_names(input.kb_ctx, nullptr, XKB_KEYMAP_COMPILE_NO_FLAGS);
	if (input.kb_map == nullptr){
		RLOG("xkb map error");
		Destroy();
		return false;
	}
	
	input.kb_state = xkb_state_new(input.kb_map);
	if (input.kb_state == nullptr){
		RLOG("xkb state error");
		Destroy();
		return false;
	}
	
	screen.image.Create(GetSize());
	screen.painter.Create(screen.image, MODE_ANTIALIASED);
	screen.sysdraw.SetTarget(&screen.painter);
	
	return true;
}
