#ifndef CTRLCORE_H
#define CTRLCORE_H

#include <RichText/RichText.h>

#ifdef  flagNOGTK
#undef  flagGTK
#define flagX11
#endif

#include <guiplatform.h>

#ifndef GUIPLATFORM_INCLUDE

	#ifdef flagTURTLE
		#define GUIPLATFORM_KEYCODES_INCLUDE <Turtle/Keys.h>
		//need to make SDL_keysym.h known before K_ enum
		#define GUIPLATFORM_INCLUDE          <Turtle/Turtle.h>
		#define GUIPLATFORM_NOSCROLL
		#define PLATFORM_TURTLE
		#define TURTLE
	#elif PLATFORM_WIN32
		#define GUIPLATFORM_INCLUDE "Win32Gui.h"
	#else
		#ifdef flagX11
			#define GUIPLATFORM_INCLUDE "X11Gui.h"
		#else
			#ifndef flagGTK
				#define flagGTK
			#endif
			#define GUIPLATFORM_INCLUDE "Gtk.h"
		#endif
	#endif

#endif

#include GUIPLATFORM_INCLUDE

NAMESPACE_UPP

#ifdef _MULTITHREADED
void EnterGuiMutex();
void LeaveGuiMutex();

int  LeaveGuiMutexAll();
void EnterGuiMutex(int n);

bool ThreadHasGuiLock();
#else
inline void EnterGuiMutex() {}
inline void LeaveGuiMutex() {}

inline int  LeaveGuiMutexAll() { return 0; }
inline void EnterGuiMutex(int) {}
inline bool ThreadHasGuiLock() { return true; }
#endif

struct GuiLock {
	GuiLock()  { EnterGuiMutex(); }
	~GuiLock() { LeaveGuiMutex(); }
};

bool ScreenInPaletteMode(); // Deprecated

typedef ImageDraw SystemImageDraw;

void SetSurface(Draw& w, const Rect& dest, const RGBA *pixels, Size srcsz, Point poff);
void SetSurface(Draw& w, int x, int y, int cx, int cy, const RGBA *pixels);

enum {
	K_DELTA        = 0x010000,

	K_ALT          = 0x080000,
	K_SHIFT        = 0x040000,
	K_CTRL         = 0x020000,

	K_KEYUP        = 0x100000,

	K_MOUSEMIDDLE  = 0x200000,
	K_MOUSERIGHT   = 0x400000,
	K_MOUSELEFT    = 0x800000,
	K_MOUSEDOUBLE  = 0x1000000,
	K_MOUSETRIPLE  = 0x2000000,

	K_SHIFT_CTRL = K_SHIFT|K_CTRL,
};

#include "MKeys.h"

bool GetShift();
bool GetCtrl();
bool GetAlt();
bool GetCapsLock();
bool GetMouseLeft();
bool GetMouseRight();
bool GetMouseMiddle();

enum {
	DELAY_MINIMAL = 0
};

void  SetTimeCallback(int delay_ms, Callback cb, void *id = NULL); // delay_ms < 0 -> periodic
void  KillTimeCallback(void *id);
bool  ExistsTimeCallback(void *id);
dword GetTimeClick();

inline
void  PostCallback(Callback cb, void *id = NULL)                { SetTimeCallback(1, cb, id); }

class TimeCallback
{
public:
	~TimeCallback()                      { Kill(); (void)dummy; }

	void Set(int delay, Callback cb)     { UPP::SetTimeCallback(delay, cb, this); }
	void Post(Callback cb)               { UPP::PostCallback(cb, this); }
	void Kill()                          { UPP::KillTimeCallback(this); }
	void KillSet(int delay, Callback cb) { Kill(); Set(delay, cb); }
	void KillPost(Callback cb)           { Kill(); Post(cb); }

private:
	byte dummy;
};

class Ctrl;

class CtrlFrame {
public:
	virtual void FrameLayout(Rect& r) = 0;
	virtual void FrameAddSize(Size& sz) = 0;
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual void FrameAdd(Ctrl& parent);
	virtual void FrameRemove();
	virtual int  OverPaint() const;

#ifdef flagSO
	CtrlFrame();
	virtual ~CtrlFrame();
#else
	CtrlFrame() {}
	virtual ~CtrlFrame() {}
#endif

private:
	CtrlFrame(const CtrlFrame&);
	void operator=(const CtrlFrame&);
};

struct NullFrameClass : public CtrlFrame {
	virtual void FrameLayout(Rect& r);
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual void FrameAddSize(Size& sz);
};

CtrlFrame& NullFrame();

class BorderFrame : public CtrlFrame {
public:
	virtual void FrameLayout(Rect& r);
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual void FrameAddSize(Size& sz);

protected:
	const ColorF *border;

public:
#ifdef flagSO
	BorderFrame(const ColorF *border);
	virtual ~BorderFrame();
#else
	BorderFrame(const ColorF *border) : border(border) {}
#endif
};

CtrlFrame& InsetFrame();
CtrlFrame& OutsetFrame();
CtrlFrame& ButtonFrame();
CtrlFrame& ThinInsetFrame();
CtrlFrame& ThinOutsetFrame();
CtrlFrame& BlackFrame();
CtrlFrame& WhiteFrame();

CtrlFrame& XPFieldFrame();

CtrlFrame& FieldFrame();
// CtrlFrame& EditFieldFrame(); //TODO remove

CtrlFrame& TopSeparatorFrame();
CtrlFrame& BottomSeparatorFrame();
CtrlFrame& LeftSeparatorFrame();
CtrlFrame& RightSeparatorFrame();

void LayoutFrameLeft(Rect& r, Ctrl *ctrl, int cx);
void LayoutFrameRight(Rect& r, Ctrl *ctrl, int cx);
void LayoutFrameTop(Rect& r, Ctrl *ctrl, int cy);
void LayoutFrameBottom(Rect& r, Ctrl *ctrl, int cy);

dword GetMouseFlags();

Point GetMousePos();
dword GetMouseFlags();

#define IMAGECLASS CtrlCoreImg
#define IMAGEFILE <CtrlCore/CtrlCore.iml>
#include <Draw/iml_header.h>

class TopWindow;
class TrayIcon;
class GLCtrl;

enum {
	DND_NONE = 0,
	DND_COPY = 1,
	DND_MOVE = 2,

	DND_ALL  = 3,
	
	DND_EXACTIMAGE = 0x80000000,
};

struct UDropTarget;

struct ClipData : Moveable<ClipData> {
	Value  data;
	String (*render)(const Value& data);

	String  Render() const                   { return (*render)(data); }

	ClipData(const Value& data, String (*render)(const Value& data));
	ClipData(const String& data);
	ClipData();
};

class PasteClip {
	friend struct UDropTarget;
	friend class  Ctrl;
	friend PasteClip sMakeDropClip(bool paste);

	GUIPLATFORM_PASTECLIP_DECLS

	byte         action;
	byte         allowed;
	bool         paste;
	bool         accepted;
	String       fmt;
	String       data;

	void GuiPlatformConstruct();

public:
	bool   IsAvailable(const char *fmt) const;
	String Get(const char *fmt) const;

	bool   Accept();

