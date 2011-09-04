#ifndef _CtrlProp_PropEdit_h_
#define _CtrlProp_PropEdit_h_

#include "CtrlPropCommon.h"

#include <LogPosCtrl/LogPosCtrl.h>
#include <ValueCtrl/ValueCtrl.h>

#define LAYOUTFILE <CtrlProp/PropEdit.lay>
#include <CtrlCore/lay.h>

class PropEditCtrl : public WithPropEditLay<ParentCtrl>
{
public:
	typedef PropEditCtrl CLASSNAME;
	PropEditCtrl();

	virtual void Updated();

	void SetCtrl(Ctrl* c) { if(ctrl != c) { ctrl = c; UpdateCtrl(); } UpdateRefresh(); }
	Ctrl* GetCtrl() const { return ctrl; }
	void ClearCtrl() { SetCtrl(NULL); }

	virtual Value GetData() const { return RawToValue(ctrl); }
	virtual void SetData(const Value& v) { SetCtrl(RawValue<Ctrl*>::Extract(v)); }

	void OnUpdateRow(int i);
	void OnUpdateCurrentRow();

	const AccessorMap& GetAccessorMap() const { return am; }

protected:
	void ReloadFactory(int i, One<Ctrl>& o);
	void ReloadAction(int i);
	void UpdateCtrl();

	Ctrl* ctrl;
	AccessorMap am;
};

class PropEdit : public WithCloserKeys<PopUpC>
{
public:
	typedef PropEdit CLASSNAME;
	PropEdit();

	virtual void Deactivate() {} //let other popups open, we would close on deactivate from PopUpC

	using PopUpC::PopUp;
	void PopUp(Ctrl* owner, Ctrl& e) { pec.SetCtrl(&e); PopUpC::PopUp(owner); }

	virtual void Serialize(Stream& s);

	PropEditCtrl pec;
};

#endif
