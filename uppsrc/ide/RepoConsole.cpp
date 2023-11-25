#include "ide.h"

UrepoConsole::UrepoConsole()
{
	CtrlLayoutExit(*this, "System Console");
	list.NoHeader().NoGrid().NoCursor().AddColumn();
	font = Courier(Ctrl::VertLayoutZoom(12));
	list.SetLineCy(max(font.GetCy(), font().Bold().GetCy(), font().Bold().Italic().GetCy()));
	exit.Hide();
	cancel.Hide();
	cancel << [=] { canceled = true; };
}

void UrepoConsole::AddResult(const String& out)
{
	const char *s = out;
	const char *b = s;
	auto Add = [&] {
		String txt(b, int(s - b));
		list.Add(AttrText("    " + txt).SetFont(font), txt);
		b = ++s;
	};
	while(*s) {
		if(*s == '\r') {
			if(s[1] == '\n') {
				Add();
				s++;
				b++;
			}
			else
			if(list.GetCount() == 0)
				Add();
			else {
				list.SetCount(list.GetCount() - 1);
				Add();
			}
		}
		else
		if(*s == '\n')
			Add();
		else
			s++;
	}
	list.GoEnd();
}

void UrepoConsole::Log(const Value& s, Color ink)
{
	list.Add(AttrText(s).SetFont(font).NormalInk(ink), s);
}

int UrepoConsole::System(const char *cmd)
{
	if(!IsOpen())
		Open();
	if(hide_password_to > hide_password_from) {
		String w = cmd;
		w = w.Mid(0, hide_password_from) + String('*', hide_password_to - hide_password_from) + w.Mid(hide_password_to);
		hide_password_to = hide_password_from = 0;
		list.Add(AttrText(w).SetFont(font().Bold()).Ink(SLtBlue()));
	}
	else
		list.Add(AttrText(cmd).SetFont(font().Bold()).Ink(SLtBlue()));
	int ii = list.GetCount();
	Ide *ide = (Ide *)TheIde();
	if(!ide)
		return -1;
	Host host;
	ide->CreateHost(host, false, false);
	LocalProcess p;
	if(!host.StartProcess(p, cmd)) {
		list.Add(AttrText("Failed to start the executable").SetFont(font().Bold()).Ink(SLtRed()));
		return -1;
	}
	String out;
	canceled = false;
	cancel.Show();
	while(p.IsRunning() && IsOpen()) {
		String h = p.Get();
		out.Cat(h);
		int lf = out.ReverseFind('\n');
		if(lf >= 0) {
			AddResult(out.Mid(0, lf + 1));
			out = out.Mid(lf + 1);
		}
		int cr = out.ReverseFind('\r');
		if(cr >= 0) {
			AddResult(out.Mid(0, cr + 1));
			out = out.Mid(cr + 1);
		}
		ProcessEvents();
		Sleep(h.GetCount() == 0); // p.Wait would be much better here!
		if(canceled)
			break;
	}
	cancel.Hide();
	out.Cat(p.Get());
	AddResult(out);
	ProcessEvents();
	int code = canceled ? -1 : p.GetExitCode();
	if(code)
		while(ii < list.GetCount()) {
			list.Set(ii, 0, AttrText((String)list.Get(ii, 1)).SetFont(font).Ink(SLtRed()));
			ii++;
		}
	return code;
}

int UrepoConsole::CheckSystem(const char *s)
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