	bool   Accept(const char *fmt);
	String GetFormat()                  { return fmt; }
	String Get() const                  { return data; }
	operator String() const             { return Get(); }
	String operator ~() const           { return Get(); }

	void   Reject()                     { accepted = false; }

	int    GetAction() const            { return action; }
	int    GetAllowedActions() const    { return allowed; }
	void   SetAction(int x)             { action = x; }

	bool   IsAccepted() const           { return accepted; }

	bool   IsQuery() const              { return !paste; }
	bool   IsPaste() const              { return paste; }

	PasteClip();
};

String  Unicode__(const WString& w);
WString Unicode__(const String& s);

void GuiPlatformAdjustDragImage(ImageBuffer& b);

Image MakeDragImage(const Image& arrow, Image sample);

const char *ClipFmtsText();
bool        AcceptText(PasteClip& clip);
String      GetString(PasteClip& clip);
WString     GetWString(PasteClip& clip);
String      GetTextClip(const String& text, const String& fmt);
String      GetTextClip(const WString& text, const String& fmt);
void        Append(VectorMap<String, ClipData>& data, const String& text);
void        Append(VectorMap<String, ClipData>& data, const WString& text);

const char *ClipFmtsImage();
bool        AcceptImage(PasteClip& clip);
Image       GetImage(PasteClip& clip);
String      GetImageClip(const Image& m, const String& fmt);
void        Append(VectorMap<String, ClipData>& data, const Image& img);

bool            IsAvailableFiles(PasteClip& clip);
bool            AcceptFiles(PasteClip& clip);
Vector<String>  GetClipFiles(const String& data);
Vector<String>  GetFiles(PasteClip& clip);
void            AppendFiles(VectorMap<String, ClipData>& data, const Vector<String>& files);

template <class T>
String ClipFmt()
{
	return String("U++ type: ") + typeid(T).name();
}

template <class T>
bool   Accept(PasteClip& clip)
{
	return clip.Accept(ClipFmt<T>());
}

String GetInternalDropId__(const char *type, const char *id);
void NewInternalDrop__(const void *ptr);
const void *GetInternalDropPtr__();

template <class T>
VectorMap<String, ClipData> InternalClip(const T& x, const char *id = "")
{
	NewInternalDrop__(&x);
	VectorMap<String, ClipData> d;
	d.Add(GetInternalDropId__(typeid(T).name(), id));
	return d;
}

template <class T>
bool IsAvailableInternal(PasteClip& d, const char *id = "")
{
	return d.IsAvailable(GetInternalDropId__(typeid(T).name(), id));
}

template <class T>
bool AcceptInternal(PasteClip& d, const char *id = "")
{
	return d.Accept(GetInternalDropId__(typeid(T).name(), id));
}

template <class T>
const T& GetInternal(PasteClip& d)
{
	return *(T *)GetInternalDropPtr__();
}

class Ctrl : public Pte<Ctrl> {
public:
	enum PlacementConstants {
		CENTER   = 0,
		MIDDLE   = 0,
		LEFT     = 1,
		RIGHT    = 2,
		TOP      = 1,
		BOTTOM   = 2,
		SIZE     = 3,

		MINSIZE  = -16380,
		MAXSIZE  = -16381,
		STDSIZE  = -16382,
	};

	class Logc {
		dword data;

		static int LSGN(dword d)       { return int16((d & 0x7fff) | ((d & 0x4000) << 1)); }

	public:
		bool  operator==(Logc q) const { return data == q.data; }
		bool  operator!=(Logc q) const { return data != q.data; }
		int   GetAlign() const         { return (data >> 30) & 3; }
		int   GetA() const             { return LSGN(data >> 15); }
		int   GetB() const             { return LSGN(data); }
		void  SetAlign(int align)      { data = (data & ~(3 << 30)) | (align << 30); }
		void  SetA(int a)              { data = (data & ~(0x7fff << 15)) | ((a & 0x7fff) << 15); }
		void  SetB(int b)              { data = (data & ~0x7fff) | (b & 0x7fff); }
		bool  IsEmpty() const;

		Logc(int al, int a, int b)     { data = (al << 30) | ((a & 0x7fff) << 15) | (b & 0x7fff); }
		Logc()                         { data = 0xffffffff; }
	};

	struct LogPos : Moveable<LogPos> {
		Logc x, y;

		bool operator==(LogPos b) const   { return x == b.x && y == b.y; }
		bool operator!=(LogPos b) const   { return !(*this == b); }

		LogPos(Logc x, Logc y)            : x(x), y(y) {}
		LogPos()                          {}
	};

	static Logc PosLeft(int pos, int size)       { return Logc(LEFT, pos, size); }
	static Logc PosRight(int pos, int size)      { return Logc(RIGHT, pos, size); }
	static Logc PosTop(int pos, int size)        { return Logc(TOP, pos, size); }
	static Logc PosBottom(int pos, int size)     { return Logc(BOTTOM, pos, size); }
	static Logc PosSize(int lpos, int rpos)      { return Logc(SIZE, lpos, rpos); }
	static Logc PosCenter(int size, int offset)  { return Logc(CENTER, offset, size); }
	static Logc PosCenter(int size)              { return Logc(CENTER, 0, size); }

	typedef bool (*MouseHook)(Ctrl *ctrl, bool inframe, int event, Point p,
	                          int zdelta, dword keyflags);
	typedef bool (*KeyHook)(Ctrl *ctrl, dword key, int count);
	typedef bool (*StateHook)(Ctrl *ctrl, int reason);

	static dword KEYtoK(dword);

private:
	Ctrl(Ctrl&);
	void operator=(Ctrl&);

private:
	struct Frame : Moveable<Frame> {
		CtrlFrame *frame;
		Rect16     view;

		Frame()    { view.Clear(); }
	};
	Ctrl        *parent;

	struct Scroll : Moveable<Scroll> {
		Rect rect;
		int  dx;
		int  dy;
	};

	struct MoveCtrl : Moveable<MoveCtrl> {
		Ptr<Ctrl>  ctrl;
		Rect       from;
		Rect       to;
	};

	friend struct UDropTarget;

	struct Top {
		GUIPLATFORM_CTRL_TOP_DECLS
		Vector<Scroll> scroll;
		VectorMap<Ctrl *, MoveCtrl> move;
		VectorMap<Ctrl *, MoveCtrl> scroll_move;
		Ptr<Ctrl>      owner;
	};

	Top         *top;
	int          exitcode;

	Ctrl        *prev, *next;
	Ctrl        *firstchild, *lastchild;//16
	LogPos       pos;//8
	Rect16       rect;
	Mitor<Frame> frame;//16
	String       info;//16
	int16        caretx, carety, caretcx, caretcy;//8

	byte         overpaint;

	bool         unicode:1;

	bool         fullrefresh:1;

	bool         transparent:1;
	bool         visible:1;
	bool         enabled:1;
	bool         wantfocus:1;
	bool         initfocus:1;
	bool         activepopup:1;
	bool         editable:1;
	bool         modify:1;
	bool         ignoremouse:1;
	bool         inframe:1;
	bool         inloop:1;
	bool         isopen:1;
	bool         popup:1;
	bool         popupgrab:1;
	byte         backpaint:2;//2
	bool         hasdhctrl:1;

