#include <CtrlLib/CtrlLib.h>
#include <Report/Report.h>

using namespace Upp;

void Print(String text)
{
	int top  = 1;
	int left = 1;
	int sizex = 4600;
	int sizey = 6800;

	Size sz(sizex-(2*top),sizey-(2*left));

	String txt = "[ $$0,0#00000000000000000000000000000000:Default]\n[{_} \n[s0; [*1   Seite $$P]&]\n[s0;*1 &]\n[ {{10000<64;h1;^t/13b/4@(229) [s0; [*1 Artikel]]\n::@2 [s0; [1 1]]\n:: [s0; [1 2]&]\n[s0; [1 3]]\n:: [s0; [1 040114]]\n:: [s0; [1 5]]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0; [1 xxx]]\n:: [s0; [1 xxx]]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]\n:: [s0;1 ]}}&]\n[s0;*2 ]";

	Report rep;
	rep.SetPageSize(sz);
	
	rep.Put(txt);

	Size size(rep.GetSize());
	Report report(size);
	
	Perform(rep,"Original");

	const Array <Drawing>& aDw = rep.GetPages();
	
	for(int i = 0; i < aDw.GetCount(); i++) 
	{
		if(i>0) 
			report.NewPage();
		report.DrawDrawing(size,aDw[i]);
	}

	Perform(report,"copy");
}

GUI_APP_MAIN
{
	Print("Just a test!");
}

