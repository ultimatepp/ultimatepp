#include <CtrlLib/CtrlLib.h>
#include <CtrlCore/CtrlCore.h>
#include <unistd.h>
#include "mbtray.h"
#include <fstream>
#include <iostream>
#include "jtrayDef.h"
//#define IMAGECLASS Tray
//#define IMAGEFILE  <TrayIcon/tray.iml>
//#include <Draw/iml.h>
using namespace std;
//class TrayIcon : public Ctrl {
class  TrayIcon : public TopWindow {
	MenuBar menu;
	TopWindow menuWin;
	TopWindow *mainWin;
	Image   img;
	virtual void Paint(Draw& w) {

        w.DrawRect(GetSize(), SWhite);

        //w.DrawText(0, 0, "Hello world!", Arial(30), Magenta);
	img = StreamRaster::LoadFileAny(trayImageFile);
        Refresh();
	if(img) {
	        //w.DrawImage(0, 0, img);
	        w.DrawImage(0, 0,30,30, img);

	}
	else
	        w.DrawText(0, 0, "No image loaded!", Arial(30).Italic());

    }
public:
	MBTrayApp* 	app;
	char* 		trayName;
	char*		trayImageFile;
	char* 		tip;
	Callback        WhenLeftDown;
	Callback        WhenLeftUp;
	Callback        WhenLeftDouble;
	Callback1<Bar&> WhenBar;
	//virtual Top 	*top;
	void            Show(bool b = true);
	void            Hide()                  { Show(false); }
	void            Break()                 { EndLoop(0); }

	void  Icon(char* trayName, char* trayImageFile);
	TrayIcon&  Tip(const char *text);
	bool FileExists(char* filename);
	void TrayEventLoop();
	void EventProc(XWindow& w, XEvent *xevent);
	//void TrayHandleEvent(XEvent *xevent);
	typedef TrayIcon CLASSNAME;
	void RegisWin(Window w);
	virtual void
	LeftDown(Point pos, dword flags) {

        puts("Left Down");
	Tip(tip);
        Refresh();

    }
	virtual void
	RightDown(Point pos, dword flags) {

        puts("Right Down");
	Rect rect = this->GetRect();
		menuWin.SetRect(rect.right,rect.top-100,100,100);
		menuWin.NoToolWindow().NoLayoutZoom();
	menuWin.OpenMain();
	menuWin.AddFrame(menu);
	menu.Set(THISBACK(MainBar));
	//menuWin.Hide();
	menuWin.Show();

        Refresh();

    }
	virtual void
	MouseMove(Point pos, dword flags) {

        puts("Mouse Move");

        Refresh();

    }
	void MainBar(Bar& bar)

    {

        bar.Add("Toggle Main Window", THISBACK(ToggleMainWindow));
	bar.Add("Quit",THISBACK(MenuQuit));

    }
	void ToggleMainWindow() {
	puts("Toggle Main Window");

	if (mainWin->IsShown())
		mainWin->Hide();
	else
		mainWin->Show();

    }
	void MenuQuit() {
		exit (EXIT_SUCCESS);
	}


	TrayIcon(TopWindow *mainWin, char *trayImageFile, char* tip);
	~TrayIcon();
};




TrayIcon::TrayIcon(TopWindow *mainWin, char *trayImageFile, char* tip)
{

		this->trayImageFile = trayImageFile;
		Icon(trayName,trayImageFile);
		this->mainWin = mainWin;
		this->tip = tip;

}


TrayIcon::~TrayIcon()
{

}

bool TrayIcon::FileExists(char* filename)
{

	bool flag = false;
	fstream fin;
	fin.open(filename,ios::in);
	if( fin.is_open() )
	{
	//cout<<"file exists"<<endl;
	flag=true;
	}
	fin.close();
return flag;
}
void TrayIcon::Show(bool b)
{

}

void TrayIcon::RegisWin(Window w) {

	if(!w) XError("XCreateWindow failed !");
	int i = Xwindow().Find(None);
	printf("Number of XWindow found: %d \n", i);
	if(i >= 0) Xwindow().SetKey(i, w);
	XWindow& cw = i >= 0 ? Xwindow()[i] : Xwindow().Add(w);
	cw.ctrl = this;
	cw.exposed = true;
	cw.owner = NULL;
	cw.xic = xim ? XCreateIC(xim,
	                         XNInputStyle, XIMPreeditNothing|XIMStatusNothing,
	                         XNClientWindow, w,
	                         XNFocusWindow, w,
	                         NULL)
	             : NULL;

	top = new Top;
	top->window = w;

	long im_event_mask = 0;

	if(cw.xic)
		XGetICValues(cw.xic, XNFilterEvents, &im_event_mask, NULL);
	XSelectInput(Xdisplay, w, ExposureMask|StructureNotifyMask|KeyPressMask|
	             FocusChangeMask|KeyPressMask|KeyReleaseMask|PointerMotionMask|
	             ButtonPressMask|ButtonReleaseMask|PropertyChangeMask|
	             VisibilityChangeMask|im_event_mask);


}

void TrayIcon::Icon(char* trayName, char* trayImageFile)
{

	if (! FileExists(trayImageFile) ) {
		printf(" You must put \"%s\" under the same directory as trayicon!\n",trayImageFile);
		return;
	}

    app = mb_tray_app_new_with_display ((unsigned char *) trayName, NULL, NULL,
				    (Display *)Xdisplay);

    mb_tray_app_main (app);	//app is created here!!!!!
    RegisWin(app->win);

}



void TrayIcon::EventProc(XWindow& w, XEvent *xevent)
{

printf("%d %d \n",(int)&w, xevent->type);

	Ctrl::EventProc(w,xevent);
	return;


}


TrayIcon& TrayIcon::Tip(const char *text)
{
 mb_tray_app_tray_send_message(app, (unsigned char*)text, 20);
	return *this;

}

class App : public TopWindow {
public:
	App() {

	}

};
GUI_APP_MAIN
{

	App testApp;

	TopWindow mainWin;
		mainWin.SetRect(400,300,200,200);
		mainWin.OpenMain();
		mainWin.Title("Main Window XXXX");
		mainWin.Hide();
	TrayIcon traywindow(&mainWin,"some_image.png","Hello World!!!");
	//traywindow.AddChild(&testApp);   // useless!

	Ctrl::EventLoop();
	//App().Run();
}
