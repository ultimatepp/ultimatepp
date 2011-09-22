#ifndef _ValueAccessorEdit_ValueAccessorEdit_h
#define _ValueAccessorEdit_ValueAccessorEdit_h

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <Property/Property.h>
#include <Gen/Gen.h>
#include <ValueCtrl/ValueCtrl.h>

NAMESPACE_UPP

class ValueAccessorEdit : public ParentCtrl {
public:
	typedef ValueAccessorEdit CLASSNAME;
	typedef ValueAccessor VA;
	
	ValueAccessorEdit() : tg(NULL) {}

	//need get set data because of serialize
	virtual void SetData(const Value& v) { if(!tg) return; tg->set(v); }
	virtual Value GetData() const { if(!tg) return Null; Value v; tg->get(v); return v; }

	virtual void SetTarget(ValueAccessor* va) { if(tg != va) { tg = va; UpdateTarget(); } Update(); }
	virtual ValueAccessor* GetTarget() const { return tg; }
	void ClearTarget() { SetTarget(NULL); }

	virtual void Serialize(Stream& s);

protected:
	virtual void UpdateTarget() =0; //should do the init on target change

	ValueAccessor* tg;
};

//this is the default ValueAccessor Editor, it loads the respective default editors for the value type of get/set
class ValueAccessorCtrl : public ValueAccessorEdit
{
public:
	typedef ValueAccessorCtrl CLASSNAME;

	virtual void Updated(); //will load (get) data from ValueAccessor target
	void OnAction() { if(!tg) return; Value v = oc->GetData(); tg->set(v); Action(); }

	One<Ctrl> oc;

protected:
	void UpdateTarget();
};

//for usage with Instancer<ValueAccessor>
typedef Instance<ValueAccessor> ValueAccessorInstance;

//depending on the ValueAccessor type, there might be a different editor or presenter
//these types should be ValueAccessorInstance derived
//all editors are found in Instancer<ValueAccessorEdit>, organized by ValueAccessorInstance::TypeOf()

#define LAYOUTFILE <ValueAccessorEdit/ValueAccessorEdit.lay>
#include <CtrlCore/lay.h>

class AccessorMapCtrl : public WithValueAccessorEditLayout<ParentCtrl>
{
public:
	typedef AccessorMapCtrl CLASSNAME;
	AccessorMapCtrl();

	virtual void Updated();

	void SetMap(AccessorMap* a) { if(am != a) { am = a; UpdateMap(); } Update(); }
	AccessorMap* GetMap() const { return am; }
	void ClearMap() { SetMap(NULL); }

	virtual Value GetData() const { return RawToValue(am); }
	virtual void SetData(const Value& v) { SetMap(RawValue<AccessorMap*>::Extract(v)); }
	virtual void Serialize(Stream& s);

protected:
	void ReloadFactory(int i, One<Ctrl>& o);
	void ReloadAction(int i);
	void UpdateMap();

	AccessorMap* am;
	ArrayMap<String, ValueAccessorEdit> avae;
};

END_UPP_NAMESPACE

#endif

