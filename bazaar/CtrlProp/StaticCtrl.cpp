#include "CtrlProp.h"
#include <CtrlLib/CtrlLib.h>

//StaticText

bool PropSetInk(StaticText& o, const Value& v) { if(!v.Is<Color>()) return false; o.SetInk(v); return true; }
bool PropGetInk(const StaticText& o, Value& v) { v = o.GetInk(); return true; }
bool PropSetText(StaticText& o, const Value& v) { o.SetText(AsString(v)); return true; }
bool PropGetText(const StaticText& o, Value& v) { v = o.GetText(); return true; }
bool PropSetAlign(StaticText& o, const Value& v) { if(!IsNumber(v)) return false; o.SetAlign(v); return true; }
bool PropGetAlign(const StaticText& o, Value& v) { v = o.GetAlign(); return true; }

CTRL_PROPERTIES(StaticText, Ctrl)
PROPERTY("ink", PropSetInk, PropGetInk)
PROPERTY("text", PropSetText, PropGetText)
PROPERTY("align", PropSetAlign, PropGetAlign)
END_CTRL_PROPERTIES

//Label

bool PropSetText(Label& o, const Value& v) { o.SetText(AsString(v)); return true; }
bool PropGetText(const Label& o, Value& v) { v = o.GetText(); return true; }

CTRL_PROPERTIES(Label, StaticText)
PROPERTY("text", PropSetText, PropGetText)
END_CTRL_PROPERTIES

//LabelBox

CTRL_PROPERTIES(LabelBox, Label)
END_CTRL_PROPERTIES

//ParentCtrl

CTRL_PROPERTIES(ParentCtrl, Ctrl)
END_CTRL_PROPERTIES

//StaticRect

bool PropSetBackground(StaticRect& o, const Value& v) { if(!v.Is<Color>()) return false; o.Background(v); return true; }
bool PropGetBackground(const StaticRect& o, Value& v) { v = o.GetBackground(); return true; }

CTRL_PROPERTIES(StaticRect, Ctrl)
PROPERTY("background", PropSetBackground, PropGetBackground)
END_CTRL_PROPERTIES

//ImageCtrl

CTRL_PROPERTIES(ImageCtrl, Ctrl)
END_CTRL_PROPERTIES

//DisplayCtrl

CTRL_PROPERTIES(DisplayCtrl, Ctrl)
END_CTRL_PROPERTIES

//Picture

CTRL_PROPERTIES(Picture, Ctrl)
END_CTRL_PROPERTIES

//SeparatorCtrl

CTRL_PROPERTIES(SeparatorCtrl, Ctrl)
END_CTRL_PROPERTIES
