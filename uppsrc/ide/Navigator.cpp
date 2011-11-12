#include "ide.h"

void AssistEditor::SyncNavigator()
{
	if(IsNavigator()) {
		browser.Load();
		SyncCursor();
	}
	navigatorframe.Show(navigator);
}

void AssistEditor::SyncCursor()
{
	String k = "(" + GetKeyDesc(IdeKeys::AK_SEARCHCODE().key[0]) + ") ";
	browser.search.NullText(String("Find ") + k);
	browser.search.Tip(IsNull(browser.search) ? String() : "Clear " + k);
	if(IsNavigator()) {
		int ii = GetCursorLine();
		String coderef;
		while(ii >= 0 && IsNull(coderef))
			coderef = GetAnnotation(ii--);
		browser.Goto(coderef, theide->editfile);
	}
}

void AssistEditor::SelectionChanged()
{
	CodeEditor::SelectionChanged();
	SyncCursor();
	SyncParamInfo();
}

void AssistEditor::BrowserGotoNF()
{
	Value scope = browser.scope.GetKey(); // do not scroll browser.item erratically
	int scopesc = browser.scope.GetScroll();
	String item = browser.item.GetKey();
	int itemsc = browser.item.GetScroll();

	String cref = browser.GetCodeRef();
	if(theide && !theide->SwapSIf(cref))
		theide->IdeGotoCodeRef(cref);

	if(scope == browser.scope.GetKey()) {
		browser.scope.ScrollTo(scopesc);
		browser.scope.ScrollIntoCursor();
		if(item == browser.item.GetKey()) {
			browser.item.ScrollTo(itemsc);
			browser.item.ScrollIntoCursor();
		}
	}
}

void AssistEditor::BrowserGoto()
{
	BrowserGotoNF();
	SetFocus();
}

void AssistEditor::GotoBrowserScope()
{
	if(browser.scope.IsCursor()) {
		Value x = browser.scope.Get(2);
		if(IsNumber(x)) {
			int file = (int)x;
			theide->EditFile(GetCppFile(file));
			return;
		}
	}
	if(browser.item.GetCount()) {
		browser.item.GoBegin();
		BrowserGoto();
	}
}

void AssistEditor::Navigator(bool nav)
{
	navigator = nav;
	navigatorframe.Show(navigator);
	if(IsNavigator()) {
		scope_item.Show();
		browser.ClearSearch();
		SetFocus();
	}
	SyncNavigator();
	SyncCursor();
}

void AssistEditor::SerializeNavigator(Stream& s)
{
	int version = 1;
	s / version;
	s % navigatorframe;
	s % navigator;
	if(version >= 1)
		s % scope_item;
	Navigator(navigator);
}

void Ide::ToggleNavigator()
{
	editor.Navigator(!editor.navigator);
}

void Ide::SearchCode()
{
	if(!editor.navigator)
		editor.Navigator(true);
	if(editor.browser.search.HasFocus() && editor.browser.IsSearch())
		editor.browser.ClearSearch();
	else {
		String id = editor.GetI().ToString();
		if(!IsNull(id)) {
			editor.browser.search <<= Filter(id, SearchItemFilter);
			editor.browser.search.SetSelection();
			editor.browser.Load();
			editor.browser.scope.GoBegin();
		}
		editor.browser.search.SetFocus();
	}
}

void Ide::SwitchHeader() {
	int c = filelist.GetCursor();
	if(c < 0) return;
	String currfile = filelist[c];
	const char *ext = GetFileExtPos(currfile);
	if(!stricmp(ext, ".h") || !stricmp(ext, ".hpp")
	|| !stricmp(ext, ".lay") || !stricmp(ext, ".iml")) {
		int f = filelist.Find(ForceExt(currfile, ".cpp"));
		if(f < 0) f = filelist.Find(ForceExt(currfile, ".c"));
		if(f < 0) f = filelist.Find(ForceExt(currfile, ".cc"));
		if(f >= 0) filelist.SetCursor(f);
	}
}
