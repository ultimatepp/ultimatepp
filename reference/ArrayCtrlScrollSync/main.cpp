#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	LineEdit  editor;
	ArrayCtrl list;
	Splitter  splitter;
	int       dosel = 0;
	
	MyApp();
};

MyApp::MyApp()
{
	Add(splitter.SizePos());
	splitter.Horz(list, editor);
	list.AddRowNumColumn().ConvertBy([=](const Value& v)->Value {
		int ii = v;
		if(ii >= editor.GetLineCount())
			return Null;
		return AsString(ii) << " " << FormatIntRoman(ii, true);
	});
	list.NoHeader().NoGrid().HideSb();
	list.NoWantFocus();
	
	String text;
	for(int i = 0; i < 1000; i++)
		text << "Line " << AsString(i) << "\n";
	editor <<= text;
	
	int cy = editor.GetFontSize().cy;
	list.SetLineCy(cy);
	
	editor.WhenAction = editor.WhenScroll = [=] {
		list.SetVirtualCount(editor.GetLineCount() + 2); // unfortunately, we need 2 more lines to cover LineEdit end of text
		list.ScrollTo(editor.GetScrollPos().y * cy);
	};
	editor.WhenSel = [=] {
		if(dosel) // avoid infinite recursion
			return;
		dosel++;
		list.SetCursor(editor.GetCursorLine());
		list.WhenAction();
		dosel--;
	};
	
	list.WhenScroll = [=] {
		editor.SetScrollPos(Point(0, list.GetScroll() / cy));
	};
	list.WhenSel = [=] {
		if(dosel) // avoid infinite recursion
			return;
		dosel++;
		int i = list.GetCursor();
		if(i >= 0 && i <= editor.GetLength()) {
			editor.SetCursor(editor.GetPos(i));
			editor.WhenAction();
		}
		dosel--;
	};

	editor.WhenAction(); // set initial scroll position in the list
	editor.WhenSel(); // set initial cursor position in the list
}

GUI_APP_MAIN
{
	MyApp().Run();
}
