#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl list;
	list.SetLineCy(30);
	list.AddColumn("Font").Sorting();
	list.AddColumn("Monospace").Sorting();
	list.AddColumn("Serif").Sorting();
	list.AddColumn("Script").Sorting();
	list.AddColumn("Sample").Sorting();
	list.ColumnWidths("122 79 62 66 1084");
	for(int i = 0; i < Font::GetFaceCount(); i++) {
		Font fnt(i, 20);
		list.Add(fnt.GetFaceName(),
		         fnt.IsFixedPitch() ? "Yes" : "",
		         fnt.IsSerif() ? "Yes" : "",
		         fnt.IsScript() ? "Yes" : "",
		         AttrText("Hello αβγδ АБГД 吗嗎 弒").SetFont(fnt));
	}
	TopWindow win;
	win.Add(list.SizePos());
	win.Run();
}
