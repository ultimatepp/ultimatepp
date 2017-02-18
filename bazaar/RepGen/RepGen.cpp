#include <plugin/pcre/Pcre.h>
#include <plugin/pcre/lib/pcre.h>
#include <RichText/RichText.h>
#include <PdfDraw/PdfDraw.h>
#include <RepGen/RepGen.h>

#define TFILE <RepGen/RepGen.t>
#include <Core/t.h>


#define dSavePtr(x)		p##x##_save = p##x; p##x = &l##x;
#define dRestPtr(x)		p##x = p##x##_save;

void SavePtr(String *p_main, String *p_new, String *p_save) {
	p_save = p_main;
	p_main = p_new;
}

void RestPtr(String *p_main, String *p_save) {
	p_main = p_save;
}

static Array<QtfRichObject> qtfim;

class RepGenReportWindow : public ReportWindow {
public:
	typedef RepGenReportWindow CLASSNAME;

	RepGenReportWindow() {
		ReportWindow();
		Maximize(true);
	};
};

RepGen::RepGen(){
	pStr_to_subst = &tpl;
	pTpl_cycle = &tplb;
	pTpl_cycle_res = &tplbr;
	pRes_sub_proc = &resultb;

	
}


void RepGen::PCRE_Replace(String& where, String s_from,String s_to) {
	RegExp r0("(.*)"+s_from+"([[:^alnum:]].*)",RegExp::MULTILINE);
	while (r0.GlobalMatch(where)) {
		if(r0.GetCount()==2) {
			where = r0[0] + Join(Split(s_to,'\n'),"&") + r0[1];
		}
	}
};

void RepGen::PCRE_ReplaceConditional(String& where, String s_from,String s_to,bool condition,String cond_start,String cond_end) {
	RegExp r0("(.*)"+cond_start+"([^#]*)"+s_from+"([^#]*)"+cond_end+"(.*)",RegExp::MULTILINE);
	while (r0.GlobalMatch(where)) {
		if(r0.GetCount()==4) {
			if (condition)
				where = r0[0] + r0[1] + Join(Split(s_to,'\n'),"&") + r0[2] + r0[3];
			else
				where = r0[0] + r0[3];
		}
	}
};

void RepGen::SubstVar(String s_from,String s_to) {
	PCRE_Replace(*pStr_to_subst, s_from, s_to);
}
void RepGen::SubstVarConditional(String s_from,String s_to,bool condition,String cond_start,String cond_end) {
	PCRE_ReplaceConditional(*pStr_to_subst, s_from, s_to, condition, cond_start, cond_end);
}

bool RepGen::ExistVar(String s) {
	RegExp r0("(.*)"+s+"(.*)",RegExp::MULTILINE);
	if (r0.GlobalMatch(*pStr_to_subst)) 
		return true;
	else
		return false;
}

void RepGen::PlaceImage(String s_from, Image im_to, Size rep_place ){
	String s;
	if (!im_to.IsEmpty() ) {
		if (rep_place.cy != 0) {
			if ( im_to.GetWidth()*100/im_to.GetHeight()>rep_place.cx*100/rep_place.cy)
				qtfim.Add(CreatePNGObject(im_to,rep_place.cx,0));
			else
				qtfim.Add(CreatePNGObject(im_to,0,rep_place.cy));
		} else {
			qtfim.Add(CreatePNGObject(im_to));
		}
		s << qtfim[qtfim.GetCount()-1];
		SubstVar(s_from,s);
	} else {
		SubstVar(s_from,"");
	}
	
};

void RepGen::RepGenCalculateBodyBeforeSubst(){
	*pTpl_cycle_res = *pTpl_cycle;
}
void RepGen::RepGenCalculateBodyAfterSubst(){
	*pRes_sub_proc += *pTpl_cycle_res;
}




