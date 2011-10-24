#ifndef _BoostPyTest_PyEx_h_
#define _BoostPyTest_PyEx_h_

//py stuff needs go first
#include <CtrlLibBoostPy/CtrlLibBoostPy.h>
#include <PyConsoleCtrl/PyConsoleCtrl.h>
#include "PyConsoleCtrl.h"
#include "world.h"

NAMESPACE_UPP

decl_export_Callback1(int);
decl_export_Gate1(int);
decl_export_Callback1(Value);
decl_export_Gate1(Value);

decl_export_Callback2(int, double);
decl_export_Gate2(int, double);

decl_export_Callback3(int, double, bool);
decl_export_Gate3(int, double, bool);

decl_export_Callback4(int, double, bool, int64);
decl_export_Gate4(int, double, bool, int64);

END_UPP_NAMESPACE

#endif
