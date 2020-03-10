#include "PopUpTest.h"

void PopUpTest::OkCB()
{
	int exitcode = pu.GetExitCode();
	String a = AsString(pu.ei.GetData());
	drop.SetLabel(String().Cat() << "dropped OK, " << a << "(" << (exitcode) << ")");
}

void PopUpTest::CancelCB()
{
	drop.SetLabel("dropped Cancel");
}

void PopUpTest::DoDrop()
{
	drop.SetLabel("dropping");
	pu.PopUp(this);
}

PopUpTest::PopUpTest()
{
	CtrlLayout(*this, "Window title");

	CtrlLayout(pu);
	pu.Acceptor(pu.ok, CKOK).Rejector(pu.cancel, CKCANCEL);
	
	pu.WhenAccept = THISBACK(OkCB);
	pu.WhenReject = THISBACK(CancelCB);

	drop <<= THISBACK(DoDrop);
	
	//init
	pu.ei.SetData(123);
}

GUI_APP_MAIN
{
	PopUpTest().Run();
}

