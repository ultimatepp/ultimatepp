#include "Debuggers.h"

#define LLOG(x)  LOG(x)

#define IMAGECLASS DbgImg
#define IMAGEFILE  <ide/Debuggers/Debuggers.iml>
#include <Draw/iml_source.h>

const char *FindTag(const char *txt, const char *tag)
{
	while(*txt) {
		const char *q = txt;
		const char *t = tag;
		while(*q == *t) {
			q++;
			t++;
			if(*t == '\0') return txt;
		}
		txt++;
	}
	return NULL;
}

const char *AfterTag(const char *txt, const char *tag)
{
	while(*txt) {
		const char *q = txt;
		const char *t = tag;
		while(*q == *t) {
			q++;
			t++;
			if(*t == '\0') return q;
		}
		txt++;
	}
	return NULL;
}

const char *AfterHeading(const char *txt, const char *heading)
{
	while(*heading)
		if(*txt++ != *heading++)
			return NULL;
	return txt;
}

void RedDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword s) const {
	w.DrawRect(r, paper);
	DrawSmartText(w, r.left, r.top, r.Width(), String(q), StdFont(), LtRed());
}

VectorMap<String, String> DataMap(const ArrayCtrl& data)
{
	GuiLock __;
	
	VectorMap<String, String> m;
	for(int i = 0; i < data.GetCount(); i++)
		m.Add(data.Get(i, 0), data.Get(i, 1));
	return m;
}

void MarkChanged(const VectorMap<String, String>& m, ArrayCtrl& data)
{
	GuiLock __;
	
	for(int i = 0; i < data.GetCount(); i++) {
		int q = m.Find(data.Get(i, 0));
		if(q >= 0 && m[q] != data.Get(i, 1))
			data.SetDisplay(i, 1, Single<RedDisplay>());
		else
			data.SetDisplay(i, 1, StdDisplay());
	}
}

void Gdb::Lock()
{
	IdeDebugLock();
	watches.Disable();
	locals.Disable();
	frame.Disable();
	dlock.Show();
}

void Gdb::Unlock()
{
	if(IdeDebugUnLock()) {
		watches.Enable();
		locals.Enable();
		frame.Enable();
		dlock.Hide();
	}
}

bool Gdb::Result(String& result, const String& s)
{
	result.Cat(s);
	int l = result.GetLength();
	int q = result.Find(GDB_PROMPT, max(0, l - 50));
	if(q >= 0) {
		result.Trim(q);
		return true;
	}
	return false;
}

String Gdb::Cmd(const char *command)
{
	if(!dbg || !dbg->IsRunning() || IdeIsDebugLock()) return Null;
#ifdef _DEBUG
	TimeStop ts;
#endif
	Lock();
	if(command) {
		LLOG("========= Cmd: " << command);
		dbg->Write(String(command) + "\n");
		PutVerbose(command);
	}
	String result;
	int ms0 = msecs();
	while(dbg) {
		String s;
		if(!dbg->Read(s)) {
			PutVerbose(result);
			PutVerbose("Debugger terminated");
			break;
		}
		if(!s.IsEmpty() && Result(result, s)) {
			LLOG(result);
			PutVerbose(result);
			break;
		}
		if(ms0 != msecs()) {
			ProcessEvents();
			ms0 = msecs();
		}
//		if(s.GetCount() == 0)
			GuiSleep(0);
		if(TTYQuit())
			Stop();
	}
	Unlock();
#ifdef _DEBUG
	if(command)
		LLOG("Cmd Time of `" << command <<"` " << ts);
#endif
	return result;
}

String Gdb::FastCmd(const char *command)
{
	if(!dbg || !dbg->IsRunning() || IdeIsDebugLock()) return Null;
	bool lock = false;
	if(command) {
		LLOG("FastCmd: " << command);
		dbg->Write(String(command) + "\n");
		PutVerbose(command);
	}
	String result;
	TimeStop ts;
	while(dbg) {
		String s;
		if(TTYQuit())
			Stop();
		if(!dbg->Read(s)) {
			LLOG(result);
			PutVerbose(result);
			PutVerbose("dbg terminated");
			break;
		}
		if(!s.IsEmpty() && Result(result, s)) {
			LLOG(result);
			LLOG("Result length: " << result.GetLength());
			if(result.GetLength() < 1000)
				PutVerbose(result);
			break;
		}
		if(s.GetCount() == 0)
			Sleep(0);
		if(ts.Elapsed() > 500) {
			if(!lock) {
				lock = true;
				Lock();
			}
			Ctrl::ProcessEvents();
		}
	}
	if(lock)
		Unlock();
#ifdef _DEBUG
	if(command)
		LLOG("Fast cmd Time of `" << command <<"` " << ts);
#endif
	return result;
}

void Gdb::Stop()
{
	if(dbg && dbg->IsRunning())
		dbg->Kill();
}

bool Gdb::IsFinished()
{
	return !dbg->IsRunning() && !IdeIsDebugLock();
}
