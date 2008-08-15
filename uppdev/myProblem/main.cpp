#include "myProblem.h"

GUI_APP_MAIN
{
	TopWindow a;
	Splitter b;
	WithmyProblemLayout<ParentCtrl> c;		
	CtrlLayout(c);	
	LineEdit d;
	b.Horz(d, c);
	a.Add(b);
	a.Run();
}