	bool         akv:1;
	bool         destroying:1;

	static  Ptr<Ctrl> eventCtrl;
	static  Ptr<Ctrl> mouseCtrl;
	static  Point     mousepos;
	static  Point     leftmousepos, rightmousepos, middlemousepos;
	static  Ptr<Ctrl> focusCtrl;
	static  Ptr<Ctrl> focusCtrlWnd;
	static  Ptr<Ctrl> lastActiveWnd;
	static  Ptr<Ctrl> caretCtrl;
	static  Rect      caretRect;
	static  Ptr<Ctrl> captureCtrl;
	static  bool      ignoreclick;
	static  bool      ignoremouseup;
	static  bool      ignorekeyup;
	static  bool      mouseinview;
	static  bool      mouseinframe;
	static  bool      globalbackpaint;
	static  bool      globalbackbuffer;
	static  bool      painting;
	static  int       LoopLevel;
	static  Ctrl     *LoopCtrl;
	static  int64     EventLoopNo;
	static  int64     EndSessionLoopNo;
	static  int64     eventid;

	static  Ptr<Ctrl>           defferedSetFocus;
	static  Vector< Ptr<Ctrl> > defferedChildLostFocus;

	static  Ptr<Ctrl> repeatTopCtrl;
	static  Point     repeatMousePos;

	static  Vector<MouseHook>& mousehook();
	static  Vector<KeyHook>&   keyhook();
	static  Vector<StateHook>& statehook();

#ifdef flagSO
	static Ptr<Ctrl> FocusCtrl();
	static void      FocusCtrl(Ptr<Ctrl> fc);
#else
	static Ptr<Ctrl> FocusCtrl() { return focusCtrl; }
	static void      FocusCtrl(Ptr<Ctrl> fc) { focusCtrl = fc; }
#endif

	void    StateDeep(int state);

	void    RemoveChild0(Ctrl *q);

	static int       FindMoveCtrl(const VectorMap<Ctrl *, MoveCtrl>& m, Ctrl *x);
	static MoveCtrl *FindMoveCtrlPtr(VectorMap<Ctrl *, MoveCtrl>& m, Ctrl *x);

	Size    PosVal(int v) const;
	void    Lay1(int& pos, int& r, int align, int a, int b, int sz) const;
	Rect    CalcRect(LogPos pos, const Rect& prect, const Rect& pview) const;
	Rect    CalcRect(const Rect& prect, const Rect& pview) const;
	void    UpdateRect0(bool sync = true);
	void    UpdateRect(bool sync = true);
	void    SetPos0(LogPos p, bool inframe);
	void    SetWndRect(const Rect& r);
	void    SyncMoves();

	static  void  EndIgnore();
	static  void  LRep();
	static  void  LHold();
	static  void  LRepeat();
	static  void  RRep();
	static  void  RHold();
	static  void  RRepeat();
	static  void  MRep();
	static  void  MHold();
	static  void  MRepeat();
	static  void  KillRepeat();
	static  void  CheckMouseCtrl();
	static  void  DoCursorShape();
	static  Image& CursorOverride();
	bool    IsMouseActive() const;
	Image   MouseEvent0(int event, Point p, int zdelta, dword keyflags);
	Image   MouseEventH(int event, Point p, int zdelta, dword keyflags);
	Image   FrameMouseEventH(int event, Point p, int zdelta, dword keyflags);
	Image   MEvent0(int e, Point p, int zd);
	Image   DispatchMouse(int e, Point p, int zd = 0);
	Image   DispatchMouseEvent(int e, Point p, int zd = 0);
	void    LogMouseEvent(const char *f, const Ctrl *ctrl, int event, Point p, int zdelta, dword keyflags);

	struct CallBox;
	static void PerformCall(CallBox *cbox);

	void    StateH(int reason);

	void    RefreshAccessKeys();
	void    RefreshAccessKeysDo(bool vis);
	static  void  DefferedFocusSync();
	static  void  SyncCaret();
	static  void  RefreshCaret();
	static  bool  DispatchKey(dword keycode, int count);
	void    SetFocusWnd();
	void    KillFocusWnd();

	static Ptr<Ctrl> dndctrl;
	static Point     dndpos;
	static bool      dndframe;
	static PasteClip dndclip;

	void    DnD(Point p, PasteClip& clip);
	static void DnDRepeat();
	static void DnDLeave();

	void    SyncLayout(int force = 0);
	bool    AddScroll(const Rect& sr, int dx, int dy);
	Rect    GetClippedView();
	void    ScrollRefresh(const Rect& r, int dx, int dy);
	void    ScrollCtrl(Top *top, Ctrl *q, const Rect& r, Rect cr, int dx, int dy);
	void    SyncScroll();
	void    Refresh0(const Rect& area);
	void    PaintCaret(SystemDraw& w);
	void    CtrlPaint(SystemDraw& w, const Rect& clip);
	void    RemoveFullRefresh();
	bool    PaintOpaqueAreas(SystemDraw& w, const Rect& r, const Rect& clip, bool nochild = false);
	void    GatherTransparentAreas(Vector<Rect>& area, SystemDraw& w, Rect r, const Rect& clip);
	Ctrl   *FindBestOpaque(const Rect& clip);
	void    UpdateArea0(SystemDraw& draw, const Rect& clip, int backpaint);
	void    UpdateArea(SystemDraw& draw, const Rect& clip);
	Ctrl   *GetTopRect(Rect& r, bool inframe, bool clip = true);
	void    DoSync(Ctrl *q, Rect r, bool inframe);
	bool    HasDHCtrl() const;
	void    SyncDHCtrl();
	void    SetInfoPart(int i, const char *txt);
	String  GetInfoPart(int i) const;

// System window interface...
	void WndShow(bool b);
	void WndSetPos(const Rect& rect);

	bool IsWndOpen() const;

	bool SetWndCapture();
	bool HasWndCapture() const;
	bool ReleaseWndCapture();

	static void SetMouseCursor(const Image& m);

	static void DoDeactivate(Ptr<Ctrl> pfocusCtrl, Ptr<Ctrl> nfocusCtrl);
	static void DoKillFocus(Ptr<Ctrl> pfocusCtrl, Ptr<Ctrl> nfocusCtrl);
	static void DoSetFocus(Ptr<Ctrl> pfocusCtrl, Ptr<Ctrl> nfocusCtrl, bool activate);

	bool SetFocus0(bool activate);
	void ActivateWnd();
	void ClickActivateWnd();
	bool SetWndFocus();
	bool HasWndFocus() const;

	void WndInvalidateRect(const Rect& r);

	void WndScrollView(const Rect& r, int dx, int dy);

	void SetWndForeground();
	bool IsWndForeground() const;

	void WndEnable(bool b);

	Rect GetWndScreenRect() const;

	void WndUpdate();
	void WndUpdate(const Rect& r);

	void WndFree();
	void WndDestroy();

	void SysEndLoop();

	String Name0() const;

	static void InitTimer();

	static String appname;

	static Size Bsize;
	static Size Dsize;
	static Size Csize;
	static bool IsNoLayoutZoom;
	static void Csizeinit();
	static void (*skin)();

