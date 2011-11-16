#include "WinFb.h"

#ifdef flagWINFB

NAMESPACE_UPP

#define LLOG(x) //DLOG(x)

bool GetShift()       { return !!(GetKeyState(VK_SHIFT) & 0x8000); }
bool GetCtrl()        { return !!(GetKeyState(VK_CONTROL) & 0x8000); }
bool GetAlt()         { return !!(GetKeyState(VK_MENU) & 0x8000); }
bool GetCapsLock()    { return !!(GetKeyState(VK_CAPITAL) & 1); }
bool GetMouseLeft()   { return !!(GetKeyState(VK_LBUTTON) & 0x8000); }
bool GetMouseRight()  { return !!(GetKeyState(VK_RBUTTON) & 0x8000); }
bool GetMouseMiddle() { return !!(GetKeyState(VK_MBUTTON) & 0x8000); }

dword fbKEYtoK(dword chr) {
	if(chr == VK_TAB)
		chr = K_TAB;
	else
	if(chr == VK_SPACE)
		chr = K_SPACE;
	else
	if(chr == VK_RETURN)
		chr = K_RETURN;
	else
		chr = chr + K_DELTA;
	if(chr == K_ALT_KEY || chr == K_CTRL_KEY || chr == K_SHIFT_KEY)
		return chr;
	if(GetCtrl()) chr |= K_CTRL;
	if(GetAlt()) chr |= K_ALT;
	if(GetShift()) chr |= K_SHIFT;
	return chr;
}

#ifdef _DEBUG

#define x_MSG(x)       { x, #x },

Tuple2<int, const char *> sWinMsg[] = {
#include <CtrlCore/Win32Msg.i>
	{0, NULL}
};

#endif

LRESULT CALLBACK fbWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GuiLock __;
#ifdef _DEBUG
	Tuple2<int, const char *> *x = FindTuple(sWinMsg, __countof(sWinMsg), message);
	if(x)
		LLOG(x->b << ", wParam: " << wParam << ", lParam: " << lParam);
#endif
	//	LLOG("Ctrl::WindowProc(" << message << ") in " << ::Name(this) << ", focus " << (void *)::GetFocus());
	switch(message) {
	case WM_PAINT:
		ASSERT(hwnd);
		if(hwnd) {
			PAINTSTRUCT ps;
			HDC dc = BeginPaint(hwnd, &ps);
			fbUpdate(dc, ps.rcPaint);
		#if 0
			Rect r = ps.rcPaint;
			::PatBlt(dc, r.left, r.top, r.GetWidth(), r.GetHeight(), DSTINVERT);
			Sleep(40);
			::PatBlt(dc, r.left, r.top, r.GetWidth(), r.GetHeight(), DSTINVERT);
		#endif
			EndPaint(hwnd, &ps);
		}
		return 0L;
	case WM_LBUTTONDOWN:
		Ctrl::DoMouseFB(Ctrl::LEFTDOWN, Point((dword)lParam));
		return 0L;
	case WM_LBUTTONUP:
		Ctrl::DoMouseFB(Ctrl::LEFTUP, Point((dword)lParam));
		return 0L;
	case WM_LBUTTONDBLCLK:
		Ctrl::DoMouseFB(Ctrl::LEFTDOUBLE, Point((dword)lParam));
		return 0L;
	case WM_RBUTTONDOWN:
		Ctrl::DoMouseFB(Ctrl::RIGHTDOWN, Point((dword)lParam));
		return 0L;
	case WM_RBUTTONUP:
		Ctrl::DoMouseFB(Ctrl::RIGHTUP, Point((dword)lParam));
		return 0L;
	case WM_RBUTTONDBLCLK:
		Ctrl::DoMouseFB(Ctrl::RIGHTDOUBLE, Point((dword)lParam));
		return 0L;
	case WM_MBUTTONDOWN:
		Ctrl::DoMouseFB(Ctrl::MIDDLEDOWN, Point((dword)lParam));
		return 0L;
	case WM_MBUTTONUP:
		Ctrl::DoMouseFB(Ctrl::MIDDLEUP, Point((dword)lParam));
		return 0L;
	case WM_MBUTTONDBLCLK:
		Ctrl::DoMouseFB(Ctrl::MIDDLEDOUBLE, Point((dword)lParam));
		return 0L;
	case WM_MOUSEMOVE:
		Ctrl::DoMouseFB(Ctrl::MOUSEMOVE, Point((dword)lParam));
		return 0L;
	case 0x20a: // WM_MOUSEWHEEL:
		{
			Point p(0, 0);
			::ClientToScreen(hwnd, p);
			Ctrl::DoMouseFB(Ctrl::MOUSEWHEEL, Point((dword)lParam) - p, (short)HIWORD(wParam));
		}
		return 0L;
	case WM_SETCURSOR:
		if(LOWORD((dword)lParam) == HTCLIENT) {
			SetCursor(NULL);
			return TRUE;
		}
		break;
//	case WM_MENUCHAR:
//		return MAKELONG(0, MNC_SELECT);
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_CHAR:
//		ignorekeyup = false;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		{
			String msgdump;
			switch(message)
			{
			case WM_KEYDOWN:    msgdump << "WM_KEYDOWN"; break;
			case WM_KEYUP:      msgdump << "WM_KEYUP"; break;
			case WM_SYSKEYDOWN: msgdump << "WM_SYSKEYDOWN"; break;
			case WM_SYSKEYUP:   msgdump << "WM_SYSKEYUP"; break;
			case WM_CHAR:       msgdump << "WM_CHAR"; break;
			}
			msgdump << " wParam = 0x" << FormatIntHex(wParam, 8)
				<< ", lParam = 0x" << FormatIntHex(lParam, 8);
			LLOG(msgdump);
			dword keycode = 0;
			if(message == WM_KEYDOWN) {
				keycode = fbKEYtoK((dword)wParam);
				if(keycode == K_SPACE)
					keycode = 0;
			}
			else
			if(message == WM_KEYUP)
				keycode = fbKEYtoK((dword)wParam) | K_KEYUP;
			else
			if(message == WM_SYSKEYDOWN /*&& ((lParam & 0x20000000) || wParam == VK_F10)*/)
				keycode = fbKEYtoK((dword)wParam);
			else
			if(message == WM_SYSKEYUP /*&& ((lParam & 0x20000000) || wParam == VK_F10)*/)
				keycode = fbKEYtoK((dword)wParam) | K_KEYUP;
			else
			if(message == WM_CHAR && wParam != 127 && wParam > 32 || wParam == 32 && fbKEYtoK(VK_SPACE) == K_SPACE)
				keycode = (dword)wParam;
			bool b = false;
			if(keycode)
				b = Ctrl::DoKeyFB(keycode, LOWORD(lParam));
//			LLOG("key processed = " << b);
//			if(b || (message == WM_SYSKEYDOWN || message == WM_SYSKEYUP)
//			&& wParam != VK_F4 && !PassWindowsKey((dword)wParam)) // 17.11.2003 Mirek -> invoke system menu
//				return 0L;
			break;
		}
		break;
//	case WM_GETDLGCODE:
//		return wantfocus ? 0 : DLGC_STATIC;
	case WM_ERASEBKGND:
		return 1L;
	case WM_SIZE:
		Ctrl::SetFramebufferSize(Size(LOWORD(lParam), HIWORD(lParam)));
		return 0L;
	case WM_HELP:
		return TRUE;
	case WM_CLOSE:
		Ctrl::EndSession();
		return 0L;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

END_UPP_NAMESPACE

#endif