void RepGen::Perform() {
	Report r;
	{
		result = "";
		
		RepGenReportStart();
		RepGenReportVar();
		if (RepGenCalculateBody) {
			PerformLoopTable("##BT","##ET",tpl);
			PerformLoopPlain("##BP","##EP",tpl);
		}

		RepGenReportFinish();
		
		result = tpl;
		Size page = Size(4500, 6700);
		r.SetPageSize(page);
		r.Margins(200,200);
		r.Put(result);
	}
	RepGenReportWindow rw;

	rw.SetButton(1, t_("QTF Export..."), 999994);
	
	int retPerform = rw.Perform(r);
	
	if (retPerform == 999994)
	{
		FileSel qtffs;

		qtffs.Type(t_("QTF file"),"*.qtf");
		qtffs.AllFilesType();
		
		if(qtffs.ExecuteSaveAs(t_("Output QTF file")))
		{
			
			SaveFile(~qtffs, result);
		}
	}


	//::Perform(r);
	//DefaultPrint(r,1);


};

void RepGen::PerformLoopTable(String start_str, String end_str, String &templ){
	String *pStr_to_subst_save;
	String *pTpl_cycle_save;
	String *pTpl_cycle_res_save;
	String *pRes_sub_proc_save;

	String lStr_to_subst;
	String lTpl_cycle;
	String lTpl_cycle_res;
	String lRes_sub_proc;
	
	
	dSavePtr(Str_to_subst);
	dSavePtr(Tpl_cycle);
	dSavePtr(Tpl_cycle_res);
	dSavePtr(Res_sub_proc);
	pStr_to_subst = &lTpl_cycle_res;
	
	String regexpstring = "(.*)::([^:]*)" + start_str + "(.*)" + end_str + "([^:}]*)(::|}})(.*)";
	RegExp r_b(regexpstring,RegExp::MULTILINE);
	while (r_b.GlobalMatch(templ)) {
		if (r_b.GetCount()==6) {
			lTpl_cycle = "::"+r_b[1]+r_b[2]+r_b[3];
			lRes_sub_proc = "";

			RepGenCalculateBody();

			templ = r_b[0] + lRes_sub_proc + r_b[4] + r_b[5];
		} else lTpl_cycle = "";
	}
	
	dRestPtr(Str_to_subst);
	dRestPtr(Tpl_cycle);
	dRestPtr(Tpl_cycle_res);
	dRestPtr(Res_sub_proc);
}

void RepGen::PerformLoopPlain(String start_str, String end_str, String &templ, bool trimlastchar ){
	String *pStr_to_subst_save;
	String *pTpl_cycle_save;
	String *pTpl_cycle_res_save;
	String *pRes_sub_proc_save;

	String lStr_to_subst;
	String lTpl_cycle;
	String lTpl_cycle_res;
	String lRes_sub_proc;
	
	
	dSavePtr(Str_to_subst);
	dSavePtr(Tpl_cycle);
	dSavePtr(Tpl_cycle_res);
	dSavePtr(Res_sub_proc);
	String s = templ;
	pStr_to_subst = &lTpl_cycle_res;
	String regexpstring = "(.*)" + start_str + "(.*)" + end_str + "(.*)";
	RegExp r_b(regexpstring,RegExp::MULTILINE);
	while (r_b.GlobalMatch(templ)) {
		if (r_b.GetCount()==3) {
			lTpl_cycle = r_b[1];
			lRes_sub_proc = "";

			RepGenCalculateBody();
			if (trimlastchar && !lRes_sub_proc.IsEmpty()) lRes_sub_proc.Trim((lRes_sub_proc.GetLength()-1));
			templ = r_b[0] + lRes_sub_proc + r_b[2];
		} else lTpl_cycle = "";
	}
	dRestPtr(Str_to_subst);
	dRestPtr(Tpl_cycle);
	dRestPtr(Tpl_cycle_res);
	dRestPtr(Res_sub_proc);
}

void RepGenSubProc::DoCalc(){
	Callback RepGenCalculateBody_save;
	RepGenCalculateBody_save = rep.RepGenCalculateBody;
	rep.RepGenCalculateBody = RepGenSubProcBody;
	rep.PerformLoopTable(start+"T", end+"T", *rep.pStr_to_subst);
	rep.PerformLoopPlain(start+"P", end+"P", *rep.pStr_to_subst, trimlastchar);
	rep.RepGenCalculateBody = RepGenCalculateBody_save;
}
