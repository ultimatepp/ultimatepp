
#include <RepGen/RepGen.h>
using namespace Upp;

#define LAYOUTFILE <RepGenTest/Select.lay>
#include <CtrlCore/lay.h>


//Clent VARs
Vector<String> STRINGS;

int STRINGS_COUNT;
int STRINGS_I;

int TOTAL;

	RepGen rep;

//Client Callbacks
void ClentCallbackReportVar() {
	LOG("ClentCallbackReportVar");
	rep.SubstVar("##NAMECORRFROM","Some Sign and Advertising Company");
	rep.SubstVar("##BANKCORRFROM","National &Program Bank");
};

void ClentCallbackReportFinish() {
	LOG("ClentCallbackReportFinish");
	rep.SubstVar("##TOTAL",Format("%`",TOTAL));
};


void ClentCallbackCalcStart() {
	LOG("ClentCallbackCalcStart");
	STRINGS_COUNT = STRINGS.GetCount();
	STRINGS_I = 0;
	TOTAL = 0;
};
void ClentCallbackCalcBody() {
	LOG("ClentCallbackCalcBody");
	Vector<String> V = Split(STRINGS[STRINGS_I],';',false);
	DUMPC(V);
	
	rep.SubstVar("##PROJECTN",TrimBoth(V.At(0)));
	rep.SubstVar("##PROJECTN",TrimBoth(V.At(0)));
	rep.SubstVar("##PROJECT`_NAME",TrimBoth(V.At(1)));
	rep.SubstVar("##DETAILID",TrimBoth(V.At(2)));
	rep.SubstVar("##COMPANY",TrimBoth(V.At(3)));
	rep.SubstVar("##WAREH`_COMP",TrimBoth(V.At(4)));
	rep.SubstVar("##QUANT",TrimBoth(V.At(5)));
	rep.SubstVar("##SIT",TrimBoth(V.At(6)));
	String pictaddr;

	if (V.GetCount()>=8 && !TrimBoth(V.At(7)).IsEmpty()) {
		pictaddr = TrimBoth(V.At(7));
		if (!pictaddr.IsEmpty()) {
#ifdef PLATFORM_X11
			pictaddr = GetHomeDirectory()+"/upp/bazaar/RepGenTest/images/"+pictaddr;
			DUMP(pictaddr);
#endif
			Image im = StreamRaster::LoadFileAny(pictaddr);
			rep.PlaceImage( "##IMAGE", im, Size(1100,700) );
//			rep.PlaceImage( "##IMAGE", im );
		}
	} else {
		rep.SubstVar("##IMAGE","");
	}

	
	TOTAL += 1;
	if(++STRINGS_I==STRINGS_COUNT) rep.LoopDone();
	
};

void ClentCallbackCalcFinish() {
	LOG("ClentCallbackCalcFinish");

};

class SelectReportDlg : public WithSelectReport<TopWindow> {
	typedef SelectReportDlg CLASSNAME;
	
public:
	SelectReportDlg();
	void arr_LeftDouble();
	//virtual 
};

SelectReportDlg::SelectReportDlg()
{
	CtrlLayoutOKCancel(*this, "Select Report template");
	ok.Ok();	
	cancel.Cancel();	
	arr.WhenLeftDouble = THISBACK( arr_LeftDouble );
}
void SelectReportDlg::arr_LeftDouble() {
	AcceptBreak(IDOK);
}


GUI_APP_MAIN
{
	
	STRINGS = Split(LoadFile(GetHomeDirectory()+"/upp/bazaar/RepGenTest/DATA.csv"), '\n', true);

	rep.RepGenReportVar       = callback(ClentCallbackReportVar);
	rep.RepGenReportFinish    = callback(ClentCallbackReportFinish);
	rep.RepGenCalculateStart  = callback(ClentCallbackCalcStart);
	rep.RepGenCalculateBody   = callback(ClentCallbackCalcBody);
	rep.RepGenCalculateFinish = callback(ClentCallbackCalcFinish);
	
	SetLanguage(GetSystemLNG()& 0xfffff);
	SelectReportDlg dlg;
	dlg.Zoomable();
	dlg.Sizeable();
	dlg.arr.AddColumn("List of Template",200);
	dlg.arr.AddColumn("s",200);
	
	dlg.arr.Add("Report Etiketji",(String)(GetHomeDirectory()+"/upp/bazaar/RepGenTest/REPORT_TEMPLATE.QTF"));
	dlg.arr.Add("Report Thumbnails",(String)(GetHomeDirectory()+"/upp/bazaar/RepGenTest/REPORT_TEMPLATE1.QTF"));
	//dlg.arr.Add("Report RusEtiketki",(String)(GetHomeDirectory()+"/upp/bazaar/Reports3/MARK-N4.QTF"));
	int i = dlg.Run();
	if(i==IDOK) {
		String s = dlg.arr.Get(1);
		rep.SetTemplate(LoadFile(s));
	
		rep.Perform();	
	}
}
