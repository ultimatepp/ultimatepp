#ifndef __TCoreCalc__
#define __TCoreCalc__

#include <TCore/TCore.h>
#include "CalcType.h"
#include "CalcNode.h"

NAMESPACE_UPP

extern void UseCalcBasic();

String GroupComp();
String GroupBitOp();
String GroupArith();
String GroupLog();
String GroupString();
String GroupDate();
String GroupArray();
String GroupConst();
String GroupTrans();

END_UPP_NAMESPACE

#endif//__TCoreCalc__
