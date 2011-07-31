#ifndef _CtrlProp_PropEdit_h_
#define _CtrlProp_PropEdit_h_

#include "CtrlPropCommon.h"

#include <LogPosCtrl/LogPosCtrl.h>
#include <ValueCtrl/ValueCtrl.h>

#include <Gen/Gen.h>

#define LAYOUTFILE <CtrlProp/PropEdit.lay>
#include <CtrlCore/lay.h>

class PropEditCtrl : public WithPropEditLay<ParentCtrl>, public Visiting<Ctrl>
{
public:
	typedef PropEditCtrl CLASSNAME;
	typedef Visiting<Ctrl> V;
	PropEditCtrl();

	virtual void Visit(Ctrl& e);
	virtual void Reload();
	virtual void Clear();
	virtual void Restore();

	void OnUpdateRow();

protected:
	ArrayMap<String, Tuple2<bool, Value> > vsav;
	AccessorMap am;
};

class PropEdit : public PopUpC
{
public:
	typedef PropEdit CLASSNAME;
	PropEdit();

	virtual void Deactivate() {} //let other popups open

	void PopUp(Ctrl* owner, Ctrl& e) { pec.Visit(e); PopUpC::PopUp(owner); }

	virtual void Rejector() { pec.Restore(); pec.Clear(); PopUpC::Rejector(); }
	virtual void Acceptor() { pec.Clear(); PopUpC::Acceptor(); }

protected:
	PropEditCtrl pec;
	Button ok, cancel;
};

#endif
