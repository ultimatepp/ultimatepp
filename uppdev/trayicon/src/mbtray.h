/*
 * Copyright (C) 2002 Matthew Allum
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef _MB_TRAY_H_
#define _MB_TRAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xmd.h>


#include "mbpixbuf.h"
//#include "jtrayDef.h"

#ifdef __cplusplus
extern "C" {
#endif
 MBPixbuf      *Pixbuf;
 MBPixbufImage *AppImage;
Window w;



/**
 * @defgroup mbtray MBTray - System tray applet toolkit
 * @brief A simple toolkit for creating xlib based tray ( aka panel )
 * 'applets'.
 * See <a href="http://www.freedesktop.org/standards/systemtray.html">System tray Spec</a>
 * and <a href="http://www.freedesktop.org/standards/xembed.html">XEmbed</a>
 * Spec for infomation on the gory details of the mechanism.
 *
 * Example: A very basic panel app. Displays an image and message when
 *          clicked on
 *
 * <pre>
 *
 * #include <libmb/mb.h>
 *
 * MBPixbuf      *Pixbuf;
 * MBPixbufImage *AppImage
 *
 * void
 * paint_callback ( MBTrayApp *app, Drawable drw )
 * {
 *   MBPixbufImage *img_scaled;
 *
 *   img_scaled = mb_pixbuf_img_scale (Pixbuf, AppIcon,
 * 				    mb_tray_app_width(app),
 * 				    mb_tray_app_height(app));
 *
 *   mb_pixbuf_img_render_to_drawable (Pixbuf, img_scaled, drw, 0, 0);
 *
 *   mb_pixbuf_img_free( Pixbuf, img_scaled );
 * }
 *
 * void
 * button_callback (MBTrayApp *app, int x, int y, Bool is_released )
 * {
 *   if (is_released)
 *     mb_tray_app_tray_send_message(app, "Hello World!", 10);
 * }
 *
 * int
 * main(int argc, char **argv)
 * {
 *   MBTrayApp *app = NULL;
 *
 *   app = mb_tray_app_new ( "Tray Demo App",
 * 			  NULL,
 * 			  paint_callback,
 * 			  &argc,
 * 			  &argv );
 *
 *   if (app == NULL) usage();
 *
 *   Pixbuf = mb_pixbuf_new(mb_tray_app_xdisplay(app),
 * 			 mb_tray_app_xscreen(app));
 *
 *   AppImage = mb_pixbuf_img_new_from_file(Pixbuf, "some_image.png")
 *
 *   mb_tray_app_set_button_callback (app, button_callback );
 *
 *   mb_tray_app_main(app);
 * }
 *
 * </pre>
 *
 * There are also numerous other examples in the matchbox distrubution source
 * ( see the util directory ).
 * These should give you more infomation on handling resizes etc.
 *
 * @{
 */

/**
 * @typedef MBTrayApp
 *
 * Opaque structure used for representing a tray app
 */
typedef struct MBTrayApp MBTrayApp;

/* TOGO ? */
typedef void (*MBTrayBackgroundCB)( void *user_data ) ;

/**
 * @typedef MBTrayAppResizeCB
 *
 * Callback for a resize.
 * Is triggered when the panel resizes your application.
 */
typedef void (*MBTrayAppResizeCB)( MBTrayApp *mb, int width, int height ) ;

/**
 * @typedef MBTrayAppPaintCB
 *
 * Callback for a handling a 'paint' or expose event.
 *
 * You application should paint to the supplied X Drawable.
 * Also see #mb_tray_app_get_background for getting a #MBPixbufImg
 * representation of the panel area below your application. Using this
 * you can alpha composite you application to the panel.
 *
 * You should make sure you paint the entire supplied drawable.
 */
typedef void (*MBTrayAppPaintCB)( MBTrayApp *mb, Drawable drawable ) ;

/**
 * @typedef MBTrayAppPaintCB
 *
 * Callback for a handling a 'context' message.
 *
 * if #mb_tray_set_context_info has been called, this callback will
 * be triggered when a user clicks on the bubble message context text.
 *
 */
