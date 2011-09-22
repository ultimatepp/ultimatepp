#ifndef _CtrlProp_PropEdit_h_
#define _CtrlProp_PropEdit_h_

#include <ValueAccessorEdit/ValueAccessorEdit.h>

NAMESPACE_UPP

class PropEditCtrl : public AccessorMapCtrl
{
public:
	typedef PropEditCtrl CLASSNAME;
	PropEditCtrl() : ctrl(NULL) { WhenAction = THISBACK(OnAction); }

	void SetCtrl(Ctrl* c) { if(ctrl != c) { ctrl = c; UpdateCtrl(); } Update(); }
	Ctrl* GetCtrl() const { return ctrl; }
	void ClearCtrl() { SetCtrl(NULL); }

	virtual Value GetData() const { return RawToValue(ctrl); }
	virtual void SetData(const Value& v) { SetCtrl(RawValue<Ctrl*>::Extract(v)); }

	void OnAction();

protected:
	void UpdateCtrl();

	Ctrl* ctrl;
	One<AccessorMap> oam;
};

class PropEdit : public WithCloserKeys<PopUpC>
{
public:
	typedef PropEdit CLASSNAME;
	PropEdit();

	virtual void Deactivate() {} //let other popups open, we would close on deactivate from PopUpC

	using PopUpC::PopUp;
	void PopUp(Ctrl* owner, Ctrl& e) { pec.SetCtrl(&e); PopUpC::PopUp(owner); }

	PropEditCtrl pec;
};

END_UPP_NAMESPACE

#endif
