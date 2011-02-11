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
		bool b = false;
		Callback cb = B::WhenAction;

		if(key == K_ENTER)
		{
			CallbackArgTarget<bool> c; c[false]();
			B::WhenAction = c[true];
			B::Key(key,count); b = true;
			B::WhenAction = cb;

			if(unfocusonenter)
			{
				if(B::HasFocus() && B::GetParent()) B::GetParent()->SetFocus();
			}
			else
			{
				if(!c) B::Action(); //if no other Action occured
			}
		}
		else
		{
			B::WhenAction.Clear();
			b = B::Key(key,count);
			B::WhenAction = cb;
		}

		return b;
	}
	virtual void LostFocus() { B::LostFocus(); B::Action(); }
	bool unfocusonenter;
};

#endif

