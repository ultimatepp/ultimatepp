#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <LayoutInh/lay.lay>
#include <CtrlCore/lay.h>

class DlgBase : public WithBase<TopWindow> {
public:

	typedef DlgBase CLASSNAME;

	DlgBase();
};

DlgBase::DlgBase()
{
	CtrlLayoutOK(*this, "");
}

class DlgDerived : public WithDerived<DlgBase> {
public:

	typedef DlgDerived CLASSNAME;

	DlgDerived();
};

DlgDerived::DlgDerived()
{
	CtrlLayout(*this, "");
}

GUI_APP_MAIN
{
	DlgDerived().Run();
}

