#include "CtrlCore.h"

#ifdef GUI_WIN

#include <winnls.h>

#include <imm.h>

namespace Upp {

void XpClear(); // Clear cached windows theme handles

#define LLOG(x)  // DLOG(x)

dword Ctrl::KEYtoK(dword chr) {
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

class NilDrawFull : public NilDraw {
	virtual bool IsPaintingOp(const Rect& r) const { return true; }
};

void AvoidPaintingCheck__()
{
	Ctrl::painting = false;
}

dword GetKeyStateSafe(dword what) {
	bool h = Ctrl::painting;
	Ctrl::painting = false;
	dword r = GetKeyState(what);
	Ctrl::painting = h;
	return r;
}

static bool pendown = false;

bool GetShift()       { return !!(GetKeyStateSafe(VK_SHIFT) & 0x8000); }
bool GetCtrl()        { return !!(GetKeyStateSafe(VK_CONTROL) & 0x8000); }
bool GetAlt()         { return !!(GetKeyStateSafe(VK_MENU) & 0x8000); }
bool GetCapsLock()    { return !!(GetKeyStateSafe(VK_CAPITAL) & 1); }
bool GetMouseLeft()   { return pendown || !!(GetKeyStateSafe(VK_LBUTTON) & 0x8000); }
bool GetMouseRight()  { return Ctrl::GetPenInfo().barrel || !!(GetKeyStateSafe(VK_RBUTTON) & 0x8000); }
bool GetMouseMiddle() { return !!(GetKeyStateSafe(VK_MBUTTON) & 0x8000); }

Point Ctrl::CurrentMousePos;

Point GetMousePos() {
	return Ctrl::CurrentMousePos;
}

bool PassWindowsKey(int wParam);

void Ctrl::DoCancelPreedit()
{
	if(!focusCtrlWnd)
		return;
	Top *top = focusCtrl->GetTop();
	if(top)
		focusCtrl->HidePreedit();
	if(top && top->hwnd) {
		HIMC himc = ImmGetContext(top->hwnd);
		if(himc && ImmGetOpenStatus(himc)) {
			ImmNotifyIME(himc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
			ImmReleaseContext(top->hwnd, himc);
		}
	}
}

/* Windows Custom Titlebar metrics

UHD

r = [0, 0] - [2854, 60] : (2854, 60)
button_width = 94

HD

r = [0, 0] - [1424, 31] : (1424, 31)
button_width = 47

Light

Bar     238 244 249
Min/max 230,234,239
close   Color(196, 43, 28)

Dark

bar     26,34,39
min/max 39,47,52
close  232,17,35

*/

int Ctrl::GetWin32TitleBarHeight(const TopWindow *tw)
{
	return max(tw->custom_titlebar_cy, IsUHDMode() ? 60 : 31);
}

int Ctrl::GetWin32TitleBarButtonWidth()
{
	return IsUHDMode() ? 94 : 47;
}

Rect Ctrl::GetTitleBarRect(const TopWindow *win) // TODO (image, cy)
{
	RECT rect;
    GetClientRect(win->GetHWND(), &rect);
    rect.top = 0;
    rect.bottom = GetWin32TitleBarHeight(win);
    return rect;
}

bool Ctrl::IsMaximized(HWND hwnd)
{
  WINDOWPLACEMENT placement = {0};
  placement.length = sizeof(WINDOWPLACEMENT);
  return GetWindowPlacement(hwnd, &placement) && placement.showCmd == SW_SHOWMAXIMIZED;
}

void Ctrl::PaintWinBarBackground(SystemDraw& w, const Rect& clip)
{
	bool custom_titlebar = false;
	auto topwin = dynamic_cast<TopWindow *>(this);
	HWND hwnd = GetHWND();
	if(topwin && topwin->IsCustomTitleBar() && hwnd) {
		Rect r = GetTitleBarRect(topwin);
		w.DrawRect(r, IsDarkTheme() ? Color(26, 34, 39) : Color(238, 244, 249));
	}
}

void Ctrl::PaintWinBar(SystemDraw& w, const Rect& clip)
{
	bool custom_titlebar = false;
	auto topwin = dynamic_cast<TopWindow *>(this);
	HWND hwnd = GetHWND();
	if(topwin && topwin->IsCustomTitleBar() && hwnd) {
		Rect r = GetTitleBarRect(topwin);
		int height = r.GetHeight();
		bool maximized = IsMaximized(hwnd);
//		draw.DrawRect(r, Yellow()); // uncomment for testing...
		int button_width = GetWin32TitleBarButtonWidth();
		
		int x = r.right;
		int n = 0;
		auto Drw = [&](const Image& m) {
			x -= button_width;
			Color ink = topwin->HasFocusDeep() ? SBlack() : Gray();
			if(topwin->active_titlebar_button == n) {
				w.DrawRect(x, r.top, button_width, height,
				           IsDarkTheme() ? n ? Color(39, 47, 52) : Color(232, 17, 35)
				                         : n ? Color(230, 234, 239) : Color(196, 43, 28));
				if(n == 0)
					ink = SWhite();
			}
			w.DrawImage(x + (button_width - m.GetWidth()) / 2, r.top + (height - m.GetHeight()) / 2, m, ink);
			n++;
		};
		Drw(CtrlCoreImg::WinClose());
		if(topwin->IsZoomable()) {
			Drw(maximized ? CtrlCoreImg::WinMaximized() : CtrlCoreImg::WinMaximize());
			Drw(CtrlCoreImg::WinMinimize());
		}

		Image icon = topwin->GetIcon();
		if(!IsNull(icon)) {
			if(max(icon.GetHeight(), icon.GetWidth()) > 32)
				icon = CachedRescale(icon, Size(32, 32));
			int ix = DPI(maximized ? 2 : 4);
			if(ix + icon.GetWidth() <= x)
				w.DrawImage(ix, (height - icon.GetHeight()) / 2, icon);
		}
	
		// TODO: Fake shadow rect?
	}
}

void Ctrl::SyncCustomTitleBars()
{
	for(Ctrl *q : GetTopWindows()) {
		auto topwin = dynamic_cast<TopWindow *>(q);
		if(topwin && topwin->IsCustomTitleBar() && topwin->active_titlebar_button >= 0) {
			HWND hwnd = topwin->GetHWND();
			if(hwnd) {
				Point p;
				::GetCursorPos(p);
				if(!topwin->GetScreenRect().Contains(p))
					InvalidateRect(hwnd, GetTitleBarRect(topwin), FALSE);
			}
		}
	}
}

LRESULT Ctrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
	GuiLock __;
	eventid++;
//	LLOG("Ctrl::WindowProc(" << message << ") in " << ::Name(this) << ", focus " << (void *)::GetFocus());
	Ptr<Ctrl> _this = this;
	HWND hwnd = GetHWND();

	POINT p;
	if(::GetCursorPos(&p))
		CurrentMousePos = p;
	
	auto topwin = dynamic_cast<TopWindow *>(this);
	bool custom_titlebar = topwin && topwin->IsCustomTitleBar();

	Rect titlebar_rect = Null;
	int titlebar_button_width = 0;
	int active_titlebar_button = -1;
	bool active_titlebar_active = false;

	if(custom_titlebar) {
		titlebar_rect = GetTitleBarRect(topwin);
		titlebar_button_width = GetWin32TitleBarButtonWidth();
		
		Point p = CurrentMousePos;
		ScreenToClient(hwnd, p);
		if(titlebar_rect.Contains(p)) {
			int q = (titlebar_rect.right - p.x) / titlebar_button_width;
			active_titlebar_button = q >= 0 && q < (topwin->IsZoomable() ? 3 : 1) ? q : -1;
		}
		
		active_titlebar_active = topwin->HasFocusDeep();
		
		if(findarg(message, WM_PAINT, WM_NCPAINT) < 0 &&
		   active_titlebar_button != topwin->active_titlebar_button ||
		   active_titlebar_active != topwin->active_titlebar_active) {
			topwin->active_titlebar_button = active_titlebar_button;
			topwin->active_titlebar_active = active_titlebar_active;
			InvalidateRect(hwnd, titlebar_rect, FALSE);
		}
	}

	cancel_preedit = DoCancelPreedit; // We really need this just once, but whatever..

	is_pen_event = (GetMessageExtraInfo() & 0xFFFFFF80) == 0xFF515700; // https://learn.microsoft.com/en-us/windows/win32/tablet/system-events-and-mouse-messages?redirectedfrom=MSDN
	
	auto MousePos = [&] {
		Point p = Point((dword)lParam);
		CurrentMousePos = p;
		::ClientToScreen(hwnd, CurrentMousePos);
		return p;
	};
	
	bool has_preedit = HasFocusDeep() && focusCtrl && !IsNull(focusCtrl->GetPreedit());

	auto StopPreedit = [&] {
		HidePreedit();
		if(HasFocusDeep())
			CancelPreedit();
	};
	
	auto ClickActivate = [&] {
		ClickActivateWnd();
		StopPreedit();
	};

	auto SysMenu = [&] {
		if(!custom_titlebar)
			return;
        BOOL isMaximized = IsMaximized(hwnd);
        MENUITEMINFO menu_item_info;
        menu_item_info.cbSize = sizeof(menu_item_info);
        menu_item_info.fMask = MIIM_STATE;
        HMENU sys_menu = GetSystemMenu(hwnd, false);
        
		auto Menu = [&](UINT item, bool enabled) {
			menu_item_info.fState = enabled ? MF_ENABLED : MF_DISABLED;
			SetMenuItemInfo(sys_menu, item, false, &menu_item_info);
		};
        
        Menu(SC_RESTORE, isMaximized);
        Menu(SC_MOVE, !isMaximized);
        Menu(SC_SIZE, !isMaximized);
        Menu(SC_MINIMIZE, true);
        Menu(SC_MAXIMIZE, !isMaximized);
        Menu(SC_CLOSE, true);

		Point p((LONG)lParam);
        BOOL result = TrackPopupMenu(sys_menu, TPM_RETURNCMD, p.x, p.y, 0, hwnd, NULL);
        if(result)
			PostMessage(hwnd, WM_SYSCOMMAND, result, 0);
	};

	switch(message) {
	case WM_POINTERDOWN:
	case WM_POINTERUPDATE:
	case WM_POINTERUP: {
			POINT p = Point((LONG)lParam);
			CurrentMousePos = p;
			ScreenToClient(hwnd, &p);
			
			pen.action = 0;
			pen.pressure = pen.rotation = Null;
			pen.tilt = Null;
			pen.eraser = pen.barrel = pen.inverted = pen.history = false;

			static BOOL (WINAPI *GetPointerType)(UINT32 pointerId, POINTER_INPUT_TYPE *pointerType);
			static BOOL (WINAPI *GetPointerInfo)(UINT32 pointerId, POINTER_INFO *pointerInfo);
			static BOOL (WINAPI *GetPointerPenInfo)(UINT32 pointerId, POINTER_PEN_INFO *penInfo);
			static BOOL (WINAPI *GetPointerPenInfoHistory)(UINT32 pointerId, UINT32 *entriesCount, POINTER_PEN_INFO *penInfo);
		
			ONCELOCK {
				DllFn(GetPointerType, "User32.dll", "GetPointerType");
				DllFn(GetPointerInfo, "User32.dll", "GetPointerInfo");
				DllFn(GetPointerPenInfo, "User32.dll", "GetPointerPenInfo");
				DllFn(GetPointerPenInfoHistory, "User32.dll", "GetPointerPenInfoHistory");
			};
		
			if(!(GetPointerType && GetPointerInfo && GetPointerPenInfo && GetPointerPenInfoHistory))
				break;

			POINTER_INPUT_TYPE pointerType;

			auto ProcessPenInfo = [&](POINTER_PEN_INFO& ppi) {
				if(ppi.penFlags & PEN_FLAG_BARREL)
					pen.barrel = true;
				if(ppi.penFlags & PEN_FLAG_INVERTED)
					pen.inverted = true;
				if(ppi.penFlags & PEN_FLAG_ERASER)
					pen.eraser = true;
				if(ppi.penMask & PEN_MASK_PRESSURE)
					pen.pressure = ppi.pressure / 1024.0;
				if(ppi.penMask & PEN_MASK_ROTATION)
					pen.rotation = ppi.rotation * M_2PI / 360;
				if(ppi.penMask & PEN_MASK_TILT_X)
					pen.tilt.x = ppi.tiltX * M_2PI / 360;
				if(ppi.penMask & PEN_MASK_TILT_Y)
					pen.tilt.y = ppi.tiltY * M_2PI / 360;
			};
			
			auto DoPen = [&](Point p) {
				GuiLock __;
				eventCtrl = this;
				Ctrl *q = this;
				if(captureCtrl){
					q = captureCtrl;
					p += GetScreenRect().TopLeft()-captureCtrl->GetScreenRect().TopLeft();
				}
				else
					for(Ctrl *t = q; t; t=q->ChildFromPoint(p)) q = t;
				
				q->Pen(p, pen, GetMouseFlags());
				SyncCaret();
				Image m = CursorOverride();
				if(IsNull(m)) SetMouseCursor(q->CursorImage(p,GetMouseFlags()));
				else SetMouseCursor(m);
			};

			UINT32 pointerId = GET_POINTERID_WPARAM(wParam);
			if(GetPointerType(pointerId, &pointerType) && pointerType == PT_PEN) {
				UINT32 hc = 256;
				Buffer<POINTER_PEN_INFO> ppit(hc);
				if(message == WM_POINTERUPDATE && GetPointerPenInfoHistory(pointerId, &hc, ppit)) {
					for(int i = hc - 1; i >= 0; i--) {
						static Point lastp=Null;
						ProcessPenInfo(ppit[i]);
						pen.history = (bool)i;
						POINT hp = ppit[i].pointerInfo.ptPixelLocation;
						if(!pen.history || hp!=lastp){
							lastp = CurrentMousePos = hp;
							ScreenToClient(hwnd, &hp);
							DoPen(hp);
						}
					}
					break;
				}
				POINTER_PEN_INFO ppi;
				if(GetPointerPenInfo(pointerId, &ppi))
					ProcessPenInfo(ppi);
				switch(message) {
				case WM_POINTERDOWN:
					pendown=true;
					pen.action = PEN_DOWN;
					ClickActivate();
					break;
				case WM_POINTERUP:
					pendown=false;
					pen.action = PEN_UP;
					break;
				}
				DoPen(p);
				break;
			}
		}
		break;
	case WM_ERASEBKGND:
		if(erasebg) {
			HDC hdc = (HDC)(wParam);
			RECT rc; GetClientRect(hwnd, &rc);
			Color c = SColorFace();
			HBRUSH brush = CreateSolidBrush(RGB(c.GetR(), c.GetG(), c.GetB()));
			FillRect(hdc, &rc, brush);
	        DeleteObject(brush);
	        erasebg = false;
		}
		return 1L;
	case WM_PAINT:
		ASSERT_(!painting || IsPanicMode(), "WM_PAINT invoked for " + Name() + " while in Paint routine");
		ASSERT(hwnd);
		if(hwnd) {
			PAINTSTRUCT ps;
			if(IsVisible())
				SyncScroll();
			HDC dc = BeginPaint(hwnd, &ps);
			fullrefresh = false;
			if(IsVisible()) {
				SystemDraw draw(dc);
				painting = true;
				UpdateArea(draw, Rect(ps.rcPaint));
				painting = false;
			}
			EndPaint(hwnd, &ps);

			UpdateDHCtrls(); // so that they are displayed withing the same WM_PAINT - looks better
		}
		return 0L;
    case WM_NCCALCSIZE:
		if(custom_titlebar && wParam) { // custom titlebar based on https://github.com/grassator/win32-window-custom-titlebar/blob/main/main.c
			UINT dpi = GetDpiForWindow(hwnd);
			
			int frame_x = GetSystemMetricsForDpi(SM_CXFRAME, dpi);
			int frame_y = GetSystemMetricsForDpi(SM_CYFRAME, dpi);
			int padding = GetSystemMetricsForDpi(92 /*SM_CXPADDEDBORDER*/, dpi);
			
			NCCALCSIZE_PARAMS *params = (NCCALCSIZE_PARAMS *)lParam;
			RECT *requested_client_rect = params->rgrc;
			
			requested_client_rect->right -= frame_x + padding;
			requested_client_rect->left += frame_x + padding;
			requested_client_rect->bottom -= frame_y + padding;
			
			if(IsMaximized(hwnd))
				requested_client_rect->top += padding;
			
			return 0;
		}
		break;
    case WM_NCHITTEST:
		CheckMouseCtrl();
		if(ignoremouse) return HTTRANSPARENT;
		if(custom_titlebar) {
			LRESULT hit = DefWindowProc(hwnd, message, wParam, lParam);
			if(findarg(hit, HTNOWHERE, HTRIGHT, HTLEFT, HTTOPLEFT, HTTOP,
			                HTTOPRIGHT, HTBOTTOMRIGHT, HTBOTTOM, HTBOTTOMLEFT) >= 0)
				return hit;
			if(active_titlebar_button == 1)
				return HTMAXBUTTON;
			// Looks like adjustment happening in NCCALCSIZE is messing with the detection
			// of the top hit area so manually fixing that.
			bool maximized = IsMaximized(hwnd);
			UINT dpi = GetDpiForWindow(hwnd);
			int frame_y = GetSystemMetricsForDpi(SM_CYFRAME, dpi);
			int padding = GetSystemMetricsForDpi(92 /*SM_CXPADDEDBORDER*/, dpi);
			Point p((LONG)lParam);
			ScreenToClient(hwnd, p);
			// We should not return HTTOP when hit-testing a maximized window 
			if(!IsMaximized(hwnd) && p.y > 0 && p.y < frame_y + padding && topwin->IsSizeable())
				return HTTOP;
			
			// Since we are drawing our own caption, this needs to be a custom test
			auto cm = topwin->GetCustomTitleBarMetrics();
			if(p.y < GetWin32TitleBarHeight(topwin) && (p.x < cm.lm || p.x >= titlebar_rect.right - cm.rm || topwin->IsCustomTitleBarDragArea(p)))
				return HTCAPTION;
			
			return HTCLIENT;
		}
		break;
	case WM_NCLBUTTONDOWN:
		ClickActivate();
		IgnoreMouseUp();
		if(active_titlebar_button >= 0)
			return 0;

		if(custom_titlebar) {
			Point p((LONG)lParam);
			ScreenToClient(hwnd, p);
			
			// Since we are drawing our own caption, this needs to be a custom test
			int height = GetWin32TitleBarHeight(topwin);
			if(p.y < height) {
				Image icon = topwin->GetIcon();
				if(!IsNull(icon)) {
					int lm = DPI(IsMaximized(hwnd) ? 2 : 4);
					if(p.x >= lm && p.x < min(icon.GetWidth(), height))
						SysMenu();
				}
			}
		}
		break;
    case WM_NCLBUTTONUP: // Map button clicks to the right messages for the window
        switch(active_titlebar_button) {
        case 0:
			PostMessageW(hwnd, WM_CLOSE, 0, 0);
			return 0;
		case 1:
			ShowWindow(hwnd, IsMaximized(hwnd) ? SW_NORMAL : SW_MAXIMIZE);
			return 0;
		case 2:
			ShowWindow(hwnd, SW_MINIMIZE);
			return 0;
        }
		break;
    case WM_NCRBUTTONUP:
		if(wParam == HTCAPTION)
			SysMenu();
		break;
	case WM_LBUTTONDOWN:
		ClickActivate();
		if(ignoreclick) return 0L;
		DoMouse(LEFTDOWN, MousePos(), 0);
		if(_this) PostInput();
		return 0L;
	case WM_LBUTTONUP:
		if(ignoreclick)
			EndIgnore();
		else
			DoMouse(LEFTUP, MousePos(), 0);
		if(_this) PostInput();
		return 0L;
	case WM_LBUTTONDBLCLK:
		ClickActivate();
		if(ignoreclick) return 0L;
		DoMouse(LEFTDOUBLE, MousePos(), 0);
		if(_this) PostInput();
		return 0L;
	case WM_RBUTTONDOWN:
		ClickActivate();
		if(ignoreclick) return 0L;
		DoMouse(RIGHTDOWN, MousePos());
		if(_this) PostInput();
		return 0L;
	case WM_RBUTTONUP:
		if(ignoreclick)
			EndIgnore();
		else
			DoMouse(RIGHTUP, MousePos());
		if(_this) PostInput();
		return 0L;
	case WM_RBUTTONDBLCLK:
		ClickActivate();
		if(ignoreclick) return 0L;
		DoMouse(RIGHTDOUBLE, MousePos());
		if(_this) PostInput();
		return 0L;
	case WM_MBUTTONDOWN:
		ClickActivate();
		if(ignoreclick) return 0L;
		DoMouse(MIDDLEDOWN, MousePos());
		if(_this) PostInput();
		return 0L;
	case WM_MBUTTONUP:
		if(ignoreclick)
			EndIgnore();
		else
			DoMouse(MIDDLEUP, MousePos());
		if(_this) PostInput();
		return 0L;
	case WM_MBUTTONDBLCLK:
		ClickActivate();
		if(ignoreclick) return 0L;
		DoMouse(MIDDLEDOUBLE, MousePos());
		if(_this) PostInput();
		return 0L;
	case WM_NCRBUTTONDOWN:
	case WM_NCMBUTTONDOWN:
		ClickActivate();
		IgnoreMouseUp();
		break;
	case WM_MOUSEMOVE:
		LLOG("WM_MOUSEMOVE: ignoreclick = " << ignoreclick);
		if(ignoreclick)
			EndIgnore();
		else {
			if(_this) DoMouse(MOUSEMOVE, MousePos());
			DoCursorShape();
		}
		return 0L;
	case 0x20a: // WM_MOUSEWHEEL:
	case 0x20e: // WM_MOUSEHWHEEL:
		if(ignoreclick) {
			EndIgnore();
			return 0L;
		}
		if(_this) {
			Point p(0, 0);
			::ClientToScreen(hwnd, p);
			DoMouse(message == 0x20e ? MOUSEHWHEEL : MOUSEWHEEL, Point((dword)lParam) - p, (short)HIWORD(wParam));
			CurrentMousePos = Point((dword)lParam);
		}
		if(_this) PostInput();
		return 0L;
	case WM_SETCURSOR:
		if((HWND)wParam == hwnd && LOWORD((dword)lParam) == HTCLIENT) {
			if(hCursor) SetCursor(hCursor);
			return TRUE;
		}
		break;
//	case WM_MENUCHAR:
//		return MAKELONG(0, MNC_SELECT);
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_CHAR:
		ignorekeyup = false;
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
#endif
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
				if(IsWindowUnicode(hwnd)) { // TRC 04/10/17: ActiveX Unicode patch
					static WCHAR surr[2];
					keycode = (dword)wParam;
					if((keycode & 0XFC00) == 0xD800) { // covert UTF16 surrogate pair to UTF32 codepoint
						surr[0] = (WCHAR)keycode;
						return 0L;
					}
					if((keycode & 0xFC00) == 0xDC00) {
						surr[1] = (WCHAR)keycode;
						keycode = ReadSurrogatePair(surr, surr + 2);
						surr[0] = 0;
						if(!keycode)
							return 0L;
					}
					else
						surr[0] = 0;
				}
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
		}
		break;
//	case WM_GETDLGCODE:
//		return wantfocus ? 0 : DLGC_STATIC;
    case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
		if(has_preedit) {
			HIMC himc = ImmGetContext(GetHWND());
			if(!himc)
				break;
			Rect pr = GetPreeditScreenRect();
			Point p = pr.TopLeft() - GetScreenRect().TopLeft();
			CANDIDATEFORM cf;
			cf.dwIndex = 0;
			cf.dwStyle = CFS_EXCLUDE;
			cf.ptCurrentPos.x = p.x;
			cf.ptCurrentPos.y = p.y;
			cf.rcArea.left    = p.x;
			cf.rcArea.top     = p.y;
			cf.rcArea.right   = p.x + DPI(20); // DPI(20) is sort of hack, but candidate windows are above or bellow anyway...
			cf.rcArea.bottom  = p.y + pr.GetHeight();
			ImmSetCandidateWindow(himc, &cf);
		/*  // todo: SetCaretPos too
			Rect r;
			::CreateCaret(hwnd, NULL, 1, pr.Height());
			::ShowCaret(hwnd);
			::SetCaretPos(p.x, p.y);
		*/	
			auto ReadString = [&](int type) -> WString {
				int len = ImmGetCompositionStringW (himc, type, NULL, 0);
				if(len > 0) {
					Buffer<char16> sw(len / 2);
					ImmGetCompositionStringW(himc, type, sw, len);
					return ToUtf32(sw, len / 2);
				}
				return Null;
			};
			if(lParam & GCS_COMPSTR) {
				ShowPreedit(ReadString(GCS_COMPSTR), ImmGetCompositionString(himc, GCS_CURSORPOS, 0, 0));
			}
			if(lParam & GCS_RESULTSTR) {
				WString h = ReadString(GCS_RESULTSTR);
				for(wchar c : h)
					DispatchKey(c, 1);
				HidePreedit();
				SyncCaret();
			}
			ImmReleaseContext(GetHWND(), himc);
			return 0L;
		}
		break;
	case WM_IME_ENDCOMPOSITION:
		if(has_preedit) {
			HidePreedit();
			return 0L;
		}
		break;
	case WM_XBUTTONDOWN: {
		UINT button = GET_XBUTTON_WPARAM(wParam);
		if(button == XBUTTON2)
			DispatchKey(K_MOUSE_FORWARD, 1);
		if(button == XBUTTON1)
			DispatchKey(K_MOUSE_BACKWARD, 1);
		return 0L;
	}
	case WM_XBUTTONUP: {
		UINT button = GET_XBUTTON_WPARAM(wParam);
		if(button == XBUTTON2)
			DispatchKey(K_MOUSE_FORWARD|K_KEYUP, 1);
		if(button == XBUTTON1)
			DispatchKey(K_MOUSE_BACKWARD|K_KEYUP, 1);
		return 0L;
	}
	case WM_DESTROY:
		PreDestroy();
		break;
	case WM_NCDESTROY:
		if(!hwnd) break;
		if(HasChildDeep(mouseCtrl) || this == ~mouseCtrl) mouseCtrl = NULL;
		if(HasChildDeep(focusCtrl) || this == ~focusCtrl) focusCtrl = NULL;
		if(HasChildDeep(focusCtrlWnd) || this == ~focusCtrlWnd) {
			LLOG("WM_NCDESTROY: clearing focusCtrlWnd = " << UPP::Name(focusCtrlWnd));
			focusCtrlWnd = NULL;
			focusCtrl = NULL;
		}
		if(::GetFocus() == NULL) {
			Ctrl *owner = GetOwner();
			if(owner && (owner->IsForeground() || IsForeground()) && !owner->SetWantFocus())
				IterateFocusForward(owner, owner);
		}
		if(IsWindowUnicode(hwnd))
			DefWindowProcW(hwnd, message, wParam, lParam);
		else
			DefWindowProc(hwnd, message, wParam, lParam);
		hwnd = NULL;
		return 0L;
	case WM_CANCELMODE:
		if(this == ~captureCtrl || HasChildDeep(captureCtrl))
			ReleaseCtrlCapture();
		break;
	case WM_SHOWWINDOW:
		visible = (BOOL) wParam;
		StateH(SHOW);
		break;
	case WM_MOUSEACTIVATE:
		LLOG("WM_MOUSEACTIVATE " << Name() << ", focusCtrlWnd = " << UPP::Name(focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
		if(!IsEnabled()) {
			if(lastActiveWnd && lastActiveWnd->IsEnabled()) {
				if(focusCtrl) { // this closes popup
					LLOG("WM_MOUSEACTIVATE -> ClickActivate for " << UPP::Name(lastActiveWnd));
					StopPreedit();
					lastActiveWnd->ClickActivateWnd();
				}
				else { // this makes child dialog active when clicked on disabled parent
					LLOG("WM_MOUSEACTIVATE -> ::SetFocus for " << UPP::Name(lastActiveWnd));
					::SetFocus(lastActiveWnd->GetHWND());
				}
			}
			else
				MessageBeep(MB_OK);
			return MA_NOACTIVATEANDEAT;
		}
		if(IsPopUp()) return MA_NOACTIVATE;
		break;
	case WM_SIZE:
	case WM_MOVE:
		if(hwnd) {
			Rect rect;
			if(activex) {
				WINDOWPLACEMENT wp;
				wp.length = sizeof(WINDOWINFO);
				::GetWindowPlacement(hwnd, &wp);
				rect = wp.rcNormalPosition;
			}
			else
				rect = GetScreenClient(hwnd);
			LLOG("WM_MOVE / WM_SIZE: screen client = " << rect);
			if(GetRect() != rect)
				SetWndRect(rect);
		#if WINCARET
			WndDestroyCaret();
			caretCtrl = NULL;
			SyncCaret();
		#endif
			SyncPreedit();
		}
		return 0L;
	case WM_HELP:
		return TRUE;
	case WM_ACTIVATE:
		LLOG("WM_ACTIVATE " << Name() << ", wParam = " << (int)wParam << ", focusCtrlWnd = " << UPP::Name(focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
		ignorekeyup = true;
	case WM_SETFOCUS:
		LLOG("WM_SETFOCUS " << Name() << ", focusCtrlWnd = " << UPP::Name(focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
		if(this != focusCtrlWnd || focusCtrl && focusCtrlWnd != focusCtrl->GetTopCtrl()) { // second condition fixes popup issue when clicking dialog parent
			if(IsEnabled()) {
				LLOG("WM_SETFOCUS -> ActivateWnd: this != focusCtrlWnd, this = "
					<< Name() << ", focusCtrlWnd = " << UPP::Name(focusCtrlWnd));
				ActivateWnd();
			}
			else {
				if(focusCtrlWnd && focusCtrlWnd->IsEnabled()) {
//					if(!IsEnabled())
//						MessageBeep(MB_OK);
					LLOG("WM_SETFOCUS -> ::SetFocus for " << UPP::Name(focusCtrlWnd) << ", this: " << UPP::Name(this));
					::SetFocus(focusCtrlWnd->GetHWND());
				}
				else
				if(lastActiveWnd && lastActiveWnd->IsEnabled()) {
					LLOG("WM_SETFOCUS -> ::SetFocus for " << UPP::Name(lastActiveWnd));
					::SetFocus(lastActiveWnd->GetHWND());
				}
				else {
					LLOG("WM_SETFOCUS - ::SetFocus(NULL)");
					::SetFocus(NULL);
				}
			}
		}
		LLOG("//WM_SETFOCUS " << (void *)hwnd << ", focusCtrlWnd = " << UPP::Name(focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
		return 0L;
	case WM_KILLFOCUS:
		LLOG("WM_KILLFOCUS " << (void *)(HWND)wParam << ", this = " << UPP::Name(this) << ", focusCtrlWnd = " << UPP::Name(focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
		LLOG("Kill " << UPP::Name(CtrlFromHWND((HWND)wParam)));
		if(!CtrlFromHWND((HWND)wParam)) {
			LLOG("WM_KILLFOCUS -> KillFocusWnd: " << UPP::Name(this));
			KillFocusWnd();
		}
		LLOG("//WM_KILLFOCUS " << (void *)(HWND)wParam << ", focusCtrlWnd = " << UPP::Name(focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
		StopPreedit();
		return 0L;
	case WM_ENABLE:
		if(!!wParam != enabled) {
			enabled = !!wParam;
			RefreshFrame();
			StateH(ENABLE);
		}
		return 0L;
#ifndef PLATFORM_WINCE
	case WM_GETMINMAXINFO:
		{
			MINMAXINFO *mmi = (MINMAXINFO *)lParam;
			Rect frmrc = Size(200, 200);
			::AdjustWindowRect(frmrc, WS_OVERLAPPEDWINDOW, FALSE);
//			Size msz = Ctrl::GetWorkArea().Deflated(-frmrc.left, -frmrc.top,
//				           frmrc.right - 200, frmrc.bottom - 200).GetSize();
//			Rect minr(Point(50, 50), min(msz, GetMinSize()));
//			Rect maxr(Point(50, 50), min(msz, GetMaxSize())); // Removed cxl&nixnixnix 2012-6-12
			Rect minr(Point(50, 50), GetMinSize());
			Rect maxr(Point(50, 50), GetMaxSize());
			dword style = ::GetWindowLong(hwnd, GWL_STYLE);
			dword exstyle = ::GetWindowLong(hwnd, GWL_EXSTYLE);
			AdjustWindowRectEx(minr, style, FALSE, exstyle);
			AdjustWindowRectEx(maxr, style, FALSE, exstyle);
			mmi->ptMinTrackSize = Point(minr.Size());
			mmi->ptMaxTrackSize = Point(maxr.Size());
			LLOG("WM_GETMINMAXINFO: MinTrackSize = " << Point(mmi->ptMinTrackSize) << ", MaxTrackSize = " << Point(mmi->ptMaxTrackSize));
			LLOG("ptMaxSize = " << Point(mmi->ptMaxSize) << ", ptMaxPosition = " << Point(mmi->ptMaxPosition));
		}
		return 0L;
#endif
	case WM_SETTINGCHANGE:
	case 0x031A: // WM_THEMECHANGED
		PostReSkin();
		break;
/*
    case WM_IME_COMPOSITION:
		HIMC himc = ImmGetContext(hwnd);
		if(!himc) break;
		CANDIDATEFORM cf;
		Rect r = GetScreenRect();
		cf.dwIndex = 0;
		cf.dwStyle = CFS_CANDIDATEPOS;
		cf.ptCurrentPos.x = r.left + caretx;
		cf.ptCurrentPos.y = r.top + carety + caretcy;
		ImmSetCandidateWindow (himc, &cf);
		break;
*/
	}
	if(hwnd) {
		LRESULT r;
		int level = LeaveGuiMutexAll();
		if(IsWindowUnicode(hwnd))
			r = DefWindowProcW(hwnd, message, wParam, lParam);
		else
			r = DefWindowProc(hwnd, message, wParam, lParam);
		EnterGuiMutex(level);
		return r;
	}
	return 0L;
}

void Ctrl::PreDestroy() {}

}

#endif