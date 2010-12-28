#ifndef _LogPosCtrl_LogPosCtrl_h
#define _LogPosCtrl_LogPosCtrl_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <LogPosCtrl/LogPosPopUp.lay>
#include <CtrlCore/lay.h>

#include <PopUpC/PopUpC.h>

const dword LOGPOS_V    = 20;
NAMESPACE_UPP
template<> inline dword ValueTypeNo(const Ctrl::LogPos*)    { return LOGPOS_V; }
END_UPP_NAMESPACE

class LogPosPopUp : public WithLogPosPopUpLayout<PopUpC> {
public:
	typedef LogPosPopUp CLASSNAME;
	LogPosPopUp();
	
	static Rect CtrlRect(Ctrl::LogPos pos, Size sz);
	static Ctrl::LogPos MakeLogPos(int ax, int ay, const Rect& r, Size sz);
	static Ctrl::LogPos MakeLogPos(Ctrl::LogPos p, const Rect& r, Size sz);
	static Ctrl::LogPos ReAlign(const Ctrl& c, const Ctrl::LogPos& npos);

	virtual void Updated();
	virtual Value GetData() const { return RawToValue(pos); }
	virtual void SetData(const Value& v) { if(!v.Is<Ctrl::LogPos>()) return; Set(RawValue<Ctrl::LogPos>::Extract(v)); Update(); }

	Ctrl::LogPos Generate() const; 	//obsolete
	Ctrl::LogPos Get() const { return pos; }
	void Set(const Ctrl::LogPos& p);
	void PopUp(Ctrl* owner, const Ctrl::LogPos& p) { Set(p); Update(); PopUpC::PopUp(owner); }

	Callback WhenSizeChange;
	Callback WhenAlignChange;
	
protected:
	void XaCB();
	void XbCB();
	void YaCB();
	void YbCB();
	void XAlignCB();
	void YAlignCB();

	Ctrl::LogPos pos;
};

NAMESPACE_UPP
template<> String AsString(const Ctrl::LogPos& pos);
END_UPP_NAMESPACE

class LogPosCtrl : public StaticText
{
public:
	typedef LogPosCtrl CLASSNAME;
	LogPosCtrl();

	virtual void LeftDown(Point p, dword keyflags);
	virtual void Updated();
	virtual Value GetData() const { return RawToValue(pos); }
	virtual void SetData(const Value& v) { if(!v.Is<Ctrl::LogPos>()) return; pos = RawValue<Ctrl::LogPos>::Extract(v); Update(); }

	Ctrl::LogPos Get() const { return pos; }
	void Set(const Ctrl::LogPos& p) { pos = p; Update(); }
	void Set(Ctrl& c) { posparent = &c; pos = c.GetPos(); Update(); }
	void Drop();
	
protected:
	void CloseDrop();
	void AcceptDrop();
	void ActionDrop();
	void OnSizeChange();
	void OnAlignChange();

	Ctrl::LogPos pos, savedpos;
	bool push;
	LogPosPopUp lc;
	Ptr<Ctrl> posparent;
};

#endif

