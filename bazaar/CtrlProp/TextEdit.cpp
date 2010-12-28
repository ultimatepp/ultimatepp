#include "CtrlProp.h"
#include <CtrlLib/CtrlLib.h>

//TextCtrl

bool PropSetUndoSteps(TextCtrl& o, const Value& v) { if(!IsNumber(v)) return false; o.UndoSteps(v); return true; }
bool PropGetUndoSteps(const TextCtrl& o, Value& v) { v = o.GetUndoSteps(); return true; }
bool PropSetProcessTab(TextCtrl& o, const Value& v) { if(!IsNumber(v)) return false; o.ProcessTab(v); return true; }
bool PropGetProcessTab(const TextCtrl& o, Value& v) { v = o.IsProcessTab(); return true; }
bool PropSetProcessEnter(TextCtrl& o, const Value& v) { if(!IsNumber(v)) return false; o.ProcessEnter(v); return true; }
bool PropGetProcessEnter(const TextCtrl& o, Value& v) { v = o.IsProcessEnter(); return true; }
bool PropSetNoBackground(TextCtrl& o, const Value& v) { if(!IsNumber(v)) return false; o.NoBackground(v); return true; }
bool PropGetNoBackground(const TextCtrl& o, Value& v) { v = o.IsNoBackground(); return true; }

CTRL_PROPERTIES(TextCtrl, Ctrl)
PROPERTY("undosteps", PropSetUndoSteps, PropGetUndoSteps)
PROPERTY("processtab", PropSetProcessTab, PropGetProcessTab)
PROPERTY("processenter", PropSetProcessEnter, PropGetProcessEnter)
PROPERTY("nobackground", PropSetNoBackground, PropGetNoBackground)
END_CTRL_PROPERTIES

//LineEdit

bool PropSetTabSize(LineEdit& o, const Value& v) { if(!IsNumber(v)) return false; o.TabSize(v); return true; }
bool PropGetTabSize(const LineEdit& o, Value& v) { v = o.GetTabSize(); return true; }
bool PropSetShowTabs(LineEdit& o, const Value& v) { if(!IsNumber(v)) return false; o.ShowTabs(v); return true; }
bool PropGetShowTabs(const LineEdit& o, Value& v) { v = o.IsShowTabs(); return true; }
bool PropSetCutLine(LineEdit& o, const Value& v) { if(!IsNumber(v)) return false; o.WithCutLine(v); return true; }
bool PropGetCutLine(const LineEdit& o, Value& v) { v = o.IsWithCutLine(); return true; }
bool PropSetNoHorzScrollbar(LineEdit& o, const Value& v) { if(!IsNumber(v)) return false; o.NoHorzScrollbar(v); return true; }
bool PropGetNoHorzScrollbar(const LineEdit& o, Value& v) { v = o.IsNoHorzScrollbar(); return true; }

CTRL_PROPERTIES(LineEdit, TextCtrl)
PROPERTY("tabsize", PropSetTabSize, PropGetTabSize)
PROPERTY("showtabs", PropSetShowTabs, PropGetShowTabs)
PROPERTY("cutline", PropSetCutLine, PropGetCutLine)
PROPERTY("nohorzsb", PropSetNoHorzScrollbar, PropGetNoHorzScrollbar)
END_CTRL_PROPERTIES

//DocEdit

bool PropSetAutoHideSb(DocEdit& o, const Value& v) { if(!IsNumber(v)) return false; o.AutoHideSb(v); return true; }
bool PropGetAutoHideSb(const DocEdit& o, Value& v) { v = o.IsAutoHideSb(); return true; }
bool PropSetUpDownLeave(DocEdit& o, const Value& v) { if(!IsNumber(v)) return false; o.UpDownLeave(v); return true; }
bool PropGetUpDownLeave(const DocEdit& o, Value& v) { v = o.IsUpDownLeave(); return true; }
bool PropSetEofLine(DocEdit& o, const Value& v) { if(!IsNumber(v)) return false; o.EofLine(v); return true; }
bool PropGetEofLine(const DocEdit& o, Value& v) { v = o.IsEofLine(); return true; }

CTRL_PROPERTIES(DocEdit, TextCtrl)
PROPERTY("autohidesb", PropSetAutoHideSb, PropGetAutoHideSb)
PROPERTY("updownleave", PropSetUpDownLeave, PropGetUpDownLeave)
PROPERTY("eofline", PropSetEofLine, PropGetEofLine)
END_CTRL_PROPERTIES
