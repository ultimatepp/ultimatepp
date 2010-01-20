#ifndef _Notify_Notify_h_
#define _Notify_Notify_h_

void NotifyInfo(const char * title, const char * message, int timeout = 10);
void NotifyWarning(const char * title, const char * message, int timeout = 10);
void NotifyError(const char * title, const char * message, int timeout = 10);


#endif
