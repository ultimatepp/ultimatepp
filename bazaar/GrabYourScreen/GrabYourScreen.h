#ifndef _GrabYourScreen_GrabYourScreen_h
#define _GrabYourScreen_GrabYourScreen_h

#include <CtrlLib/CtrlLib.h>
//#include <RichEdit/RichEdit.h>
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
	~GrabScreen();
	
	void Init(GrabYourScreen &program);
	
private:
	void ButGrab_Push();
	void ButSnap_Push();
	void SwGrabMode_Action();
	void Timer();
	
	GrabYourScreen *program;
	Image canvasImage;
	int keyId;
	TimeCallback timeCallback;
};

class GrabBook : public WithBook<StaticRect> {
public:
	typedef GrabBook CLASSNAME;
	GrabBook();
	~GrabBook();

	void Init();
	
private:	
	void OnPaint(Painter &painter);
	void OnReset();
	void OnCapture();
	void OnSavePDF();
	void Timer();

	int keyId;
	String folder;
	TimeCallback timeCallback;
};

class GrabYourScreen : public TopWindow {
public:
	typedef GrabYourScreen CLASSNAME;
	GrabYourScreen();
	
	void Init();
	
private:
	void Exit();
		
	TabCtrl mainTab;
	GrabScreen grabScreen;
	GrabBook grabBook;
};

#endif
