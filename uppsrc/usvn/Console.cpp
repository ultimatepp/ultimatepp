#include "usvn.h"

SysConsole::SysConsole()
{
	CtrlLayoutExit(*this, "System Console");
	list.NoHeader().NoGrid().NoCursor().AddColumn();
	font = Courier(Ctrl::VertLayoutZoom(12));
	list.SetLineCy(font.Info().GetHeight());
	exit.Hide();
}

void SysConsole::AddResult(const String& out)
{
	Vector<String> h = Split(out, CharFilterCrLf);
	for(int i = 0; i < h.GetCount(); i++) {
		String s = "    " + h[i];
		list.Add(AttrText(s).SetFont(font), s);
	}
	list.GoEnd();
}

int SysConsole::System(const char *cmd)
{
	if(!IsOpen())
		Open();
	list.Add(AttrText(cmd).SetFont(font().Bold()).Ink(LtBlue));
	int ii = list.GetCount();
	LocalProcess p;
	if(!p.Start(cmd))
		return -1;
	String out;
	while(p.IsRunning()) {
		String h = p.Get();
		out.Cat(h);
		int lf = out.ReverseFind('\n');
		if(lf >= 0) {
			AddResult(out.Mid(0, lf + 1));
			out = out.Mid(lf + 1);
		}
		ProcessEvents();
		Sleep(h.GetCount() == 0); // p.Wait would be much better here!
	}
	out.Cat(p.Get());
	AddResult(out);
	ProcessEvents();
	int code = p.GetExitCode();
	if(code)
		while(ii < list.GetCount()) {
			list.Set(ii, 0, AttrText((String)list.Get(ii, 1)).SetFont(font).Ink(LtRed));
			ii++;
		}
	return code;
}

int SysConsole::CheckSystem(const char *s)
{
	int exitcode = System(s);
	if(exitcode) {
		if(exitcode < 0)
			AddResult("Error running " + String(s));
		else
			AddResult("exitcode = " + FormatInt(exitcode));
	}
	return exitcode;
}
