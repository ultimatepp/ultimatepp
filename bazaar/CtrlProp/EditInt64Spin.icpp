#include "CtrlPropCommon.h"
#include <CtrlLib/CtrlLib.h>

bool PropSetInc(const Value& v, EditInt64Spin& o) { if(!IsNumber(v)) return false; o.SetInc(v); return true; }
bool PropGetInc(Value& v, const EditInt64Spin& o) { v = o.GetInc(); return true; }
bool PropSetShowSpin(const Value& v, EditInt64Spin& o) { if(!IsNumber(v)) return false; o.ShowSpin(v); return true; }
bool PropGetShowSpin(Value& v, const EditInt64Spin& o) { v = o.IsShowSpin(); return true; }

PROPERTIES(EditInt64Spin, EditInt64)
PROPERTY("inc", PropSetInc, PropGetInc)
PROPERTY("showspin", PropSetShowSpin, PropGetShowSpin)
END_PROPERTIES

PROPS(Ctrl, EditInt64Spin, EditInt64)
