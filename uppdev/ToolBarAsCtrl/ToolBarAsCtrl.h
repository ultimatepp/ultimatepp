#ifndef _ToolBarAsCtrl_ToolBarAsCtrl_h
#define _ToolBarAsCtrl_ToolBarAsCtrl_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ToolBarAsCtrl/ToolBarAsCtrl.lay>
#include <CtrlCore/lay.h>

class ToolBarAsCtrl : public WithToolBarAsCtrlLayout<TopWindow> {
private:
	ToolBar tb;
	EditString h;

public:
	virtual void Paint(Draw& w) { w.DrawRect(GetSize(), LtCyan); }

	typedef ToolBarAsCtrl CLASSNAME;
	ToolBarAsCtrl();
	void DoNothing();
	void ShowToolBarButtons();
	void Layout();
	void MyBar(Bar& tb);
};

#endif
