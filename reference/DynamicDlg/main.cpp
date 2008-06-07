#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct XmlDlg : public TopWindow {
	Array<Label>           label;
	ArrayMap<String, Ctrl> ctrl;

	bool Load(const char *xml);
};

bool XmlDlg::Load(const char *xml)
{
	int y = 10;
	try {
		XmlParser p(xml);
		while(!p.IsTag())
			p.Skip();
		p.PassTag("dialog");
		int linecy = Draw::GetStdFontCy() + 4;
		while(!p.End())
			if(p.TagE("option")) {
				Add(ctrl.Create<Option>(p["id"]).SetLabel(p["label"]).TopPos(y, linecy).LeftPos(10, 100));
				y += linecy + 4;
			}
			else
			if(p.TagE("edit")) {
				Add(label.Add().SetLabel(p["label"]).TopPos(y, linecy).LeftPos(10, 30));
				Add(ctrl.Create<EditString>(p["id"]).TopPos(y, linecy).LeftPos(40, 60));
				y += linecy + 4;
			}
			else
				p.Skip();
	}
	catch(XmlError e) {
		Exclamation("XML error: " + e);
		return false;
	}
	SetRect(0, 0, 110, y + 10);
	return true;
}

GUI_APP_MAIN
{
	XmlDlg dlg;
	if(!dlg.Load(LoadFile(GetDataFile("dialog.xml"))))
		return;
	dlg.Run();
	int q = dlg.ctrl.Find("E1");
	if(q >= 0)
		PromptOK("E1 value: " + DeQtf(AsString(~dlg.ctrl[q])));
}
