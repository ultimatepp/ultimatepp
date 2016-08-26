#ifndef __TCoreCalc__
#define __TCoreCalc__

#include <TCore/TCore.h>
#include "CalcType.h"
#include "CalcNode.h"

namespace Upp {

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

}

#endif//__TCoreCalc__
