#ifndef _Reports3_RepGen_h_ 
#define _Reports3_RepGen_h_

#include <Core/Core.h>
#include <Report/Report.h>

using namespace Upp;

//Module maximized window
class RepGen;

class RepGenSubProc :Moveable<RepGenSubProc> {
public:
	typedef RepGenSubProc CLASSNAME;
	RepGenSubProc(RepGen &rep0, String start0, String end0, bool TrimLastChar = false) : rep(rep0),trimlastchar(TrimLastChar) {start = start0; end = end0; };
	RepGen& rep;
	~RepGenSubProc() {};
	String start,end;
	bool trimlastchar;
	//Callback RepGenSubProcStart;
	Callback RepGenSubProcBody;
	//Callback RepGenSubProcFinish;
	void DoCalc();
};

class RepGen  {
	friend class RepGenSubProc;
private:
	String tpl,tplb,tplbr;
	String result,resultb;
	bool ENDCALCULATE;
	bool IN_BP_LOOP;
	String *pStr_to_subst;
	String *pTpl_cycle;
	String *pRes_sub_proc;
	void PerformLoopTable(String start_str, String end_str, String &templ);
	void PerformLoopPlain(String start_str, String end_str, String &templ, bool trimlastchar=false);

	
	void PCRE_Replace(String& where, String s_from,String s_to);
	void PCRE_ReplaceConditional(String& where, String s_from,String s_to,bool condition,String cond_start,String cond_end);

public:
	String *pTpl_cycle_res;
	
	typedef RepGen CLASSNAME;
	RepGen();
	~RepGen() {};
	void SetTemplate(String tpl0) { tpl=Join(Split(tpl0,'\n'),""); };
	
	Callback RepGenReportStart;
	Callback RepGenReportVar;
	Callback RepGenReportFinish;
	Callback RepGenCalculateStart;
	Callback RepGenCalculateBody;
	Callback RepGenCalculateFinish;
	void RepGenCalculateBodyBeforeSubst();
	void RepGenCalculateBodyAfterSubst();
	
	void SubstVar(String s_from,String s_to);
	void SubstVarConditional(String s_from,String s_to,bool condition, String cond_start="##IF",String cond_end="##FI");
	bool ExistVar(String s);
	void PlaceImage          (String s_from, Image im_to, Size rep_place = Size(0,0));
	
	void LoopDone(bool indicator=true) { ENDCALCULATE = indicator; };

	void Perform();
};

#endif
