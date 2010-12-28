#include "CtrlProp.h"
#include <CtrlLib/CtrlLib.h>

//EditField

bool PropSetPassword(EditField& o, const Value& v) { if(!IsNumber(v)) return false; o.Password(v); return true; }
bool PropGetPassword(const EditField& o, Value& v) { v = o.IsPassword(); return true; }
bool PropSetAutoFormat(EditField& o, const Value& v) { if(!IsNumber(v)) return false; o.AutoFormat(v); return true; }
bool PropGetAutoFormat(const EditField& o, Value& v) { v = o.IsAutoFormat(); return true; }
bool PropSetClickSelect(EditField& o, const Value& v) { if(!IsNumber(v)) return false; o.ClickSelect(v); return true; }
bool PropGetClickSelect(const EditField& o, Value& v) { v = o.IsClickSelect(); return true; }
bool PropSetInitCaps(EditField& o, const Value& v) { if(!IsNumber(v)) return false; o.InitCaps(v); return true; }
bool PropGetInitCaps(const EditField& o, Value& v) { v = o.IsInitCaps(); return true; }
bool PropSetMaxChars(EditField& o, const Value& v) { if(!IsNumber(v)) return false; o.MaxChars(v); return true; }
bool PropGetMaxChars(const EditField& o, Value& v) { v = o.GetMaxChars(); return true; }
bool PropSetNoBackground(EditField& o, const Value& v) { if(!IsNumber(v)) return false; o.NoBackground(v); return true; }
bool PropGetNoBackground(const EditField& o, Value& v) { v = o.IsNoBackground(); return true; }
bool PropSetAlignRight(EditField& o, const Value& v) { if(!IsNumber(v)) return false; o.AlignRight(v); return true; }
bool PropGetAlignRight(const EditField& o, Value& v) { v = o.IsAlignRight(); return true; }

CTRL_PROPERTIES(EditField, Ctrl)
PROPERTY("password", PropSetPassword, PropGetPassword)
PROPERTY("autoformat", PropSetAutoFormat, PropGetAutoFormat)
PROPERTY("clickselect", PropSetClickSelect, PropGetClickSelect)
PROPERTY("initcaps", PropSetInitCaps, PropGetInitCaps)
PROPERTY("maxchars", PropSetMaxChars, PropGetMaxChars)
PROPERTY("nobackground", PropSetNoBackground, PropGetNoBackground)
PROPERTY("alignright", PropSetAlignRight, PropGetAlignRight)
END_CTRL_PROPERTIES

//EditString

bool PropSetMaxLen(EditString& o, const Value& v) { if(!IsNumber(v)) return false; o.MaxLen(v); return true; }
bool PropGetMaxLen(const EditString& o, Value& v) { v = o.GetMaxLength(); return true; }
bool PropSetTrimLeft(EditString& o, const Value& v) { if(!IsNumber(v)) return false; o.TrimLeft(v); return true; }
bool PropGetTrimLeft(const EditString& o, Value& v) { v = o.IsTrimLeft(); return true; }
bool PropSetTrimRight(EditString& o, const Value& v) { if(!IsNumber(v)) return false; o.TrimRight(v); return true; }
bool PropGetTrimRight(const EditString& o, Value& v) { v = o.IsTrimRight(); return true; }
bool PropSetNotNull(EditString& o, const Value& v) { if(!IsNumber(v)) return false; o.NotNull(v); return true; }
bool PropGetNotNull(const EditString& o, Value& v) { v = o.IsNotNull(); return true; }

CTRL_PROPERTIES(EditString, EditField)
PROPERTY("maxlen", PropSetMaxLen, PropGetMaxLen)
PROPERTY("trimleft", PropSetTrimLeft, PropGetTrimLeft)
PROPERTY("trimrigth", PropSetTrimRight, PropGetTrimRight)
PROPERTY("notnull", PropSetNotNull, PropGetNotNull)
END_CTRL_PROPERTIES

//EditInt

