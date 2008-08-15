#ifndef _main_hpp_
#define _main_hpp_

#include <CtrlLib/CtrlLib.h>
#define LAYOUTFILE <uppCanvas/uppCanvas.lay>
#include <CtrlCore/lay.h>

#include <uppCanvas/Canvas.hpp>

struct App : public WithMainWinLayout<TopWindow> {
	typedef App CLASSNAME;
	Canvas canvas;
	Button BtnClose;
	Button BtnClearSheet;
	Button BtnBGColor;

	ColorPopUp colorPopUp;

	App();
	//UserEvents
	void BtnClearSheetClick();
	void BtnCloseClick();
	void BtnBGColorClick();

	void OnColorSelect();
	//Callbacks for the canvas
	void OnCanvasPaint(Draw& w);
	void OnCanvasMouseLeft(Point p, dword keyflags);
};

#endif
