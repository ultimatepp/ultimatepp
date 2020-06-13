#include "SysInfo_in.h"


namespace Upp {

struct KeyCodes {
	String key; 
	int code;
}; 

#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)

Array <String> GetWinRegSubkeys(const String& key, HKEY base) {
	HKEY hkey;
	Array <String> subkeys;
	if(RegOpenKeyEx(base, key, 0, KEY_READ, &hkey) != ERROR_SUCCESS)
		return subkeys;
	char temp[_MAX_PATH];
	dword len;
	for(dword dw = 0; len = sizeof(temp), RegEnumKeyEx(hkey, dw, temp, &len, 0, 0, 0, 0) == ERROR_SUCCESS; dw++)
		subkeys.Add(temp);
	RegCloseKey(hkey);
	return subkeys;
}

void Mouse_LeftDown() 	{mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);}
void Mouse_LeftUp()		{mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);}
void Mouse_MiddleDown()	{mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);}
void Mouse_MiddleUp()	{mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);}
void Mouse_RightDown()	{mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);}
void Mouse_RightUp()	{mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);}


bool Mouse_SetPos(int xMove, int yMove, int64 windowId) {
    int left, top, right, bottom;
    
    if (windowId != 0) {
        Window_GetRect(windowId, left, top, right, bottom);
        xMove = xMove + left;
        yMove = yMove + top;
    }
    SetCursorPos(xMove, yMove);
    
    return true;
}

bool Mouse_GetPos(int &x, int &y) {
    POINT p;
    
    GetCursorPos (&p);
    x = p.x;
    y = p.y;
    
    return true;
}

KeyCodes keyCodes[60] = {
	"NUMPAD7", 	VK_NUMPAD7, 	"BACK", 	VK_BACK, 
	"NUMPAD8", 	VK_NUMPAD8,		"TAB", 		VK_TAB,
	"NUMPAD9", 	VK_NUMPAD9, 	"RETURN", 	VK_RETURN,
	"MULTIPLY", VK_MULTIPLY, 	"SHIFT",	VK_SHIFT,
	"ADD",		VK_ADD, 	 	"CONTROL",	VK_CONTROL,
	"SEPARATOR", VK_SEPARATOR,	"MENU", 	VK_MENU,
	"SUBTRACT", VK_SUBTRACT, 	"PAUSE", 	VK_PAUSE,
	"DECIMAL",	VK_DECIMAL,		"CAPITAL", 	VK_CAPITAL,
	"DIVIDE",	VK_DIVIDE, 		"ESCAPE",	VK_ESCAPE,
	"F1", 		VK_F1, 			"SPACE", 	VK_SPACE,
	"F2", 		VK_F2,	 		"END",		VK_END,
	"F3",		VK_F3, 			"HOME",		VK_HOME,
	"F4",		VK_F4, 			"LEFT", 	VK_LEFT,
	"F5",		VK_F5,		 	"UP", 		VK_UP,
	"F6",		VK_F6,		 	"RIGHT",	VK_RIGHT,
	"F7",		VK_F7,		 	"DOWN",		VK_DOWN,
	"F8",		VK_F8,		 	"PRINT",	VK_PRINT,
	"F9",		VK_F9,		 	"SNAPSHOT",	VK_SNAPSHOT,
	"F10",		VK_F10,		 	"INSERT",	VK_INSERT,
	"F11",		VK_F11,		 	"DELETE",	VK_DELETE,
	"F12",		VK_F12,		 	"LWIN",		VK_LWIN,
	"NUMLOCK",	VK_NUMLOCK,	 	"RWIN",		VK_RWIN,
	"SCROLL",	VK_SCROLL,	 	"NUMPAD0",	VK_NUMPAD0,
	"LSHIFT",	VK_LSHIFT,	 	"NUMPAD1", 	VK_NUMPAD1,
	"RSHIFT",	VK_RSHIFT,	 	"NUMPAD2",	VK_NUMPAD2,
	"LCONTROL",	VK_LCONTROL, 	"NUMPAD3",	VK_NUMPAD3,
	"RCONTROL",	VK_RCONTROL, 	"NUMPAD4",	VK_NUMPAD4,
	"LMENU",	VK_LMENU, 		"NUMPAD5",	VK_NUMPAD5,
	"RMENU",	VK_RMENU, 		"NUMPAD6",	VK_NUMPAD6,
	/*"PGUP", 	XK_Page_Up, 	"PGDOWN", 	XK_Page_Down
	"CAPSLOCK", XK_Caps_Lock, 	"BACKSPACE",XK_BackSpace	*/
	"", 0
};

