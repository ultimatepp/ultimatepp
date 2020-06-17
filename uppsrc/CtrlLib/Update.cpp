#include "CtrlLib.h"

namespace Upp {

#ifdef GUI_WIN
#ifndef PLATFORM_WINCE

#define LLOG(x)

static String s_updatedir;
static String s_updater;

void UpdateSetDir(const char *path)
{
	s_updatedir = path;
}

void UpdateSetUpdater(const char *exename)
{
	s_updater = exename;
}

String UpdateGetDir()
{
	return Nvl(s_updatedir, GetIniKey("UPDATE"));
}

Time FileTimeToTime(const FILETIME& time) {
	SYSTEMTIME t;
	FileTimeToSystemTime(&time, &t);
	return Time(t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
}

bool NoUpdate(const char *dsf, const char *srf, int& len) {
	FindFile dst(dsf);
	FindFile src(srf);
	if(!src) return true;
	len = (int)src.GetLength();
	if(!dst) return false;
	return abs(FileTimeToTime(dst.GetLastWriteTime()) - FileTimeToTime(src.GetLastWriteTime()))
		    < 100 && len == dst.GetLength();
}

void UpdateFile(String dst, String src)
{
	if(src.IsEmpty()) return;
	String filename = GetFileName(dst);
	int len;
	if(NoUpdate(dst, src, len)) return;
	String tmp = dst + ".tmp";
	Progress pi;
	pi.Title((t_("Updating ") + filename).ToWString());
	pi.SetText(t_("Updating ") + filename);
//	pi.Open();
	FileIn in(src);
	if(!in)
	{
		Exclamation(Format(t_("Error opening file [* \1%s\1]."), src));
		return;
	}
	FileDelete(tmp);
	FileOut out(tmp);
	dword n = 0;
	char buffer[2048];
	if(!out)
	{
		Exclamation(Format(t_("Error creating file [* \1%s\1]."), tmp));
		return;
	}
	for(;;)
	{
		dword i = in.Get(buffer, 2048);
		if(i == 0) break;
		out.Put(buffer, i);
		n += i;
		pi.Set(n, len);
		if(pi.Canceled() && PromptYesNo(t_("Aborting update is likely to cause trouble when running the application for the next time.\nContinue?")))
		{
			out.Close();
			FileDelete(tmp);
			return;
		}
	}
	out.SetTime(in.GetTime());
	out.Close();
	if(out.IsError())
	{
		Exclamation(Format(t_("Error writing file [* \1%s\1]."), tmp));
		FileDelete(tmp);
		return;
	}
	String old = dst + ".old";
	pi.SetText(Format(t_("Overwriting %s"), dst));
	pi.SetTotal(10000);
	int start = msecs();
	for(;;)
	{
		SetFileAttributes(dst, 0);
		FileDelete(old);
		FileMove(dst, old);
		if(FileMove(tmp, dst))
			return;
		if(pi.SetPosCanceled(msecs(start) % 10000) && PromptYesNo(t_("Aborting update is likely to cause trouble when running the application for the next time.\nContinue?")))
			return;
		Sleep(500);
	}
}

void UpdateFile(const char *filename)
{
	String dst = GetExeDirFile(filename);
	String src = UpdateGetDir();
	if(IsNull(src))
		return;
	UpdateFile(dst, AppendFileName(src, filename));
}

static String FixArg(String s)
{
	if(s.Find(' ') < 0 && s.Find('\"') < 0)
		return s;
	String ucmd;
	ucmd << '\"';
	for(const char *p = s; *p; p++)
		if(*p == '\"')
			ucmd << "\"\"";
		else
			ucmd << *p;
	ucmd << '\"';
	return ucmd;
}

void SelfUpdate() {
	char dst[512];
	UpdateFile(Nvl(s_updater, String("UPDATER.EXE")));
	::GetModuleFileName(NULL, dst, 512);
	{
		String src = UpdateGetDir();
		if(src.IsEmpty()) return;
		src = AppendFileName(src, GetFileNamePos(dst));
		int dummy;
		if(NoUpdate(dst, src, dummy)) return;
		String commandline;
		const Vector<String>& cmd = CommandLine();
		for(int i = 0; i < cmd.GetCount(); i++) {
			commandline += ' ';
			commandline += FixArg(cmd[i]);
		}
		if(WinExec(GetExeDirFile("updater.exe") + " " + GetFileName(dst) + commandline, SW_SHOWNORMAL) <= 31)
			return;
	}
	exit(0);
}

bool SelfUpdateSelf()
{
	const Vector<String>& cmdline = CommandLine();
	String exe = GetExeFilePath();

	if(cmdline.GetCount() >= 2 && cmdline[0] == "-update")
	{ // updater
		String exec = FixArg(cmdline[1]);
		UpdateFile(exec, exe);
		for(int i = 2; i < cmdline.GetCount(); i++)
			exec << ' ' << FixArg(cmdline[i]);
		WinExec(exec, SW_SHOWNORMAL);
		return true;
	}

	String src = UpdateGetDir();
	if(src.IsEmpty()) return false;
	src = AppendFileName(src, GetFileNamePos(exe));
	int dummy;
	if(NoUpdate(exe, src, dummy)) return false;

	String commandline;
	commandline << src << " -update " << FixArg(exe);
	for(int i = 0; i < cmdline.GetCount(); i++)
		commandline << ' ' << FixArg(cmdline[i]);
	LLOG("SelfUpdateSelf (running updater): " << commandline);
	return WinExec(commandline, SW_SHOWNORMAL) >= 32;
}

#endif
#endif

}
