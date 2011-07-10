#include "Common.h"
using namespace boost::python;

NAMESPACE_UPP

void EditIntSetMin(EditInt& c, int m) { c.MinMax(m, c.GetMax()); }
void EditIntSetMax(EditInt& c, int m) { c.MinMax(c.GetMin(), m); }
void EditIntSetNotNull(EditInt& c, bool b) { c.NotNull(b); }

void EditIntSpinSetInc(EditIntSpin& c, int m) { c.SetInc(m); }
void EditIntSpinShowSpin(EditIntSpin& c, bool b) { c.ShowSpin(b); }

void export_EditInt()
{
ONCELOCK
{
	class_<EditInt, bases<EditField>, boost::noncopyable>("EditInt", "Upp EditInt")
		.add_property("max", &EditInt::GetMax, &EditIntSetMax)
		.add_property("min", &EditInt::GetMin, &EditIntSetMin)
		.add_property("notnull", &EditInt::IsNotNull, &EditIntSetNotNull)
	;
	class_<EditIntNotNull, bases<EditInt>, boost::noncopyable>("EditIntNotNull", "Upp EditIntNotNull")
	;
	class_<EditIntSpin, bases<EditInt>, boost::noncopyable>("EditIntSpin", "Upp EditIntSpin")
		.add_property("inc", &EditIntSpin::GetInc, &EditIntSpinSetInc)
		.add_property("showspin", &EditIntSpin::IsSpinVisible, &EditIntSpinShowSpin)
	;
	class_<EditIntNotNullSpin, bases<EditIntSpin>, boost::noncopyable>("EditIntNotNullSpin", "Upp EditIntNotNullSpin")
	;
}
}

END_UPP_NAMESPACE
