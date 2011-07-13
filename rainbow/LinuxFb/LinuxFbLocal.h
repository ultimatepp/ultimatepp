#ifndef _LinuxFb_LinuxFbLocal_h
#define _LinuxFb_LinuxFbLocal_h

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

extern bool fbEndSession;

extern int fbfd;
extern struct fb_var_screeninfo vinfo;
extern struct fb_fix_screeninfo finfo;
extern long int screensize;
extern char *fbp;

extern int mouse_fd;
extern bool mouse_imps2;
extern Point mousep;
extern int mouseb;

extern int keyboard_fd;

int has_imps2(int fd);
int set_imps2(int fd, int b);
int readevents(int ms);
void handle_mouse();
void handle_keyboard();

END_UPP_NAMESPACE

#endif

