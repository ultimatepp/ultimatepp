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
	virtual void Undo();

	void SetCtrl(Ctrl* c) { if(ctrl != c) { ctrl = c; UpdateCtrl(); } UpdateRefresh(); }
	Ctrl* GetCtrl() const { return ctrl; }
	void ClearCtrl() { SetCtrl(NULL); }

	virtual Value GetData() const { return RawToValue(~ctrl); }
	virtual void SetData(const Value& v) { SetCtrl(RawValue<Ctrl*>::Extract(v)); }

protected:
	void ReloadFactory(int i, One<Ctrl>& o);
	void ReloadAction(int i);
	void UpdateCtrl();

	void OnUpdateRow(int i);
	void OnUpdateCurrentRow();

	Ptr<Ctrl> ctrl;
	ArrayMap<String, Tuple2<bool, Value> > vsav;
	AccessorMap am;
};

class PropEdit : public PopUpC
{
public:
	typedef PropEdit CLASSNAME;
	PropEdit();

	virtual void Deactivate() {} //let other popups open

	void PopUp(Ctrl* owner, Ctrl& e) { pec.SetCtrl(&e); PopUpC::PopUp(owner); }

	virtual void Rejector() { pec.Undo(); pec.ClearCtrl(); PopUpC::Rejector(); }
	virtual void Acceptor() { pec.ClearCtrl(); PopUpC::Acceptor(); }

	PropEditCtrl pec;
	Button ok, cancel;
};

#endif
