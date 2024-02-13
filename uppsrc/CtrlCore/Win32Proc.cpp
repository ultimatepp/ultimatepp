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

LRESULT Ctrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
	GuiLock __;
	eventid++;
//	LLOG("Ctrl::WindowProc(" << message << ") in " << ::Name(this) << ", focus " << (void *)::GetFocus());
	Ptr<Ctrl> _this = this;
	HWND hwnd = GetHWND();

	cancel_preedit = DoCancelPreedit; // We really need this just once, but whatever..

	is_pen_event = (GetMessageExtraInfo() & 0xFFFFFF80) == 0xFF515700; // https://learn.microsoft.com/en-us/windows/win32/tablet/system-events-and-mouse-messages?redirectedfrom=MSDN
	
	POINT p;
	if(::GetCursorPos(&p))
		CurrentMousePos = p;

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
	case WM_PALETTECHANGED:
		if((HWND)wParam == hwnd)
			break;
	case WM_QUERYNEWPALETTE:
		if(!SystemDraw::AutoPalette()) break;
		{
			HDC hDC = GetDC(hwnd);
			HPALETTE hOldPal = SelectPalette(hDC, GetQlibPalette(), FALSE);
			int i = RealizePalette(hDC);
			SelectPalette(hDC, hOldPal, TRUE);
			RealizePalette(hDC);
			ReleaseDC(hwnd, hDC);
			LLOG("Realized " << i << " colors");
			if(i) InvalidateRect(hwnd, NULL, TRUE);
			return i;
		}
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
				HPALETTE hOldPal;
				if(draw.PaletteMode() && SystemDraw::AutoPalette()) {
					hOldPal = SelectPalette(dc, GetQlibPalette(), TRUE);
					int n = RealizePalette(dc);
					LLOG("In paint realized " << n << " colors");
				}
				painting = true;
				UpdateArea(draw, Rect(ps.rcPaint));
				painting = false;
				if(draw.PaletteMode() && SystemDraw::AutoPalette())
					SelectPalette(dc, hOldPal, TRUE);
			}
			EndPaint(hwnd, &ps);

			UpdateDHCtrls(); // so that they are displayed withing the same WM_PAINT - looks better
		}
		return 0L;
	case WM_NCHITTEST:
		CheckMouseCtrl();
		if(ignoremouse) return HTTRANSPARENT;
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
	case WM_NCLBUTTONDOWN:
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
	case WM_ERASEBKGND:
		return 1L;
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
	case 0x031A: // WM_THEMECHANGED
		XpClear();
		break;
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
/*	case WM_SETTINGCHANGE:
	case 0x031A: // WM_THEMECHANGED
		ReSkin();
		RefreshLayoutDeep();
		RefreshFrame();
		break;
*/
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
		if(IsWindowUnicode(hwnd)) // TRC 04/10/17: ActiveX unicode patch
			return DefWindowProcW(hwnd, message, wParam, lParam);
		else
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0L;
}

void Ctrl::PreDestroy() {}

}

#endif