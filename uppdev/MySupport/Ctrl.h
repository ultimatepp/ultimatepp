#ifndef _src_support_Ctrl_h_
#define _src_support_Ctrl_h_

#include <Core/Core.h>
#include <CtrlLib/CtrlLib.h>
#include <CtrlCore/CtrlCore.h>

#include <CtrlLib/CtrlLib.h>
#include <MySupport/MyKeys.h>
#include <MySupport/MyHelp.h>
#include <MySupport/MyPrompt.h>
#include <MySupport/MyProgress.h>

#define IMAGECLASS commonImg
#define IMAGEFILE <MySupport/common.iml>
#include <Draw/iml_header.h>

// layout
#define LAYOUTFILE <MySupport/Ctrl.lay>
#include <CtrlCore/lay.h>

bool DropSelect(String& s, const char *title, const char *label, const Vector<String>& lst);
bool PromptPanelOKCancel(const String& tag, const String& text);
void DirSel(EditField& f, FrameRight<Button>& b);
static void sSetFolder(EditField *f);

class SupDropList : public DropList {
protected:
	int PreviousIndex;
	int CurrIndex;
	Callback _action;

private:
	void KeepCurrIndex()												{ PreviousIndex=CurrIndex; CurrIndex=GetIndex(); return; }
	void NewAction()														{ KeepCurrIndex(); _action(); return; }

public:
	// methods
	void GoBackPrevious()												{ if(PreviousIndex>=0) SetIndex(PreviousIndex); }
	void SetIndex(int i)												{ SetData(GetKey(i)); KeepCurrIndex(); }
	
	// operators overload
	Callback  operator<<=(Callback action)	 	 	{ _action=action; WhenAction=THISBACK(NewAction); return _action; }
	Callback& operator<<(Callback action)   		{ _action=action; return WhenAction<<THISBACK(NewAction); }

	typedef SupDropList CLASSNAME;

	// constructor	
	SupDropList()																{ PreviousIndex = -1; CurrIndex = -1; }
};

Zoom ZoomP(Ctrl &ctrl, int percent);

#endif