	friend void  InitRichTextZoom();
	friend void  AvoidPaintingCheck__();
	friend dword GetKeyStateSafe(dword what);
	friend void  CtrlSetDefaultSkin(void (*fn1)(), void (*fn2)());
	friend class DHCtrl;
	friend class ViewDraw;
	friend class TopWindow;
	friend class TrayIcon;
	friend class GLCtrl;
	friend class WaitCursor;
	friend struct UDropSource;
	friend class DnDAction;
	friend class PasteClip;

	typedef Ctrl CLASSNAME;

	void        GuiPlatformConstruct();
	void        GuiPlatformDestruct();
	void        GuiPlatformRemove();
	void        GuiPlatformGetTopRect(Rect& r) const;
	bool        GuiPlatformRefreshFrameSpecial(const Rect& r);
	bool        GuiPlatformSetFullRefreshSpecial();
	static void GuiPlatformSelection(PasteClip& d);

#ifdef GUIPLATFORM_CTRL_DECLS_INCLUDE
	#include GUIPLATFORM_CTRL_DECLS_INCLUDE
#else
	GUIPLATFORM_CTRL_DECLS
#endif

	static void InstallPanicBox();

private:
			void    DoRemove();

protected:
	static void     TimerProc(dword time);

			Ctrl&   Unicode()                         { unicode = true; return *this; }

public:
	enum StateReason {
		FOCUS      = 10,
		ACTIVATE   = 11,
		DEACTIVATE = 12,
		SHOW       = 13,
		ENABLE     = 14,
		EDITABLE   = 15,
		OPEN       = 16,
		CLOSE      = 17,
		POSITION   = 100,
		LAYOUTPOS  = 101,
	};

	enum MouseEvents {
		BUTTON        = 0x0F,
		ACTION        = 0xF0,

		MOUSEENTER    = 0x10,
		MOUSEMOVE     = 0x20,
		MOUSELEAVE    = 0x30,
		CURSORIMAGE   = 0x40,
		MOUSEWHEEL    = 0x50,

		DOWN          = 0x80,
		UP            = 0x90,
		DOUBLE        = 0xa0,
		REPEAT        = 0xb0,
		DRAG          = 0xc0,
		HOLD          = 0xd0,
		TRIPLE        = 0xe0,

		LEFTDOWN      = LEFT|DOWN,
		LEFTDOUBLE    = LEFT|DOUBLE,
		LEFTREPEAT    = LEFT|REPEAT,
		LEFTUP        = LEFT|UP,
		LEFTDRAG      = LEFT|DRAG,
		LEFTHOLD      = LEFT|HOLD,
		LEFTTRIPLE    = LEFT|TRIPLE,

		RIGHTDOWN     = RIGHT|DOWN,
		RIGHTDOUBLE   = RIGHT|DOUBLE,
		RIGHTREPEAT   = RIGHT|REPEAT,
		RIGHTUP       = RIGHT|UP,
		RIGHTDRAG     = RIGHT|DRAG,
		RIGHTHOLD     = RIGHT|HOLD,
		RIGHTTRIPLE   = RIGHT|TRIPLE,

		MIDDLEDOWN     = MIDDLE|DOWN,
		MIDDLEDOUBLE   = MIDDLE|DOUBLE,
		MIDDLEREPEAT   = MIDDLE|REPEAT,
		MIDDLEUP       = MIDDLE|UP,
		MIDDLEDRAG     = MIDDLE|DRAG,
		MIDDLEHOLD     = MIDDLE|HOLD,
		MIDDLETRIPLE   = MIDDLE|TRIPLE
	};

	enum {
		NOBACKPAINT,
		FULLBACKPAINT,
		TRANSPARENTBACKPAINT,
		EXCLUDEPAINT,
	};

	static  Vector<Ctrl *> GetTopCtrls();
	static  Vector<Ctrl *> GetTopWindows();
	static  void   CloseTopCtrls();

	static  void   InstallMouseHook(MouseHook hook);
	static  void   DeinstallMouseHook(MouseHook hook);

	static  void   InstallKeyHook(KeyHook hook);
	static  void   DeinstallKeyHook(KeyHook hook);

	static  void   InstallStateHook(StateHook hook);
	static  void   DeinstallStateHook(StateHook hook);
	
	static  int    RegisterSystemHotKey(dword key, Callback cb);
	static  void   UnregisterSystemHotKey(int id);

	virtual bool   Accept();
	virtual void   Reject();
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;
	virtual void   Serialize(Stream& s);
	virtual void   Jsonize(JsonIO& jio);
	virtual void   Xmlize(XmlIO& xio);
	virtual void   SetModify();
	virtual void   ClearModify();
	virtual bool   IsModified() const;

	virtual void   Paint(Draw& w);
	virtual int    OverPaint() const;

	virtual void   CancelMode();

	virtual void   Activate();
	virtual void   Deactivate();
	virtual void   DeactivateBy(Ctrl *new_focus);

	virtual Image  FrameMouseEvent(int event, Point p, int zdelta, dword keyflags);
	virtual Image  MouseEvent(int event, Point p, int zdelta, dword keyflags);
	virtual void   MouseEnter(Point p, dword keyflags);
	virtual void   MouseMove(Point p, dword keyflags);
	virtual void   LeftDown(Point p, dword keyflags);
	virtual void   LeftDouble(Point p, dword keyflags);
	virtual void   LeftTriple(Point p, dword keyflags);
	virtual void   LeftRepeat(Point p, dword keyflags);
	virtual void   LeftDrag(Point p, dword keyflags);
	virtual void   LeftHold(Point p, dword keyflags);
	virtual void   LeftUp(Point p, dword keyflags);
	virtual void   RightDown(Point p, dword keyflags);
	virtual void   RightDouble(Point p, dword keyflags);
	virtual void   RightTriple(Point p, dword keyflags);
	virtual void   RightRepeat(Point p, dword keyflags);
	virtual void   RightDrag(Point p, dword keyflags);
	virtual void   RightHold(Point p, dword keyflags);
	virtual void   RightUp(Point p, dword keyflags);
	virtual void   MiddleDown(Point p, dword keyflags);
	virtual void   MiddleDouble(Point p, dword keyflags);
	virtual void   MiddleTriple(Point p, dword keyflags);
	virtual void   MiddleRepeat(Point p, dword keyflags);
	virtual void   MiddleDrag(Point p, dword keyflags);
	virtual void   MiddleHold(Point p, dword keyflags);
	virtual void   MiddleUp(Point p, dword keyflags);
	virtual void   MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void   MouseLeave();

	virtual void   DragAndDrop(Point p, PasteClip& d);
	virtual void   FrameDragAndDrop(Point p, PasteClip& d);
	virtual void   DragRepeat(Point p);
	virtual void   DragEnter();
	virtual void   DragLeave();
	virtual String GetDropData(const String& fmt) const;
	virtual String GetSelectionData(const String& fmt) const;

	virtual Image  CursorImage(Point p, dword keyflags);

	virtual bool   Key(dword key, int count);
	virtual void   GotFocus();
	virtual void   LostFocus();
	virtual bool   HotKey(dword key);

	virtual dword  GetAccessKeys() const;
	virtual void   AssignAccessKeys(dword used);

