#ifndef _PopUpC_PopUpC_h
#define _PopUpC_PopUpC_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class PopUpC : public StaticRect
{
public:
	typedef PopUpC CLASSNAME;

	virtual void Acceptor();
	virtual void Rejector();
	virtual void PopUp(Ctrl* owner);
	virtual void Deactivate();

	Callback WhenSelect;
	Callback WhenCancel;
};

#endif

