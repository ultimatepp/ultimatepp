#include "Common.h"
using namespace boost::python;

NAMESPACE_UPP

void EditDoubleSetMin(EditDouble& c, int m) { c.MinMax(m, c.GetMax()); }
void EditDoubleSetMax(EditDouble& c, int m) { c.MinMax(c.GetMin(), m); }
void EditDoubleSetNotNull(EditDouble& c, bool b) { c.NotNull(b); }

void EditDoubleSpinSetInc(EditDoubleSpin& c, int m) { c.SetInc(m); }
void EditDoubleSpinShowSpin(EditDoubleSpin& c, bool b) { c.ShowSpin(b); }

void export_EditDouble()
{
ONCELOCK
{
	class_<EditDouble, bases<EditField>, boost::noncopyable>("EditDouble", "Upp EditDouble")
		.add_property("max", &EditDouble::GetMax, &EditDoubleSetMax)
		.add_property("min", &EditDouble::GetMin, &EditDoubleSetMin)
		.add_property("notnull", &EditDouble::IsNotNull, &EditDoubleSetNotNull)
	;
	class_<EditDoubleNotNull, bases<EditDouble>, boost::noncopyable>("EditDoubleNotNull", "Upp EditDoubleNotNull")
	;
	class_<EditDoubleSpin, bases<EditDouble>, boost::noncopyable>("EditDoubleSpin", "Upp EditDoubleSpin")
		.add_property("inc", &EditDoubleSpin::GetInc, &EditDoubleSpinSetInc)
		.add_property("showspin", &EditDoubleSpin::IsSpinVisible, &EditDoubleSpinShowSpin)
	;
	class_<EditDoubleNotNullSpin, bases<EditDoubleSpin>, boost::noncopyable>("EditDoubleNotNullSpin", "Upp EditDoubleNotNullSpin")
	;
}
}

END_UPP_NAMESPACE
