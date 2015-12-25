#include "GrabYourScreen.h"

#include <plugin/jpg/jpg.h>

GrabScreen::GrabScreen() {
	CtrlLayout(*this);
}

void GrabScreen::Init(GrabYourScreen &_program) {
	program = &_program;
	editFileNameGrab <<= AppendFileName(GetDesktopFolder(), "ScreenGrab.avi");
	editFileNameGrab.Type(t_("Video file"), ".avi");
	editTime <<= 5;
	editFrameRate <<= 1; 
	opGrabMouse <<= true;
	swGrabMode.Add("Desktop");
	swGrabMode.Add("Window");
#if defined(PLATFORM_WIN32) 
	swGrabMode.Add("Rectangle");
#endif
	swGrabMode.MinCaseHeight(20);
	swGrabMode.SetData(0);
	swGrabMode.WhenAction = THISBACK(SwGrabMode_Action);
	SwGrabMode_Action();
#if defined(PLATFORM_WIN32) 	
	butGrab.WhenPush = THISBACK(ButGrab_Push);
#else
	butGrab.Enable(false);
	editFileNameGrab.Enable(false);
	editTime.Enable(false);
	editFrameRate.Enable(false);
	opGrabMouse.Enable(false);
	editLeft.Hide();
	editTop.Hide();
	editWidth.Hide();
	editHeight.Hide();
	left.Hide();
	top.Hide();
	width.Hide();
	height.Hide();
#endif
	
	editLeft <<= 100;
	editWidth <<= 1000;
	editTop <<= 100;
	editHeight <<= 900;
	
	String extension;
	editFileNameSnap <<= AppendFileName(GetDesktopFolder(), "ScreenSnap.jpg");
	editFileNameSnap.Type(t_("Image file"), ".jpg");
	butSnap.WhenPush = THISBACK(ButSnap_Push);
	
	Array<int64> hWnd, processId;
	Array<String> name, fileName, caption;
	GetWindowsList(hWnd, processId, name, fileName, caption, false);
	Sort(caption);
	for (int i = 0; i < caption.GetCount(); ++i) {
		if (!Trim(caption[i]).IsEmpty())
			editWindowTitle.Add(caption[i]);
	}
	editWindowTitle.SetData(editWindowTitle.GetValue(0));
	
	SetTimeCallback(-1000, THISBACK(Timer));
}

void GrabScreen::SwGrabMode_Action() {
	switch((int)swGrabMode.GetData()) {
	case 0:
		editLeft.Enable(false);
		editTop.Enable(false);
		editWidth.Enable(false);
		editHeight.Enable(false);
		editWindowTitle.Enable(false);
		break;
	case 1:
		editLeft.Enable(false);
		editTop.Enable(false);
		editWidth.Enable(false);
		editHeight.Enable(false);
		editWindowTitle.Enable(true);
		break;
	case 2:
		editLeft.Enable(true);
		editTop.Enable(true);
		editWidth.Enable(true);
		editHeight.Enable(true);
		editWindowTitle.Enable(false);
		break;
	}
}

void GrabScreen::ButGrab_Push() {
#if defined(PLATFORM_WIN32) 
	FileDelete(editFileNameGrab.GetData().ToString());
	
	bool ret;
	if (swGrabMode.GetData() == 0) 
		ret = Record_Desktop(editFileNameGrab, editTime, editFrameRate, opGrabMouse);
	else if (swGrabMode.GetData() == 1) 
		ret = Record_Window(editFileNameGrab, editTime, GetWindowIdFromCaption(~editWindowTitle, false), editFrameRate, opGrabMouse);
	else if (swGrabMode.GetData() == 2) 
		ret = Record_DesktopRectangle(editFileNameGrab, editTime, editLeft, editTop, editWidth, editHeight, editFrameRate, opGrabMouse);
	else
		throw Exc("Unexpected value");
	if (!ret)
		Exclamation("Error on grabbing");
#endif
}

void GrabScreen::ButSnap_Push() {
/*	program->Minimize();
	
	TopWindow win;
	Button b;
	b.SetLabel("CLOSE");
	b <<= win.Breaker();
	StaticImage desktop;
	desktop.Set(Snap_Desktop());
	win.Add(desktop.SizePos());
	win.Add(b.LeftPos(10, 100).TopPos(10, 30));
	
	win.FullScreen().Run();
	
	program->Overlap();
*/	
	String fileName = ~editFileNameSnap;
	FileDelete(fileName);
	
	String ext = GetFileExt(fileName);
	
	if (ext == ".png") {
		PNGEncoder encoder;
		if (!encoder.SaveFile(fileName, canvasImage))
			Exclamation(Format(t_("Impossible to save %s"), fileName));
	} else if (ext == ".jpg") {	
		JPGEncoder encoder(90);
		if (!encoder.SaveFile(fileName, canvasImage))
			Exclamation(Format(t_("Impossible to save %s"), fileName));
	} else
		Exclamation(Format(t_("File format \"%s\" not found"), ext));
}

void GrabScreen::Timer() {
	NON_REENTRANT_V;
	
	switch(int(~swGrabMode)) {
	case 0: 	canvasImage = Snap_Desktop();
				break;
	case 1:		canvasImage = Snap_Window(GetWindowIdFromCaption(~editWindowTitle, false));
				break;
	case 2:		canvasImage = Snap_DesktopRectangle(editLeft, editTop, editWidth, editHeight);
				break;
	}
	
	canvas.SetBackground(canvasImage);
}