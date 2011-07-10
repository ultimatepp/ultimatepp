#ifndef _CtrlLibBoostPy_CtrlLibBoostPy_h_
#define _CtrlLibBoostPy_CtrlLibBoostPy_h_

#include "Common.h"

//FIXME remove, since it's bazaar
#include "ValueCtrl.h"

NAMESPACE_UPP

//CtrlCore.cpp
void export_Logc();
void export_LogPos();
void export_CtrlCore();
void export_TopWindow();

//Misc.cpp
void export_Prompts();

//Pusher.cpp
void export_Pusher();

//Button.cpp
void export_Button();

//ButtonOption.cpp
void export_ButtonOption();

//Option.cpp
void export_Option();

//Switch.cpp
void export_Switch();

//EditField.cpp
void export_EditField();

//EditString.cpp
void export_EditString();

//EditInt.cpp
void export_EditInt();

//EditInt64.cpp
void export_EditInt64();

//EditDouble.cpp
void export_EditDouble();

//Progress.cpp
void export_Progress();

//SliderCtrl.cpp
void export_SliderCtrl();

//template.cpp
void export_Foo();

///
void export_CtrlLib();

END_UPP_NAMESPACE

#endif
