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

enum CK
{
	CKOK = 0x1, //enter -> acceptor(IDOK)
	CKCANCEL = 0x2, //esc -> rejector(IDCANCEL)
	CKOKCANCEL = CKOK | CKCANCEL, //both above
	CKEXITACCEPT = 0x4, //esc or enter both -> acceptor(IDEXIT)
	CKEXITREJECT = 0x8, //esc or enter both -> rejector(IDEXIT)
	CKEXITBREAK = 0x10, //esc or enter both -> breaker(IDEXIT)
};


//can be used with both TopWindow and PopUpC
//will perform close actions with the usual keys, but doesnt need the Buttons ok,cancel,exit
template<class B>
class WithCloserKeys : public B
{
public:
	typedef WithCloserKeys<B> CLASSNAME;

	WithCloserKeys(int t = CKEXITACCEPT) : cktype(t), ckkeyupactive(true) {}
	virtual bool HotKey(dword key)
	{
		if(B::HotKey(key)) return true;
		bool up = key & K_KEYUP;
		if(up ^ ckkeyupactive) return false;
		key = key & ~K_KEYUP;

		switch(cktype)
		{
			case CKOK:
				if(key == K_ENTER) { B::AcceptBreak(IDOK); return true; }
				break;
			case CKCANCEL:
				if(key == K_ESCAPE) { B::RejectBreak(IDCANCEL); return true; }
				break;
			case CKOKCANCEL:
				if(key == K_ENTER) { B::AcceptBreak(IDOK); return true; }
				if(key == K_ESCAPE) { B::RejectBreak(IDCANCEL); return true; }
				break;
			case CKEXITACCEPT:
				if(key == K_ENTER || key == K_ESCAPE) { B::AcceptBreak(IDEXIT); return true; }
				break;
			case CKEXITREJECT:
				if(key == K_ENTER || key == K_ESCAPE) { B::RejectBreak(IDEXIT); return true; }
				break;
			case CKEXITBREAK:
				if(key == K_ENTER || key == K_ESCAPE) { B::Break(IDEXIT); return true; }
				break;
		}
		return false;
	}
public:
	int cktype;
	bool ckkeyupactive;
};

END_UPP_NAMESPACE

#endif

