//#ifndef JTRAYDEF
//	#define JTRAYDEF
	#include "jtrayDef.h"
//#endif

#include "mbtray.h"

static int
error_handler(Display     *display,
	      XErrorEvent *error)
{
   trapped_error_code = error->error_code;
   return 0;
}

static void
trap_errors(void)
{
   trapped_error_code = 0;
   old_error_handler = XSetErrorHandler(error_handler);
}

static int
untrap_errors(void)
{
   XSetErrorHandler(old_error_handler);
   return trapped_error_code;
}

static unsigned long
_get_server_time(Display* dpy)
{
  XEvent xevent;
  Atom timestamp_atom = XInternAtom(dpy, "_MB_DOCK_TIMESTAMP", False);
  unsigned char c = 'a';

  XChangeProperty (dpy, RootWindow(dpy, DefaultScreen(dpy)),
		   timestamp_atom, timestamp_atom,
		   8, PropModeReplace, &c, 1);
  for (;;) {
    XMaskEvent(dpy, PropertyChangeMask, &xevent);
        if (xevent.xproperty.atom == timestamp_atom)
	  return xevent.xproperty.time;
  }
}


//static
Bool
get_xevent_timed( MBTrayApp* mb, XEvent* event_return )
{
  static struct timeval tv_tmp;

  if (mb->poll_fd == -1 && mb->poll_timeout == NULL)
    {
      XNextEvent(mb->dpy, event_return);
      return True;
    }

  XFlush(mb->dpy);

  if (XPending(mb->dpy) == 0)
    {
      int fd = ConnectionNumber(mb->dpy);
      fd_set readset;
      FD_ZERO(&readset);
      FD_SET(fd, &readset);
      if (mb->poll_fd != -1)
	FD_SET(mb->poll_fd, &readset);

      if (mb->poll_timeout)
	{
	  if (tv_tmp.tv_sec <= 0 && tv_tmp.tv_usec <= 0)
	    {
	      /* select sets tv_tmp with time left so we let it decrement
	       * to 0 and then reset.
               * XXX According to man page Im not sure how portable this is.
	      */
	      memcpy(&tv_tmp, mb->poll_timeout, sizeof(struct timeval));
	    }
	}

      if (select( (fd > mb->poll_fd) ? fd+1 : mb->poll_fd+1 ,
		  &readset, NULL, NULL,
		  ( mb->poll_timeout ) ? &tv_tmp : NULL
		  ) == 0) /* XXX set up tv */
	{
	  if (mb->poll_timeout && mb->poll_cb)
	    mb->poll_cb( mb );
	  return False;
	} else {
	  if ( mb->poll_fd != -1 && FD_ISSET(mb->poll_fd, &readset)
	       && mb->poll_cb )
	    mb->poll_cb( mb );

	  if ( FD_ISSET( fd, &readset) )
	    {
	      XNextEvent(mb->dpy, event_return);
	      return True;
	    }

	  return False;
	}
    } else {
      XNextEvent(mb->dpy, event_return);
      return True;
    }
}

void
mb_tray_app_set_poll_timeout ( MBTrayApp *mb,
			       struct  timeval *tv )
{
  if (mb->poll_timeout) free (mb->poll_timeout);

  if (tv == NULL)
    {
      mb->poll_timeout = NULL;
      return;
    }

  mb->poll_timeout = malloc(sizeof(struct timeval));
  memcpy(mb->poll_timeout, tv, sizeof(struct timeval));

}

void
mb_tray_app_set_poll_fd ( MBTrayApp *mb,
			  int        fd )
{
  mb->poll_fd = fd;
}

void
mb_tray_app_set_user_data ( MBTrayApp *mb,
			    void      *data )
{
  mb->user_data = data;
}

void
mb_tray_app_set_session ( MBTrayApp *mb,
		      Bool       data )
{
  mb->show_session_data = data;
}

Bool
mb_tray_app_get_session ( MBTrayApp *mb )
{
  return mb->show_session_data;
}

void*
mb_tray_app_get_user_data (MBTrayApp *mb)
{
  return mb->user_data;
}

void
mb_tray_app_set_resize_callback ( MBTrayApp *mb,
				  MBTrayAppResizeCB resize_cb )
{
  mb->resize_cb = resize_cb;
}

void
mb_tray_app_set_paint_callback (MBTrayApp              *mb,
				MBTrayAppPaintCB  paint_cb )
{
  mb->paint_cb = paint_cb;
}

void
mb_tray_app_set_context_callback (MBTrayApp              *mb,
				  MBTrayAppContextCB      context_cb )
{
  mb->context_cb = context_cb;
}