bool PropSetMin(EditInt& o, const Value& v) { if(!IsNumber(v)) return false; o.MinMax(v, o.GetMax()); return true; }
bool PropGetMin(const EditInt& o, Value& v) { v = o.GetMin(); return true; }
bool PropSetMax(EditInt& o, const Value& v) { if(!IsNumber(v)) return false; o.MinMax(o.GetMin(), v); return true; }
bool PropGetMax(const EditInt& o, Value& v) { v = o.GetMax(); return true; }
bool PropSetNotNull(EditInt& o, const Value& v) { if(!IsNumber(v)) return false; o.NotNull(v); return true; }
bool PropGetNotNull(const EditInt& o, Value& v) { v = o.IsNotNull(); return true; }

CTRL_PROPERTIES(EditInt, EditField)
PROPERTY("min", PropSetMin, PropGetMin)
PROPERTY("max", PropSetMax, PropGetMax)
PROPERTY("notnull", PropSetNotNull, PropGetNotNull)
END_CTRL_PROPERTIES

//EditInt64

bool PropSetMin(EditInt64& o, const Value& v) { if(!IsNumber(v)) return false; o.MinMax(v, o.GetMax()); return true; }
bool PropGetMin(const EditInt64& o, Value& v) { v = o.GetMin(); return true; }
bool PropSetMax(EditInt64& o, const Value& v) { if(!IsNumber(v)) return false; o.MinMax(o.GetMin(), v); return true; }
bool PropGetMax(const EditInt64& o, Value& v) { v = o.GetMax(); return true; }
bool PropSetNotNull(EditInt64& o, const Value& v) { if(!IsNumber(v)) return false; o.NotNull(v); return true; }
bool PropGetNotNull(const EditInt64& o, Value& v) { v = o.IsNotNull(); return true; }

CTRL_PROPERTIES(EditInt64, EditField)
PROPERTY("min", PropSetMin, PropGetMin)
PROPERTY("max", PropSetMax, PropGetMax)
PROPERTY("notnull", PropSetNotNull, PropGetNotNull)
END_CTRL_PROPERTIES

//EditDouble

bool PropSetMin(EditDouble& o, const Value& v) { if(!IsNumber(v)) return false; o.MinMax(v, o.GetMax()); return true; }
bool PropGetMin(const EditDouble& o, Value& v) { v = o.GetMin(); return true; }
bool PropSetMax(EditDouble& o, const Value& v) { if(!IsNumber(v)) return false; o.MinMax(o.GetMin(), v); return true; }
bool PropGetMax(const EditDouble& o, Value& v) { v = o.GetMax(); return true; }
bool PropSetNotNull(EditDouble& o, const Value& v) { if(!IsNumber(v)) return false; o.NotNull(v); return true; }
bool PropGetNotNull(const EditDouble& o, Value& v) { v = o.IsNotNull(); return true; }

CTRL_PROPERTIES(EditDouble, EditField)
PROPERTY("min", PropSetMin, PropGetMin)
PROPERTY("max", PropSetMax, PropGetMax)
PROPERTY("notnull", PropSetNotNull, PropGetNotNull)
END_CTRL_PROPERTIES

//NotNull variants

CTRL_PROPERTIES(EditIntNotNull, EditInt)
END_CTRL_PROPERTIES

CTRL_PROPERTIES(EditInt64NotNull, EditInt64)
END_CTRL_PROPERTIES

CTRL_PROPERTIES(EditDoubleNotNull, EditDouble)
END_CTRL_PROPERTIES

//EditIntSpin

bool PropSetInc(EditIntSpin& o, const Value& v) { if(!IsNumber(v)) return false; o.SetInc(v); return true; }
bool PropGetInc(const EditIntSpin& o, Value& v) { v = o.GetInc(); return true; }
bool PropSetShowSpin(EditIntSpin& o, const Value& v) { if(!IsNumber(v)) return false; o.ShowSpin(v); return true; }
bool PropGetShowSpin(const EditIntSpin& o, Value& v) { v = o.IsShowSpin(); return true; }

CTRL_PROPERTIES(EditIntSpin, EditInt)
PROPERTY("inc", PropSetInc, PropGetInc)
PROPERTY("showspin", PropSetShowSpin, PropGetShowSpin)
END_CTRL_PROPERTIES

//EditInt64Spin

bool PropSetInc(EditInt64Spin& o, const Value& v) { if(!IsNumber(v)) return false; o.SetInc(v); return true; }
bool PropGetInc(const EditInt64Spin& o, Value& v) { v = o.GetInc(); return true; }
bool PropSetShowSpin(EditInt64Spin& o, const Value& v) { if(!IsNumber(v)) return false; o.ShowSpin(v); return true; }
bool PropGetShowSpin(const EditInt64Spin& o, Value& v) { v = o.IsShowSpin(); return true; }