	virtual void   PostInput(); // Deprecated

	virtual void   ChildFrameMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags);
	virtual void   ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags);
	virtual void   ChildGotFocus();
	virtual void   ChildLostFocus();
	virtual void   ChildAdded(Ctrl *child);
	virtual void   ChildRemoved(Ctrl *child);
	virtual void   ParentChange();

	virtual void   State(int reason);

	virtual void   Layout();

	virtual Size   GetMinSize() const;
	virtual Size   GetStdSize() const;
	virtual Size   GetMaxSize() const;

	virtual bool   IsShowEnabled() const;

	virtual Rect   GetOpaqueRect() const;
	virtual Rect   GetVoidRect() const ;

	virtual void   Updated();

	virtual void   Close();

	virtual bool   IsOcxChild();

	virtual String GetDesc() const;

	Callback       WhenAction;

	void             AddChild(Ctrl *child);
	void             AddChild(Ctrl *child, Ctrl *insafter);
	void             AddChildBefore(Ctrl *child, Ctrl *insbefore);
	void             RemoveChild(Ctrl *child);
	Ctrl            *GetParent() const           { return parent; }
	Ctrl            *GetLastChild() const        { return lastchild; }
	Ctrl            *GetFirstChild() const       { return firstchild; }
	Ctrl            *GetPrev() const             { return parent ? prev : NULL; }
	Ctrl            *GetNext() const             { return parent ? next : NULL; }
	int              GetChildIndex(const Ctrl *child) const;
	Ctrl            *GetIndexChild(int i) const;
	int              GetChildCount() const;

	bool             IsChild() const             { return parent; }

	Ctrl            *ChildFromPoint(Point& pt) const;

	bool             IsForeground() const;
	void             SetForeground();

	const Ctrl      *GetTopCtrl() const;
	Ctrl            *GetTopCtrl();
	const Ctrl      *GetOwner() const;
	Ctrl            *GetOwner();
	const Ctrl      *GetTopCtrlOwner() const;
	Ctrl            *GetTopCtrlOwner();

	Ctrl            *GetOwnerCtrl();
	const Ctrl      *GetOwnerCtrl() const;

	const TopWindow *GetTopWindow() const;
	TopWindow       *GetTopWindow();

	const TopWindow *GetMainWindow() const;
	TopWindow       *GetMainWindow();

	Ctrl&            SetFrame(int i, CtrlFrame& frm);
	Ctrl&            SetFrame(CtrlFrame& frm)            { return SetFrame(0, frm); }
	Ctrl&            AddFrame(CtrlFrame& frm);
	const CtrlFrame& GetFrame(int i = 0) const           { return *frame[i].frame; }
	CtrlFrame&       GetFrame(int i = 0)                 { return *frame[i].frame; }
	void             RemoveFrame(int i);
	void             RemoveFrame(CtrlFrame& frm);
	void             InsertFrame(int i, CtrlFrame& frm);
	int              FindFrame(CtrlFrame& frm);
	int              GetFrameCount() const               { return frame.GetCount(); }
	void             ClearFrames();

	bool        IsOpen() const;

	void        Shutdown()                               { destroying = true; }
	bool        IsShutdown() const                       { return destroying; }

	Ctrl&       SetPos(LogPos p, bool inframe);

	Ctrl&       SetPos(LogPos p);
	Ctrl&       SetPos(Logc x, Logc y)                   { return SetPos(LogPos(x, y)); }
	Ctrl&       SetPosX(Logc x);
	Ctrl&       SetPosY(Logc y);

	void        SetRect(const Rect& r);
	void        SetRect(int x, int y, int cx, int cy);
	void        SetRectX(int x, int cx);
	void        SetRectY(int y, int cy);

	Ctrl&       SetFramePos(LogPos p);
	Ctrl&       SetFramePos(Logc x, Logc y)              { return SetFramePos(LogPos(x, y)); }
	Ctrl&       SetFramePosX(Logc x);
	Ctrl&       SetFramePosY(Logc y);

	void        SetFrameRect(const Rect& r);
	void        SetFrameRect(int x, int y, int cx, int cy);
	void        SetFrameRectX(int x, int cx);
	void        SetFrameRectY(int y, int cy);

	bool        InFrame() const                          { return inframe; }
	bool        InView() const                           { return !inframe; }
	LogPos      GetPos() const                           { return pos; }

	void        RefreshLayout()                          { SyncLayout(1); }
	void        RefreshLayoutDeep()                      { SyncLayout(2); }
	void        RefreshParentLayout()                    { if(parent) parent->RefreshLayout(); }
	
	void        UpdateLayout()                           { SyncLayout(); }
	void        UpdateParentLayout()                     { if(parent) parent->UpdateLayout(); }

	Ctrl&       LeftPos(int a, int size = STDSIZE);
	Ctrl&       RightPos(int a, int size = STDSIZE);
	Ctrl&       TopPos(int a, int size = STDSIZE);
	Ctrl&       BottomPos(int a, int size = STDSIZE);
	Ctrl&       HSizePos(int a = 0, int b = 0);
	Ctrl&       VSizePos(int a = 0, int b = 0);
	Ctrl&       SizePos();
	Ctrl&       HCenterPos(int size = STDSIZE, int delta = 0);
	Ctrl&       VCenterPos(int size = STDSIZE, int delta = 0);

	Ctrl&       LeftPosZ(int a, int size = STDSIZE);
	Ctrl&       RightPosZ(int a, int size = STDSIZE);
	Ctrl&       TopPosZ(int a, int size = STDSIZE);
	Ctrl&       BottomPosZ(int a, int size = STDSIZE);
	Ctrl&       HSizePosZ(int a = 0, int b = 0);
	Ctrl&       VSizePosZ(int a = 0, int b = 0);
	Ctrl&       HCenterPosZ(int size = STDSIZE, int delta = 0);
	Ctrl&       VCenterPosZ(int size = STDSIZE, int delta = 0);

	Rect        GetRect() const;
	Rect        GetScreenRect() const;

	Rect        GetView() const;
	Rect        GetScreenView() const;
	Size        GetSize() const;

	Rect        GetVisibleScreenRect() const;
	Rect        GetVisibleScreenView() const;

	Rect        GetWorkArea() const;

	Size        AddFrameSize(int cx, int cy) const;
	Size        AddFrameSize(Size sz) const              { return AddFrameSize(sz.cx, sz.cy); }

	void        Refresh(const Rect& r);
	void        Refresh(int x, int y, int cx, int cy);
	void        Refresh();
	bool        IsFullRefresh() const                    { return fullrefresh; }

	void        RefreshFrame(const Rect& r);
	void        RefreshFrame(int x, int y, int cx, int cy);
	void        RefreshFrame();
	
	static bool IsPainting()                             { return painting; }

	void        ScrollView(const Rect& r, int dx, int dy);
	void        ScrollView(int x, int y, int cx, int cy, int dx, int dy);
	void        ScrollView(int dx, int dy);
	void        ScrollView(const Rect& r, Size delta)    { ScrollView(r, delta.cx, delta.cy); }
	void        ScrollView(Size delta)                   { ScrollView(delta.cx, delta.cy); }

	void        Sync();
	void        Sync(const Rect& r);

	static Image OverrideCursor(const Image& m);

	void        DrawCtrl(Draw& w, int x = 0, int y = 0);
	void        DrawCtrlWithParent(Draw& w, int x = 0, int y = 0);

	bool    HasChild(Ctrl *ctrl) const;
	bool    HasChildDeep(Ctrl *ctrl) const;

	Ctrl&   IgnoreMouse(bool b = true)                   { ignoremouse = b; return *this; }
	Ctrl&   NoIgnoreMouse()                              { return IgnoreMouse(false); }
	bool    IsIgnoreMouse() const                        { return ignoremouse; }
	bool    HasMouse() const;
	bool    HasMouseDeep() const;
	bool    HasMouseInFrame(const Rect& r) const;
	bool    HasMouseIn(const Rect& r) const;
	Point   GetMouseViewPos() const;
	static Ctrl *GetMouseCtrl();

	static void IgnoreMouseClick();
	static void IgnoreMouseUp();
	static void UnIgnoreMouse();

	bool    SetCapture();
	bool    ReleaseCapture();
	bool    HasCapture() const;
	static bool  ReleaseCtrlCapture();
	static Ctrl *GetCaptureCtrl();

	bool    SetFocus();
	bool    HasFocus() const                   { return FocusCtrl() == this; }
	bool    HasFocusDeep() const;
	Ctrl&   WantFocus(bool ft = true)          { wantfocus = ft; return *this; }
	Ctrl&   NoWantFocus()                      { return WantFocus(false); }
	bool	IsWantFocus() const                { return wantfocus; }
	bool    SetWantFocus();
	Ctrl&   InitFocus(bool ft = true)          { initfocus = ft; return *this; }
	Ctrl&   NoInitFocus()                      { return InitFocus(false); }
	bool    IsInitFocus() const                { return initfocus; }
	Ctrl   *GetFocusChild() const              { return HasChild(FocusCtrl()) ? ~FocusCtrl() : NULL; }
	Ctrl   *GetFocusChildDeep() const          { return HasChildDeep(FocusCtrl()) ? ~FocusCtrl() : NULL; }

	void    CancelModeDeep();

	void    SetCaret(int x, int y, int cx, int cy);
	void    SetCaret(const Rect& r);
	Rect    GetCaret() const;
	void    KillCaret();

	static Ctrl *GetFocusCtrl()                { return FocusCtrl(); }

	static Ctrl *GetEventTopCtrl()             { return eventCtrl; }

	static bool  IterateFocusForward(Ctrl *ctrl, Ctrl *top, bool noframe = false, bool init = false, bool all = false);
	static bool  IterateFocusBackward(Ctrl *ctrl, Ctrl *top, bool noframe = false, bool all = false);

	static dword AccessKeyBit(int accesskey);
	dword   GetAccessKeysDeep() const;
	void    DistributeAccessKeys();
	bool    VisibleAccessKeys();

	void    Show(bool show = true);
	void    Hide()                             { Show(false); }
	bool    IsShown() const                    { return visible; }
	bool    IsVisible() const;

	void    Enable(bool enable = true);
	void    Disable()                          { Enable(false); }
	bool    IsEnabled() const                  { return enabled; }

	Ctrl&   SetEditable(bool editable = true);
	Ctrl&   SetReadOnly()                      { return SetEditable(false); }
	bool    IsEditable() const                 { return editable; }
	bool    IsReadOnly() const                 { return !editable; }

	void    ResetModify()                      { modify = false; }
	bool    IsModifySet() const                { return modify; }

	void    UpdateRefresh();
	void    Update();
	void    Action();
	void    UpdateAction();
	void    UpdateActionRefresh();

	Ctrl&   BackPaint(int bp = FULLBACKPAINT)  { backpaint = bp; return *this; }
	Ctrl&   TransparentBackPaint()             { backpaint = TRANSPARENTBACKPAINT; return *this; }
	Ctrl&   NoBackPaint()                      { return BackPaint(NOBACKPAINT); }
	Ctrl&   BackPaintHint();
	int     GetBackPaint() const               { return backpaint; }
	Ctrl&   Transparent(bool bp = true)        { transparent = bp; return *this; }
	Ctrl&   NoTransparent()                    { return Transparent(false); }
	bool    IsTransparent() const              { return transparent; }

	Ctrl&   Info(const char *txt)              { info = txt; return *this; }
	String  GetInfo() const                    { return info; }

	Ctrl&   Tip(const char *txt);
	Ctrl&   HelpLine(const char *txt);
	Ctrl&   Description(const char *txt);
	Ctrl&   HelpTopic(const char *txt);
	Ctrl&   LayoutId(const char *txt);

	String  GetTip() const;
	String  GetHelpLine() const;
	String  GetDescription() const;
	String  GetHelpTopic() const;
	String  GetLayoutId() const;

	void    Add(Ctrl& ctrl)                    { AddChild(&ctrl); }
	Ctrl&   operator<<(Ctrl& ctrl)             { Add(ctrl); return *this; }

	void    Remove();

	Value        operator~() const             { return GetData(); }
	const Value& operator<<=(const Value& v)   { SetData(v); return v; }
	bool         IsNullInstance() const        { return GetData().IsNull(); }

	Callback     operator<<=(Callback action)  { WhenAction = action; return action; }
	Callback&    operator<<(Callback action)   { return WhenAction << action; }

	void    SetTimeCallback(int delay_ms, Callback cb, int id = 0);
	void    KillTimeCallback(int id = 0);
	void    KillSetTimeCallback(int delay_ms, Callback cb, int id);
	bool    ExistsTimeCallback(int id = 0) const;
	void    PostCallback(Callback cb, int id = 0);
	void    KillPostCallback(Callback cb, int id);
	
	enum { TIMEID_COUNT = 1 };

	static Ctrl *GetActiveCtrl();
	static Ctrl *GetActiveWindow();

	static Ctrl *GetVisibleChild(Ctrl *ctrl, Point p, bool pointinframe);

	void   PopUp(Ctrl *owner = NULL, bool savebits = true, bool activate = true, bool dropshadow = false,
	             bool topmost = false);

	void   SetAlpha(byte alpha);

	static bool IsWaitingEvent();
	static bool ProcessEvent(bool *quit = NULL);
	static bool ProcessEvents(bool *quit = NULL);

	bool   IsPopUp() const          { return popup; }

	static void  EventLoop(Ctrl *loopctrl = NULL);
	static int   GetLoopLevel()     { return LoopLevel; }
	static Ctrl *GetLoopCtrl()      { return LoopCtrl; }

	void   EndLoop();
	void   EndLoop(int code);
	bool   InLoop() const;
	bool   InCurrentLoop() const;
	int    GetExitCode() const;

	static PasteClip& Clipboard();
	static PasteClip& Selection();

	void   SetSelectionSource(const char *fmts);

	int    DoDragAndDrop(const char *fmts, const Image& sample, dword actions,
	                     const VectorMap<String, ClipData>& data);
	int    DoDragAndDrop(const char *fmts, const Image& sample = Null, dword actions = DND_ALL);
	int    DoDragAndDrop(const VectorMap<String, ClipData>& data, const Image& sample = Null,
	                     dword actions = DND_ALL);
	static Ctrl *GetDragAndDropSource();
	static Ctrl *GetDragAndDropTarget();
	bool   IsDragAndDropSource()    { return this == GetDragAndDropSource(); }
	bool   IsDragAndDropTarget()    { return this == GetDragAndDropTarget(); }
	static Size  StdSampleSize()    { return Size(126, 106); }

	void SetMinSize(Size sz) {} // see CtrlLayout template and WindowCtrl...

