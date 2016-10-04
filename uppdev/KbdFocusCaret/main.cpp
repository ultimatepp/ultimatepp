#include <CtrlLib/CtrlLib.h>

struct KbdFocusCaret : public TopWindow {
	String text;
	virtual void GotFocus() { Refresh(); }
	virtual void LostFocus() { Refresh(); }
	virtual bool Key(dword key, int count) {
		text = GetKeyDesc(key);
		Refresh();
		return true;
	}
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), SWhite);
		w.DrawText(0, 0, text);
		w.DrawText(0, 30, HasFocus() ? "Has Focus" : "No Focus");
	}
	KbdFocusCaret() {
		Sizeable().Zoomable();
		SetCaret(60, 60, 10, 10);
	}
};

GUI_APP_MAIN
{
	KbdFocusCaret().Run();
}
