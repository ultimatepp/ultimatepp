#ifndef _WithEnterAction_WithEnterAction_h
#define _WithEnterAction_WithEnterAction_h

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

template<class B>
class WithEnterAction : public B {
public:
	typedef WithEnterAction CLASSNAME;
	WithEnterAction() : unfocusonenter(false), multipleenter(false), informed(false) { B::WantFocus(); }

public:
	virtual bool Key(dword key, int count)
	{
		Callback cb = B::WhenAction;
		bool up = key & K_KEYUP;
		dword purekey = key & ~K_KEYUP;

		if(purekey == K_ENTER)
		{
			CallbackArgTarget<bool> c; c[false]();
			B::WhenAction = c[true];
			bool b = B::Key(key, count);
			B::WhenAction = cb;

			if(!multipleenter && informed) return false;

			//not informed, we generally process up and down enter, though we dont need up
			if(up) return true;

			if(unfocusonenter && (B::HasFocus() && B::GetParent()))
				B::GetParent()->SetFocus();
			else
			{
				informed = true;
				if(!c) B::Action(); //prevent double invoke
			}
			return true;
		}

		B::WhenAction.Clear();
		bool b = B::Key(key, count);
		B::WhenAction = cb;
		if(!up && b) informed = false; //prepare for resend on lostfocus or on enter after each keypress
		return b;
	}
	virtual void LostFocus() { B::LostFocus(); if(!informed) { informed = true; B::Action(); } }
	virtual bool Accept() { if(!B::Accept()) return false; if(!informed) { informed = true; B::Action(); } return true; }
	virtual void Reject() { informed = true; B::Reject(); }

	bool unfocusonenter;
	bool multipleenter;
protected:
	bool informed;
};

#endif