public:
	static void SetSkin(void (*skin)());

	static const char *GetZoomText();
	static void SetZoomSize(Size sz, Size bsz = Size(0, 0));
	static int  HorzLayoutZoom(int cx);
	static int  VertLayoutZoom(int cy);
	static Size LayoutZoom(int cx, int cy);
	static Size LayoutZoom(Size sz);
	static void NoLayoutZoom();
	static void GetZoomRatio(Size& m, Size& d);

	static int  HZoom(int cx)                            { return HorzLayoutZoom(cx); }

	static bool ClickFocus();
	static void ClickFocus(bool cf);

	static Rect   GetVirtualWorkArea();
	static Rect   GetVirtualScreenArea();
	static Rect   GetPrimaryWorkArea();
	static Rect   GetPrimaryScreenArea();
	static void   GetWorkArea(Array<Rect>& rc);
	static Rect   GetWorkArea(Point pt);
	static Rect   GetMouseWorkArea()                     { return GetWorkArea(GetMousePos()); }
	static int    GetKbdDelay();
	static int    GetKbdSpeed();
	static bool   IsAlphaSupported();
	static Rect   GetDefaultWindowRect();
	static String GetAppName();
	static void   SetAppName(const String& appname);
	static bool   IsCompositedGui();

	static void   GlobalBackPaint(bool b = true);
	static void   GlobalBackPaintHint();
	static void   GlobalBackBuffer(bool b = true);

	static void   ReSkin();

	String        Name() const;
	static String Name(Ctrl *ctrl);