void PressKeyVK(int keyVK, bool hold = false, bool release = false, bool compatible = false) {    
    long nScan, nExtended;
        
    nScan = MapVirtualKey(keyVK, 2);
    nExtended = 0;
    if (nScan == 0)
        nExtended = KEYEVENTF_EXTENDEDKEY;
    nScan = MapVirtualKey(keyVK, 0);
    
    if (compatible)
        nExtended = 0;
    
    if (!release)
        keybd_event(static_cast<BYTE>(keyVK), static_cast<BYTE>(nScan), nExtended, 0);
    
    if (!hold)
        keybd_event(static_cast<BYTE>(keyVK), static_cast<BYTE>(nScan), KEYEVENTF_KEYUP | nExtended, 0);
}


#if defined(__MINGW32__)
//	#define	MAPVK_VK_TO_VSC		0
//	#define	MAPVK_VSC_TO_VK   	1
	#ifndef MAPVK_VK_TO_CHAR
		#define	MAPVK_VK_TO_CHAR  	2
	#endif
//	#define	MAPVK_VSC_TO_VK_EX 	3
#endif
//#define	MAPVK_VK_TO_VSC_EX 	4


// This is less nice but more compatible for Notepad and MSWord for example
void PressKey(wchar key, bool hold = false, bool release = false) {
	if ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z') || (key >= '0' && key <= '9')) {
		HKL hKeyboardLayout = ::GetKeyboardLayout(0);
    	SHORT nVK = VkKeyScanExW(key, hKeyboardLayout);
    	UINT nScan = MapVirtualKeyExW(nVK, MAPVK_VK_TO_CHAR, hKeyboardLayout);
		if (!release) 
        	keybd_event(static_cast<BYTE>(nVK), static_cast<BYTE>(nScan), 0, 0);
    	if (!hold) 
        	keybd_event(static_cast<BYTE>(nVK), static_cast<BYTE>(nScan), KEYEVENTF_KEYUP, 0);
	} else {
		String numStr = FormatIntDec(key, 5, '0');
		PressKeyVK(VK_LMENU, true);
		PressKeyVK(VK_NUMPAD0 + numStr[0] - '0');
		PressKeyVK(VK_NUMPAD0 + numStr[1] - '0');
		PressKeyVK(VK_NUMPAD0 + numStr[2] - '0');
		PressKeyVK(VK_NUMPAD0 + numStr[3] - '0');
		PressKeyVK(VK_NUMPAD0 + numStr[4] - '0');
		PressKeyVK(VK_LMENU, false, true);
	}
}

bool GetKeyLockStatus(bool &caps, bool &num, bool &scroll) {
    caps = GetKeyState(VK_CAPITAL);
    num = GetKeyState(VK_NUMLOCK);
    scroll = GetKeyState(VK_SCROLL);
    return true;
}

bool SetKeyLockStatus(bool caps, bool num, bool scroll) {
	bool capsnow, numnow, scrollnow;
	
	GetKeyLockStatus(capsnow, numnow, scrollnow);
	if (capsnow != caps)
		PressKeyVK(VK_CAPITAL);
	if (numnow != num)
		PressKeyVK(VK_NUMLOCK);
	if (scrollnow != scroll)
		PressKeyVK(VK_SCROLL);
	return true;
}

