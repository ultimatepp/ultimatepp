#include <CtrlLib/CtrlLib.h>
#include <RichEdit/RichEdit.h>

using namespace Upp;

GUI_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	RDUMP(sizeof(Image));
	RDUMP(sizeof(RichText));
	RDUMP(sizeof(Ctrl::LogPos));
	RDUMP(sizeof(LabelBase));
	RLOG("=============");
	RDUMP(sizeof(Ctrl));
	RDUMP(sizeof(ScrollBar));
	RDUMP(sizeof(HeaderCtrl));
	RDUMP(sizeof(Button));
	RDUMP(sizeof(Switch));
	RDUMP(sizeof(Label));
	RDUMP(sizeof(EditField));
	RDUMP(sizeof(EditString));
	RDUMP(sizeof(EditInt));
	RDUMP(sizeof(SpinButtons));
	RDUMP(sizeof(EditIntSpin));
	RDUMP(sizeof(DisplayPopup));
	RDUMP(sizeof(PopUpTable));
	RDUMP(sizeof(DropList));
	RDUMP(sizeof(WithDropChoice<EditString>));
	RDUMP(sizeof(ArrayCtrl));
	RDUMP(sizeof(TreeCtrl));
	RDUMP(sizeof(TreeCtrl::Node));
	RDUMP(sizeof(FileSel));
	RDUMP(sizeof(RichTextView));
	RDUMP(sizeof(ColumnList));
	RDUMP(sizeof(RichEdit));
}
