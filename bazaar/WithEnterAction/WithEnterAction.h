#ifndef _WithEnterAction_WithEnterAction_h
#define _WithEnterAction_WithEnterAction_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

template<class B>
class WithEnterAction : public B {
public:
	typedef WithEnterAction CLASSNAME;

public:
	virtual bool Key(dword key, int count)
	{
		if(key == K_ENTER)
		if(GetParent()) B::GetParent()->SetFocus();
		return B::Key(key,count);
	}
	virtual void LostFocus() { B::LostFocus(); WhenAction(); }

	Callback     operator<<=(Callback action)  { WhenAction = action; return action; }
	Callback&    operator<<(Callback action)   { return WhenAction << action; }

	Callback       WhenAction;
};

#endif

