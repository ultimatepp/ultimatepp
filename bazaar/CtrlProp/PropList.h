#ifndef _CtrlProp_PropList_h_
#define _CtrlProp_PropList_h_

#include "CtrlPropCommon.h"

#include <LogPosCtrl/LogPosCtrl.h>
#include <ValueCtrl/ValueCtrl.h>

#include <Gen/Gen.h>

#define LAYOUTFILE <CtrlProp/PropList.lay>
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
	virtual void Acceptor() { plc.Clear(); PopUpC::Acceptor(); }

protected:
	PropListCtrl plc;
	Button exit;
};

#endif
