#include "Notify.h"
#include <Core/Core.h>
using namespace Upp;

enum NotificationType
{
	NT_Info,
	NT_Warning,
	NT_Error
};

#if defined(PLATFORM_LINUX) && !defined(flagNOGTK)

#include <glib.h>
#include <libnotify/notify.h>

void Message(NotificationType nt, const char * title, const char * message, int timeout)
{
	if (!notify_init (title))
		return;
	NotifyNotification* notification;
	GError* error = NULL;
	 
	notification = notify_notification_new (title, message
					, nt == NT_Info ? "gtk-dialog-info"
					: nt == NT_Warning ? "gtk-dialog-warning"
					: "gtk-dialog-error", NULL);
	notify_notification_set_timeout(notification, timeout * 1000);
	notify_notification_show (notification, &error);
	notify_uninit ();
}
#else
	// #error Unsupported platform! ???
	void Message(NotificationType nt, const char * title, const char * message, int timeout) {}
#endif

void NotifyInfo(const char * title, const char * message, int timeout)
{
	Message(NT_Info, title, message, timeout);
}

void NotifyWarning(const char * title, const char * message, int timeout)
{
	Message(NT_Warning, title, message, timeout);
}

void NotifyError(const char * title, const char * message, int timeout)
{
	Message(NT_Error, title, message, timeout);
}

#if defined(flagTEST)

CONSOLE_APP_MAIN
{
	NotifyInfo("UNotify", "It display a passive pop-up notification");
	NotifyWarning("UNotify", "Watch out!");
	NotifyError("Err", "System failure!");
}

#endif
