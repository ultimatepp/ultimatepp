#include "ThreadCrash.h"

ThreadCrash::ThreadCrash()
{
	CtrlLayout(*this, "Window title");
	AddFrame(bar);
	bar.Set(THISBACK(MakeToolBar));
	arr.AddColumn("Number");
	Add(arr.SizePos());
}

void ThreadCrash::MakeToolBar(Bar &bar)
{
	bar.Add("Push me", CtrlImg::new_doc(), THISBACK(RefreshOptionThr)).Key(K_F5);
}

void ThreadCrash::RefreshOption()
{
	arr.Clear();
	for(int i = 0; i < 1000; i++)
		arr.Add(i);
}

void ThreadCrash::RefreshOptionThr()
{
	thr.Run(THISBACK(RefreshOption));
}

GUI_APP_MAIN
{
	ThreadCrash().Run();
}
