#ifndef __TCtrlLibCalc__
#define __TCtrlLibCalc__

#include <TCore/TCoreCalc.h>
#include <TCtrlLib/Help/TCtrlLibHelp.h>
#include "CalcGuiType.h"

namespace Upp {

void RunDlgCalc(CalcContext& context);

void UseHelpCalc();
void HelpCalcPutIndex();
//void HelpCalcPutGroups();

HelpCalcMap GetHelpCalcGroup(const String& groupname);

void AddHelpCalcGroups(DropList& dest, const String& prefix = "@");
void AddHelpCalcGroup(DropList& dest, const String& group);

}

#endif//__TCtrlLibCalc__
