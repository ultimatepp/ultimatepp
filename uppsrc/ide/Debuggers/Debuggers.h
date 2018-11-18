#ifndef __Debuggers__
#define __Debuggers__

#define DR_LOG(x) // RLOG(x)

#include "GdbUtils.h"

#include <ide/Common/Common.h>

#include <HexView/HexView.h>

#define  IMAGECLASS DbgImg
#define  IMAGEFILE  <ide/Debuggers/Debuggers.iml>
#include <Draw/iml_header.h>

#define  LAYOUTFILE    <ide/Debuggers/Gdb.lay>
#include <CtrlCore/lay.h>

typedef uint64 adr_t;

String CreateDebugTTY();
void   KillDebugTTY();
bool   TTYQuit();

String GdbCommand(bool console);

int CharFilterReSlash(int c);

class DbgDisas : public Ctrl {
public:
	virtual void Paint(Draw& w);
	virtual void Layout();
	virtual void LeftDown(Point p, dword);
	virtual void MouseWheel(Point, int zdelta, dword);
	virtual bool Key(dword key, int);
	virtual void GotFocus();
	virtual void LostFocus();

private:
	Index<adr_t> addr;
	struct Inst : Moveable<Inst> {
		String bytes;
		String code;
		String args;
	};

	ScrollBar    sb;
	Vector<Inst> inst;
	Index<adr_t> taddr;
	int          codecx;
	adr_t        low, high;
	int          cursor;
	int          ip;
	Image        ipimg;
	bool         mode64;
	Font         opfont;

	Size GetBox() const;
	void Scroll();

	typedef DbgDisas CLASSNAME;

public:
	Event<>  WhenCursor;
	Event<>  WhenFocus;

	void  Clear();
	void  Add(adr_t adr, const String& code, const String& args, const String& bytes = Null);
	void  AddT(adr_t tadr)              { taddr.Add(tadr); }

	void  WriteClipboard();

	void  Set(const String& s);
	bool  InRange(adr_t adr)            { return addr.Find(adr) >= 0; }
	void  SetCursor(adr_t adr);
	adr_t GetCursor() const             { return cursor >= 0 ? addr[cursor] : 0; }
	void  SetIp(adr_t adr, const Image& img);
	void  Mode64(bool b)                { mode64 = b; }

	DbgDisas();
};

struct Dbg {
	virtual void Stop();
	virtual bool IsFinished();


	virtual bool      Result(String& result, const String& s) = 0;

	typedef Dbg CLASSNAME;

	Dbg();
};

struct RedDisplay : public Display {
	void Paint(Draw& w, const Rect& r, const Value& q,
	           Color ink, Color paper, dword s) const;
};

const char *FindTag(const char *txt, const char *tag);
const char *AfterTag(const char *txt, const char *tag);
const char *AfterHeading(const char *txt, const char *heading);
VectorMap<String, String> DataMap(const ArrayCtrl& data);
void MarkChanged(const VectorMap<String, String>& m, ArrayCtrl& data);

#define GDB_PROMPT "<u++dbg-" "q98klwr835f427>"

#include "Gdb.h"

#define KEYGROUPNAME "Debugger"
#define KEYNAMESPACE PdbKeys
#define KEYFILE      <ide/Debuggers/Pdb.key>
#include             <CtrlLib/key_header.h>

#ifdef PLATFORM_WIN32
#include "Pdb.h"
#endif

#endif
