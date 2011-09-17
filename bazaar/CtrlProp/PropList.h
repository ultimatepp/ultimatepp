#ifndef _CtrlProp_PropList_h_
#define _CtrlProp_PropList_h_

#include <Property/Property.h>
#include <Gen/VTypes.h>
#include <ValueCtrl/ValueCtrl.h>

NAMESPACE_UPP

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

	virtual Value GetData() const { return RawToValue(ctrl); }
	virtual void SetData(const Value& v) { SetCtrl(RawValue<Ctrl*>::Extract(v)); }

	virtual void Serialize(Stream& s) { GuiLock __; for(Ctrl *q = GetFirstChild(); q; q = q->GetNext()) q->Serialize(s); }

protected:
	Ctrl* ctrl;
};

class PropList : public WithCloserKeys<PopUpC>
{
public:
	typedef PropList CLASSNAME;
	PropList();

	void PopUp(Ctrl* owner, Ctrl& e) { plc.SetCtrl(&e); PopUpC::PopUp(owner); }

	PropListCtrl plc;
};

END_UPP_NAMESPACE

#endif
