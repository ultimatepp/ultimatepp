#include <plugin/pcre/Pcre.h>
#include <plugin/pcre/lib/pcre.h>

#include <RepGen/RepGen.h>

static Array<QtfRichObject> qtfim;

class RepGenReportWindow : public ReportWindow {
public:
	typedef RepGenReportWindow CLASSNAME;

	RepGenReportWindow() {
		ReportWindow::ReportWindow();
		Maximize();
	};
};

void RepGen::PCRE_Replace(String& where, String s_from,String s_to) {
	TIMING("PCRE_Replace");
	RegExp r0("(.*)"+s_from+"(.*)",RegExp::MULTILINE);
	while (r0.GlobalMatch(where)) {
		TIMING("PCRE_Replace::Inside while");
		if(r0.GetCount()==2) {
			where = r0[0] + s_to + r0[1];
		}
	}
};

void RepGen::PlaceImage(String s_from, Image im_to, Size rep_place ){
	TIMING("PlaceImage");
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
		SubstVar("##IMAGE",s);
	} else {
		SubstVar("##IMAGE","");
	}
	
};





void RepGen::Perform() {
	Report r;
	{
		TIMING("Perform");
		result = "";
		
		IN_BP_LOOP = false;
	
		RegExp r_b("(.*)::([^:]*)##BT(.*)##ET([^:}]*)(::|}})(.*)",RegExp::MULTILINE);
		RegExp r_bp("(.*)##BP(.*)##EP(.*)",RegExp::MULTILINE);
		RepGenReportStart.Execute();
		RepGenReportVar.Execute();
		if (RepGenCalculateBody) {
			IN_BP_LOOP = true;
			while (r_b.GlobalMatch(tpl)) {
				TIMING("Perform::1-st loop");
				if (r_b.GetCount()==6) {
					tplb = "::"+r_b[1]+r_b[2]+r_b[3];
					resultb = "";
					ENDCALCULATE = false;
					RepGenCalculateStart();
					while (ENDCALCULATE == false) {
						TIMING("Perform::1-st loop::inside detail loop");
						tplbr = tplb;
		
						RepGenCalculateBody();
		
						resultb += tplbr;
					}
					RepGenCalculateFinish();
					tpl = r_b[0] + resultb + r_b[4] + r_b[5];
					DUMP(r_b[4]);
				} else tplb = "";
			}
			IN_BP_LOOP = false;

			IN_BP_LOOP = true;
			while (r_bp.GlobalMatch(tpl)) {
				TIMING("Perform::3-d loop");
				if (r_bp.GetCount()==3) {
					tplb = r_bp[1];
					resultb = "";
					ENDCALCULATE = false;
					RepGenCalculateStart();
					while (ENDCALCULATE == false) {
						TIMING("Perform::3-d loop::inside detail loop");
						tplbr = tplb;
		
						RepGenCalculateBody();
		
						resultb += tplbr;
					}
					RepGenCalculateFinish.Execute();
					tpl = r_bp[0] + resultb + r_bp[2];
				} else tplb = "";
			}
		}
		IN_BP_LOOP = false;
		RepGenReportFinish.Execute();
		
		result = tpl;
		Size page = Size(4500, 6700);
		r.SetPageSize(page);
		r.Margins(200,200);
		r.Put(result);
	}
	RepGenReportWindow().Perform(r);
	//DefaultPrint(r,1);


};
