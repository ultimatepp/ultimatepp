#ifndef _UltimatePlayer_TaskbarButton_h_
#define _UltimatePlayer_TaskbarButton_h_

#include <ShellAPI.h>

enum
{
    UM_TASKBAR = WM_USER + 1024,
};

class TaskbarButton : public Ctrl
{
		Image icon;
	public:
	    typedef TaskbarButton CLASSNAME;
	
	    TaskbarButton();
	    ~TaskbarButton();
	    
	    
	
	    TaskbarButton&  Icon(const Image &img);
	    TaskbarButton&  Tip(const char *text);	
	    void			Init(HWND hwnd);
			
	public:
	    Callback        WhenDouble;
	    Callback        WhenLeftUp;
	    Callback        WhenLeftDouble;
	    Callback1<Bar&> WhenBar;
	    
	    void PassNotification(UINT lParam);
	
	protected:
	    void Flush(dword msg);
	

	protected:
	    String         tip;
	    NOTIFYICONDATA nid;
};


#endif
