#ifndef _CtrlProp_CtrlPropCommon_h
#define _CtrlProp_CtrlPropCommon_h

#include <Property/Property.h>
#include <Gen/VTypes.h>
#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

DEC_PROPERTIES(Ctrl, EmptyClass)
DEC_PROPERTIES(Pusher, Ctrl)
DEC_PROPERTIES(Button, Pusher)
DEC_PROPERTIES(ButtonOption, Ctrl)
DEC_PROPERTIES(Option, Pusher)
DEC_PROPERTIES(Switch, Ctrl)
DEC_PROPERTIES(EditField, Ctrl)
DEC_PROPERTIES(EditString, EditField)
DEC_PROPERTIES(EditInt, EditField)
DEC_PROPERTIES(EditInt64, EditField)
DEC_PROPERTIES(EditDouble, EditField)
DEC_PROPERTIES(EditStringNotNull, EditString)
DEC_PROPERTIES(EditIntNotNull, EditInt)
DEC_PROPERTIES(EditInt64NotNull, EditInt64)
DEC_PROPERTIES(EditDoubleNotNull, EditDouble)
DEC_PROPERTIES(EditIntSpin, EditInt)
DEC_PROPERTIES(EditInt64Spin, EditInt64)
DEC_PROPERTIES(EditDoubleSpin, EditDouble)
DEC_PROPERTIES(EditIntNotNullSpin, EditIntSpin)
DEC_PROPERTIES(EditInt64NotNullSpin, EditInt64Spin)
DEC_PROPERTIES(EditDoubleNotNullSpin, EditDoubleSpin)
DEC_PROPERTIES(EditDate, EditField)
DEC_PROPERTIES(EditTime, EditField)
DEC_PROPERTIES(EditDateNotNull, EditDate)
DEC_PROPERTIES(EditTimeNotNull, EditTime)
DEC_PROPERTIES(TextCtrl, Ctrl)
DEC_PROPERTIES(LineEdit, TextCtrl)
DEC_PROPERTIES(DocEdit, TextCtrl)
DEC_PROPERTIES(StaticText, Ctrl)
DEC_PROPERTIES(Label, StaticText)
DEC_PROPERTIES(LabelBox, Label)
DEC_PROPERTIES(ParentCtrl, Ctrl)
DEC_PROPERTIES(StaticRect, Ctrl)
DEC_PROPERTIES(ImageCtrl, Ctrl)
DEC_PROPERTIES(DisplayCtrl, Ctrl)
DEC_PROPERTIES(Picture, Ctrl)
DEC_PROPERTIES(SeparatorCtrl, Ctrl)
DEC_PROPERTIES(SliderCtrl, Ctrl)

END_UPP_NAMESPACE

#endif

