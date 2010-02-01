#ifndef PLATFORM_WINCE
class WinMetaFile {
	Size size;
	mutable HENHMETAFILE hemf;

	void     ChkP() const                   { ASSERT(!IsPicked()); }
	void     Init();
	void     Pick(pick_ WinMetaFile& src);
	void     Copy(const WinMetaFile& src);

public:
	void          Attach(HENHMETAFILE emf);
	HENHMETAFILE *Detach();

	bool     IsPicked() const               { return (uintptr_t) hemf == 0xffffffff; }

	operator bool() const                   { ChkP(); return hemf; }
	Size     GetSize() const                { ChkP(); return hemf ? size : Size(0, 0); }

	void     Clear();

	void     Paint(Draw& w, const Rect& r) const;
	void     Paint(Draw& w, int x, int y, int cx, int cy) const;

	void     Serialize(Stream& s);

	void     ReadClipboard();
	void     WriteClipboard() const;
	bool     Load(const char *file);

	WinMetaFile()                                 { Init(); }
	WinMetaFile(HENHMETAFILE hemf);
	WinMetaFile(HENHMETAFILE hemf, Size sz);
	WinMetaFile(const char *file);

	WinMetaFile(pick_ WinMetaFile& src)           { Pick(src); }
	WinMetaFile(const WinMetaFile& src, int)      { Copy(src); }
	void     operator=(pick_ WinMetaFile& src)    { Clear(); Pick(src); }
	void     operator<<=(const WinMetaFile& src)  { Clear(); Copy(src); }

	~WinMetaFile()                                { Clear(); }

	HENHMETAFILE GetHEMF() const                  { ChkP(); return hemf; }
};

class WinMetaFileDraw : public Draw {
	Size size;

public:
	bool        Create(HDC hdc, int cx, int cy, const char *app = NULL, const char *name = NULL, const char *file = NULL);
	bool        Create(int cx, int cy, const char *app = NULL, const char *name = NULL, const char *file = NULL);
	WinMetaFile Close();

	WinMetaFileDraw() {}
	WinMetaFileDraw(HDC hdc, int cx, int cy, const char *app = NULL, const char *name = NULL, const char *file = NULL);
	WinMetaFileDraw(int cx, int cy, const char *app = NULL, const char *name = NULL, const char *file = NULL);
	~WinMetaFileDraw();
};
#endif

class ScreenDraw : public Draw {
public:
	ScreenDraw(bool ic = false);
	~ScreenDraw();
};

#ifndef PLATFORM_WINCE
class PrintDraw : public Draw {
public:
	virtual void StartPage();
	virtual void EndPage();

private:
	void  InitPrinter();
public:
	void  Abort()                                       { aborted = true; }

	PrintDraw(HDC hdc, const char *jobname);
	~PrintDraw();
};
#endif

inline bool     BitBlt(HDC ddc, Point d, HDC sdc, const Rect& s, dword rop = SRCCOPY)
{ return BitBlt(ddc, d.x, d.y, s.Width(), s.Height(), sdc, s.left, s.top, rop); }

inline bool     StretchBlt(HDC ddc, const Rect& r, HDC sdc, const Rect& s, dword rop = SRCCOPY)
{ return StretchBlt(ddc, r.left, r.top, r.Width(), r.Height(), sdc, s.left, s.top, s.Width(), s.Height(), rop); }

inline bool     PatBlt(HDC dc, const Rect& r, dword rop = PATCOPY)
{ return PatBlt(dc, r.left, r.top, r.Width(), r.Height(), rop); }

inline void     MoveTo(HDC hdc, Point pt)                         { MoveToEx(hdc, pt.x, pt.y, 0); }
inline void     LineTo(HDC hdc, Point pt)                         { LineTo(hdc, pt.x, pt.y); }

inline void     DrawLine(HDC hdc, Point p, Point q)               { MoveTo(hdc, p); LineTo(hdc, q); }
inline void     DrawLine(HDC hdc, int px, int py, int qx, int qy) { MoveToEx(hdc, px, py, 0); LineTo(hdc, qx, qy); }

#ifndef PLATFORM_WINCE
inline void     DrawArc(HDC hdc, const Rect& rc, Point p, Point q){ Arc(hdc, rc.left, rc.top, rc.right, rc.bottom, p.x, p.y, q.x, q.y); }
#endif
inline void     DrawCircle(HDC hdc, int x, int y, int radius)     { Ellipse(hdc, x - radius, y - radius, x + radius + 1, y + radius + 1); }
inline void     DrawCircle(HDC hdc, Point centre, int radius)     { DrawCircle(hdc, centre.x, centre.y, radius); }
inline void     DrawEllipse(HDC hdc, const Rect& rc)              { Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom); }

inline void     DrawRect(HDC hdc, const Rect& rc)                 { Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom); }