#endif 

#ifdef PLATFORM_POSIX

bool Mouse_GetPos(int &x, int &y) {
	SetSysInfoX11ErrorHandler();
	_XDisplay *dpy = XOpenDisplay (NULL);
	if (!dpy) {
		SetX11ErrorHandler();
		return false; 
	}   
	bool ret = false;
	Window root, child;
	Window r = DefaultRootWindow(dpy);
	int retx, rety;
	int wx, wy;
	unsigned int keys_buttons;
   	if (XQueryPointer(dpy, r, &root, &child, &retx, &rety, &wx, &wy, &keys_buttons)) {
   		x = wx;
   		y = wy;
   		ret = true;
	} else 
		x = y = -1;
		
	XCloseDisplay (dpy);
	SetX11ErrorHandler();
	
	return ret;
}

bool Mouse_SetPos(int x, int y, int64 windowId) {
	SetSysInfoX11ErrorHandler();
	_XDisplay *dpy = XOpenDisplay (NULL);
	if (!dpy) {
		SetX11ErrorHandler();
		return false;
	}   
    int left, top, right, bottom;
    Window r = DefaultRootWindow(dpy);
    if (windowId != 0) {
        Window_GetRect(windowId, left, top, right, bottom);
        x = x + left;
        y = y + top;
    }     
	XWarpPointer(dpy, None, r, 0, 0, 0, 0, x, y);
	XCloseDisplay (dpy);
	SetX11ErrorHandler();
	
	return true;
}

#endif

// libxtst-dev
#if defined(PLATFORM_POSIX) && !defined(flagNO_XTEST)

void Mouse_FakeClick(int button, int press) {
	_XDisplay *dpy = XOpenDisplay(NULL);
	XTestFakeButtonEvent(dpy, button, press, CurrentTime);
	XFlush(dpy);
	XCloseDisplay(dpy);
}

void Mouse_LeftDown() 	{Mouse_FakeClick(1, True);}
void Mouse_LeftUp() 	{Mouse_FakeClick(1, False);}
void Mouse_MiddleDown() {Mouse_FakeClick(2, True);}
void Mouse_MiddleUp() 	{Mouse_FakeClick(2, False);}
void Mouse_RightDown() 	{Mouse_FakeClick(3, True);}
void Mouse_RightUp() 	{Mouse_FakeClick(3, False);}

void PressKeyVK(int key, _XDisplay *dpy = NULL) {
	bool local = false;
	if (!dpy) {
		if (!(dpy = XOpenDisplay(NULL)))
			return;
		local = true;
	}
	XTestFakeKeyEvent(dpy, XKeysymToKeycode(dpy, key), True, CurrentTime);
	XTestFakeKeyEvent(dpy, XKeysymToKeycode(dpy, key), False, CurrentTime);
	if (local) {
		XFlush(dpy);
		XCloseDisplay(dpy);
	}
}

