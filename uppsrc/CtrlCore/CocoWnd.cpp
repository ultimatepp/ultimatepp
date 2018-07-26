#include <CtrlCore/CtrlCore.h>

#ifdef PLATFORM_COCOA

NAMESPACE_UPP

#define LLOG(x)   //   LOG(x)
#define LOGTIMING 0

#ifdef _DEBUG
#define LOGMESSAGES 0
#endif

#define ELOGW(x)  // RLOG(GetSysTime() << ": " << x) // Only activate in MT!
#define ELOG(x)   // RLOG(GetSysTime() << ": " << x)

bool Ctrl::IsAlphaSupported()
{
	return false;
}

// Vector<Callback> Ctrl::hotkey;

int Ctrl::RegisterSystemHotKey(dword key, Function<void ()> cb)
{
/*	ASSERT(key >= K_DELTA);
	int q = hotkey.GetCount();
	for(int i = 0; i < hotkey.GetCount(); i++)
		if(!hotkey[i]) {
			q = i;
			break;
		}
	hotkey.At(q) = cb;
	dword mod = 0;
	if(key & K_ALT)
		mod |= MOD_ALT;
	if(key & K_SHIFT)
		mod |= MOD_SHIFT;
	if(key & K_CTRL)
		mod |= MOD_CONTROL;
	
	return RegisterHotKey(NULL, q, mod, key & 0xffff) ? q : -1;*/
	return -1;
}

void Ctrl::UnregisterSystemHotKey(int id)
{
/*	if(id >= 0 && id < hotkey.GetCount()) {
		UnregisterHotKey(NULL, id);
		hotkey[id].Clear();
	}*/
}

Rect Ctrl::GetWndScreenRect() const
{
	GuiLock __;
	Rect r;
	return r;
}

void Ctrl::WndShow(bool b)
{
	GuiLock __;
}

void Ctrl::WndUpdate()
{
	GuiLock __;
}

void Ctrl::SetAlpha(byte alpha)
{
	GuiLock __;
}

int Ctrl::GetKbdDelay()
{
	GuiLock __;  
	return 500;
}

int Ctrl::GetKbdSpeed()
{
	GuiLock __;
	return 1000 / 32;
}

void Ctrl::WndSetPos(const Rect& rect)
{
	GuiLock __;
}

void Ctrl::WndUpdate(const Rect& r)
{
	GuiLock __;

}

void  Ctrl::WndScrollView(const Rect& r, int dx, int dy)
{
	GuiLock __;
}

ViewDraw::ViewDraw(Ctrl *ctrl)
{
	EnterGuiMutex();
}

ViewDraw::~ViewDraw()
{
	LeaveGuiMutex();
}

Vector<WString> SplitCmdLine__(const char *cmd)
{
	Vector<WString> out;
	while(*cmd)
		if((byte)*cmd <= ' ')
			cmd++;
		else if(*cmd == '\"') {
			WString quoted;
			while(*++cmd && (*cmd != '\"' || *++cmd == '\"'))
				quoted.Cat(FromSystemCharset(String(cmd, 1)).ToWString());
			out.Add(quoted);
		}
		else {
			const char *begin = cmd;
			while((byte)*cmd > ' ')
				cmd++;
			out.Add(String(begin, cmd).ToWString());
		}
	return out;
}

void Ctrl::SysEndLoop()
{
}

void FinishDragRect(Ctrl& q)
{
}

END_UPP_NAMESPACE

#endif