typedef void (*MBTrayAppContextCB)( MBTrayApp *mb ) ;

/**
 * @typedef MBTrayAppButtonEventCB
 *
 * Callback for a button event.
 */
typedef void (*MBTrayAppButtonEventCB)( MBTrayApp *mb_tray_app,
					int        x,
                                        int        y,
                                        Bool       is_release ) ;

/**
 * @typedef MBTrayAppXEventCB
 *
 * Callback for an X event. Also see #mb_tray_app_get_xevent_mask and
 * #mb_tray_app_set_xevent_mask
 */
typedef void (*MBTrayAppXEventCB)( MBTrayApp *mb_tray_app, XEvent *event ) ;

/**
 * @typedef MBTrayAppThemeChangeCB
 *
 * Callback for a theme change.
 */
typedef void (*MBTrayAppThemeChangeCB)( MBTrayApp *mb_tray_app, char *theme_name ) ;

/**
 * @typedef MBTrayAppPollCB
 *
 * Callback for a Timeout event.
 */
typedef void (*MBTrayAppPollCB)( MBTrayApp *mb_tray_app) ;


/**
 * Constructs a new tray app instance.
 *
 * After calling this, one should then set other required callbacks
 * before starting the panels event loop with #mb_tray_app_main
 * ( The actaul docking process occurs here ).
 *
 * The resize_cb will get called when you panel resizes your application.
 * See MBTrayAppResizeCB
 *
 * The paint_cb will get called when your application recieves an expose
 * expose event and should update its contents accordingly.
 * See #MBTrayAppPaintCB
 *
 * mb_tray_app_new will check for and remove the arguments from argv;
 * <ul>
 *  <li>--display, -d &lt;Display&gt; X11 Display to connect to</li>
 *  <li>--offset,  -o &lt;+-integer&gt; requested panel position</li>
 * </ul>
 *
 * NOTES:
 * The enviromental var 'SYSTEM_TRAY_ID' will read if set. This specifys the
 * system tray to connect to. Defaults to 1.
 *
 * @param app_name Name of tray application
 * @param resize_cb application resize callback
 * @param paint_cb  application paint callback
 * @param argc      command line parameter count
 * @param argv      command line parameter vector
 * @returns a MBTrayApp object, NULL on failiure.
 */
void
_set_win_context_hint(MBTrayApp *mb);
void
_map_tray_window (MBTrayApp *mb);

void
handle_property(MBTrayApp       *mb,
		XPropertyEvent *event );
void
_expose( MBTrayApp       *mb,
	       XExposeEvent    *event );
void
handle_expose( MBTrayApp       *mb,
	       XExposeEvent    *event );
void
handle_configure(MBTrayApp       *mb,
		 XConfigureEvent *event );
void
handle_button( MBTrayApp    *mb,
	       XButtonEvent *event ,
	       Bool          is_release);
Bool get_xevent_timed( MBTrayApp* mb, XEvent* event_return );
MBTrayApp*
mb_tray_app_new (unsigned char         *app_name,
		 MBTrayAppResizeCB      resize_cb,
		 MBTrayAppPaintCB       paint_cb);

/**
 * Constructs a new tray app instance.
 *
 * Just like #mb_tray_app_new , but allow the X Display to be
 * specified. This function is intended to aid integration with
 * toolkits like Gtk.
 *
 * @param app_name Name of tray application
 * @param resize_cb application resize callback
 * @param paint_cb  application paint callback
 * @param argc      command line parameter count
 * @param argv      command line parameter vector
 * @param display   Pre-existing X Display
 * @returns a MBTrayApp object, NULL on failiure.
 */
MBTrayApp*
mb_tray_app_new_with_display ( unsigned char         *app_name,
			       MBTrayAppResizeCB      resize_cb,
			       MBTrayAppPaintCB       paint_cb,
			       Display               *display);

/**
 * Sets the callback function for any extra x event processing.
 * Also see #mb_tray_app_get_xevent_mask and #mb_tray_app_set_xevent_mask
 *
 * @param mb_tray_app Tray instance
 * @param xevent_cb x11 event callback
 */
