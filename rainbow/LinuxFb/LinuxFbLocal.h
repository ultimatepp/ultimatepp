#ifndef _LinuxFb_LinuxFbLocal_h
#define _LinuxFb_LinuxFbLocal_h

#include <CtrlCore/CtrlCore.h>

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <sys/types.h>
#include <termios.h>

#include <linux/vt.h>
#include <linux/kd.h>
//conflicts with upp
//#include <linux/keyboard.h>

NAMESPACE_UPP

//video
extern int fbfd;
extern struct fb_var_screeninfo vinfo;
extern struct fb_fix_screeninfo finfo;
extern long int screensize;
extern char *fbp;

//mouse
extern int mouse_fd;
extern bool mouse_imps2;
extern Point mousep;
extern dword mouseb;

//keyb
extern int keyb_fd;
extern int cvt;
extern dword modkeys;

int has_imps2(int fd);
int set_imps2(int fd, int b);
void dupkmap(int fd);

void handle_mouse();
void handle_keyboard();
void switchvt(int ii);

void SaveModKeys(dword keycode, dword pressed);
dword fbKEYtoK(dword keycode);
dword TranslateUnicode(dword keycode);

END_UPP_NAMESPACE

#endif

