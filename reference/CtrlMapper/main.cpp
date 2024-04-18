#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <CtrlMapper/CtrlMapper.lay>
#include <CtrlCore/lay.h>

struct MyStruct {
	String text;
	bool   option;
	double distance;
};

struct MyDlg : WithMyDlgLayout<TopWindow> {
	typedef MyDlg CLASSNAME;
	
	void Map(CtrlMapper& m, MyStruct& s);
	
	MyDlg();
};

MyDlg::MyDlg()
{
	CtrlLayoutOK(*this, "My Dialog");
}

void MyDlg::Map(CtrlMapper& m, MyStruct& s)
{
	m
		(text, s.text)
		(option, s.option)
		(distance, s.distance, 0.001)
	;
}

GUI_APP_MAIN
{
	MyStruct data;
	data.text = "Some text";
	data.option = true;
	data.distance = 1700;

	MyDlg dlg;
	dlg.Map(CtrlMapper().ToCtrls(), data);
	if(dlg.Run() == IDOK)
		dlg.Map(CtrlMapper().ToValues(), data);
	
	PromptOK(String() << "text: [* \1" << data.text << "\1]&"
	                  << "option: [* " << data.option << "]&"
	                  << "distance: [* " << data.distance << " m]");
}
