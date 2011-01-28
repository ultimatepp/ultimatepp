#include "CtrlProp.h"
#include <CtrlLib/CtrlLib.h>

bool PropSetData(Ctrl& o, const Value& v) { o.SetData(v); return true; }
bool PropGetData(const Ctrl& o, Value& v) { v = o.GetData(); return true; }
bool PropEnable(Ctrl& o, const Value& v) { if(!IsNumber(v)) return false; o.Enable(v); return true; }
bool PropIsEnabled(const Ctrl& o, Value& v) { v = o.IsEnabled(); return true; }
bool PropShow(Ctrl& o, const Value& v) { if(!IsNumber(v)) return false; o.Show(v); return true; }
bool PropIsVisible(const Ctrl& o, Value& v) { v = o.IsVisible(); return true; }
bool PropEditable(Ctrl& o, const Value& v) { if(!IsNumber(v)) return false; o.SetEditable(v); return true; }
bool PropIsEditable(const Ctrl& o, Value& v) { v = o.IsEditable(); return true; }
bool PropFocus(Ctrl& o, const Value& v) { if(!IsNumber(v)) return false; bool b = v; if(b) o.SetFocus(); else if(o.GetParent()) o.GetParent()->SetFocus(); return true; }
bool PropHasFocus(const Ctrl& o, Value& v) { v = o.HasFocus(); return true; }
bool PropModify(Ctrl& o, const Value& v) { if(!IsNumber(v)) return false; bool b = v; if(b) o.SetModify(); else o.ResetModify(); return true; }
bool PropIsModified(const Ctrl& o, Value& v) { v = o.IsModified(); return true; }
bool PropSetTip(Ctrl& o, const Value& v) { o.Tip(AsString(v)); return true; }
bool PropGetTip(const Ctrl& o, Value& v) { v = o.GetTip(); return true; }
bool PropWantFocus(Ctrl& o, const Value& v) { if(!IsNumber(v)) return false; o.WantFocus(v); return true; }
bool PropIsWantFocus(const Ctrl& o, Value& v) { v = o.IsWantFocus(); return true; }
bool PropInitFocus(Ctrl& o, const Value& v) { if(!IsNumber(v)) return false; o.InitFocus(v); return true; }
bool PropIsInitFocus(const Ctrl& o, Value& v) { v = o.IsInitFocus(); return true; }
bool PropBackPaint(Ctrl& o, const Value& v) { if(!IsNumber(v)) return false; o.BackPaint(((bool(v))?(Ctrl::FULLBACKPAINT):(Ctrl::NOBACKPAINT))); return true; }
bool PropIsBackPaint(const Ctrl& o, Value& v) { v = bool(o.GetBackPaint()!=Ctrl::NOBACKPAINT); return true; }
bool PropTransparent(Ctrl& o, const Value& v) { if(!IsNumber(v)) return false; o.Transparent(v); return true; }
bool PropIsTransparent(const Ctrl& o, Value& v) { v = o.IsTransparent(); return true; }

bool PropRefresh(Ctrl& o, const Value& v) { o.Refresh(); return true; }

bool PropSetLogPos(Ctrl& o, const Value& v) { if(!v.Is<Ctrl::LogPos>()) return false; o.SetPos(RawValue<Ctrl::LogPos>::Extract(v)); return true; }
bool PropGetLogPos(const Ctrl& o, Value& v) { v = RawToValue(o.GetPos()); return true; }

bool PropGetTypeInfo(const Ctrl& o, Value& v) { v = String(typeid(o).name()); return true; }

CTRL_PROPERTIES(Ctrl, RecurseDone)
PROPERTY("data", PropSetData, PropGetData)
PROPERTY("enable", PropEnable, PropIsEnabled)
PROPERTY("show", PropShow, PropIsVisible)
PROPERTY("editable", PropEditable, PropIsEditable)
PROPERTY("logpos", PropSetLogPos, PropGetLogPos)
PROPERTY("focus", PropFocus, PropHasFocus)
PROPERTY("modify", PropModify, PropIsModified)
PROPERTY("tip", PropSetTip, PropGetTip)
PROPERTY("wantfocus", PropWantFocus, PropIsWantFocus)
PROPERTY("initFocus", PropInitFocus, PropIsInitFocus)
PROPERTY("backpaint", PropBackPaint, PropIsBackPaint)
PROPERTY("transparent", PropTransparent, PropIsTransparent)
//PROPERTY_SET("refresh", PropRefresh)
PROPERTY_GET("type", PropGetTypeInfo)
END_CTRL_PROPERTIES
