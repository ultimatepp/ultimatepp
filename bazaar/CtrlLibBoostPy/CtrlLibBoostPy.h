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

//EditCtrl.cpp
void export_EditCtrl();

//Progress.cpp
void export_Progress();

//SliderCtrl.cpp
void export_SliderCtrl();

///
void export_CtrlLib();

END_UPP_NAMESPACE

#endif