void
mb_tray_app_set_xevent_callback ( MBTrayApp *mb,
				  MBTrayAppXEventCB xevent_cb )
{
  mb->xevent_cb = xevent_cb;
}

void
mb_tray_app_set_button_callback (MBTrayApp              *mb,
				 MBTrayAppButtonEventCB  button_cb )
{
  mb->button_cb = button_cb;
}

void
mb_tray_app_set_timeout_callback (MBTrayApp              *mb,
				  MBTrayAppPollCB         poll_cb,
                                  struct  timeval        *tv)
{
  if (mb->poll_timeout) free (mb->poll_timeout);

  mb->poll_timeout = malloc(sizeof(struct timeval));
  memcpy(mb->poll_timeout, tv, sizeof(struct timeval));

  mb->poll_cb = poll_cb;
}

void
mb_tray_app_set_theme_change_callback (MBTrayApp              *mb,
				       MBTrayAppThemeChangeCB  theme_cb )
{
  mb->theme_cb = theme_cb;
}

char *
mb_tray_app_get_theme_name (MBTrayApp *mb)
{
  return mb->theme_name;
}

static void
_set_icon_hint(MBTrayApp *mb, MBPixbuf *pb, MBPixbufImage *img)
{

 CARD32 *data = NULL;

  data = malloc(sizeof(CARD32)*((img->width*img->height)+2));
  if (data)
    {
      unsigned char r,g,b,a;
      int i = 2, x, y;
      data[0] = img->width;
      data[1] = img->height;


      for (y=0; y<img->height; y++)
	for (x=0; x<img->width; x++)
	  {
	    mb_pixbuf_img_get_pixel (pb, img, x, y, &r, &g, &b, &a);
	    data[i] = ( (a << 24) | (r << 16)| (g << 8) | b );
	    i++;
	  }


      XChangeProperty(mb->dpy, mb->win, mb->atoms[ATOM_NET_WM_ICON] ,
		      XA_CARDINAL, 32, PropModeReplace,
		      (unsigned char *)data,
		      (img->width*img->height)+2);
      free(data);
    }
}

void
mb_tray_app_set_icon(MBTrayApp *mb, MBPixbuf *pb, MBPixbufImage *img)
{
  if (img == NULL) return;

  if (mb->win != None && mb->pb_ext_ref)
    _set_icon_hint(mb, mb->pb_ext_ref, img);
  else
    mb->pb_ext_ref = pb;

  if (img != mb->img_icon)
    {
      if (mb->img_icon) mb_pixbuf_img_free(pb, mb->img_icon);
      mb->img_icon = mb_pixbuf_img_clone(pb, img);
    }
}

void
mb_tray_app_get_geometry ( MBTrayApp *mb,
			   int       *x,
			   int       *y,
			   int       *w,
			   int       *h )
{
  if (x) *x = mb->x;
  if (y) *y = mb->y;
  if (w) *w = mb->w;
  if (h) *h = mb->h;
}

int
mb_tray_app_offset ( MBTrayApp *mb )
{
  return mb->offset;
}


Bool
mb_tray_app_tray_is_vertical ( MBTrayApp *mb )
{
  return mb->tray_is_vertical;
}
 void
 default_paint_callback ( MBTrayApp *app, Drawable drw )
 {
   MBPixbufImage *img_scaled;

   img_scaled = mb_pixbuf_img_scale (Pixbuf, AppImage,
 				    mb_tray_app_width(app),
 				    mb_tray_app_height(app));

   mb_pixbuf_img_render_to_drawable (Pixbuf, img_scaled, drw, 0, 0);

   mb_pixbuf_img_free( Pixbuf, img_scaled );
 }

 void
 default_button_callback (MBTrayApp *app, int x, int y, Bool is_released )
 {
   if (is_released)
     mb_tray_app_tray_send_message(app, (unsigned char*)"Hello World!", 10);
 }

MBTrayApp*
mb_tray_app_new ( unsigned char         *app_name,
		  MBTrayAppResizeCB      resize_cb,
		  MBTrayAppPaintCB       paint_cb)
{
  return mb_tray_app_new_with_display (app_name,
				       resize_cb,
				       paint_cb,
				       NULL);
}


