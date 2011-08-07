#ifndef _CtrlProp_PropList_h_
#define _CtrlProp_PropList_h_

#include "CtrlPropCommon.h"

#include <LogPosCtrl/LogPosCtrl.h>
#include <ValueCtrl/ValueCtrl.h>

#define LAYOUTFILE <CtrlProp/PropList.lay>
#include <CtrlCore/lay.h>

class PropListCtrl : public WithPropListLay<ParentCtrl>
{
public:
	typedef PropListCtrl CLASSNAME;
	PropListCtrl();

	virtual void Updated();

	void SetCtrl(Ctrl* c) { ctrl = c; UpdateRefresh(); }
	Ctrl* GetCtrl() const { return ctrl; }
	void ClearCtrl() { SetCtrl(NULL); }

	virtual Value GetData() const { return RawToValue(~ctrl); }
	virtual void SetData(const Value& v) { SetCtrl(RawValue<Ctrl*>::Extract(v)); }

protected:
	Ptr<Ctrl> ctrl; //the current found child
};

class PropList : public PopUpC
{
public:
	typedef PropList CLASSNAME;
	PropList();

	void PopUp(Ctrl* owner, Ctrl& e) { plc.SetCtrl(&e); PopUpC::PopUp(owner); }
	virtual void Acceptor() { plc.ClearCtrl(); PopUpC::Acceptor(); }

protected:
	PropListCtrl plc;
	Button exit;
};

#endif
