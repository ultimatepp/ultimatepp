#ifndef _StylerTest_StylerTest_h
#define _StylerTest_StylerTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <Styler/Styler.h>

#define LAYOUTFILE <StylerTest/StylerTest.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS StylerTestImg
#define IMAGEFILE <StylerTest/StylerTest.iml>
#include <Draw/iml_header.h>

class StylerTest : public WithStylerTestLayout<TopWindow> {
public:
	typedef StylerTest CLASSNAME;
	StylerTest();

	void ChangeStyle();
	void InvertColors();
	void ColorizedSkin();

	void ReloadChamCB();
	void ReloadCB();
	void Menu(Bar& bar);
	
	void InitDummys();
	void ApplyOwnStyle();

	Array<Ctrl> arc;
	StylerCtrl sc;
	ChamStyleCtrl chc;
};

#endif

