#ifndef _CtrlLib_Lang_h_
#define _CtrlLib_Lang_h_

class LNGCtrl : public StaticRect
{
public:
	virtual Value GetData() const;
	virtual void  SetData(const Value& v);

	virtual Size  GetMinSize() const;

private:
	DropList hi;
	DropList lo;

	void LoadLo();
	void Hi();
	void Lo();

public:
	typedef LNGCtrl CLASSNAME;

	LNGCtrl& DropFocus(bool b = true)    { hi.DropFocus(b); lo.DropFocus(b); return *this; }
	LNGCtrl& NoWantFocus()               { hi.NoWantFocus(); lo.NoWantFocus(); return *this; }

	LNGCtrl();
	virtual ~LNGCtrl();
};

#endif
