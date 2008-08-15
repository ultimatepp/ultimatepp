
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "mbtray.h"

#include <strings.h>


#define SYSTEM_TRAY_REQUEST_DOCK    0
#define SYSTEM_TRAY_BEGIN_MESSAGE   1
#define SYSTEM_TRAY_CANCEL_MESSAGE  2


#define _NET_SYSTEM_TRAY_ORIENTATION_HORZ 0
#define _NET_SYSTEM_TRAY_ORIENTATION_VERT 1

#define MAX_SUPPORTED_XEMBED_VERSION   1

#define XEMBED_MAPPED          (1 << 0)

/* XEMBED messages */
#define XEMBED_EMBEDDED_NOTIFY          0
#define XEMBED_WINDOW_ACTIVATE          1
#define XEMBED_WINDOW_DEACTIVATE        2
#define XEMBED_REQUEST_FOCUS            3
#define XEMBED_FOCUS_IN                 4
#define XEMBED_FOCUS_OUT                5
#define XEMBED_FOCUS_NEXT               6
#define XEMBED_FOCUS_PREV               7
/* 8-9 were used for XEMBED_GRAB_KEY/XEMBED_UNGRAB_KEY */
#define XEMBED_MODALITY_ON              10
#define XEMBED_MODALITY_OFF             11
#define XEMBED_REGISTER_ACCELERATOR     12
#define XEMBED_UNREGISTER_ACCELERATOR   13
#define XEMBED_ACTIVATE_ACCELERATOR     14

enum {
  ATOM_SYSTEM_TRAY = 0,
  ATOM_SYSTEM_TRAY_OPCODE,
  ATOM_XEMBED_INFO,
  ATOM_XEMBED,
  ATOM_MANAGER,
  ATOM_NET_SYSTEM_TRAY_MESSAGE_DATA,
  ATOM_KDE_SYSTEM_TRAY,
  ATOM_MB_PANEL_BG,
  ATOM_NET_WM_ICON,
  ATOM_NET_WM_PID,
  ATOM_NET_SYSTEM_TRAY_ORIENTATION,
  ATOM_MB_THEME,
  ATOM_UTF8_STRING,
  ATOM_NET_WM_NAME,
  ATOM_MB_SYSTEM_TRAY_CONTEXT,
  TRAY_ATOMS_CNT,
};

/* Protocol Atoms */
enum {
  ATOM_WM_DELETE_WINDOW = 0,
  ATOM_WM_PROTOCOLS,
  ATOM_WM_NORMAL_HINTS,
};

#ifdef DEBUG
#define TRAYDBG(txt, args... ) \
        fprintf(stderr, "TRAY-DEBUG: " txt "\n", ##args )
#else
#define TRAYDBG(txt, args... ) /* nothing */
#endif




static int trapped_error_code = 0;
static int (*old_error_handler) (Display *d, XErrorEvent *e);

static Bool
tray_is_vertical_via_hint(MBTrayApp *mb);

/*
void
_map_tray_window (MBTrayApp *mb);
void
handle_property(MBTrayApp       *mb,
		XPropertyEvent *event );
void
handle_configure(MBTrayApp       *mb,
		 XConfigureEvent *event );
void
handle_button( MBTrayApp    *mb,
	       XButtonEvent *event ,
	       Bool          is_release);
void
handle_configure(MBTrayApp       *mb,
		 XConfigureEvent *event );
void
handle_expose( MBTrayApp       *mb,
	       XExposeEvent    *event ) ;

static void
_set_win_context_hint(MBTrayApp *mb);

*/
