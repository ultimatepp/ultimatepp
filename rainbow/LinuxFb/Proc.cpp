#include "LinuxFb.h"

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

NAMESPACE_UPP

#define LLOG(x)       LOG(x)

	int fbfd = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long int screensize = 0;
	char *fbp = 0;

//FIXME get input events
bool GetShift()       { return false; }//!!(GetKeyState(VK_SHIFT) & 0x8000); }
bool GetCtrl()        { return false; }//!!(GetKeyState(VK_CONTROL) & 0x8000); }
bool GetAlt()         { return false; }//!!(GetKeyState(VK_MENU) & 0x8000); }
bool GetCapsLock()    { return false; }//!!(GetKeyState(VK_CAPITAL) & 1); }
bool GetMouseLeft()   { return false; }//!!(GetKeyState(VK_LBUTTON) & 0x8000); }
bool GetMouseRight()  { return false; }//!!(GetKeyState(VK_RBUTTON) & 0x8000); }
bool GetMouseMiddle() { return false; }//!!(GetKeyState(VK_MBUTTON) & 0x8000); }


bool FBEndSession()
{
	return false;//fbEndSession;
}

bool FBIsWaitingEvent()
{
	//MSG msg;
	return false; //PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
}

bool FBProcessEvent(bool *quit)
{
/*
	MSG msg;
	if(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		if(msg.message == WM_QUIT && quit)
			*quit = true;
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
		return true;
	}
*/
	static int i = 10;
	if(--i > 0) return true; //fake message processing
	return false;
}

void FBSleep(int ms)
{
//	MsgWaitForMultipleObjects(0, NULL, FALSE, ms, QS_ALLINPUT);
}

void FBUpdate(const Vector<Rect>& inv)
{
//	if(fbHWND)
//		for(int i = 0; i < inv.GetCount(); i++)
//			::InvalidateRect(fbHWND, inv[i], false);

	//FIXME direct paint
	//here currently just copy over
/*
	int x = 0, y = 0;
	long int location = 0;
	x = 100; y = 100;       // Where we are going to put the pixel

	// Figure out where in memory to put the pixel
	for (y = 100; y < 300; y++)
		for (x = 100; x < 300; x++) {

			location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
					(y+vinfo.yoffset) * finfo.line_length;

			if (vinfo.bits_per_pixel == 32) {
				*(fbp + location) = 100;        // Some blue
				*(fbp + location + 1) = 15+(x-100)/2;     // A little green
				*(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
				*(fbp + location + 3) = 0;      // No transparency
			} else  { //assume 16bpp
				int b = 10;
				int g = (x-100)/6;     // A little green
				int r = 31-(y-100)/16;    // A lot of red
				unsigned short int t = r<<11 | g << 5 | b;
				*((unsigned short int*)(fbp + location)) = t;
			}

		}
*/
	memcpy(fbp, (const char*)~framebuffer, framebuffer.GetLength() * sizeof(RGBA));
}

void FBUpdate(const Rect& inv)
{
//	if(fbHWND)
//		::InvalidateRect(fbHWND, inv, false);
	memcpy(fbp, (const char*)~framebuffer, framebuffer.GetLength() * sizeof(RGBA));
}

void FBInit(const String& fbdevice)
{
	Ctrl::InitFB();

	// Open the file for reading and writing
	fbfd = open(fbdevice, O_RDWR);
	if (!fbfd) {
		printf("Error: cannot open framebuffer device.\n");
		exit(1);
	}
	RLOG("The framebuffer device was opened successfully.\n");

	// Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Error reading fixed information.\n");
		exit(2);
	}

	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Error reading variable information.\n");
		exit(3);
	}

	RLOG(vinfo.xres << "x" << vinfo.yres << " @ " << vinfo.bits_per_pixel);

	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	// Map the device to memory
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
					fbfd, 0);
	if ((intptr_t)fbp == -1) {
		printf("Error: failed to map framebuffer device to memory.\n");
		exit(4);
	}
	RLOG("The framebuffer device was mapped to memory successfully.\n");
	
	//FIXME
	/*
	munmap(fbp, screensize);
	close(fbfd);
	*/
	
	framebuffer.Create(vinfo.xres, vinfo.yres);
}

END_UPP_NAMESPACE
