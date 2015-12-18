#ifndef _GrabYourScreen_GrabYourScreen_h
#define _GrabYourScreen_GrabYourScreen_h

#include <CtrlLib/CtrlLib.h>
#include <RichEdit/RichEdit.h>
#include <Report/Report.h>
#include <Controls4U/Controls4U.h>
#include <SysInfo/SysInfo.h>

using namespace Upp;

#define LAYOUTFILE <GrabYourScreen/GrabYourScreen.lay>
#include <CtrlCore/lay.h>

class GrabYourScreen;

class GrabScreen : public WithScreen<StaticRect> {
public:
	typedef GrabScreen CLASSNAME;
	GrabScreen();
	
	void Init(GrabYourScreen &program);
	void ButGrab_Push();
	void ButSnap_Push();
	void SwGrabMode_Action();

private:
	GrabYourScreen *program;
};

class GrabBook : public WithBook<StaticRect> {
public:
	typedef GrabBook CLASSNAME;
	GrabBook();
	~GrabBook();
	
	void Init();
	void OnPaint(Painter &painter);
	void OnReset();
	void OnCapture();
	void OnSavePDF();
	void Timer();

	int keyId;
	String folder;
};

class GrabYourScreen : public TopWindow {
public:
	typedef GrabYourScreen CLASSNAME;
	GrabYourScreen();
	void Init();
	void Exit();
		
	TabCtrl mainTab;
	GrabScreen grabScreen;
	GrabBook grabBook;
};

#endif
