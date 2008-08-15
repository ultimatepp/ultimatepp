#ifndef _PopUp_PopUp_h
#define _PopUp_PopUp_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <PopUp/PopUp.lay>
#include <CtrlCore/lay.h>



class popup : public WithpopupLayout<TopWindow> {
public:
	typedef popup CLASSNAME;

	WithDlgPopUp<StaticRect> dlg;
	void PopDlg();
	void DlgClose();
	Rect rr;
	Size ss;

	popup();
};

#endif
