#ifndef _CtrlProp_CtrlPropEdit_h_
#define _CtrlProp_CtrlPropEdit_h_

#include "CtrlPropCommon.h"

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <LogPosCtrl/LogPosCtrl.h>
#include <ValueCtrl/ValueCtrl.h>

#include <Gen/Gen.h>

#define LAYOUTFILE <CtrlProp/CtrlPropEdit.lay>
#include <CtrlCore/lay.h>

class PropListCtrl : public WithPropListLay<ParentCtrl>, public Visiting<Ctrl>
{
public:
	typedef PropListCtrl CLASSNAME;
	typedef Visiting<Ctrl> V;
	PropListCtrl();

	virtual void Reload();
};

class PropList : public PopUpC
{
public:
	typedef PropList CLASSNAME;
	PropList();

	void PopUp(Ctrl* owner, Ctrl& e) { plc.Visit(e); PopUpC::PopUp(owner); }
	virtual void Acceptor();

protected:
	PropListCtrl plc;
	Button exit;
};

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

	virtual void Rejector();
	virtual void Acceptor();

protected:
	PropEditCtrl pec;
	Button ok, cancel;
};

#endif
