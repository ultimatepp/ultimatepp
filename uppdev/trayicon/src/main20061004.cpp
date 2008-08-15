#include <CtrlLib/CtrlLib.h>
#include "mbtray.h"
#include <fstream>
#include <iostream>
#include "jtrayDef.h"
//#define IMAGECLASS Tray
//#define IMAGEFILE  <TrayIcon/tray.iml>
//#include <Draw/iml.h>
using namespace std;
//class TrayIcon : private Ctrl {
class TrayIcon : public TopWindow {


public:
	MBTrayApp* 		app;
	char* trayName;
	char*			trayImageFile;
	Callback        WhenLeftDown;
	Callback        WhenLeftUp;
	Callback        WhenLeftDouble;
	Callback1<Bar&> WhenBar;

	void            Show(bool b = true);
	void            Hide()                                 { Show(false); }
	void            Break()                                { EndLoop(0); }
	void            Run()                                  {
									TrayEventLoop();
									EventLoop(this);
								}

	void  Icon(char* trayName, char* trayImageFile);
	TrayIcon&  Tip(const char *text);
	bool FileExists(char* filename);
	void TrayEventLoop();
	void TrayHandleEvent(XEvent *xevent);
	typedef TrayIcon CLASSNAME;

	TrayIcon();
	~TrayIcon();
};

TrayIcon::TrayIcon():TopWindow()
{

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
/*
	if(b == visible)
		return;
	if(visible)
		Notify(NIM_DELETE);
	visible = b;
	if(visible)
		Notify(NIM_ADD);
*/
}

void TrayIcon::Icon(char* trayName, char* trayImageFile)
{
//    trayName= "Testing TrayIcon";        //only work when they are put here but why?????
//    trayImageFile= "some_image.png";
	if (! FileExists(trayImageFile) ) {
		printf(" You must put \"%s\" under the same directory as trayicon!\n",trayImageFile);
		return;
	}
    app = mb_tray_app_new_with_display ((unsigned char *) trayName, NULL, NULL,
				    (Display *)Xdisplay);
    Pixbuf = mb_pixbuf_new ((Display *)Xdisplay, Xscreenno);
    //XMapWindow(Xdisplay, w);
    AppImage = mb_pixbuf_img_new_from_file (Pixbuf, trayImageFile);
    //AppImage = mb_pixbuf_img_new_from_file (Pixbuf,"some_image.png" );
    //mb_tray_app_set_button_callback (app, button_callback );
    mb_tray_app_main (app);	//app is created here!!!!!


}

void TrayIcon::TrayEventLoop()
{

XEvent xevent;
	while (1)
	{
		if (get_xevent_timed(app, &xevent)) {
			TrayHandleEvent( &xevent);
		}
     	}
}
void TrayIcon::TrayHandleEvent(XEvent *xevent)
{
//	mb_tray_handle_xevent(app, xevent);
switch (xevent->type)
	{
	case KeyPress:
		/*Close the program if q is pressed.*/
		if (XK_q == XLookupKeysym (&xevent->xkey, 0)) {
			exit (EXIT_SUCCESS);
		}
		break;
	case ReparentNotify:
	  TRAYDBG("%s() Reparent Notify event\n", __func__ );
	  break;
	case ConfigureNotify:
	  TRAYDBG("%s() Config Notify event\n", __func__ );
	  handle_configure(app, &xevent->xconfigure);
	  break;
	case Expose:
	  TRAYDBG("%s() Expose event\n", __func__ );
	  handle_expose(app, &xevent->xexpose);
		printf ("Exposed!!!!\n");
	  break;
	case ButtonPress:
	  TRAYDBG("%s() Button Press Notify event\n", __func__ );
	  handle_button(app, &xevent->xbutton, false);
		printf ("You pressed button %d\n", xevent->xbutton.button);

	  break;
	case ButtonRelease:
	  handle_button(app, &xevent->xbutton, true);
		printf ("You released button %d\n", xevent->xbutton.button);

	  break;
	case PropertyNotify:
	  handle_property(app, &xevent->xproperty);
	  break;
	case ClientMessage:
	  if (xevent->xclient.message_type == app->atoms[ATOM_XEMBED])
	    {
	      switch (xevent->xclient.data.l[1])
		{
		case XEMBED_EMBEDDED_NOTIFY:
		case XEMBED_WINDOW_ACTIVATE:
		  _map_tray_window(app);
		  break;
		}
	    }
}
}


TrayIcon& TrayIcon::Tip(const char *text)
{
/*
	tip = text;
	Notify(NIM_MODIFY);
*/
	return *this;

}

class App : public TrayIcon{
	MenuBar menu;
	StatusBar status;
public:
	void Info() {
//		Icon(Tray::Icon1());
		PromptOK("TrayIcon launched this prompt!");
//		Icon(Tray::Icon());
	}

	void Menu(Bar& bar) {
		bar.Add("Info..", THISBACK(Info));
		bar.Separator();
		bar.Add("Exit", THISBACK(Break));
	}

	typedef App CLASSNAME;

	App() : TrayIcon(){
                AddFrame(menu);
		menu.Set(THISBACK(Menu));
		trayName= "Testing TrayIcon";        //can't be here but why?????
		trayImageFile= "some_image.png";
		WhenBar = THISBACK(Menu);
		WhenLeftDouble = THISBACK(Info);
		Icon(trayName,trayImageFile);
		Tip("This is U++ tray icon");

	}
};

GUI_APP_MAIN
{
	App().Run();
}
