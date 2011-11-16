#include "LinuxFbLocal.h"

#define LLOG(x)       //LOG(x)

NAMESPACE_UPP

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
dword mouseb = 0;

//keyb
int keyb_fd = -1;
int cvt = -1;

//

int oldvt;
struct termios oldtermios;
int oldmode;

int entervt()
{
	struct termios kt;

	if(cvt > 0) {
		struct vt_stat vtst;
		if(ioctl(keyb_fd, VT_GETSTATE, &vtst) == 0)
			oldvt = vtst.v_active;
		if(ioctl(keyb_fd, VT_ACTIVATE, cvt) == 0)
			ioctl(keyb_fd, VT_WAITACTIVE, cvt);
	}

	if(tcgetattr(keyb_fd, &oldtermios) < 0) {
		fprintf(stderr, "Error: saving terminal attributes");
		return -1;
	}

	if(ioctl(keyb_fd, KDGKBMODE, &oldmode) < 0) {
		fprintf(stderr, "Error: saving keyboard mode");
		return -1;
	}

	kt = oldtermios;
	kt.c_lflag &= ~(ICANON | ECHO | ISIG);
	kt.c_iflag &= ~(ISTRIP | IGNCR | ICRNL | INLCR | IXOFF | IXON);
	kt.c_cc[VMIN] = 0;
	kt.c_cc[VTIME] = 0;

	if(tcsetattr(keyb_fd, TCSAFLUSH, &kt) < 0) {
		fprintf(stderr, "Error: setting new terminal attributes");
		return -1;
	}

	if(ioctl(keyb_fd, KDSKBMODE, K_MEDIUMRAW) < 0) {
		fprintf(stderr, "Error: setting keyboard in mediumraw mode");
		return -1;
	}

	if(ioctl(keyb_fd, KDSETMODE, KD_GRAPHICS) < 0) {
		fprintf(stderr, "Error: setting keyboard in graphics mode");
		return -1;
	}

	ioctl(keyb_fd, VT_LOCKSWITCH, 1);
	
	return 0;
}

void switchvt_pre()
{
	ioctl(keyb_fd, KDSETMODE, KD_TEXT);
	ioctl(keyb_fd, VT_UNLOCKSWITCH, 1);
}

void switchvt_post()
{
	ioctl(keyb_fd, VT_LOCKSWITCH, 1);
	ioctl(keyb_fd, KDSETMODE, KD_GRAPHICS);
}

int switched_away = 0;

void switchvt(int ii)
{
	struct vt_stat vtst;

	/* Figure out whether or not we're switching to a new console */
	if(ioctl(keyb_fd, VT_GETSTATE, &vtst) < 0)
	{
		fprintf(stderr, "Error: could not read tty state");
		return;	
	}
	if(ii == vtst.v_active)
		return;

	LLOG("trying to switch to VT " << ii);

	GuiLock __;

	switchvt_pre();

	if(ioctl(keyb_fd, VT_ACTIVATE, ii) == 0) {
		ioctl(keyb_fd, VT_WAITACTIVE, ii);
		switched_away = 1;
		return;
	}

	switchvt_post();
}

void leavevt()
{
	if(oldmode < 0) return;

	if(ioctl(keyb_fd, KDSETMODE, KD_TEXT) < 0)
		fprintf(stderr, "Error: setting keyboard in text mode");
		
	if(ioctl(keyb_fd, KDSKBMODE, oldmode) < 0)
		fprintf(stderr, "Error: restoring keyboard mode");

	if(tcsetattr(keyb_fd, TCSAFLUSH, &oldtermios) < 0)
		fprintf(stderr, "Error: setting new terminal attributes");

	oldmode = -1;

	ioctl(keyb_fd, VT_UNLOCKSWITCH, 1);

	if(oldvt > 0)
		ioctl(keyb_fd, VT_ACTIVATE, oldvt);
}

int pend = 0;

//returns 0 if timeout, 1 for mouse, 2 for keyboard
//common for waitforevents and sleep 
int readevents(int ms)
{
	fd_set fdset;
	int max_fd;
	static struct timeval to;
	to.tv_sec = ms / 1000;
	to.tv_usec = ms % 1000 * 1000;

	if(switched_away) {
		struct vt_stat vtst;
		GuiLock __;
		if((ioctl(keyb_fd, VT_GETSTATE, &vtst) == 0) &&
		     vtst.v_active == cvt) {
			switched_away = 0;
			switchvt_post();
		}
	}

	FD_ZERO(&fdset);
	max_fd = 0;
	if(mouse_fd >= 0) {
		FD_SET(mouse_fd, &fdset);
		if(max_fd < mouse_fd) max_fd = mouse_fd;
	}
	if(keyb_fd >= 0) {
		FD_SET(keyb_fd, &fdset);
		if(max_fd < keyb_fd) max_fd = keyb_fd;
	}
	if(select(max_fd+1, &fdset, NULL, NULL, &to) > 0) {
		if(mouse_fd >= 0) {
			if(FD_ISSET(mouse_fd, &fdset)) return 1;
		}
		if(keyb_fd >= 0) {
			if(FD_ISSET(keyb_fd, &fdset)) return 2;
		}
	}
	return 0;
}

