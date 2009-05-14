#ifndef _I18NCtrlLayoutTest_I18NCtrlLayoutTest_h
#define _I18NCtrlLayoutTest_I18NCtrlLayoutTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <I18NCtrlLayoutTest/Layout.lay>
#include <CtrlCore/lay.h>

class MainDlg : public WithMainDlg<TopWindow> {
	MenuBar m_menu;
	typedef MainDlg CLASSNAME;

public:
	void OnFile();
	void OnPallette();
	void OnPrinter();
	void OnMenu(Bar& bar );
	void OnLanguage(Bar& bar );
	void OnChangeLanguage(int language );

	MainDlg();
};

#endif