void PressKey(wchar key, _XDisplay *dpy = NULL) {
	bool local = false;
	if (!dpy) {
		if (!(dpy = XOpenDisplay(NULL)))
			return;
		local = true;
	}
	//wchar k = key;
	if (key > 0x00ff)
    	key = key | 0x01000000;
 	
 	bool shift = false;
	KeyCode code = XKeysymToKeycode(dpy, key);
	if (code != 0) { 
		if (XkbKeycodeToKeysym(dpy, code, 0, 0) != key) {
			if (XkbKeycodeToKeysym(dpy, code, 1, 0) == key) 
				shift = true;
			else
				code = 0;
		}
	} else {  
		int firstKeycode, maxKeycode;
		int keysymsPerKeycode;
		
		XDisplayKeycodes(dpy, &firstKeycode, &maxKeycode);
		KeySym *keysyms = XGetKeyboardMapping(dpy, firstKeycode, maxKeycode - firstKeycode + 1, &keysymsPerKeycode);
      	int indx = (maxKeycode - firstKeycode - 1)*keysymsPerKeycode;
		keysyms[indx] = key;
      	XChangeKeyboardMapping(dpy, firstKeycode, keysymsPerKeycode, keysyms, maxKeycode-firstKeycode);
      	XSync(dpy, False);
      	code = maxKeycode-1;
      	if (XkbKeycodeToKeysym(dpy, code, 0, 0) != key) {
			if (XkbKeycodeToKeysym(dpy, code, 1, 0) == key) 
				shift = true;
		}
    }
	if (code != 0) {
		if (shift)
			XTestFakeKeyEvent(dpy, XKeysymToKeycode(dpy, XK_Shift_L), True, CurrentTime);	
		XTestFakeKeyEvent(dpy, code, True,  CurrentTime);
		XTestFakeKeyEvent(dpy, code, False, CurrentTime);
	 	if (shift)
			XTestFakeKeyEvent(dpy, XKeysymToKeycode(dpy, XK_Shift_L), False, CurrentTime);	
	} 	
	if (local) {
 		XFlush(dpy);
 		XCloseDisplay(dpy);
	}
}

bool GetKeyLockStatus0(bool &caps, bool &num, bool &scroll, _XDisplay *dpy) {
	int x, y, xx, yy;
	Window dm1, dm2;
	unsigned int sKbdState;
	
	if(!XQueryPointer(dpy, DefaultRootWindow(dpy), &dm1, &dm2, &x, &y, &xx, &yy, &sKbdState))
		return false;

	caps   = sKbdState & LockMask;
	num    = sKbdState & Mod2Mask;
	scroll = sKbdState & Mod5Mask;
	
	return true;
}

bool GetKeyLockStatus(bool &caps, bool &num, bool &scroll) {
	_XDisplay *dpy;
	if (!(dpy = XOpenDisplay(NULL)))
		return false;

	if (!GetKeyLockStatus0(caps, num, scroll, dpy)) {
		XCloseDisplay(dpy);
		return false;
	}
	XFlush(dpy);
	XCloseDisplay(dpy);
	return true;
}

bool SetKeyLockStatus(bool caps, bool num, bool scroll) {
	_XDisplay *dpy;
	if (!(dpy = XOpenDisplay(NULL)))
		return false;
			
	bool oldcaps, oldnum, oldscroll; 
	if (!GetKeyLockStatus0(oldcaps, oldnum, oldscroll, dpy)) {
		XCloseDisplay(dpy);
		return false;
	}
	if (caps != oldcaps) 
		PressKeyVK(XK_Caps_Lock, dpy);
	if (num != oldnum) 
		PressKeyVK(XK_Num_Lock, dpy);
	if (scroll != oldscroll) 
		PressKeyVK(XK_Scroll_Lock, dpy);

	XFlush(dpy);
	XCloseDisplay(dpy);
	return true;
}

