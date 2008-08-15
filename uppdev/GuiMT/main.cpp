#include "CtrlLib/CtrlLib.h"

#define LAYOUTFILE <GuiMT/Divisors.lay>
#include <CtrlCore/lay.h>

struct Divisors : public WithDivisorsLayout<TopWindow> {
	typedef Divisors CLASSNAME;

	void TestNumber();
	void ShowResult(int line, String result);
	void SetTitle();

	Divisors();
	~Divisors();
};


struct DivisorsInfo {
	int       line;
	uint64    number;
	Divisors *gui;
};

void WorkerThread(DivisorsInfo f)
{
	String r;
	int divisors = 0;
	uint64 max = (uint64)sqrt((double)f.number) + 1;
	for(uint64 i = 1; i < max; i++) {
		if(Thread::IsShutdownThreads())
			break;
		if(f.number % i == 0) {
			r << ' ' << i;
			PostCallback(callback2(f.gui, &Divisors::ShowResult, f.line, "working..." + r));
			divisors++;
		}
	}
	r << ' ' << f.number;
	PostCallback(callback2(f.gui, &Divisors::ShowResult, f.line, AsString(divisors + 1) + ": " + r));
}

void Divisors::TestNumber()
{
	if(IsNull(editor))
		return;
	DivisorsInfo f;
	f.number = stou64((String)~editor);
	f.line = table.GetCount();
	table.Add(AsString(f.number), "working...");
	f.gui = this;
	Thread().Run(callback1(WorkerThread, f));
	SetTitle();
}

void Divisors::ShowResult(int line, String result)
{
	table.Set(line, 1, result);
	SetTitle();
}

void Divisors::SetTitle()
{
	Title(AsString(Thread::GetCount()) + " threads");
}

Divisors::Divisors()
{
	CtrlLayout(*this, "Window title");
	Sizeable().Zoomable();
	push <<= THISBACK(TestNumber);
	editor.SetFilter(CharFilterDigit);
	table.AddColumn("Number");
	table.AddColumn("Divisors");
	table.ColumnWidths("135 527");
	table.NoCursor();
	SetTitle();
}

Divisors::~Divisors()
{
	Thread::ShutdownThreads();
}

GUI_APP_MAIN
{
	Divisors().Run();
}