MBTrayApp*
mb_tray_app_new_with_display ( unsigned char         *app_name,
			       MBTrayAppResizeCB      resize_cb,
			       MBTrayAppPaintCB       paint_cb,
			       Display               *display)
{
  MBTrayApp *mb = NULL;

  char *dpy_name     = NULL;
  int i;

  mb = malloc(sizeof(MBTrayApp));
  memset(mb, 0, sizeof(MBTrayApp));

  mb->show_session_data = True;


 /* Set up mb-> screen parameter */
  if (display == NULL)
    {
      if ((mb->dpy = XOpenDisplay(dpy_name)) == NULL)
	{
	  fprintf(stderr, "Cannot open display: %s\n", dpy_name ? dpy_name : " ");
	  free(mb); 		/* XXX Set an error */
	  return NULL;
	}
    }
  else mb->dpy = display;

  if (getenv("MB_SYNC"))
    XSynchronize (mb->dpy, True);

  mb->screen   = DefaultScreen(mb->dpy);
  mb->win_root = DefaultRootWindow(mb->dpy);
  mb->win = None;
  mb->w = 16;
  mb->h = 16;

  mb->drawable = None;

  mb->resize_cb = resize_cb;
  if (paint_cb != NULL)
    mb->paint_cb = paint_cb;
  else
  	mb->paint_cb = default_paint_callback;

  mb->button_cb = NULL;
  mb->xevent_cb = NULL;
  mb->poll_cb   = NULL;

  mb->poll_fd = -1;
  mb->poll_timeout = NULL;

  mb->tray_id = 0;

  mb->app_name = (unsigned char*)(app_name ? strdup((char*)app_name) : strdup("unnamed"));

  mb->have_cached_bg = False;
  mb->cached_bg      = NULL;

  mb->pb_ext_ref     = NULL;
  mb->img_icon       = NULL;

  mb->is_hidden         = False;


  mb->base_event_mask = mb->event_mask
    = StructureNotifyMask|ExposureMask|ButtonPressMask|ButtonReleaseMask;

  return mb;

}

void
mb_tray_app_set_name (MBTrayApp     *mb,
		      unsigned char *name)
{
  if (mb->app_name) free(mb->app_name);
  mb->app_name = (unsigned char*)strdup((char*)name);
}

void
mb_tray_app_set_context_info (MBTrayApp     *mb,
			      unsigned char *info)
{
  if (info == NULL) return;

  if (mb->context_info) free(mb->context_info);

  mb->context_info = (unsigned char*)strdup((char*)info);

  if (mb->win) _set_win_context_hint(mb);

}

//static
void
handle_button( MBTrayApp    *mb,
	       XButtonEvent *event ,
	       Bool          is_release)
{

  if (mb->button_cb && event->window == mb->win )
    mb->button_cb(mb, event->x, event->y, is_release) ;
}

//static void
void
handle_configure(MBTrayApp       *mb,
		 XConfigureEvent *event )
{
  static int had_configure;

  TRAYDBG("%s() got configure\n", __func__);

  if (event->window != mb->win) return;

  if (mb->is_hidden) return;



  mb->have_cached_bg = False;

  mb->x = event->x;
  mb->y = event->y;

  TRAYDBG("%s() x is now %i\n", __func__, mb->x);

  mb->offset = (mb->tray_is_vertical ? mb->y : mb->x);

  if (event->width != mb->w || event->height != mb->h || !had_configure)
    {

      mb->w = event->width;
      mb->h = event->height;

      TRAYDBG("%s() recondigured to %ix%i \n", __func__, mb->w, mb->h);

      if (mb->drawable != None) XFreePixmap(mb->dpy, mb->drawable);

      mb->drawable = XCreatePixmap(mb->dpy, mb->win_root, mb->w, mb->h,
				   DefaultDepth(mb->dpy, mb->screen));

      if (mb->resize_cb) mb->resize_cb(mb, mb->w, mb->h);

      had_configure = 1;
    }
  else mb_tray_app_repaint (mb); /* Assume X/Y Has changed */

}

static Bool
tray_is_vertical_via_hint(MBTrayApp *mb)
{
  Atom realType;
  unsigned long n;
  unsigned long extra;
  int format;
  int status;
  long *value = NULL;

  status = XGetWindowProperty(mb->dpy, mb->win_tray,
			      mb->atoms[ATOM_NET_SYSTEM_TRAY_ORIENTATION],
			      0L, 1L, False, XA_CARDINAL, &realType,
			      &format, &n, &extra,
			      (unsigned char **) &value);

  if (status == Success && n && value != NULL && value[0])
    {
      XFree(value);
      return True;
    }

  if (value) XFree(value);

  return False;
}

static void
set_theme_via_root_prop(MBTrayApp *mb)
{

}

