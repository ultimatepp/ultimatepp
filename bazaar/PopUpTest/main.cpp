#include "PopUpTest.h"

void PopUpTest::OkCB()
{
	String a = AsString(pu.ei.GetData());
	drop.SetLabel(String().Cat() << "dropped OK, " << a);
}

void PopUpTest::CancelCB()
{
	drop.SetLabel("dropped Cancel");
}

void PopUpTest::DoDrop()
{
	drop.SetLabel("dropping");
	pu.ei.SetData(123);
	pu.PopUp(this);
}

PopUpTest::PopUpTest()
{
	CtrlLayout(*this, "Window title");

	CtrlLayout(pu);
	pu.ok <<= callback(&pu, &PopUpC::Acceptor);
	pu.cancel <<= callback(&pu, &PopUpC::Rejector);
	pu.WhenSelect = THISBACK(OkCB);
	pu.WhenCancel = THISBACK(CancelCB);

	drop <<= THISBACK(DoDrop);
}

GUI_APP_MAIN
{
	PopUpTest().Run();
}