void
mb_tray_app_set_xevent_callback (MBTrayApp         *mb_tray_app,
				 MBTrayAppXEventCB  xevent_cb );

/**
 * Sets the callback function for a mouse button press/release.
 *
 * @param mb_tray_app Tray instance
 * @param button_cb button callback
 */
void
mb_tray_app_set_button_callback (MBTrayApp              *mb_tray_app,
				 MBTrayAppButtonEventCB  button_cb );

/**
 * Sets the callback function for a theme change
 *
 * @param mb_tray_app Tray instance
 * @param theme_cb theme callback
 */
void
mb_tray_app_set_theme_change_callback (MBTrayApp              *mb_tray_app,
				       MBTrayAppThemeChangeCB  theme_cb );

/**
 * Sets the callback function for a theme change
 *
 * @param mb_tray_app Tray instance
 * @param poll_cb timeout callback
 * @param tv      a timeval struct specifying the timeout.
 */
void
mb_tray_app_set_timeout_callback (MBTrayApp              *mb_tray_app,
				  MBTrayAppPollCB         poll_cb,
                                  struct  timeval        *tv);

/**
 * Sets the callback function to be notifed when the panel app is resized.
 *
 * @param mb_tray_app Tray instance
 * @param resize_cb timeout callback
 */
void
mb_tray_app_set_resize_callback (MBTrayApp *mb_tray_app,
				 MBTrayAppResizeCB resize_cb );

/**
 * Sets the callback function for panel app paint
 *
 * @param mb_tray_app Tray instance
 * @param paint_cb paint callback
 */
void
mb_tray_app_set_paint_callback (MBTrayApp              *mb_tray_app,
				MBTrayAppPaintCB  paint_cb );

/**
 * Sets the 'context' callback
 *
 * @param mb_tray_app Tray instance
 * @param context_cb context callback
 */
void
mb_tray_app_set_context_callback (MBTrayApp              *mb_tray_app,
				  MBTrayAppContextCB      context_cb );

/**
 * Requests a new size for the application. It is not guarenteed
 * your app will get this size. Your application should expected
 * to handle the size supplied to it ( via the resize callback).
 *
 * You should take care with this function as not to end up in an
 * infinite loop continually re-requesting the same failed parameters.
 *
 * @param mb_tray_app Tray instance
 * @param width request width in pixels
 * @param height request height in pixels
 */
void
mb_tray_app_request_size (MBTrayApp *mb_tray_app,
                          int        width,
                          int        height );

/**
 * Requests the app positional offset in terms of the panels orientation
 * from the origin of the tray.
 *
 * Note its unlikely many trays will allow free positioning. The matchbox
 * tray uses positive/negative offset values to determine what side of the
 * tray the app is located on docking.
 *
 * @param mb_tray_app Tray instance
 * @param offset      The panel app offset
 */
void
mb_tray_app_request_offset (MBTrayApp *mb_tray_app,
			    int        offset);

/**
 * Set timing for any timeout callbacks.
 *
 * @param mb_tray_app Tray instance
 * @param  tv      a timeval struct specifying the timeout. Pass NUll to clear
 *                 any timeout callbacks.
 */
void
mb_tray_app_set_poll_timeout (MBTrayApp      *mb_tray_app,
			      struct timeval *tv );

/**
 * Requests the app is repainted - the paint callback will get called.
 *
 * @param mb_tray_app Tray instance
 */
void
mb_tray_app_repaint (MBTrayApp *mb_tray_app);

/**
 * Sends a messsage to the tray, used to visually inform or notify the user.
 *
 * @param mb_tray_app Tray instance
 * @param msg         Message to send ( assumed UTF8 )
 * @param timeout     Time in seconds for message to stay visible.
 */
void
mb_tray_app_tray_send_message (MBTrayApp     *mb_tray_app,
			       unsigned char *msg,
			       int            timeout);

/**
 * Sets the tray app name.
 *
 * @param mb_tray_app Tray instance
 * @param name        The tray app name
 */
void
mb_tray_app_set_name (MBTrayApp     *mb_tray_app,
		      unsigned char *name);

