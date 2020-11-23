#include "SSH.h"

namespace Upp {

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::GetName(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

bool SshShell::Run(int mode_, const String& terminal, Size pagesize, const String& tmodes)
{
	mode  = mode_;
	psize = pagesize;
	
	if(RequestTerminal(terminal, psize, tmodes) && X11Init() && RequestShell() && ConsoleInit())
		ProcessEvents(queue);
	return Shut(IsError() ? GetErrorDesc() : Null);
}

void SshShell::ReadWrite(String& in, const void* out, int out_len)
{
	switch(mode) {
		case GENERIC: {
			if(out_len > 0)
				WhenOutput(out, out_len);
#ifdef PLATFORM_POSIX
			if(xenabled)
				X11Loop();
#endif
			WhenInput();
			break;
		}
		case CONSOLE: {
			if(out_len > 0)
				ConsoleWrite(out, out_len);
#ifdef PLATFORM_POSIX
			if(xenabled)
				X11Loop();
			ConsoleRead();
			#if defined(PLATFORM_MACOS)
			// We are using sigtimedwait on POSIX-compliant systems.
			// Unfortunately MacOS didn't implement it. This is a simple workaround for MacOS.
			// It relies on ioctl, which is implemented on MacOS.
			Size sz = GetConsolePageSize();
			resized = !IsNull(sz) && sz != psize;
			if(resized)
				LLOG("Window size changed.");
			#else
			// We need to catch the WINCH signal. To this end we'll use a POSIX compliant kernel
			// function: sigtimedwait. To speed up, we'll simply poll for the monitored event.
			sigset_t set;
			sigemptyset(&set);
			sigaddset(&set, SIGWINCH);
			sigprocmask(SIG_BLOCK, &set, nullptr);

			struct timespec timeout;
			Zero(timeout); // Instead of waiting, we simply poll.

			auto rc = sigtimedwait(&set, nullptr, &timeout);
			if(rc < 0 && errno != EAGAIN)
				ThrowError(-1, "sigtimedwait() failed.");
			if(rc > 0)
				LLOG("SIGWINCH received.");
			resized = rc > 0;
			#endif
#elif PLATFORM_WIN32
			// This part is a little bit tricky. We need to handle Windows console events here.
			// But we cannot simply ignore the events we don't look for. We need to actively
			// remove them from the event queue. Otherwise they'll cause erratic behaviour, and
			// a lot of head ache. Thus to filter out these unwanted events, or the event's that
			// we don't want to get in our way, we'll first peek at the console event queue to
			// see if they met our criteria and remove them one by one as we encounter, using
			// the ReadConsoleInput method.

			auto rc = WaitForSingleObject(stdinput, ssh->waitstep);
			switch(rc) {
				case WAIT_OBJECT_0:
					break;
				case WAIT_TIMEOUT:
				case WAIT_ABANDONED:
					return;
				default:
					ThrowError(-1, "WaitForSingleObject() failed.");
			}

			DWORD n = 0;
			INPUT_RECORD ir[1];

			if(!PeekConsoleInput(stdinput, ir, 1, &n))
				ThrowError(-1, "Unable to peek console input events.");
			if(n) {
				switch(ir[0].EventType) {
					case KEY_EVENT:
						// Ignore key-ups.
						if(!ir[0].Event.KeyEvent.bKeyDown)
							break;
						ConsoleRead();
						return;
					case WINDOW_BUFFER_SIZE_EVENT:
						LLOG("WINDOW_BUFFER_SIZE_EVENT received.");
						resized = true;
						break;
					case MENU_EVENT:
					case MOUSE_EVENT:
					case FOCUS_EVENT:
						break;
					default:
						ThrowError(-1, "Unknown console event type encountered.");
				}
				if(!ReadConsoleInput(stdinput, ir, 1, &n))
					ThrowError(-1, "Unable to filter console input events.");
			}
#endif
			break;
		}
		default:
			NEVER();
	}
	if(resized)
		Resize();
}

void SshShell::Resize()
{
	if(mode == CONSOLE)
		PageSize(GetConsolePageSize());

	int n = 0;
	do {
		n = SetPtySz(psize);
		if(n < 0)
			Wait();
	}
	while(!IsTimeout() && !IsEof() && n < 0);
	resized = false;
}

bool SshShell::ConsoleInit()
{
	if(mode != CONSOLE)
		return true;
	
	return Ssh::Run([=]() mutable {
#ifdef PLATFORM_WIN32
		stdinput = GetStdHandle(STD_INPUT_HANDLE);
		if(!stdinput)
			ThrowError(-1, "Unable to obtain a handle for stdin.");
		stdoutput = GetStdHandle(STD_OUTPUT_HANDLE);
		if(!stdoutput)
			ThrowError(-1, "Unable to obtain a handle for stdout.");
#endif
		ConsoleRawMode();
		return true;
	});
}

#ifdef PLATFORM_POSIX
void SshShell::ConsoleRead()
{
	if(!EventWait(STDIN_FILENO, WAIT_READ, 0))
		return;
	Buffer<char> buffer(ssh->chunk_size);
	auto n = read(STDIN_FILENO, buffer, size_t(ssh->chunk_size));
	if(n > 0)
		Send(String(buffer, n));
	else
	if(n == -1 && errno != EAGAIN)
		ThrowError(-1, "Couldn't read input from console.");
}

void SshShell::ConsoleWrite(const void* buffer, int len)
{
	if(!EventWait(STDOUT_FILENO, WAIT_WRITE, 0))
		return;
	auto n = write(STDOUT_FILENO, buffer, size_t(len));
	if(n == -1 && errno != EAGAIN)
		ThrowError(-1, "Couldn't write output to console.");
}

void SshShell::ConsoleRawMode(bool b)
{
	if(!channel || mode != CONSOLE)
		return;

	if(b) {
		termios nflags;
		Zero(nflags);
		Zero(tflags);
		tcgetattr(STDIN_FILENO, &nflags);
		tflags = nflags;
		cfmakeraw(&nflags);
		tcsetattr(STDIN_FILENO, TCSANOW, &nflags);
	}
	else
	if(rawmode)
		tcsetattr(STDIN_FILENO, TCSANOW, &tflags);
	rawmode = b;
}

Size SshShell::GetConsolePageSize()
{
	winsize wsz;
	Zero(wsz);
	if(ioctl(STDIN_FILENO, TIOCGWINSZ, &wsz) == 0)
		return Size(wsz.ws_col, wsz.ws_row);
	LLOG("Warning: ioctl() failed. Couldn't read local terminal page size.");
	return Null;
}

#elif PLATFORM_WIN32

void SshShell::ConsoleRead()
{
	DWORD n = 0;
	const int RBUFSIZE = 1024 * 16;
	Buffer<char> buffer(RBUFSIZE);
	if(!ReadConsole(stdinput, buffer, RBUFSIZE, &n, nullptr))
		ThrowError(-1, "Couldn't read input from console.");
	if(n > 0)
		Send(String(buffer, n));
}

void SshShell::ConsoleWrite(const void* buffer, int len)
{
	DWORD n = 0;
	if(!WriteConsole(stdoutput, buffer, len, &n, nullptr))
		ThrowError(-1, "Couldn't Write output to console.");
}

void SshShell::ConsoleRawMode(bool b)
{
	if(!channel || mode != CONSOLE)
		return;

	if(b) {
		GetConsoleMode(stdinput, &tflags);
		DWORD nflags = tflags;
		nflags &= ~ENABLE_LINE_INPUT;
		nflags &= ~ENABLE_ECHO_INPUT;
		nflags |= ENABLE_WINDOW_INPUT;
		SetConsoleMode(stdinput, nflags);
	}
	else
	if(rawmode)
		SetConsoleMode(stdinput, tflags);
	rawmode = b;
}

Size SshShell::GetConsolePageSize()
{
	CONSOLE_SCREEN_BUFFER_INFO cinf;
	Zero(cinf);
	if(GetConsoleScreenBufferInfo((HANDLE) _get_osfhandle(1), &cinf))
		return Size(cinf.dwSize.X, cinf.dwSize.Y);
	LLOG("Warning: Couldn't read local terminal page size.");
	return Null;
}

#endif

bool SshShell::X11Init()
{
	if(!xenabled)
		return true;

	return Ssh::Run([=]() mutable {
#ifdef PLATFORM_POSIX
		int rc = libssh2_channel_x11_req(*channel, xscreen);
		if(!WouldBlock(rc) && rc < 0)
			ThrowError(rc);
		if(!rc)
			LLOG("X11 tunnel succesfully initialized.");
		return !rc;
#elif PLATFORM_WIN32
		ThrowError(-1, "X11 tunneling is not (yet) supported on Windows platform");
		return false;
#endif
	});
}

void SshShell::X11Loop()
{
#ifdef PLATFORM_POSIX
	if(xrequests.IsEmpty())
		return;

	for(int i = 0; i < xrequests.GetCount(); i++) {
		SshX11Handle xhandle = xrequests[i].a;
		SOCKET sock = xrequests[i].b;

		if(EventWait(sock, WAIT_WRITE, 0)) {
			int rc = static_cast<int>(
				libssh2_channel_read(xhandle, xbuffer, size_t(xbuflen))
				);
			if(!WouldBlock(rc) && rc < 0)
				ThrowError(-1, "[X11]: Read failed.");
			if(rc > 0)
				write(sock, xbuffer, rc);
		}
		if(EventWait(sock, WAIT_READ, 0)) {
			int rc =  static_cast<int>(
				read(sock, xbuffer, size_t(xbuflen))
				);
			if(rc > 0)
				libssh2_channel_write(xhandle, (const char*) xbuffer, size_t(rc));
		}
		if(libssh2_channel_eof(xhandle) == 1) {
			LLOG("[X11] EOF received.");
			close(sock);
			xrequests.Remove(i);
			i = 0;
		}
	}
#endif
}

SshShell& SshShell::ForwardX11(const String& host, int display, int screen, int bufsize)
{
	if(!xenabled) {
		xenabled = true;
#ifdef PLATFORM_POSIX
		xhost    = host;
		xdisplay = display;
		xscreen  = screen;
		xbuflen  = clamp(bufsize, ssh->chunk_size, INT_MAX);
		xbuffer.Alloc(xbuflen);
#endif
	}
	return *this;
}

bool SshShell::AcceptX11(SshX11Handle xhandle)
{
#ifdef PLATFORM_POSIX
	if(xhandle && xenabled) {
		auto sock = socket(AF_UNIX, SOCK_STREAM, 0);
		if(sock < 0) {
			LLOG("Couldn't create UNIX socket.");
			return false;
		}
		auto path = Format("%s/.X11-unix/X%d", GetTempPath(), xdisplay);

		struct sockaddr_un addr;
		Zero(addr);
		addr.sun_family = AF_UNIX;
		memcpy(addr.sun_path, ~path, path.GetLength());

		if(connect(sock, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
			LLOG("Couldn't connect to " << path);
			close(sock);
			return false;
		}

		LLOG("X11 connection accepted.");

		auto& xr = xrequests.Add();
		xr.a = xhandle;
		xr.b = sock;
		return true;
	}
#endif
	return false;
}


SshShell::SshShell(SshSession& session)
: SshChannel(session)
, mode(GENERIC)
, rawmode(false)
, resized(false)
, xenabled(false)
#ifdef PLATFORM_POSIX
, xdisplay(0)
, xscreen(0)
, xbuflen(1024 * 1024)
#elif PLATFORM_WIN32
, stdinput(nullptr)
, stdoutput(nullptr)
#endif
{
    ssh->otype = SHELL;
	Zero(tflags);
}

SshShell::~SshShell()
{
	ConsoleRawMode(false);

}
}