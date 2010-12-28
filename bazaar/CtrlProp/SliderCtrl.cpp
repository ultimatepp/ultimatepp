#include "CtrlProp.h"
#include <CtrlLib/CtrlLib.h>

//SliderCtrl

bool PropSetMin(SliderCtrl& o, const Value& v) { if(!IsNumber(v)) return false; o.MinMax(v, o.GetMax()); return true; }
bool PropGetMin(const SliderCtrl& o, Value& v) { v = o.GetMin(); return true; }
bool PropSetMax(SliderCtrl& o, const Value& v) { if(!IsNumber(v)) return false; o.MinMax(o.GetMin(), v); return true; }
bool PropGetMax(const SliderCtrl& o, Value& v) { v = o.GetMax(); return true; }
bool PropSetStep(SliderCtrl& o, const Value& v) { if(!IsNumber(v)) return false; o.Step(v); return true; }
bool PropGetStep(const SliderCtrl& o, Value& v) { v = o.GetStep(); return true; }

CTRL_PROPERTIES(SliderCtrl, Ctrl)
PROPERTY("min", PropSetMin, PropGetMin)
PROPERTY("max", PropSetMax, PropGetMax)
PROPERTY("step", PropSetStep, PropGetStep)
END_CTRL_PROPERTIES

