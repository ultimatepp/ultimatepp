#ifndef _LogPosCtrl_LogPosCtrl_h
#define _LogPosCtrl_LogPosCtrl_h

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <PopUpC/PopUpC.h>
#include <Gen/VTypes.h>

#define LAYOUTFILE <LogPosCtrl/LogPosPopUp.lay>
#include <CtrlCore/lay.h>

class LogPosPopUp : public WithLogPosPopUpLayout<WithCloserKeys<PopUpC> > {
public:
	typedef LogPosPopUp CLASSNAME;
	LogPosPopUp();
	
	static Rect CtrlRect(Ctrl::LogPos pos, Size sz);
	static Rect CtrlRectZ(Ctrl::LogPos pos, Size sz);
	static Ctrl::LogPos MakeLogPos(int ax, int ay, const Rect& r, Size sz);
	static Ctrl::LogPos MakeLogPos(Ctrl::LogPos p, const Rect& r, Size sz);
	static Ctrl::LogPos MakeLogPos(Ctrl::LogPos p, const Ctrl::LogPos& pos, Size sz);
	static Ctrl::LogPos MakeLogPos(Ctrl::LogPos p, const Ctrl& c);

	virtual void Updated();
	virtual Value GetData() const { return RichToValue(pos); }
	virtual void SetData(const Value& v) { if(!v.Is<Ctrl::LogPos>()) return; Set(RichValue<Ctrl::LogPos>::Extract(v)); Update(); }

	Ctrl::LogPos Generate() const; 	//obsolete
	Ctrl::LogPos Get() const { return pos; }
	void Set(const Ctrl::LogPos& p);

	using PopUpC::PopUp;
	void PopUp(Ctrl* owner, const Ctrl::LogPos& p) { Set(p); Update(); PopUpC::PopUp(owner); }

	virtual void Serialize(Stream& s) { s % pos; }

	Callback WhenSizeChange;
	Callback WhenAlignChange;
	
	void XaCB();
	void XbCB();
	void YaCB();
	void YbCB();

	void LRCB() { XAlign(); DoAlignAction(); }
	void TBCB() { YAlign(); DoAlignAction(); }

	void LTCB() { l = true; r = false; t = true; b = false; XAlign(); YAlign(); DoAlignAction(); }
	void RTCB() { l = false; r = true; t = true; b = false; XAlign(); YAlign(); DoAlignAction(); }
	void LBCB() { l = true; r = false; t = false; b = true; XAlign(); YAlign(); DoAlignAction(); }
	void RBCB() { l = false; r = true; t = false; b = true; XAlign(); YAlign(); DoAlignAction(); }

	void HCCB() { l = false; r = false; XAlign(); DoAlignAction(); }
	void VCCB() { t = false; b = false; YAlign(); DoAlignAction(); }
	void CCCB() { l = false; r = false; t = false; b = false; XAlign(); YAlign(); DoAlignAction(); }

	void HSCB() { l = true; r = true; XAlign(); DoAlignAction(); }
	void VSCB() { t = true; b = true; YAlign(); DoAlignAction(); }
	void SSCB() { l = true; r = true; t = true; b = true; XAlign(); YAlign(); DoAlignAction(); }

protected:
	void XAlign();
	void YAlign();
	void DoAlignAction() { WhenAlignChange(); UpdateAction(); }

	Ctrl::LogPos pos;
};

class LogPosCtrl : public StaticText
{
public:
	typedef LogPosCtrl CLASSNAME;
	LogPosCtrl();

	virtual void LeftDown(Point p, dword keyflags);
	virtual void Updated();

	void SetBaseSize(const Size& sz) { bsz = sz; }
	void ClearBaseSize() { bsz = Null; }
	Size GetBaseSize() const { return bsz; }

	virtual Value GetData() const { return RichToValue(pos); }
	virtual void SetData(const Value& v) { if(!v.Is<Ctrl::LogPos>()) return; pos = RichValue<Ctrl::LogPos>::Extract(v); Update(); }

	Ctrl::LogPos Get() const { return pos; }
	void Set(const Ctrl::LogPos& p, const Size& sz = Null) { pos = p; bsz = sz; Update(); }
	void Set(Ctrl& c) { pos = c.GetPos(); bsz = c.GetParent() ? c.GetParent()->GetSize() : Null; Update(); }
	void Drop();

	virtual void Serialize(Stream& s) { s % pos; }

	void OnReject();
	void OnAccept();
	void OnAction();
	void OnSizeChange();
	void OnAlignChange();

	LogPosPopUp lc;
	
protected:
	Ctrl::LogPos pos;
	bool push;
	Size bsz; //base info, for recalculation
};

#endif

