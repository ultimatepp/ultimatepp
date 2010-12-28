#include "CtrlProp.h"
#include <CtrlLib/CtrlLib.h>

bool PropSetLabel(Pusher& o, const Value& v) { if(!IsString(v)) return false; o.SetLabel(String(v)); return true; }
bool PropGetLabel(const Pusher& o, Value& v) { v = o.GetLabel(); return true; }
bool PropSetClickFocus(Pusher& o, const Value& v) { if(!IsNumber(v)) return false; o.ClickFocus(v); return true; }
bool PropGetClickFocus(const Pusher& o, Value& v) { v = o.IsClickFocus(); return true; }
bool PropGetVisualState(const Pusher& o, Value& v) { v = o.GetVisualState(); return true; }

CTRL_PROPERTIES(Pusher, Ctrl)
PROPERTY("label", PropSetLabel, PropGetLabel)
PROPERTY("clickfocus", PropSetClickFocus, PropGetClickFocus)
//PROPERTY_GET("visualstate", PropGetVisualState)
END_CTRL_PROPERTIES

CTRL_PROPERTIES(Button, Pusher)
END_CTRL_PROPERTIES

bool PropSetLabel(ButtonOption& o, const Value& v) { if(!IsString(v)) return false; o.SetLabel(v); return true; }
bool PropGetLabel(const ButtonOption& o, Value& v) { v = o.GetLabel(); return true; }

CTRL_PROPERTIES(ButtonOption, Ctrl)
PROPERTY("label", PropSetLabel, PropGetLabel)
END_CTRL_PROPERTIES

bool PropSetBlackEdge(Option& o, const Value& v) { if(!IsNumber(v)) return false; o.BlackEdge(v); return true; }
bool PropGetBlackEdge(const Option& o, Value& v) { v = o.IsBlackEdge(); return true; }
bool PropSetSwitchImage(Option& o, const Value& v) { if(!IsNumber(v)) return false; o.SwitchImage(v); return true; }
bool PropGetSwitchImage(const Option& o, Value& v) { v = o.IsSwitchImage(); return true; }
bool PropSetThreeState(Option& o, const Value& v) { if(!IsNumber(v)) return false; o.ThreeState(v); return true; }
bool PropGetThreeState(const Option& o, Value& v) { v = o.IsThreeState(); return true; }
bool PropSetShowLabel(Option& o, const Value& v) { if(!IsNumber(v)) return false; o.ShowLabel(v); return true; }
bool PropGetShowLabel(const Option& o, Value& v) { v = o.IsShowLabel(); return true; }
bool PropSetNotNull(Option& o, const Value& v) { if(!IsNumber(v)) return false; o.NotNull(v); return true; }
bool PropGetNotNull(const Option& o, Value& v) { v = o.IsNotNull(); return true; }

CTRL_PROPERTIES(Option, Pusher)
PROPERTY("blackedge", PropSetBlackEdge, PropGetBlackEdge)
PROPERTY("switchimage", PropSetSwitchImage, PropGetSwitchImage)
PROPERTY("threestate", PropSetThreeState, PropGetThreeState)
PROPERTY("showlabel", PropSetShowLabel, PropGetShowLabel)
PROPERTY("notnull", PropSetNotNull, PropGetNotNull)
END_CTRL_PROPERTIES

bool PropSetLabel(Switch& o, const Value& v) { o.SetLabel(AsString(v)); return true; }
bool PropGetLabel(const Switch& o, Value& v) { v = o.GetLabel(); return true; }

CTRL_PROPERTIES(Switch, Ctrl)
PROPERTY("label", PropSetLabel, PropGetLabel)
END_CTRL_PROPERTIES


