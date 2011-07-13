#include "LinuxFbLocal.h"

#define LLOG(x)       //LOG(x)

NAMESPACE_UPP

bool fbEndSession = false;

//video
int fbfd = -1;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize = 0;
char *fbp = 0;

//mouse
int mouse_fd = -1;
bool mouse_imps2 = false;
Point mousep;
int mouseb = 0;

//keyb
int keyboard_fd = -1;

//event
int pend = 0;

bool FBEndSession()
{
	return fbEndSession;
}

bool FBIsWaitingEvent()
{
	pend = readevents(0);
	return pend > 0;
}

bool FBProcessEvent(bool *quit)
{
	if(pend)
	{
		if(pend & 1) handle_mouse();
		if(pend & 2) handle_keyboard();
		pend = 0; //need to reset, since with repeated call is not updated here, would stuck
		return true;
	}
	return false;
}

void FBSleep(int ms)
{
	pend = readevents(ms); //interruptable sleep
}

void FBUpdate(const Rect& inv)
{
	const ImageBuffer& framebuffer = Ctrl::GetFrameBuffer();
	memcpy(fbp, ~framebuffer, framebuffer.GetLength() * sizeof(RGBA));
}

void FBFlush()
{

}

void FBInit(const String& fbdevice)
{
	Ctrl::InitFB();

	fbfd = open(fbdevice, O_RDWR);
	if (!fbfd) {
		printf("Error: cannot open framebuffer device.\n");
		exit(1);
	}
	LLOG("The framebuffer device was opened successfully.\n");

	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Error reading fixed information.\n");
		exit(2);
	}

	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Error reading variable information.\n");
		exit(3);
	}
	RLOG("FB opened: " << fbdevice << ": " << vinfo.xres << "x" << vinfo.yres << " @ " << vinfo.bits_per_pixel);

	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8; //bytes

	fbp = (char*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if ((intptr_t)fbp == -1) {
		printf("Error: failed to map framebuffer device to memory.\n");
		exit(4);
	}
	LLOG("The framebuffer device was mapped to memory successfully.\n");
	
	Ctrl::SetFramebufferSize(Size(vinfo.xres, vinfo.yres));
	
	//Mouse stuff
	mousep.Clear();
	
	static const char *mice[] = {
	    "/dev/input/mice"
	    , "/dev/usbmouse"
	    , "/dev/psaux"
	    , NULL
	};

	for(int i=0; (mouse_fd < 0) && mice[i]; ++i) {
		mouse_fd = open(mice[i], O_RDWR, 0);
		if(mouse_fd < 0) mouse_fd = open(mice[i], O_RDONLY, 0);
		if(mouse_fd >= 0) {
			set_imps2(mouse_fd, 1);
			if(mouse_imps2 = has_imps2(mouse_fd)) {
				RLOG("IMPS2 mouse enabled: " << mice[i]);
			}
		}
	}
	
	pend = 4; //fake video expose event to cause first paint
}

void FBDeInit()
{
	Ctrl::ExitFB();
	munmap(fbp, screensize);
	close(fbfd);
}

END_UPP_NAMESPACE
