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

void Dbg::Lock()
{
	IdeDebugLock();
	watches.Disable();
	locals.Disable();
	frame.Disable();
	dlock.Show();
}

void Dbg::Unlock()
{
	if(IdeDebugUnLock()) {
		watches.Enable();
		locals.Enable();
		frame.Enable();
		dlock.Hide();
	}
}

String Dbg::Cmd(const char *command)
{
	if(!dbg || !dbg->IsRunning() || IdeIsDebugLock()) return Null;
#ifdef _DEBUG
	TimeStop ts;
#endif
	Lock();
	if(command) {
		LLOG("Cmd: " << command);
		dbg->Write(String(command) + "\n");
		PutVerbose(command);
	}
	String result;
	int ms0 = msecs();
	while(dbg) {
		String s;
		LLOG("About to read");
		if(!dbg->Read(s)) {
			LLOG("Read: " << result);
			PutVerbose(result);
			PutVerbose("Debugger terminated");
			break;
		}
		if(!s.IsEmpty() && Result(result, s)) {
			LLOG("Processed " << result);
			PutVerbose(result);
			break;
		}
		LLOG("Unprocessed Result length: " << s);
		GuiSleep(0);
		if(ms0 != msecs()) {
			ProcessEvents();
			ms0 = msecs();
		}
		if(TTYQuit())
			Stop();
	}
	Unlock();
#ifdef _DEBUG
	if(command)
		LLOG("Time of `" << command <<"` " << ts);
#endif
	return result;
}

String Dbg::FastCmd(const char *command)
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
//		if(!lock)
			Sleep(0);
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
		if(ts.Elapsed() > 4000) {
			if(!lock) {
				lock = true;
				Lock();
			}
//			Ctrl::GuiSleep()(20);
			Ctrl::ProcessEvents();
		}
	}
	if(lock)
		Unlock();
#ifdef _DEBUG
	if(command)
		LLOG("Time of `" << command <<"` " << ts);
#endif
	return result;
}

void Dbg::Stop()
{
	if(dbg && dbg->IsRunning())
		dbg->Kill();
}

bool Dbg::IsFinished()
{
	return !dbg->IsRunning() && !IdeIsDebugLock();
}

Dbg::Dbg()
{
	CtrlLayout(regs);
	regs.Height(regs.GetLayoutSize().cy);
	AddReg("eax", &regs.eax);
	AddReg("ebx", &regs.ebx);
	AddReg("ecx", &regs.ecx);
	AddReg("edx", &regs.edx);
	AddReg("esi", &regs.esi);
	AddReg("edi", &regs.edi);
	AddReg("ebp", &regs.ebp);
	AddReg("esp", &regs.esp);
	regs.Color(SColorLtFace);
	regs.AddFrame(TopSeparatorFrame());
	regs.AddFrame(RightSeparatorFrame());

	locals.NoHeader();
	locals.AddColumn("", 1);
	locals.AddColumn("", 6);
	watches.NoHeader();
	watches.AddColumn("", 1).Edit(watchedit);
	watches.AddColumn("", 6);
	watches.Inserting().Removing();
	autos.NoHeader();
	autos.AddColumn("", 1);
	autos.AddColumn("", 6);
	Add(tab.SizePos());
	tab.Add(watches.SizePos(), "Watches");
	tab.Add(locals.SizePos(), "Locals");
	tab.Add(autos.SizePos(), "Autos");
	Add(frame.HSizePos(200, 0).TopPos(2, EditField::GetStdHeight()));
	frame.Ctrl::Add(dlock.SizePos());
	dlock = "  Running..";
	dlock.SetFrame(BlackFrame());
	dlock.SetInk(Red);
	dlock.NoTransparent();
	dlock.Hide();

	CtrlLayoutOKCancel(quickwatch, "Quick watch");
	quickwatch.WhenClose = quickwatch.Breaker(IDCANCEL);
	quickwatch.value.SetReadOnly();
	quickwatch.value.SetFont(CourierZ(12));
	quickwatch.Sizeable().Zoomable();
	quickwatch.NoCenter();
	quickwatch.SetRect(0, 150, 300, 400);
	quickwatch.Icon(DbgImg::QuickWatch());

	Transparent();
}
