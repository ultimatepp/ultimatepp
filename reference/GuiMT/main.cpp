#include "CtrlLib/CtrlLib.h"

using namespace Upp;

#define LAYOUTFILE <GuiMT/Divisors.lay>
#include <CtrlCore/lay.h>

// This example shows older way how to do multithreded GUI using PostCallback to communicate
// between worker thread and main GUI thread.
// While PostCallback is still useful in certain circumstances, new applications are
// perhaps better off using GuiLock and Call, as demostrated in GuiLock reference example.

struct Divisors : public WithDivisorsLayout<TopWindow> {
	volatile Atomic terminated;
	volatile Atomic threads;

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
	String r1, r2;
	int divisors = 0;
	uint64 max = (uint64)sqrt((double)f.number) + 1;
	for(uint64 i = 1; i < max; i++) {
		if(f.gui->terminated)
			break;
		if(f.number % i == 0) {
			r1 << ' ' << i;
			divisors++;
			uint64 j = f.number / i;
			if (j != i) {
				r2 = " " + AsString(j) + r2;
				divisors++;
			}
			PostCallback([=] { f.gui->ShowResult(f.line, "working..." + r1 + r2); });
		}
	}
	PostCallback([=] { f.gui->ShowResult(f.line, AsString(divisors) + ": " + r1 + r2); });
	AtomicDec(f.gui->threads);
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
	AtomicInc(threads);
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
	int n = threads;
	Title(AsString(n) + " threads");
}

Divisors::Divisors()
{
	CtrlLayout(*this, "Window title");
	Sizeable().Zoomable();
	push << [=] { TestNumber(); };
	editor.SetFilter(CharFilterDigit);
	table.AddColumn("Number");
	table.AddColumn("Divisors");
	table.ColumnWidths("135 527");
	table.NoCursor();
	terminated = 0;
	threads = 0;
	SetTitle();
}

Divisors::~Divisors()
{
	terminated = 1;
	while(threads)
		Sleep(10);
}

GUI_APP_MAIN
{
	Divisors().Run();
}
