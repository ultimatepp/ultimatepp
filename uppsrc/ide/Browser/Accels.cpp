#include "Browser.h"

class AccelDlg : public WithAccelLayout<TopWindow> {
	typedef AccelDlg CLASSNAME;
	
	Array<DropList>    style;
	Array<DropList>    font;
	Array<DropList>    height;
	Array<ColorPusher> ink;
	Array<ColorPusher> paper;

public:
	const RichStyles *textstyles;

	AccelDlg();
};

AccelDlg::AccelDlg()
{
	CtrlLayoutOKCancel(*this, "Styling keys");
	
	list.AddColumn("Key");
	list.AddColumn("Paragraph style");
	list.AddColumn("Font");
	list.AddColumn("Height");
	list.AddColumn("Ink");
	list.AddColumn("Paper");
	list.SetLineCy(EditField::GetStdHeight() + 2);
	list.NoGrid().EvenRowColor();
	
	for(int i = 0; i < 20; i++) {
		list.Add((i >= 10 ? "Shift+Alt+" : "Alt+") + AsString(i % 10));
		DropList& st = style.At(i);
		st.Add(Null, "-");
		for(int j = 0; j < textstyles->GetCount(); j++)
			st.Add(textstyles->GetKey(j), (*textstyles)[j].name);
		list.SetCtrl(i, 1, st);
		list.SetCtrl(i, 2, font.At(i).Add(Null));
		list.SetCtrl(i, 3, height.At(i).Add(Null));
		list.SetCtrl(i, 4, ink.At(i).NullText("<no change>"));
		list.SetCtrl(i, 5, paper.At(i).NullText("<no change>"));
	}
}

void TopicEditor::Accels()
{
	AccelDlg dlg;
	dlg.textstyles = &editor.Get().GetStyles();
	dlg.Run();
}