//

void FBQuitSession()
{
	Ctrl::EndSession();
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

	//keep queue busy, see SDLFb/Win.cpp for why
	//this indicates that some stuff is pending, returning true in FBProcessEvent
	//while nothing is actually processed
	pend |= 4;
}

void FBInitUpdate()
{

}

void FBUpdate(const Rect& inv)
{
	if(switched_away) return; //backdraw
	//FIXME accelerate
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
		fprintf(stderr, "Error: cannot open framebuffer device.\n");
		exit(-1);
	}
	LLOG("The framebuffer device was opened successfully.\n");

	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		fprintf(stderr, "Error reading fixed information.\n");
		exit(-2);
	}

	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		fprintf(stderr, "Error reading variable information.\n");
		exit(-3);
	}
	RLOG("Framebuffer opened: " << fbdevice << ": " << vinfo.xres << "x" << vinfo.yres << " @ " << vinfo.bits_per_pixel);

	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8; //bytes

	fbp = (char*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if((intptr_t)fbp == -1) {
		fprintf(stderr, "Error: failed to map framebuffer device to memory.\n");
		exit(-4);
	}
	LLOG("The framebuffer device was mapped to memory successfully.\n");

	Size fbsz(vinfo.xres, vinfo.yres);
	Ctrl::SetFramebufferSize(fbsz);

	//mouse

	//mousep = fbsz / 2;
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
				LLOG("IMPS2 mouse enabled: " << mice[i]);
			}
			else
				RLOG("no IMPS2 mouse present");
		}
		else
			fprintf(stderr, "Error: failed to open %s.\n", mice[i]);
	}

	//keyb

	static const char* const tty0[] = {
		"/dev/tty0"
		, "/dev/vc/0"
		, NULL
	};
	static const char* const vcs[] = {
		"/dev/vc/"
		, "/dev/tty"
		, NULL
	};

	int tfd = -1;
	for(int i=0; tty0[i] && (tfd < 0); ++i)
		tfd = open(tty0[i], O_WRONLY, 0);
	if(tfd < 0)
		tfd = dup(0);
	ASSERT(tfd>=0);

	ioctl(tfd, VT_OPENQRY, &cvt);
	close(tfd);
	LLOG("probable new VT: " << cvt);

	if(geteuid() != 0)
	{
		fprintf(stderr, "Error: not running as ROOT, mouse handling pobably unavailable\n");
	}
	else if(cvt > 0) {
		LLOG("try to open the NEW assigned VT: " << cvt);
		for(int i=0; vcs[i] && (keyb_fd < 0); ++i) {
			char path[32];
			snprintf(path, 32, "%s%d", vcs[i], cvt);
			keyb_fd = open(path, O_RDWR, 0);

			if(keyb_fd < 0)
				continue;	
			
			LLOG("TTY path opened: " << path);
			tfd = open("/dev/tty", O_RDWR, 0);
			if(tfd >= 0) {
				LLOG("detaching from local stdin/out/err");
				ioctl(tfd, TIOCNOTTY, 0);
				close(tfd);
			}
			else
				fprintf(stderr, "Error: detaching from local stdin/out/err\n");
		}
	}

	if(keyb_fd < 0) {
		LLOG("Using already assigned VT, must not detach");
		struct vt_stat vtst;

		keyb_fd = open("/dev/tty", O_RDWR);

		if(ioctl(keyb_fd, VT_GETSTATE, &vtst) < 0) {
			cvt = 0;
		} else {
			cvt = vtst.v_active;
		}
	}

	if(cvt>0)
		fprintf(stdout, "started on VT %d\n", cvt);

	ASSERT(keyb_fd>=0);
	oldmode = -1;

	{
		int d;
		if(ioctl(keyb_fd, KDGKBMODE, &d) < 0) {
			close(keyb_fd);
			keyb_fd = -1;
			fprintf(stderr, "Error: opening a console terminal");
			exit(5);
		}
	}

	LLOG("tty opened: " << keyb_fd);

	dupkmap(keyb_fd);

	entervt();
	
	pend = 4; //fake video expose event to cause first paint
}

void FBDeInit()
{
	Ctrl::ExitFB();
	munmap(fbp, screensize);
	close(fbfd);
	
	leavevt();
	close(mouse_fd);
}

END_UPP_NAMESPACE