void
handle_property(MBTrayApp       *mb,
		XPropertyEvent *event )
{

  if (event->window != mb->win
      && event->window != mb->win_tray
      && event->window != mb->win_root) return;

  /* TRAYDBG("%s() Prop Notify event\n", __func__ ); */

  if (event->atom == mb->atoms[ATOM_NET_SYSTEM_TRAY_ORIENTATION])
    {
      mb->tray_is_vertical = tray_is_vertical_via_hint(mb);
    }
  else if (event->atom == mb->atoms[ATOM_MB_PANEL_BG])
    {
      mb->have_cached_bg = False;
      handle_expose( mb, NULL );
    }
  else if (event->atom == mb->atoms[ATOM_MB_THEME])
    {
      set_theme_via_root_prop(mb);
    }
}


void
_expose( MBTrayApp       *mb,
	       XExposeEvent    *event )
{
	 mb->drawable = XCreatePixmap(mb->dpy, mb->win_root, mb->w, mb->h,
				   DefaultDepth(mb->dpy, mb->screen));
	mb->paint_cb(mb, mb->drawable);
	  XSetWindowBackgroundPixmap(mb->dpy, mb->win, mb->drawable);
	  XClearWindow(mb->dpy, mb->win);
	  XFlush(mb->dpy);
}


void
handle_expose( MBTrayApp       *mb,
	       XExposeEvent    *event )
{
  if ( mb->paint_cb && mb->drawable)
    {
      mb->paint_cb(mb, mb->drawable );
      if (mb->win)
	{
	  XSetWindowBackgroundPixmap(mb->dpy, mb->win, mb->drawable);
	  XClearWindow(mb->dpy, mb->win);
	  XFlush(mb->dpy);
	}
    }
}

void
mb_tray_app_repaint (MBTrayApp *mb)
{
  handle_expose( mb, NULL );
}

void
mb_tray_app_request_size (MBTrayApp *mb,
                          int        width,
                          int        height )
{
  if (mb->win == None)
    {
      mb->w = width;
      mb->h = height;
    }
  else
    {
      XResizeWindow(mb->dpy, mb->win, width, height);
    }
}

void
mb_tray_app_request_offset (MBTrayApp *mb,
			    int        offset)
{
  if (mb->win == None)
    mb->offset = offset;
}

Bool
mb_tray_app_get_absolute_coords (MBTrayApp *mb, int *x, int *y)
{
  Window child;
  XTranslateCoordinates (mb->dpy,
			 mb->win,
			 mb->win_root, 0,0, x, y,&child);
  return True; 			/* XXX should check for X error */
}

MBPixbufImage *
mb_tray_app_get_background(MBTrayApp *mb, MBPixbuf *pb)
{
  Atom realType;
  unsigned long n;
  unsigned long extra;
  int format;
  int status;
  char *value = NULL;
  MBPixbufImage *img_result = NULL;
  XColor xcol;
  XWindowAttributes win_attr;
  Bool no_cache = False;

  if (mb->have_cached_bg && mb->cached_bg)
    {
      /* TRAYDBG("%s() return cache \n", __func__);  -- too verbose*/
      return mb_pixbuf_img_clone(pb, mb->cached_bg);
    }

  status = XGetWindowProperty(pb->dpy, mb->win_tray,
			      mb->atoms[ATOM_MB_PANEL_BG],
			      0L, 512L, False,
			      XA_STRING, &realType,
			      &format, &n, &extra,
			      (unsigned char **) &value);

  if (status == Success && value != NULL && strlen(value) > 4)
    {
      TRAYDBG("%s() got background %s\n", __func__, value);

      if (strncasecmp("pxm", value, 3) == 0)
	{
	  Pixmap foreign_pxm = (Pixmap)atol(value+4);

	  TRAYDBG("%s() pixmap id is %li\n", __func__, foreign_pxm);

	  /* Check the window is mapped before we try any pixmap grabbing */
	  if (XGetWindowAttributes(mb->dpy, mb->win, &win_attr)
	      && ( win_attr.map_state == IsUnmapped
		   /*|| win_attr.map_state == IsUnviewable*/ ))
	    {
	      no_cache = True;
	    }
	  else if (foreign_pxm)
	    {
	      trap_errors();

	      img_result = mb_pixbuf_img_new_from_drawable(pb,
							   foreign_pxm,
							   None,
							   mb->x,
							   mb->y,
							   mb->w,
							   mb->h);

	      if (untrap_errors())
		{
		  TRAYDBG("%s() Got an X error. img tray is %p",
			  __func__, img_result);
		  img_result = NULL;
		}
	    }

	  TRAYDBG("%s() copying pixmap from +%i+%i %ix%i \n", __func__,
		  mb->x,
		  mb->y,
		  mb->w,
		  mb->h);
	}
      else
	{
	  xcol.pixel = (Pixmap)atol(value+4);

	  XQueryColor(pb->dpy, DefaultColormap(pb->dpy, pb->scr), &xcol);

	  TRAYDBG("%s() got col %x, %x, %x\n", __func__, xcol.red,
		  xcol.green, xcol.blue);

	  img_result = mb_pixbuf_img_new(pb, mb->w, mb->h) ;
	  mb_pixbuf_img_fill(pb, img_result,
			     xcol.red >> 8,
			     xcol.green >> 8,
			     xcol.blue >> 8, 0);
	}
    } else {
      TRAYDBG("%s() failed to get bg prop\n",__func__);
    }

  if (img_result == NULL)
    {				/* Blank - approx Gnome Panel colors*/
      img_result = mb_pixbuf_img_rgb_new(pb, mb->w, mb->h);
      mb_pixbuf_img_fill(pb, img_result, 0xe2, 0xe2, 0xde, 0);
      TRAYDBG("%s() img_result is NULL !\n",__func__);
    }

  if (value) XFree(value);

  if (!no_cache) /* Dont cache when were unmapped */
    {
      if (mb->cached_bg) mb_pixbuf_img_free(pb, mb->cached_bg);
      mb->cached_bg = mb_pixbuf_img_clone(pb, img_result);
      mb->have_cached_bg = True;
    }

  return img_result;
}

