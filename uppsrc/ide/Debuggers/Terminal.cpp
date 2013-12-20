#include "Debuggers.h"

#ifdef PLATFORM_POSIX

void Do_ps(VectorMap<String, String>& pids, Index<String>& ttys, const String& cmd)
{
	pids.Clear();
	ttys.Clear();
	Vector<String> ps = Split(Sys(cmd), '\n');
	for(int i = 1; i < ps.GetCount(); i++) {
		Vector<String> ss = Split(ps[i], ' ');
		if(ss.GetCount() >= 2) {
			if(IsDigit(*ss[0]) && IsAlNum(*ss[1])) {
				pids.Add(ss[1], ss[0]);
				ttys.FindAdd(ss[1]);
			}
		}
	}
}

static VectorMap<String, String> sPid;
static String sTTY;

String CreateDebugTTY()
{ // The method is far from ideal and reliable, but should work most of time
	sTTY.Clear();

	Index<String> tty0, tty;
	Do_ps(sPid, tty0, "ps -A");

	IGNORE_RESULT(chdir(GetHomeDirectory()));

	static const char *term[] = {
		"/usr/bin/mate-terminal -e 'tail -f /dev/null'",
		"/usr/bin/gnome-terminal -e 'tail -f /dev/null'",
		"/usr/bin/konsole -e 'tail -f /dev/null'",
		"/usr/bin/xterm -e 'tail -f /dev/null'",
	};
	
	for(int i = 0; i < __countof(term); i++) {
		if(FileExists(Split(term[i], ' ')[0])) {
			LocalProcess(term[i]).Detach();
			for(int i = 0; i < 10; i++) { // Wait for the new TTY to appear
				Do_ps(sPid, tty, "ps -A");
				for(int i = 0; i < tty.GetCount(); i++)
					if(tty0.Find(tty[i]) < 0) {
						sTTY = tty[i];
						return "/dev/" + sTTY;
					}
				Sleep(200);
			}
		}
	}
	return Null;
}

void KillDebugTTY()
{
	Index<String> tty;
	Do_ps(sPid, tty, "ps -A");
	if(sTTY.GetCount())
		for(int q = sPid.Find(sTTY); q >= 0; q = sPid.FindNext(q)) {
			Sys("kill -9 " + sPid[q]);
		}
	sTTY.Clear();
}

bool TTYQuit()
{
	if(sTTY.GetCount()) {
		int fd = open("/dev/" + sTTY, O_RDWR | O_NONBLOCK);
		if(fd < 0)
			return true;
		close(fd);
		return false;
	}
	return false;
}
#else

String CreateDebugTTY() { return Null; }
void KillDebugTTY()     {}
bool TTYQuit()          { return false; }

#endif

String GdbCommand(bool console)
{
	String gdb = "gdb ";
#ifdef PLATFORM_POSIX
	if(console) {
		String tty = CreateDebugTTY();
		if(tty.GetCount())
			gdb << "-tty=" << tty << ' ';
	}
#endif
	return gdb;
}

