
#include "mbtray.h"
#define NIL (0)			// A name for the void pointer
class JTrayIcon
{
public:
  MBTrayApp * app;
  Display *currDisplay;
  int currScreen;
  char *trayImageFile;
  char *trayName;

    JTrayIcon (Display * currDisplay, int currScreen, char *trayName,
	       char *trayImageFile)
  {
    this->currDisplay = currDisplay;
    this->currScreen = currScreen;
    this->trayImageFile = trayImageFile;
    this->trayName = trayName;
    setupTrayIcon ();
  }
  void setupTrayIcon ()
  {
    app =
      mb_tray_app_new_with_display ((unsigned char *) trayName, NULL, NULL,
				    currDisplay);
    Pixbuf = mb_pixbuf_new (currDisplay, currScreen);
    //XMapWindow(currDisplay, w);
    AppImage = mb_pixbuf_img_new_from_file (Pixbuf, trayImageFile);
    //mb_tray_app_set_button_callback (app, button_callback );
    mb_tray_app_main (app);	//app is created here!!!!!


  }

};
int
//main2 (int argc, char **argv)
main2()
{


  Display *currDisplay = XOpenDisplay (NULL);	// parameter inside XOpenDisplay must be NULL but why?
  int currScreen = DefaultScreen (currDisplay);

  w = XCreateWindow (currDisplay, DefaultRootWindow (currDisplay), 0, 0,
		     600, 100, 0, CopyFromParent, CopyFromParent, CopyFromParent, NIL, 0);
   /* make the window actually appear on the screen. */

  XMapWindow(currDisplay, w);
  JTrayIcon jtrayicon (currDisplay, currScreen, "Tray Demo App","some_image.png");

  /* flush all pending requests to the X server, and wait until */
  /* they are processed by the X server.                        */
  //XSync(currDisplay, False);

  //XFlush(currDisplay);
}