/**
 * Sets text for extra button in window message.
 *
 * @param mb_tray_app Tray instance
 * @param info        context button text
 */
void
mb_tray_app_set_context_info (MBTrayApp     *mb_tray_app,
			      unsigned char *info);

/**
 * Sets the tray app icon.
 *
 * @param mb_tray_app Tray instance
 * @param pixbuf mbpixbuf ref
 * @param image  mbpixbuf image to set as icon
 */
void
mb_tray_app_set_icon(MBTrayApp     *mb_tray_app,
		     MBPixbuf      *pixbuf,
		     MBPixbufImage *image);

/**
 * Enable app to show session data so tray can 'remember it' for
 * future sessions.
 * Defaults to True
 *
 * @param mb_tray_app Tray instance
 * @param val         Boolean indicating action
 */
void
mb_tray_app_set_session ( MBTrayApp *mb_tray_app,
			  Bool       val );

/**
 * Get session state.
 *
 * @param mb_tray_app Tray instance
 * @returns True is sessioning active, False otherwise
 */
Bool
mb_tray_app_get_session ( MBTrayApp *mb_tray_app );

/**
 * Gets the tray app co-ordinates relative to the root window.
 *
 * @param mb_tray_app Tray instance
 * @param x X co-ord
 * @param y Y co-ord
 * @returns True on success, False on faliure
 */
Bool
mb_tray_app_get_absolute_coords (MBTrayApp  *mb_tray_app,
				 int        *x,
                                 int        *y  );

/**
 * Set any user data to be attached to the tray app instance
 *
 * @param mb_tray_app Tray instance
 * @param data void pointer to user data
 */
void
mb_tray_app_set_user_data (MBTrayApp *mb_tray_app,
			   void      *data );

/**
 * Gets any user data to be attached to the tray app instance
 *
 * @param mb_tray_app Tray instance
 * @returns the user data.
 */
void*
mb_tray_app_get_user_data (MBTrayApp *mb_tray_app);


Bool
mb_tray_app_tray_is_vertical (MBTrayApp *mb_tray_app );

/**
 * Gets the panel background for the area covered by the tray app.
 *
 * @param mb_tray_app Tray instance
 * @param pixbuf      mb pixbuf object
 * @returns an mbpixbuf of the panel background covered by the panel app.
 */
MBPixbufImage *
mb_tray_app_get_background (MBTrayApp *mb_tray_app,
			    MBPixbuf  *pixbuf);

/**
 * Returns the current theme
 *
 * @param mb_tray_app Tray instance
 * @returns the name of the current theme, or NULL if no theme is set.
 */
char *
mb_tray_app_get_theme_name (MBTrayApp *mb_tray_app);

/**
 * Returns the current X Event mask used by the tray app
 *
 * @param mb_tray_app Tray instance
 * @returns the current X event mask
 */
long
mb_tray_app_get_xevent_mask (MBTrayApp *mb_tray_app );

/**
 * Safely sets the X Event mask used to select what events
 * the xevent callback recieves.
 *
 * @param mb_tray_app Tray instance
 * @param mask requested X event mask
 */
void
mb_tray_app_set_xevent_mask (MBTrayApp *mb_tray_app, long mask);


/**
 * Gets the tray apps X display
 *
 * @param mb_tray_app Tray instance
 * @returns An Xlib display
 */
Display*
mb_tray_app_xdisplay (MBTrayApp *mb_tray_app);

/**
 * Gets the tray app X screen
 *
 * @param mb_tray_app Tray instance
 * @returns An Xlib screen
 */
int
mb_tray_app_xscreen (MBTrayApp *mb_tray_app);

/**
 * Gets the tray app root window
 *
 * @param mb_tray_app Tray instance
 * @returns An Xlib root window
 */
Window
mb_tray_app_xrootwin(MBTrayApp *mb_tray_app);

/**
 * Gets the tray app window
 *
 * @param mb_tray_app Tray instance
 * @returns An Xlib window
 */
Window
mb_tray_app_xwin (MBTrayApp *mb_tray_app);

/**
 * Gets the tray width
 *
 * @param mb_tray_app Tray instance
 * @returns the tray apps width in pixels
 */
