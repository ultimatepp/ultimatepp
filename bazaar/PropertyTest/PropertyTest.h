#ifndef _PropertyTest_PropertyTest_h
#define _PropertyTest_PropertyTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <Property/Property.h>

#define LAYOUTFILE <PropertyTest/PropertyTest.lay>
#include <CtrlCore/lay.h>

class PropertyTest : public WithPropertyTestLayout<TopWindow> {
public:
	typedef PropertyTest CLASSNAME;
	PropertyTest();
	
	void GetD (Value& a) const { a = "abc"; }
	void SetD(const Value& a) { RLOG(a); }

	bool GetDa (Value& a) const { a = "abc"; return true; }
	bool SetDa(const Value& a) { if(!IsNumber(a)) return false; RLOG(a); return true; }
	
	ValueProperty vp;
	ValueAccessor va;
};

#endif