#ifdef _DEBUG
	virtual void   Dump() const;
	virtual void   Dump(Stream& s) const;

	static bool LogMessages;
#endif

	static void ShowRepaint(int ms);

	static bool MemoryCheck;

	static void GuiSleep(int ms);

	static void SetTimerGranularity(int ms);

	static void Call(Callback cb);

#ifdef _MULTITHREADED
	static bool IsShutdownThreads()                     { return Thread::IsShutdownThreads(); }
	static void ShutdownThreads();
#endif
	
	static int64 GetEventId()                           { return eventid; }

	Ctrl();
	virtual ~Ctrl();
};

inline Size GetScreenSize()  { return Ctrl::GetVirtualScreenArea().GetSize(); } // Deprecated

bool   GuiPlatformHasSizeGrip();
void   GuiPlatformGripResize(TopWindow *q);
Color  GuiPlatformGetScreenPixel(int x, int y);
void   GuiPlatformAfterMenuPopUp();

Font FontZ(int face, int height = 0);

Font StdFontZ(int height = 0);
Font SansSerifZ(int height = 0);
Font SerifZ(int height = 0);
Font MonospaceZ(int height = 0);

Font ScreenSansZ(int height = 0);
Font ScreenSerifZ(int height = 0);
Font ScreenFixedZ(int height = 0);
Font RomanZ(int height = 0);
Font ArialZ(int height = 0);
Font CourierZ(int height = 0);

int   EditFieldIsThin();
Value TopSeparator1();
Value TopSeparator2();
int   FrameButtonWidth();
int   ScrollBarArrowSize();
Color FieldFrameColor();

enum { GUISTYLE_FLAT, GUISTYLE_CLASSIC, GUISTYLE_XP, GUISTYLE_X };
int GUI_GlobalStyle();

int GUI_DragFullWindow();

enum { GUIEFFECT_NONE, GUIEFFECT_SLIDE, GUIEFFECT_FADE };
int GUI_PopUpEffect();

int GUI_ToolTipDelay();

int GUI_DropShadows();
int GUI_AltAccessKeys();
int GUI_AKD_Conservative();
int GUI_DragDistance();
int GUI_DblClickTime();
int GUI_WheelScrollLines();

int GUI_HiDPI();

void GUI_GlobalStyle_Write(int);
void GUI_DragFullWindow_Write(int);
void GUI_PopUpEffect_Write(int);
void GUI_ToolTipDelay_Write(int);
void GUI_DropShadows_Write(int);
void GUI_AltAccessKeys_Write(int);
void GUI_AKD_Conservative_Write(int);
void GUI_DragDistance_Write(int);
void GUI_DblClickTime_Write(int);
void GUI_WheelScrollLines_Write(int);
void GUI_HiDPI_Write(int);

void  EditFieldIsThin_Write(int);
void  TopSeparator1_Write(Value);
void  TopSeparator2_Write(Value);
void  FrameButtonWidth_Write(int);
void  ScrollBarArrowSize_Write(int);
void  FieldFrameColor_Write(Color);

String Name(const Ctrl *ctrl);
String Desc(const Ctrl *ctrl);
void   Dump(const Ctrl *ctrl);

inline Ctrl *operator<<(Ctrl *parent, Ctrl& child)
{
	parent->Add(child);
	return parent;
}

inline unsigned GetHashValue(Ctrl *x)
{
	return (unsigned)(intptr_t)x;
}

String GetKeyDesc(dword key);

Vector< Ptr<Ctrl> > DisableCtrls(const Vector<Ctrl *>& ctrl, Ctrl *exclude = NULL);
void EnableCtrls(const Vector< Ptr<Ctrl> >& ctrl);

template <class T>
class FrameCtrl : public T, public CtrlFrame {
public:
	virtual void FrameAdd(Ctrl& parent) { parent.Add(*this); }
	virtual void FrameRemove()          { this->Ctrl::Remove(); }

	FrameCtrl()                         { this->NoWantFocus(); }
};

template <class T>
class FrameLR : public FrameCtrl<T> {
public:
	virtual void FrameAddSize(Size& sz) { sz.cx += (cx ? cx : sz.cy) * this->IsShown(); }

protected:
	int cx;

public:
	FrameLR& Width(int _cx)             { cx = _cx; this->RefreshParentLayout(); return *this; }
	int      GetWidth() const           { return cx; }
	FrameLR()                           { cx = 0; }
};

template <class T>
class FrameLeft : public FrameLR<T> {
public:
	virtual void FrameLayout(Rect& r) {
		LayoutFrameLeft(r, this, this->cx ? this->cx : FrameButtonWidth());
	}
};

template <class T>
class FrameRight : public FrameLR<T> {
public:
	virtual void FrameLayout(Rect& r) {
		LayoutFrameRight(r, this, this->cx ? this->cx : FrameButtonWidth());
	}
};

template <class T>
class FrameTB : public FrameCtrl<T> {
public:
	virtual void FrameAddSize(Size& sz) { sz.cy += (cy ? cy : sz.cx) * this->IsShown(); }

protected:
	int cy;

public:
	FrameTB& Height(int _cy)            { cy = _cy; this->RefreshParentLayout(); return *this; }
	int      GetHeight() const          { return cy; }
	FrameTB()                           { cy = 0; }
};

