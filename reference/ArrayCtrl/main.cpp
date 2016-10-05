#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ArrayCtrl/dlg.lay>
#include <CtrlCore/lay.h>

struct ArrayCtrlExample : public WithDlgLayout<TopWindow> {
	EditIntSpin myedit;

	void DoubleClick();

	ArrayCtrlExample();
};

Id ID1("ID1");
Id ID2("ID2");
Id ID3("ID3");

struct MyConvert : public Convert {
	Value Format(const Value& v) const {
		ValueArray va = v;
		return String().Cat() << va[0] << " - " << va[1] << " - " << va[2];
	}
};

struct MyDisplay : public Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
	                   Color ink, Color paper, dword style) const {
		w.DrawRect(r, paper);
		w.DrawEllipse(r, q);
	}
};

void ArrayCtrlExample::DoubleClick()
{
	if(array.IsCursor())
		PromptOK(String().Cat() << "ID1: " << array.Get(ID1) << ", first column" << array.Get(0));
}

ArrayCtrlExample::ArrayCtrlExample()
{
	CtrlLayoutExit(*this, "ArrayCtrl example");

	array.AddColumn("integer");
	array.AddIndex(ID1);
	array.AddColumn(ID2, "combined").Add(ID1).AddIndex(ID3).SetConvert(Single<MyConvert>());
	array.AddColumn("editable").Edit(myedit);
	array.AddColumn("with display").SetDisplay(Single<MyDisplay>());
	array.AddCtrl(editor);
	array.AddRowNumColumn("rownum");
	array.WhenLeftDouble = [=] { DoubleClick(); };
	array.ColumnWidths("51 120 60 102 62");

	for(int i = 0; i < 20; i++)
		array.Add(i, AsString(i * 10), GetSysDate(), i * 3, rand() % 1000,
		          Color(byte(i * 77), byte(i * 200), byte(i * 135)), FormatIntRoman(i));
}

GUI_APP_MAIN
{
	ArrayCtrlExample().Run();
}