static void
_set_xembed_info (MBTrayApp *mb, int flags)
{
   CARD32 list[2];

   list[0] = MAX_SUPPORTED_XEMBED_VERSION;
   list[1] = flags;
   XChangeProperty (mb->dpy, mb->win, mb->atoms[ATOM_XEMBED_INFO],
		    mb->atoms[ATOM_XEMBED_INFO] , 32,
		    PropModeReplace, (unsigned char *) list, 2);
}

//static
void
_map_tray_window (MBTrayApp *mb)
{
  XMapRaised(mb->dpy, mb->win);
  _set_xembed_info (mb, XEMBED_MAPPED);
}

static void
_send_tray_message_data( MBTrayApp *mb, unsigned char *data )
{
   XEvent ev;
   memset(&ev, 0, sizeof(ev));

   ev.xclient.type = ClientMessage;
   ev.xclient.window = mb->win;
   ev.xclient.message_type = mb->atoms[ATOM_NET_SYSTEM_TRAY_MESSAGE_DATA];
   ev.xclient.format = 8;

   memcpy(ev.xclient.data.b, data, sizeof(unsigned char)*20);

   trap_errors();

   XSendEvent(mb->dpy, mb->win_tray, False, NoEventMask, &ev);

   if (untrap_errors() && mb_want_warnings())
     fprintf(stderr, "mbtray : X error %i on message send\n",
	     trapped_error_code );
}

static void
_send_tray_opcode(
		  MBTrayApp *mb,
		  Window     win,
		  long message,
		  long data1,
		  long data2,
		  long data3
		  ){
   XEvent ev;

   memset(&ev, 0, sizeof(ev));
   ev.xclient.type = ClientMessage;
   ev.xclient.window = win;
   ev.xclient.message_type = mb->atoms[ATOM_SYSTEM_TRAY_OPCODE];
   ev.xclient.format = 32;
   ev.xclient.data.l[0] = _get_server_time(mb->dpy);
   ev.xclient.data.l[1] = message;
   ev.xclient.data.l[2] = data1;
   ev.xclient.data.l[3] = data2;
   ev.xclient.data.l[4] = data3;

   trap_errors();
   XSendEvent(mb->dpy, mb->win_tray, False, NoEventMask, &ev);
   XSync(mb->dpy, False);
   if (untrap_errors() && mb_want_warnings())
     fprintf(stderr, "Tray.c : X error %i on opcode send\n",
	     trapped_error_code );
}

long
mb_tray_app_get_xevent_mask (MBTrayApp *mb )
{
  return mb->event_mask;
}

void
mb_tray_app_set_xevent_mask (MBTrayApp *mb, long mask)
{
  mb->event_mask = (mb->base_event_mask|mask);
  if (mb->win) XSelectInput(mb->dpy, mb->win, mb->event_mask);
}
/////////////////////////////////////////// XChangePropeties : name, context_hint, docking ///////////////////////////////////////
static void
_set_win_utf8_name(MBTrayApp *mb)
{
  XChangeProperty (mb->dpy, mb->win,
		   mb->atoms[ATOM_NET_WM_NAME],
		   mb->atoms[ATOM_UTF8_STRING],
		   8,
		   PropModeReplace, mb->app_name, strlen((char*)mb->app_name));
}