int
mb_tray_app_width (MBTrayApp *mb_tray_app);

/**
 * Gets the tray height
 *
 * @param mb_tray_app Tray instance
 * @returns the tray apps height in pixels
 */
int
mb_tray_app_height (MBTrayApp *mb_tray_app);

/**
 * Gets the apps offset from the panels origin
 *
 * @param mb_tray_app Tray instance
 * @returns the current offset
 */
int
mb_tray_app_offset ( MBTrayApp *mb_tray_app ) ;

/**
 *
 * Inits the main loop. Only needed if you plan on not using
 * mbtrays main loop - eg if using with GTK.
 *
 * @param mb_tray_app Tray instance
 */
void
mb_tray_app_main_init ( MBTrayApp *mb_tray_app );

/**
 * Runs the tray app main loop until #mb_tray_app_main_quit is called
 *
 * @param mb_tray_app Tray instance
 */
void
mb_tray_app_main (MBTrayApp *mb_tray_app);

/**
 * Hides ( undocks ) the tray app.
 *
 * @param mb_tray_app Tray instance
 */
void
mb_tray_app_hide (MBTrayApp *mb_tray_app );

/**
 * Unhides ( redocks ) the tray app.
 *
 * @param mb_tray_app Tray instance
 */
void
mb_tray_app_unhide (MBTrayApp *mb_tray_app );


/**
 * Stops #mb_tray_app_main
 *
 * @param mb_tray_app Tray instance
 */
void
mb_tray_app_main_quit (MBTrayApp *mb_tray_app);

/**
 * Runs a single iteration of the mainloop
 *
 * @param mb_tray_app Tray instance
 * @param xevent      X event to process
 */
void
mb_tray_handle_xevent (MBTrayApp *mb_tray_app,
		       XEvent    *xevent);



/* ---------- Older stuff below ----------------------------------------- */


int  mb_tray_init(Display* dpy, Window win);

Window mb_tray_get_window(void);

void mb_tray_init_session_info(Display *d, Window win, char **argv, int argc);

void mb_tray_handle_event(Display *dpy, Window win, XEvent *an_event);

void mb_tray_send_message(Display *d, Window win,
			  unsigned char* msg, int timeout);

void mb_tray_map_window (Display* dpy, Window win);

void
mb_tray_bg_change_cb_set(MBTrayBackgroundCB bg_changed_cb, void *user_data);

Bool mb_tray_get_bg_col(Display *dpy, XColor *xcol);

void
mb_tray_window_icon_set(Display *dpy, Window win_panel, MBPixbufImage *img);

void mb_tray_unmap_window (Display* dpy, Window win);

MBPixbufImage *mb_tray_get_bg_img(MBPixbuf *pb, Window win);

/** @} */
struct MBTrayApp
{

  unsigned char *app_name;
  unsigned char *context_info;


  Window  win, win_root, win_tray;
  Display *dpy;
  int screen;

  MBTrayBackgroundCB       background_cb;
  MBTrayAppResizeCB        resize_cb;
  MBTrayAppPaintCB         paint_cb;
  MBTrayAppButtonEventCB   button_cb;
  MBTrayAppXEventCB        xevent_cb;
  MBTrayAppPollCB          poll_cb;
  MBTrayAppThemeChangeCB   theme_cb;
  MBTrayAppContextCB       context_cb;

  int x, y, w, h;

  int offset;

  Pixmap drawable;

  int poll_fd;
  struct timeval *poll_timeout;

  void *user_data;

  //Atom atoms[TRAY_ATOMS_CNT];
  Atom atoms[100];
  Atom atoms_wm_protocols[3];

  Bool tray_is_vertical;

  MBPixbufImage *img_icon;

  Bool is_hidden;
  Bool show_session_data;

  int tray_id;


  char *theme_name;

  long  event_mask;
  long  base_event_mask;

  Bool           have_cached_bg;
  MBPixbufImage *cached_bg;

  MBPixbuf *pb_ext_ref; 	/* XXX should be considered unsafe */
};   //struct MBTrayApp


#ifdef __cplusplus
}
#endif

#endif
