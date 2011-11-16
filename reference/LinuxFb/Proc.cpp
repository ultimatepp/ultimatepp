#include "LinuxFbLocal.h"

NAMESPACE_UPP

#define LLOG(x)       //LOG(x)

bool GetMouseLeft()   { return mouseb & (1<<0); }
bool GetMouseRight()  { return mouseb & (1<<1); }
bool GetMouseMiddle() { return mouseb & (1<<2); }

void purgefd(int fd)
{
	fd_set fdset;
	struct timeval tv;
	char temp[64];

	FD_ZERO(&fdset);
	FD_SET(fd, &fdset);
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	while(select(fd+1, &fdset, 0, 0, &tv) > 0)
		read(fd, temp, sizeof(temp));
}

int set_imps2(int fd, int b)
{
	uint8 set[] = {0xf3, 200, 0xf3, 100, 0xf3, 80};
	uint8 reset[] = {0xff};
	int retval = 0;

	if(b && write(fd, &set, sizeof(set)) != sizeof(set))
		return 0;
	//SDL says not to reset, some mice wont work with it
	if(!b && write(fd, &reset, sizeof (reset)) != sizeof(reset))
		return 0;

	purgefd(fd);
	return 1;
}

int has_imps2(int fd)
{
	uint8 query = 0xF2;
	purgefd(fd);
	if(write(fd, &query, sizeof (query)) != sizeof(query))
		return 0;

	uint8 ch = 0;
	fd_set fdset;
	struct timeval tv;
	while(1) {
		FD_ZERO(&fdset);
		FD_SET(fd, &fdset);
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		if(select(fd+1, &fdset, 0, 0, &tv) < 1) break;
		if(read(fd, &ch, sizeof(ch)) != sizeof(ch)) break;
		switch(ch) {
			case 0xFA:
			case 0xAA: continue;
			case 3:
			case 4: return 1;
			default: return 0;			
		}
	}
	return 0;
}

dword lastbdowntime[8] = {0};
dword isdblclick[8] = {0};

void handle_button(dword ct, dword mouseb, dword bm, int i,
	dword df, dword rf, dword dbf, dword uf)
{
	dword m = (1<<i);
	if(bm & m)
	{
		if(mouseb & m) //down
		{
			if(isdblclick[i] && (abs(int(ct) - int(lastbdowntime[i])) < 400))
			{
				Ctrl::DoMouseFB(dbf, mousep);
				isdblclick[i] = 0; //reset, to go ahead sending repeats
			}
			else if(!isdblclick[i])
			{
				//Ctrl::DoMouseFB(rf, mousep);
			}
			else
			{
				lastbdowntime[i] = ct;
				isdblclick[i] = 0; //prepare for repeat
				Ctrl::DoMouseFB(df, mousep);
			}
		}
		else //up
		{
			isdblclick[i] = 1; //indicate maybe a dblclick
			Ctrl::DoMouseFB(uf, mousep);
		}
	}
}

void handle_mouse()
{
	static int offs = 0;
	static unsigned char buf[BUFSIZ];
	static int rel = 1;

	int i, n;
	int b = 0;
	int dx = 0, dy = 0, dz = 0;
	int rdsize = (!mouse_imps2)?(3):(4);
	
	n = read(mouse_fd, &buf[offs], BUFSIZ-offs);
	if(n < 0) return;
	n += offs;

	for(i=0; i<(n-(rdsize-1)); i += rdsize) {
		if((buf[i] & 0xC0) != 0) {
			i -= (rdsize-1);
			continue;
		}
		if(!mouse_imps2)
		{
			b = (buf[i] & 0x07); /*MRL*/
			dx = (buf[i] & 0x10) ? buf[i+1] - 256 : buf[i+1];
			dy = (buf[i] & 0x20) ? -(buf[i+2] - 256) : -buf[i+2];
		}
		else
		{
			b = (buf[i] & 0xC7); /*54...MRL*/
			dx = (buf[i] & 0x10) ? buf[i+1] - 256 : buf[i+1];
			dy = (buf[i] & 0x20) ? -(buf[i+2] - 256) : -buf[i+2]; 
			dz = (char)buf[i+3];
		}

		if(dx || dy)
		{
			mousep.x += dx;
			mousep.y += dy;
			mousep.x = minmax(mousep.x, 0, int(vinfo.xres));
			mousep.y = minmax(mousep.y, 0, int(vinfo.yres));
			Ctrl::DoMouseFB(Ctrl::MOUSEMOVE, mousep);
		}


		dword bm = b ^ mouseb;
		mouseb = b; //for GetMouse*
		dword ct = GetTickCount();

		handle_button(ct, mouseb, bm, 0, Ctrl::LEFTDOWN, Ctrl::LEFTREPEAT, Ctrl::LEFTDOUBLE, Ctrl::LEFTUP);
		handle_button(ct, mouseb, bm, 1, Ctrl::RIGHTDOWN, Ctrl::RIGHTREPEAT, Ctrl::RIGHTDOUBLE, Ctrl::RIGHTUP);
		handle_button(ct, mouseb, bm, 2, Ctrl::MIDDLEDOWN, Ctrl::MIDDLEREPEAT, Ctrl::MIDDLEDOUBLE, Ctrl::MIDDLEUP);

		if(dz) Ctrl::DoMouseFB(Ctrl::MOUSEWHEEL, mousep, dz*120);
	}
	if(i < n) {
		memcpy(buf, &buf[i], (n-i));
		offs = (n-i);
	} else
		offs = 0;
}

void handle_keyboard()
{
	unsigned char buf[BUFSIZ];
	int n;
	int keyup;
	int keycode;

	n = read(keyb_fd, buf, BUFSIZ);
	for(int i=0; i<n; ++i) {
		keycode = buf[i] & 0x7F;
		keyup = (buf[i] & 0x80)?(K_KEYUP):(0);
		bool b;

		SaveModKeys(keycode, !keyup);

		//we dont support both sides, fall back to left
		switch(keycode)
		{
			case SCANCODE_RIGHTALT: keycode = SCANCODE_LEFTALT; break;	
			case SCANCODE_RIGHTSHIFT: keycode = SCANCODE_LEFTSHIFT; break;	
			case SCANCODE_RIGHTCONTROL: keycode = SCANCODE_LEFTCONTROL; break;	
		}

		dword uppcode = fbKEYtoK(keycode) | keyup;

		if(!keyup && uppcode == K_SPACE)
			uppcode = 0; //prevent double send with unicode

		//vtswitch
		int vtck = uppcode & (K_DELTA | 0xFFFF);
		switch(vtck) {
		    case K_F11:
		    case K_F12:
				vtck -= 18; //dirty hack: after F10 doesnt come F11, see vgakeyboard.h
		    case K_F1:
		    case K_F2:
		    case K_F3:
		    case K_F4:
		    case K_F5:
		    case K_F6:
		    case K_F7:
		    case K_F8:
		    case K_F9:
		    case K_F10:
			if(GetCtrl() && GetAlt()) {
				int ii = vtck - K_F1 + 1;
				if(!keyup)
					switchvt(ii);
				return;
			}
		}

		//first, the upp keycode
		if(uppcode)
			b = Ctrl::DoKeyFB(uppcode, 1);

		//second, the unicode translation for a keypress
		if(!keyup)
		{
			dword unicode = TranslateUnicode(keycode);
			if(unicode >= 32 && unicode != 127)
				b = Ctrl::DoKeyFB(unicode, 1);
		}

		//helper quit
		if(uppcode == (K_SHIFT_CTRL | K_ESCAPE))
			Ctrl::EndSession();
	}
}

END_UPP_NAMESPACE