//static
void
_set_win_context_hint(MBTrayApp *mb)
{
  if (mb->win != None && mb->context_info != None)
    {
      XChangeProperty (mb->dpy, mb->win,
		       mb->atoms[ATOM_MB_SYSTEM_TRAY_CONTEXT],
		       mb->atoms[ATOM_UTF8_STRING],
		       8,
		       PropModeReplace,
		       mb->context_info, strlen((char*)mb->context_info));
    }
}

static void
_init_docking (MBTrayApp *mb )
{
  XWindowAttributes win_tray_attr;
  XSizeHints size_hints;
  pid_t this_pid;

  /* we found the tray, so now figure out what the app params should be */

  /* Dont attemp to dock if tray marked hidden */
  if (mb->is_hidden)
    return;

  TRAYDBG("%s() offset is %i, is vertical: %i\n",
	  __func__, mb->offset, mb->tray_is_vertical);

  mb->tray_is_vertical = tray_is_vertical_via_hint(mb);

  if (XGetWindowAttributes(mb->dpy, mb->win_tray, &win_tray_attr))
    {
       /* Note 4 is default total mb panel margin, but it doesn't really
	* make any difference. Sizing is less magic nowadays with
	* Everything working via configures.
       */
      if (mb->tray_is_vertical)
	mb->h = mb->w = win_tray_attr.width - 4;
      else
	mb->w = mb->h = win_tray_attr.height - 4;

      TRAYDBG("%s() got tray ( %li ) attributes as %ix%i\n",
	      __func__, mb->win_tray, win_tray_attr.width, win_tray_attr.height);

    }

  if (mb->w < 10 && mb->h < 10)
    {
      /*** XXX HACK to work round gnome panel issues XXX ***

      - Gnome panel system tray does not expect apps to scale up
        - seems not to allow apps change width.
      - The tray win is not the actual parent. so difficult if possible
        at all to put a guess at the initial 'right' size.
      - We just set to 32x32 and hope for the best.
      - Dunno what hapens with KDE
      - Not that bigger deal anyway, as long as GTK tray applets run ok
        in mbdock.

      ***/
      mb->h = mb->w = 32;
    }

  TRAYDBG("%s() init size %i x %i\n",
	  __func__, mb->w, mb->h);

  if (mb->resize_cb) mb->resize_cb(mb, mb->w, mb->h);

  mb->win = XCreateSimpleWindow(mb->dpy, mb->win_root,
				mb->tray_is_vertical ? 0 : mb->offset,
				mb->tray_is_vertical ? mb->offset : 0,
				mb->w, mb->h, 0,
				BlackPixel(mb->dpy, mb->screen),
				WhitePixel(mb->dpy, mb->screen));

  /* Select for interested events on panel window */
  XSelectInput(mb->dpy, mb->win, mb->event_mask);

  XFlush(mb->dpy);

  /* XXX Not needed yet
  XSetWMProtocols(mb->dpy, mb->win, atoms_wm_protocols,
		  sizeof(atoms_wm_protocols) / sizeof(Atom));
  */


  if (mb->pb_ext_ref)
    mb_tray_app_set_icon(mb, mb->pb_ext_ref, mb->img_icon);

  this_pid = getpid();
  XChangeProperty (mb->dpy, mb->win, mb->atoms[ATOM_NET_WM_PID], XA_CARDINAL,
		   32, PropModeReplace, (unsigned char *)&this_pid, 1);


  size_hints.flags = PSize | PMinSize | PMaxSize;

  TRAYDBG("%s() offset is %i, is vertical: %i\n",
	  __func__, mb->offset, mb->tray_is_vertical);

  size_hints.max_width  = 128;
  size_hints.min_width  = mb->w;
  size_hints.width      = mb->w;
  size_hints.max_height = 128;
  size_hints.min_height = mb->h;
  size_hints.height     = mb->h;

  TRAYDBG("%s() set w: %i, h: %i\n", __func__, mb->w, mb->h);

  XSetStandardProperties(mb->dpy, mb->win, (char*)mb->app_name,
			 NULL, 0, NULL, 0, &size_hints);

  _set_win_utf8_name(mb);

  _set_win_context_hint(mb);

  _send_tray_opcode(mb, mb->win_tray, SYSTEM_TRAY_REQUEST_DOCK, mb->win, 0, 0);

  XSelectInput(mb->dpy, mb->win_tray, PropertyChangeMask);
}    //static void _init_docking

