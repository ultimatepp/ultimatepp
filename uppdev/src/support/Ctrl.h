#ifndef _src_support_Ctrl_h_
#define _src_support_Ctrl_h_

#include <Core/Core.h>
#include <CtrlLib/CtrlLib.h>
#include <CtrlCore/CtrlCore.h>

#include <CtrlLib/CtrlLib.h>

#define IMAGECLASS commonImg
#define IMAGEFILE <src/support/common.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS commonImg
#define IMAGEFILE <src/support/common.iml>
#include <Draw/iml_source.h>

// layout
#define LAYOUTFILE <src/support/Ctrl.lay>
#include <CtrlCore/lay.h>

bool DropSelect(String& s, const char *title, const char *label, const Vector<String>& lst);
bool PromptPanelOKCancel(const String& tag, const String& text);

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

/*
class MyVectorText : public VectorRect {
public:
	typedef MyVectorText CLASSNAME;

	MyVectorText();
};

class MyVectorLine : public VectorLine {
public:
	typedef MyVectorLine CLASSNANE;

	MyVectorLine()										{ BeginCap(0).EndCap(0); }
};
*/

Zoom ZoomP(Ctrl &ctrl, int percent);

#endif
