#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ArrayCtrlEdits/ArrayCtrlEdits.lay>
#include <CtrlCore/lay.h>

struct App : public WithAppLayout<TopWindow> {
	EditDouble editdbl;

	App();
};

struct SqrNumberConvert : Convert {
	virtual Value Format(const Value& q) const {
		return IsNull(q) ? (double)Null : (double)q * (double)q;
	}
};

void EditDblMinMaxFactory(One<Ctrl>& ctrl)
{
	ctrl.Create<EditDouble>().MinMax(0, 1);
}

App::App()
{
	CtrlLayout(*this);
	list.AddColumn("Number").Edit(editdbl);
	list.AddColumnAt(0, "Number^2").SetConvert(Single<SqrNumberConvert>());
	list.AddColumn("Text").Ctrls<EditString>();
	list.AddColumn("Limited number").Ctrls(EditDblMinMaxFactory);
	list.AddCtrl(editor);
	list.AddCtrl(option);
	list.Appending().Removing();
	list.SetLineCy(EditField::GetStdHeight());
	list.ColumnWidths("49 63 74 96");
}

GUI_APP_MAIN
{
	App().Run();
}
