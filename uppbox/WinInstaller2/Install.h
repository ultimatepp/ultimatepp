#ifndef _WinInstaller_Install_h_
#define _WinInstaller_Install_h_

//////////////////////////////////////////////////////////////////////
// smallprg: small (in terms of requirements) progress window.

#include <Core/Core.h>
using namespace Upp;

void Error(char *sz = "Installation files are damaged");

void LZMAExtract(const char *ptr, int len, const char *outdir, Gate2<int, int> progress);

extern "C"
BOOL BrowseFolder( HWND hwndOwner, LPTSTR lpszDir, LPCTSTR lpszTitle );

class ProgressMeter {
public:
	enum
	{
		FILTER   = 0x01,
		THROW    = 0x02,
		NOCANCEL = 0x04,
		HIDDEN   = 0x08,
	};

	ProgressMeter(int step = 1, const char *title = NULL, int total = 0,
		int flags = FILTER, HWND parent = 0);
	ProgressMeter(const char *title, int step = 1, int total = 0,
		int flags = THROW, HWND parent = 0);
	~ProgressMeter();

	bool           Create();
	void           Destroy();
	bool           IsOpen() const                      { return window; }

	ProgressMeter& Step();
	bool           Cancelled();     // English spelling mistake, should read 'Canceled'
	bool           StepCancelled(); // ditto
	bool           SetPosCancelled(int pos); // ditto
	void           Refresh()                           { Cancelled(); }

	bool           IgnoreStep(int pos) const;

	ProgressMeter& Caption(const char *_caption);
	const String&  GetCaption() const                  { return caption; }

	ProgressMeter& Filter(bool _fm = true);
	ProgressMeter& NoFilter()                          { return Filter(false); }
	bool           IsFilter() const                    { return flags & FILTER; }

	ProgressMeter& Throw(bool _th = true);
	ProgressMeter& NoThrow()                           { return Throw(false); }
	bool           IsThrow() const                     { return flags & THROW; }

	ProgressMeter& AllowCancel(bool _ac = true);
	ProgressMeter& NoAllowCancel()                     { return AllowCancel(false); }
	bool           IsAllowCancel() const               { return !(flags & NOCANCEL); }

	ProgressMeter& Show(bool _show = true);
	ProgressMeter& Hide()                              { return Show(false); }
	bool           IsVisible() const                   { return !(flags & HIDDEN); }

	ProgressMeter& SetRange(int total);
	int            GetRange() const                    { return total; }

	ProgressMeter& SetChunk(int n)                     { chunk = n; return *this; }
	int            GetChunk() const                    { return chunk; }

	ProgressMeter& SetPos(int pos);
	ProgressMeter& Reset()                             { return SetPos(0); }
	int            GetPos() const                      { return pos; }

	ProgressMeter& SetText(const char *s, bool reset = true);
	const String&  GetText() const                     { return text; }

	ProgressMeter& Set(int pos, int total);

	operator Gate2<int, int> ()                        { return callback(this, &ProgressMeter::OnStep); }
	bool           OnStep(int done, int total);

protected:
	static LRESULT CALLBACK StaticProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT  Proc(UINT message, WPARAM wParam, LPARAM lParam);

	void           Enable(bool en);

protected:
	HWND           parent;
	HWND           window;
	HWND           progress;

	Vector<HWND>   enable;

	bool           cancel;
	bool           visible;
	int            flags;
	int            total;
	int            pos;
	int            chunk;
	int            next_ticks;
	String         text;
	String         old_text;
	String         caption;
};

#endif