void
mb_tray_app_tray_send_message(MBTrayApp *mb, unsigned char* msg, int timeout)
{
   unsigned char buf[20];
   int msg_len = strlen((char*)msg);
   int id = 12345; /* TODO id should unique */
   int bytes_sent = 0;

   _send_tray_opcode(mb, mb->win, SYSTEM_TRAY_BEGIN_MESSAGE, timeout, msg_len, id );

   while ( bytes_sent < msg_len )
   {
      if ( (msg_len - bytes_sent) > 20)
      {
	 memcpy(buf, &msg[bytes_sent], sizeof(unsigned char)*20);
      } else {
	 memcpy(buf, &msg[bytes_sent],
		sizeof(unsigned char)*(msg_len - bytes_sent) );
      }
      _send_tray_message_data (mb, buf );
      bytes_sent += 20;
   }
}

void
mb_tray_app_hide (MBTrayApp *mb )
{
  if (mb->is_hidden == False)
    {
      mb->is_hidden = True;
      if (mb->win != None)
	XDestroyWindow(mb->dpy, mb->win);
      mb->win = None;
    }
}

void
mb_tray_app_unhide (MBTrayApp *mb )
{
  if (mb->is_hidden == True)
    {
      mb->is_hidden = False;
      _init_docking (mb);
    }
}

void
mb_tray_app_main_init ( MBTrayApp *mb ) /* XXX Figure out better name */  // init + docking
{

  char tray_atom_spec[128];
  XWindowAttributes root_attr;

  if (getenv("SYSTEM_TRAY_ID"))
    mb->tray_id = atoi(getenv("SYSTEM_TRAY_ID"));

  snprintf(tray_atom_spec, 128, "_NET_SYSTEM_TRAY_S%i", mb->tray_id);

  /* XXX we should move this into one call to avoid round trips */
   mb->atoms[ATOM_SYSTEM_TRAY]
     = XInternAtom(mb->dpy, tray_atom_spec, False);

   mb->atoms[ATOM_SYSTEM_TRAY_OPCODE]
     =  XInternAtom (mb->dpy, "_NET_SYSTEM_TRAY_OPCODE", False);

   mb->atoms[ATOM_XEMBED_INFO]
      = XInternAtom(mb->dpy, "_XEMBED_INFO", False);

   mb->atoms[ATOM_XEMBED]
      = XInternAtom(mb->dpy, "_XEMBED", False);

   mb->atoms[ATOM_MANAGER]
      = XInternAtom(mb->dpy, "MANAGER", False);

   mb->atoms[ATOM_NET_SYSTEM_TRAY_MESSAGE_DATA]
     = XInternAtom(mb->dpy, "_NET_SYSTEM_TRAY_MESSAGE_DATA", False);

   mb->atoms[ATOM_NET_WM_ICON]
     = XInternAtom(mb->dpy, "_NET_WM_ICON", False);

   mb->atoms[ATOM_UTF8_STRING]
     = XInternAtom(mb->dpy, "UTF8_STRING", False);

   mb->atoms[ATOM_NET_WM_NAME]
     = XInternAtom(mb->dpy, "_NET_WM_NAME", False);

   mb->atoms[ATOM_MB_PANEL_BG]
     = XInternAtom(mb->dpy, "_MB_PANEL_BG", False);

   mb->atoms[ATOM_NET_SYSTEM_TRAY_ORIENTATION]
     = XInternAtom(mb->dpy, "_NET_SYSTEM_TRAY_ORIENTATION", False);

   mb->atoms[ATOM_NET_WM_PID]
     = XInternAtom(mb->dpy, "_NET_WM_PID", False);

   mb->atoms[ATOM_MB_THEME]
     = XInternAtom(mb->dpy, "_MB_THEME_NAME", False);

   /* XXX Protocols not need yet ...
    mb->atoms_wm_protocols[ATOM_WM_DELETE_WINDOW] =
     XInternAtom(display, "WM_DELETE_WINDOW",False);

   mb->atoms_wm_protocols[ATOM_WM_PROTOCOLS] =
     XInternAtom(display, "WM_PROTOCOLS",False),;

   mb->atoms_wm_protocols[ATOM_WM_NORMAL_HINTS] =
     XInternAtom(display, "WM_NORMAL_HINTS", False);
   */

   mb->atoms[ATOM_MB_SYSTEM_TRAY_CONTEXT]
     = XInternAtom(mb->dpy, "_MB_SYSTEM_TRAY_CONTEXT", False);

   XGetWindowAttributes(mb->dpy, mb->win_root, &root_attr);

   XSelectInput(mb->dpy, mb->win_root,
		root_attr.your_event_mask
		|PropertyChangeMask
		|StructureNotifyMask);

#if 0
   if (mb->tray_id < 0)  /* if traynum is -ive we print win to stdout and */
     {		         /* ignore the system tray protocol.              */
       fprintf(stdout, "%li\n", mb->win);
       fclose(stdout);
       /* return mb; */
       return;
     }
#endif

   /* Set up standard hints */

   XGrabServer (mb->dpy);
   mb->win_tray = XGetSelectionOwner(mb->dpy, mb->atoms[ATOM_SYSTEM_TRAY]);
   XUngrabServer (mb->dpy);
  // XFlush (mb->dpy);

   if (mb->win_tray) _init_docking (mb);

   set_theme_via_root_prop(mb);
}    // void mb_tray_app_main_init

