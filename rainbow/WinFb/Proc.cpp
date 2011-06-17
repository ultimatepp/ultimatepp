#include "WinFb.h"

NAMESPACE_UPP

bool GetShift()       { return !!(GetKeyState(VK_SHIFT) & 0x8000); }
bool GetCtrl()        { return !!(GetKeyState(VK_CONTROL) & 0x8000); }
bool GetAlt()         { return !!(GetKeyState(VK_MENU) & 0x8000); }
bool GetCapsLock()    { return !!(GetKeyState(VK_CAPITAL) & 1); }
bool GetMouseLeft()   { return !!(GetKeyState(VK_LBUTTON) & 0x8000); }
bool GetMouseRight()  { return !!(GetKeyState(VK_RBUTTON) & 0x8000); }
bool GetMouseMiddle() { return !!(GetKeyState(VK_MBUTTON) & 0x8000); }

LRESULT CALLBACK fbWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GuiLock __;
//	LLOG("Ctrl::WindowProc(" << message << ") in " << ::Name(this) << ", focus " << (void *)::GetFocus());
	switch(message) {
	case WM_PAINT:
		ASSERT(hwnd);
		if(hwnd) {
			PAINTSTRUCT ps;
			HDC dc = BeginPaint(hwnd, &ps);
/*
				SystemDraw draw(dc);
	#ifndef PLATFORM_WINCE
				HPALETTE hOldPal;
				if(draw.PaletteMode() && SystemDraw::AutoPalette()) {
					hOldPal = SelectPalette(dc, GetQlibPalette(), TRUE);
					int n = RealizePalette(dc);
					LLOG("In paint realized " << n << " colors");
				}
	#endif
				painting = true;
				UpdateArea(draw, Rect(ps.rcPaint));
				painting = false;
	#ifndef PLATFORM_WINCE
				if(draw.PaletteMode() && SystemDraw::AutoPalette())
					SelectPalette(dc, hOldPal, TRUE);
	#endif
*/
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
		Ctrl::DoMouseFB(Ctrl::MOUSEWHEEL, Point((dword)lParam), (short)HIWORD(wParam));
		return 0L;
	case WM_SETCURSOR:
		SetCursor(NULL);
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
#if 0
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
				<< ", lParam = 0x" << FormatIntHex(lParam, 8)
				<< ", ignorekeyup = " << (ignorekeyup ? "true" : "false");
			LLOG(msgdump);
			dword keycode = 0;
			if(message == WM_KEYDOWN) {
				keycode = KEYtoK((dword)wParam);
				if(keycode == K_SPACE)
					keycode = 0;
			}
			else
			if(message == WM_KEYUP)
				keycode = KEYtoK((dword)wParam) | K_KEYUP;
			else
			if(message == WM_SYSKEYDOWN /*&& ((lParam & 0x20000000) || wParam == VK_F10)*/)
				keycode = KEYtoK((dword)wParam);
			else
			if(message == WM_SYSKEYUP /*&& ((lParam & 0x20000000) || wParam == VK_F10)*/)
				keycode = KEYtoK((dword)wParam) | K_KEYUP;
			else
			if(message == WM_CHAR && wParam != 127 && wParam > 32 || wParam == 32 && KEYtoK(VK_SPACE) == K_SPACE) {
#ifdef PLATFORM_WINCE
				keycode = wParam;
#else
				if(IsWindowUnicode(hwnd)) // TRC 04/10/17: ActiveX Unicode patch
					keycode = (dword)wParam;
				else {
					char b[20];
					::GetLocaleInfo(MAKELCID(LOWORD(GetKeyboardLayout(0)), SORT_DEFAULT),
					                LOCALE_IDEFAULTANSICODEPAGE, b, 20);
					int codepage = atoi(b);
					if(codepage >= 1250 && codepage <= 1258)
						keycode = ToUnicode((dword)wParam, codepage - 1250 + CHARSET_WIN1250);
					else
						keycode = (dword)wParam;
				}
#endif
			}
			bool b = false;
			if(keycode) {
				b = DispatchKey(keycode, LOWORD(lParam));
				SyncCaret();
				if(_this) PostInput();
			}
//			LOG("key processed = " << b);
			if(b || (message == WM_SYSKEYDOWN || message == WM_SYSKEYUP)
			&& wParam != VK_F4 && !PassWindowsKey((dword)wParam)) // 17.11.2003 Mirek -> invoke system menu
				return 0L;
			break;
#endif
		}
		break;
//	case WM_GETDLGCODE:
//		return wantfocus ? 0 : DLGC_STATIC;
	case WM_ERASEBKGND:
		return 1L;
	case WM_SIZE:
	case WM_MOVE:
		return 0L;
	case WM_HELP:
		return TRUE;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

END_UPP_NAMESPACE
