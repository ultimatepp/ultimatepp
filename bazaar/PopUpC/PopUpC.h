#ifndef _PopUpC_PopUpC_h
#define _PopUpC_PopUpC_h

#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

class PopUpC : public StaticRect
{
public:
	typedef PopUpC CLASSNAME;
	PopUpC() { WhenDeactivate = THISBACK1(AcceptBreakIgn, int(IDEXIT)); }

	virtual void PopUp(Ctrl* owner);
	virtual void Deactivate();

	virtual void Title(const char*) {} //dummy, for use with CtrlLayout stuff

	virtual void Reject()
	{
		StaticRect::Reject();
		if(!backup.IsEmpty())
			Restore();
	}

	void Break(int ID);
	bool AcceptBreak(int ID);
	void AcceptBreakIgn(int ID) { AcceptBreak(ID); }
	void RejectBreak(int ID);

	PopUpC& Breaker(Ctrl& m, int ID) { m.WhenAction = THISBACK1(Break, ID); return *this; }
	PopUpC& Acceptor(Ctrl& m, int ID) { m.WhenAction = THISBACK1(AcceptBreakIgn, ID); return *this; }
	PopUpC& Rejector(Ctrl& m, int ID) { m.WhenAction = THISBACK1(RejectBreak, ID); return *this; }

	void Backup()
	{
		StringStream s;
		this->Serialize(s);
		backup = s;
	}
	
	void Restore()
	{
		StringStream s(backup);
		this->Serialize(s);
	}

	Callback WhenBreak;
	Callback WhenAccept;
	Callback WhenReject;
	Callback WhenDeactivate;

protected:
	String backup;
};

//can be used with both TopWindow and PopUpC
//will perform close actions with the usual keys, but doesnt need the Buttons ok,cancel,exit
template<class B>
class WithCloserKeys : public B
{
public:
	typedef WithCloserKeys<B> CLASSNAME;
	typedef WithCloserKeys<B> WCK;

	enum
	{
		OK = 0x1, //enter -> acceptor(IDOK)
		CANCEL = 0x2, //esc -> rejector(IDCANCEL)
		OKCANCEL = OK | CANCEL, //both above
		EXITACCEPT = 0x4, //esc or enter both -> acceptor(IDEXIT)
		EXITREJECT = 0x8, //esc or enter both -> rejector(IDEXIT)
		EXITBREAK = 0x10, //esc or enter both -> breaker(IDEXIT)
	};

	WithCloserKeys(int t = EXITACCEPT) : type(t) {}
	virtual bool HotKey(dword key)
	{
		if(B::HotKey(key)) return true;
		bool up = key & K_KEYUP;
		if(!up) return false;
		key = key & ~K_KEYUP;

		switch(type)
		{
			case OK:
				if(key == K_ENTER) { B::AcceptBreak(IDOK); return true; }
				break;
			case CANCEL:
				if(key == K_ESCAPE) { B::RejectBreak(IDCANCEL); return true; }
				break;
			case OKCANCEL:
				if(key == K_ENTER) { B::AcceptBreak(IDOK); return true; }
				if(key == K_ESCAPE) { B::RejectBreak(IDCANCEL); return true; }
				break;
			case EXITACCEPT:
				if(key == K_ENTER || key == K_ESCAPE) { B::AcceptBreak(IDEXIT); return true; }
				break;
			case EXITREJECT:
				if(key == K_ENTER || key == K_ESCAPE) { B::RejectBreak(IDEXIT); return true; }
				break;
			case EXITBREAK:
				if(key == K_ENTER || key == K_ESCAPE) { B::Break(IDEXIT); return true; }
				break;
		}
		return false;
	}
public:
	int type;
};

END_UPP_NAMESPACE

#endif

