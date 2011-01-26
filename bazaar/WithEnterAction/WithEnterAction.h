#ifndef _WithEnterAction_WithEnterAction_h
#define _WithEnterAction_WithEnterAction_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

template<class B>
class WithEnterAction : public B {
public:
	typedef WithEnterAction CLASSNAME;
	WithEnterAction() : unfocusonenter(false) {}

public:
	virtual bool Key(dword key, int count)
	{
		if(key == K_ENTER)
		{
			if(unfocusonenter && B::GetParent()) B::GetParent()->SetFocus();
			bool b = B::Key(key,count);
			if(!unfocusonenter) B::Action();
		}

		Callback cb = B::WhenAction;
		B::WhenAction.Clear();
		bool b = B::Key(key,count);
		B::WhenAction = cb;
		return b;
	}
	virtual void LostFocus() { B::LostFocus(); B::Action(); }
	bool unfocusonenter;
};

#endif

