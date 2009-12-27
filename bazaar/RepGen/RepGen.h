#ifndef _Reports3_RepGen_h_
#define _Reports3_RepGen_h_

#include <Report/Report.h>
#include <Core/Core.h>

using namespace Upp;

//Module maximized window


class RepGen  {
private:
	String tpl,tplb,tplbr;
	String result,resultb;
	bool ENDCALCULATE;
	bool IN_BP_LOOP;
	
	void PCRE_Replace(String& where, String s_from,String s_to);
public:
	typedef RepGen CLASSNAME;
	RepGen() {};
	void Perform();
	
	Callback RepGenReportStart;
	Callback RepGenReportVar;
	Callback RepGenReportFinish;
	Callback RepGenCalculateStart;
	Callback RepGenCalculateBody;
	Callback RepGenCalculateFinish;

	void SubstVar(String s_from,String s_to) {
		if (!IN_BP_LOOP)
			PCRE_Replace(tpl, s_from, s_to);
		else
			PCRE_Replace(tplbr, s_from, s_to);
	};
	
	void SetTemplate(String tpl0) { tpl=Join(Split(tpl0,'\n'),""); };
	void LoopDone(bool indicator=true) { ENDCALCULATE = indicator; };
	void PlaceImage          (String s_from, Image im_to, Size rep_place = Size(0,0));
	
};

#endif