void
mb_tray_app_main (MBTrayApp *mb )     // init + docking + event trapping
{
 // XEvent xevent;

  mb_tray_app_main_init ( mb );
  /*
   while (1)
     {
       if (get_xevent_timed(mb, &xevent))
	 mb_tray_handle_xevent(mb, &xevent);
     }
  */
}

void
mb_tray_handle_xevent(MBTrayApp *mb, XEvent *xevent)
{
  if (mb->win_tray != None && !mb->is_hidden)
    {
      if (mb->xevent_cb)
	mb->xevent_cb (mb, xevent);

      switch (xevent->type)
	{
	case KeyPress:
		/*Close the program if q is pressed.*/
		/* if (XK_q == XLookupKeysym (&xevent->xkey, 0)) {
			exit (EXIT_SUCCESS);
		} */
		break;
	case ReparentNotify:
	  TRAYDBG("%s() Reparent Notify event\n", __func__ );
	  break;
	case ConfigureNotify:
	  TRAYDBG("%s() Config Notify event\n", __func__ );
	  handle_configure(mb, &xevent->xconfigure);
	  break;
	case Expose:
	  TRAYDBG("%s() Expose event\n", __func__ );
	  handle_expose(mb, &xevent->xexpose);
		printf ("Exposed!!!!\n");
	  break;
	case ButtonPress:
	  TRAYDBG("%s() Button Press Notify event\n", __func__ );
	  handle_button(mb, &xevent->xbutton, False);
		printf ("You pressed button %d\n", xevent->xbutton.button);
		//XMapWindow(mb->dpy, w);
	  break;
	case ButtonRelease:
	  handle_button(mb, &xevent->xbutton, True);
		printf ("You released button %d\n", xevent->xbutton.button);
		  if (mb->win != None) {
		//XDestroyWindow(mb->dpy, w);
		//XWithdrawWindow(mb->dpy, w,mb->screen);
		}
	  break;
	case PropertyNotify:
	  handle_property(mb, &xevent->xproperty);
	  break;
	case ClientMessage:
	  if (xevent->xclient.message_type == mb->atoms[ATOM_XEMBED])
	    {
	      switch (xevent->xclient.data.l[1])
		{
		case XEMBED_EMBEDDED_NOTIFY:
		case XEMBED_WINDOW_ACTIVATE:
		  _map_tray_window(mb);
		  break;
		}
	    }
	  if (xevent->xclient.message_type == mb->atoms[ATOM_MB_SYSTEM_TRAY_CONTEXT])
	    {
	      if (mb->context_cb)
		mb->context_cb(mb);
	      break;
	    }

	}
    } else {
      switch (xevent->type)
	{
	case ClientMessage:
	  if (xevent->xclient.message_type == mb->atoms[ATOM_MANAGER])
	    {
	      if (xevent->xclient.data.l[1] == mb->atoms[ATOM_SYSTEM_TRAY])
		{
		  TRAYDBG("AHA!, the dock is in my sights\n");
		  XGrabServer (mb->dpy);
		  mb->win_tray = XGetSelectionOwner(mb->dpy,
						    mb->atoms[ATOM_SYSTEM_TRAY]);
		  XUngrabServer (mb->dpy);
		  XFlush (mb->dpy);
		  if (mb->win_tray && !mb->is_hidden) _init_docking (mb);
		}
	    }
	  break;
	}

      /* Handle event callback for hidden apps */
      if (mb->is_hidden && mb->xevent_cb)
	mb->xevent_cb (mb, xevent);
    }
}

Display* mb_tray_app_xdisplay (MBTrayApp *mb) { return mb->dpy; }

int mb_tray_app_xscreen (MBTrayApp *mb)  { return mb->screen; }

Window mb_tray_app_xrootwin(MBTrayApp *mb) { return mb->win_root; }

Window mb_tray_app_xwin (MBTrayApp *mb) { return mb->win; }

int mb_tray_app_width (MBTrayApp *mb) { return mb->w; }

int mb_tray_app_height (MBTrayApp *mb) { return mb->h; }

Window mb_tray_app_get_tray_win (MBTrayApp *mb) { return mb->win_tray; }