KeyCodes keyCodes[] = {
	{"NUMPAD7", 	XK_KP_7},	{"BACK", 	XK_BackSpace}, 
	{"NUMPAD8", 	XK_KP_8},	{"TAB", 	XK_Tab},
	{"NUMPAD9", 	XK_KP_9}, 	{"RETURN", 	XK_Return},
	{"MULTIPLY", XK_KP_Multiply},{"SHIFT",	XK_Shift_Lock},
	{"ADD",		XK_KP_Add}, 	 {"CONTROL",XK_Control_L},
	{"SEPARATOR", XK_KP_Separator},{"MENU", XK_Super_L},
	{"SUBTRACT", XK_KP_Subtract},{"PAUSE", 	XK_Pause},
	{"DECIMAL",	XK_KP_Decimal},	/*"CAPITAL", 	VK_CAPITAL,*/
	{"DIVIDE",	XK_KP_Divide},	{"ESCAPE",	XK_Escape},
	{"F1", 		XK_F1}, 		{"SPACE",XK_KP_Space},
	{"F2", 		XK_F2},	 		{"END",		XK_End},
	{"F3",		XK_F3}, 		{"HOME",XK_Home},
	{"F4",		XK_F4}, 		{"LEFT",XK_Left},
	{"F5",		XK_F5},		 	{"UP", 		XK_Up},
	{"F6",		XK_F6},		 	{"RIGHT",	XK_Right},
	{"F7",		XK_F7},		 	{"DOWN",	XK_Down},
	{"F8",		XK_F8},		 	{"PRINT",	XK_Sys_Req},
	{"F9",		XK_F9},		 	/*"SNAPSHOT",	VK_SNAPSHOT,*/
	{"F10",		XK_F10},		{"INSERT",	XK_Insert},
	{"F11",		XK_F11},		{"DELETE",	XK_Delete},
	{"F12",		XK_F12},		{"LWIN",	XK_Meta_L},
	{"NUMLOCK",	XK_Num_Lock}, 	{"RWIN",	XK_Meta_R},
	{"SCROLL",	XK_Scroll_Lock},{"NUMPAD0",	XK_KP_0},
	{"LSHIFT",	XK_Shift_L},	{"NUMPAD1", XK_KP_1},
	{"RSHIFT",	XK_Shift_R},	{"NUMPAD2",	XK_KP_2},
	{"LCONTROL",	XK_Control_L}, 	{"NUMPAD3",	XK_KP_3},
	{"RCONTROL",	XK_Control_R}, 	{"NUMPAD4",	XK_KP_4},
	{"LMENU",	XK_Super_L},	{"NUMPAD5",	XK_KP_5},
	{"RMENU",	XK_Super_R}, 	{"NUMPAD6",	XK_KP_6},
	{"PGUP", 	XK_Page_Up}, 	{"PGDOWN", 	XK_Page_Down},
	{"CAPSLOCK", XK_Caps_Lock},	{"BACKSPACE",XK_BackSpace},
	{"",0}
};
#endif

#if defined(PLATFORM_WIN32) || !defined(flagNO_XTEST)

void Mouse_LeftClick()
{
    Mouse_LeftDown(); 
    Mouse_LeftUp(); 
}
void Mouse_RightClick()
{
    Mouse_RightDown();
    Mouse_RightUp();
}
void Mouse_MiddleClick()
{
    Mouse_MiddleDown();
    Mouse_MiddleUp();
}
void Mouse_LeftDblClick()
{
	Mouse_LeftClick();
	Mouse_LeftClick();
}
void Mouse_MiddleDblClick()
{
	Mouse_MiddleClick();
	Mouse_MiddleClick();
}
void Mouse_RightDblClick()
{
	Mouse_RightClick();
	Mouse_RightClick();
}

int GetKeyCode(String key) {
	for (int i = 0; keyCodes[i].code != 0; ++i)
		if (keyCodes[i].key == key)
			return keyCodes[i].code;
	return 0;
}

void Keyb_SendKeys(String text, long finalDelay, long delayBetweenKeys)
{
	Array <wchar> virt;
	bool inKey = false;
	String key = "";
	WString wtext(text);
	for (int i = 0; i < wtext.GetCount(); ++i) {
		bool vk = false;
		Sleep(delayBetweenKeys);
		wchar c = wtext[i];
		if (c == '{')
			inKey = true;
		else if (c == '}') {
			if (key == "{") 
				c = '{';
			else {
				c = GetKeyCode(key);
				vk = true;
			}
			inKey = false;
			key = "";
		} else if (inKey == 1)
			key.Cat(c);
		else if (c == '\n') {
			c = GetKeyCode("RETURN");
			vk = true;
		}
 		if (inKey == false) {
			if (!vk) 
 				PressKey(c);
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
			else {
				PressKeyVK(c, true);
				virt.Add(c);
			}
#endif
		}
	}
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
	for (int i = 0; i < virt.GetCount(); ++i)
		PressKeyVK(virt[i], false, true);
#endif
	Sleep(finalDelay);
}

#endif

}