CTRL_PROPERTIES(EditInt64Spin, EditInt64)
PROPERTY("inc", PropSetInc, PropGetInc)
PROPERTY("showspin", PropSetShowSpin, PropGetShowSpin)
END_CTRL_PROPERTIES

//EditDoubleSpin

bool PropSetInc(EditDoubleSpin& o, const Value& v) { if(!IsNumber(v)) return false; o.SetInc(v); return true; }
bool PropGetInc(const EditDoubleSpin& o, Value& v) { v = o.GetInc(); return true; }
bool PropSetShowSpin(EditDoubleSpin& o, const Value& v) { if(!IsNumber(v)) return false; o.ShowSpin(v); return true; }
bool PropGetShowSpin(const EditDoubleSpin& o, Value& v) { v = o.IsShowSpin(); return true; }

CTRL_PROPERTIES(EditDoubleSpin, EditDouble)
PROPERTY("inc", PropSetInc, PropGetInc)
PROPERTY("showspin", PropSetShowSpin, PropGetShowSpin)
END_CTRL_PROPERTIES

//NotNull variants

CTRL_PROPERTIES(EditIntNotNullSpin, EditIntSpin)
END_CTRL_PROPERTIES

CTRL_PROPERTIES(EditInt64NotNullSpin, EditInt64Spin)
END_CTRL_PROPERTIES

CTRL_PROPERTIES(EditDoubleNotNullSpin, EditDoubleSpin)
END_CTRL_PROPERTIES

//EditDate

bool PropSetMin(EditDate& o, const Value& v) { if(!IsNumber(v)) return false; o.MinMax(v, o.GetMax()); return true; }
bool PropGetMin(const EditDate& o, Value& v) { v = o.GetMin(); return true; }
bool PropSetMax(EditDate& o, const Value& v) { if(!IsNumber(v)) return false; o.MinMax(o.GetMin(), v); return true; }
bool PropGetMax(const EditDate& o, Value& v) { v = o.GetMax(); return true; }
bool PropSetNotNull(EditDate& o, const Value& v) { if(!IsNumber(v)) return false; o.NotNull(v); return true; }
bool PropGetNotNull(const EditDate& o, Value& v) { v = o.IsNotNull(); return true; }

CTRL_PROPERTIES(EditDate, EditField)
PROPERTY("min", PropSetMin, PropGetMin)
PROPERTY("max", PropSetMax, PropGetMax)
PROPERTY("notnull", PropSetNotNull, PropGetNotNull)
END_CTRL_PROPERTIES

//EditTime

bool PropSetMin(EditTime& o, const Value& v) { if(!IsNumber(v)) return false; o.MinMax(v, o.GetMax()); return true; }
bool PropGetMin(const EditTime& o, Value& v) { v = o.GetMin(); return true; }
bool PropSetMax(EditTime& o, const Value& v) { if(!IsNumber(v)) return false; o.MinMax(o.GetMin(), v); return true; }
bool PropGetMax(const EditTime& o, Value& v) { v = o.GetMax(); return true; }
bool PropSetNotNull(EditTime& o, const Value& v) { if(!IsNumber(v)) return false; o.NotNull(v); return true; }
bool PropGetNotNull(const EditTime& o, Value& v) { v = o.IsNotNull(); return true; }
bool PropSetSeconds(EditTime& o, const Value& v) { if(!IsNumber(v)) return false; o.Seconds(v); return true; }
bool PropGetSeconds(const EditTime& o, Value& v) { v = o.IsSeconds(); return true; }

CTRL_PROPERTIES(EditTime, EditField)
PROPERTY("min", PropSetMin, PropGetMin)
PROPERTY("max", PropSetMax, PropGetMax)
PROPERTY("notnull", PropSetNotNull, PropGetNotNull)
PROPERTY("seconds", PropSetSeconds, PropGetSeconds)
END_CTRL_PROPERTIES

//NotNull variants

CTRL_PROPERTIES(EditDateNotNull, EditDate)
END_CTRL_PROPERTIES

CTRL_PROPERTIES(EditTimeNotNull, EditTime)
END_CTRL_PROPERTIES
