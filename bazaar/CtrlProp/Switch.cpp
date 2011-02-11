#include "CtrlPropCommon.h"
#include <CtrlLib/CtrlLib.h>

bool PropSetLabel(const Value& v, Switch& o) { o.SetLabel(AsString(v)); return true; }
bool PropGetLabel(Value& v, const Switch& o) { v = o.GetLabel(); return true; }

PROPERTIES(Switch, Ctrl)
PROPERTY("label", PropSetLabel, PropGetLabel)
END_PROPERTIES

PROPS(Ctrl, Switch, Ctrl)