template <class T>
class FrameTop : public FrameTB<T> {
public:
	virtual void FrameLayout(Rect& r) {
		LayoutFrameTop(r, this, this->cy ? this->cy : r.Width());
	}
};

template <class T>
class FrameBottom : public FrameTB<T> {
public:
	virtual void FrameLayout(Rect& r) {
		LayoutFrameBottom(r, this, this->cy ? this->cy : r.Width());
	}
};

class Modality {
	Ptr<Ctrl>           active;
	bool                fore_only;
	Vector< Ptr<Ctrl> > enable;

public:
	void Begin(Ctrl *modal, bool fore_only = false);
	void End();

	~Modality()      { End(); }
};

class LocalLoop : public Ctrl {
public:
	virtual void CancelMode();

private:
	Ctrl *master;

public:
	void  Run();
	void  SetMaster(Ctrl& m)      { master = &m; }
	Ctrl& GetMaster() const       { return *master; }

	LocalLoop()                   { master = NULL; }
};

enum {
	DRAWDRAGRECT_SOLID, DRAWDRAGRECT_NORMAL, DRAWDRAGRECT_DASHED
};

void DrawDragRect(Ctrl& q, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                  Color color, int type, int animation);
void FinishDragRect(Ctrl& q);

bool PointLoop(Ctrl& ctrl, const Vector<Image>& ani, int ani_ms);
bool PointLoop(Ctrl& ctrl, const Image& img);

class RectTracker : public LocalLoop {
public:
	virtual void  LeftUp(Point, dword);
	virtual void  RightUp(Point, dword);
	virtual void  MouseMove(Point p, dword);
	virtual Image CursorImage(Point, dword);

public:
	struct Rounder {
		virtual Rect Round(const Rect& r) = 0;
		virtual ~Rounder() {}
	};

protected:
	Rect            rect;
	int             tx, ty;
	Rect            maxrect;
	Size            minsize, maxsize;
	bool            keepratio;
	Rect            clip;
	Color           color;
	Image           cursorimage;
	int             width;
	int             pattern;
	int             animation;
	int             panim;
	Rounder        *rounder;

	Rect            org;
	Rect            o;
	Point           op;

	Rect            Round(const Rect& r);

	virtual void    DrawRect(Rect r1, Rect r2);

public:
	Callback1<Rect>  sync;
	Callback1<Rect&> round;

	RectTracker&    SetCursorImage(const Image& m) { cursorimage = m; return *this; }
	RectTracker&    MinSize(Size sz)               { minsize = sz; return *this; }
	RectTracker&    MaxSize(Size sz)               { maxsize = sz; return *this; }
	RectTracker&    MaxRect(const Rect& mr)        { maxrect = mr; return *this; }
	RectTracker&    Clip(const Rect& c)            { clip = c; return *this; }
	RectTracker&    Width(int n)                   { width = n; return *this; }
	RectTracker&    SetColor(Color c)              { color = c; return *this; }
	RectTracker&    Pattern(int p)                 { pattern = p; return *this; }
	RectTracker&    Dashed()                       { return Pattern(DRAWDRAGRECT_DASHED); }
	RectTracker&    Solid()                        { return Pattern(DRAWDRAGRECT_SOLID); }
	RectTracker&    Normal()                       { return Pattern(DRAWDRAGRECT_NORMAL); }
	RectTracker&    Animation(int step_ms = 40)    { animation = step_ms; return *this; }
	RectTracker&    KeepRatio(bool b)              { keepratio = b; return *this; }
	RectTracker&    Round(Rounder& r)              { rounder = &r; return *this; }

	Rect            Get()                          { return rect; }

	Rect Track(const Rect& r, int tx = ALIGN_RIGHT, int ty = ALIGN_BOTTOM);
	int  TrackHorzLine(int x0, int y0, int cx, int line);
	int  TrackVertLine(int x0, int y0, int cy, int line);

	RectTracker(Ctrl& master);
	virtual ~RectTracker();
};

class WaitCursor {
	Image   prev;
	bool    flag;

public:
	void    Show();

	WaitCursor(bool show = true);
	~WaitCursor();
};

class AutoWaitCursor : public WaitCursor {
protected:
	int&   avg;
	int    time0;

public:
	void Cancel()                   { time0 = 0; }

	AutoWaitCursor(int& avg);
	~AutoWaitCursor();
};

void    ClearClipboard();
void    AppendClipboard(const char *format, const byte *data, int length);
void    AppendClipboard(const char *format, const String& data);
void    AppendClipboard(const char *format, const Value& data, String (*render)(const Value& data));
void    AppendClipboard(const char *format, const ClipData& data);
void    AppendClipboard(const VectorMap<String, ClipData>& data);
String  ReadClipboard(const char *format);
bool    IsClipboardAvailable(const char *format);

inline  void WriteClipboard(const char *format, const String& data)
	{ ClearClipboard(); AppendClipboard(format, data); }

void    AppendClipboardText(const String& s);
String  ReadClipboardText();
void    AppendClipboardUnicodeText(const WString& s);
WString ReadClipboardUnicodeText();
bool    IsClipboardAvailableText();

inline  void WriteClipboardText(const String& s)
	{ ClearClipboard(); AppendClipboardText(s); }
inline  void WriteClipboardUnicodeText(const WString& s)
	{ ClearClipboard(); AppendClipboardUnicodeText(s); }

template <class T>
inline void AppendClipboardFormat(const T& object) {
	AppendClipboard(typeid(T).name(), StoreAsString(const_cast<T&>(object)));
}

template <class T>
inline void WriteClipboardFormat(const T& object) {
	ClearClipboard();
	AppendClipboardFormat(object);
}

template <class T>
inline bool ReadClipboardFormat(T& object)
{
	String s = ReadClipboard(typeid(T).name());
	return !IsNull(s) && LoadFromString(object, s);
}

template <class T>
bool IsClipboardFormatAvailable()
{
	return IsClipboardAvailable(typeid(T).name());
}

template <class T>
inline T ReadClipboardFormat() {
	T object;
	ReadClipboardFormat(object);
	return object;
}

Image  ReadClipboardImage();
void   AppendClipboardImage(const Image& img);

inline void WriteClipboardImage(const Image& img)
	{ ClearClipboard(); AppendClipboardImage(img); }

bool (*&DisplayErrorFn())(const Value& v);
inline bool DisplayError(const Value& v) { return DisplayErrorFn()(v); }

void       EncodeRTF(Stream& stream, const RichText& richtext, byte charset,
	Size dot_page_size = Size(4960, 7015), const Rect& dot_margin = Rect(472, 472, 472, 472),
	void *context = NULL);
String     EncodeRTF(const RichText& richtext, byte charset,
	Size dot_page_size = Size(4960, 7015), const Rect& dot_margin = Rect(472, 472, 472, 472),
	void *context = NULL);
String     EncodeRTF(const RichText& richtext, byte charset, int dot_page_width);
String     EncodeRTF(const RichText& richtext);
RichText   ParseRTF(const char *rtf);

#include <CtrlCore/TopWindow.h>

#include GUIPLATFORM_INCLUDE_AFTER

END_UPP_NAMESPACE

#endif
