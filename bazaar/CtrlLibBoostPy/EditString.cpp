#include "Common.h"
using namespace boost::python;

NAMESPACE_UPP

void EditStringMaxLen(EditString& c, int m) { c.MaxLen(m); }
void EditStringTrimLeft(EditString& c, bool b) { c.TrimLeft(b); }
void EditStringTrimRight(EditString& c, bool b) { c.TrimRight(b); }
void EditStringSetNotNull(EditString& c, bool b) { c.NotNull(b); }

void export_EditString()
{
ONCELOCK
{
	class_<EditString, bases<EditField>, boost::noncopyable>("EditString", "Upp EditString")
		.add_property("maxlen", &EditString::GetMaxLength, &EditStringMaxLen)
		.add_property("trimleft", &EditString::IsTrimLeft, &EditStringTrimLeft)
		.add_property("trimright", &EditString::IsTrimRight, &EditStringTrimRight)
		.add_property("notnull", &EditString::IsNotNull, &EditStringSetNotNull)
	;
	class_<EditStringNotNull, bases<EditString>, boost::noncopyable>("EditStringNotNull", "Upp EditStringNotNull")
	;
}
}

END_UPP_NAMESPACE
