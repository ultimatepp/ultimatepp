#include <CtrlLib/CtrlLib.h>
#include <winver.h>
#include <imagehlp.h>

using namespace Upp;

#define DLLFILENAME "imagehlp.dll"
#define DLIHEADER   "imagehlp.dli"
#define DLIMODULE   Imagehlp
#include <Core/dli.h>

Vector<unsigned > address;
Vector<String>    name;
String            mapname;

void LoadMap(String fn)
{
	String fpos = GetFileName(fn);
	int dp = fpos.Find('.');
	if(dp >= 0)
		fpos.Trim(dp);
	fn = AppendFileName(GetFileDirectory(fn), fpos) + ".map";
	if(mapname == fn)
		return;
	mapname = fn;
	address.Clear();
	name.Clear();
	String map = LoadFile(fn);
	if(IsNull(map))
		return;
	const char *p = map;
	while(*p)
		if(*p++ == '\n' && !memcmp(p, " 0001:", 6)) {
			p += 5;
			while(IsXDigit(*++p))
				;
			while(*p == ' ')
				p++;
			const char *b = p;
			while((byte)*p > ' ')
				p++;
			const char *e = p;
			while(*p == ' ')
				p++;
			if(e > b && IsXDigit(*p)) {
				unsigned addr = strtoul(p, 0, 16);
				address.Add(addr);
				name.Add(String(b, e));
			}
		}
	IndexSort(address, name, StdLess<unsigned>());
}

String Fname(dword addr, bool& recognized) {
	int pos = BinFindIndex(address, addr + 1) - 1;
	String s;
	s.Cat(Format("0x%08x", (int)addr));
	if(pos >= 0) {
		dword off = addr - address[pos];
		if(off < 8000) {
			s << ": ";
			if(Imagehlp()) {
				char nm[1024];
				Imagehlp().UnDecorateSymbolName(name[pos], nm, 1024, UNDNAME_NO_ACCESS_SPECIFIERS|
					                          UNDNAME_NO_MEMBER_TYPE|UNDNAME_NO_MS_KEYWORDS);
				s << nm;
			}
			else
				s << name[pos];
			s << Format(" + 0x%x bytes", (int)off);
			recognized = true;
			return s;
		}
	}
	s << "       = " << Format("%d [%d, %d]", (int)addr, (int)LOWORD(addr), (int)HIWORD(addr));
	recognized = false;
	return s;
}

String LoadCrashFile(String filename)
{
	LoadMap(filename);
	FileIn in(filename);
	String text;
	text << "Crash log " << GetFileName(filename) << "\n";
	if(name.GetCount() == 0)
		text << "No mapfile found" << "\n";
	dword version;
	dword code, address;
	int npar;
	in % version;
	in % code % address % npar;
	Vector<dword> info;
	Vector<dword> stack;
	bool r;
	int i;
	while(npar--)
		in % info.Add();
	switch(code) {
	case EXCEPTION_ACCESS_VIOLATION:
		text << "Access violation ";
		if(info.GetCount() >= 2)
			text << (info[0] ? "writing" : "reading") << Format(" at 0x%08x", (int)info[1]);
		break;
	#define MEX(id) case id: text << #id; break;
		MEX(EXCEPTION_DATATYPE_MISALIGNMENT)
		MEX(EXCEPTION_BREAKPOINT)
		MEX(EXCEPTION_SINGLE_STEP)
		MEX(EXCEPTION_ARRAY_BOUNDS_EXCEEDED)
		MEX(EXCEPTION_FLT_DENORMAL_OPERAND)
		MEX(EXCEPTION_FLT_DIVIDE_BY_ZERO)
		MEX(EXCEPTION_FLT_INEXACT_RESULT)
		MEX(EXCEPTION_FLT_INVALID_OPERATION)
		MEX(EXCEPTION_FLT_OVERFLOW)
		MEX(EXCEPTION_FLT_STACK_CHECK)
		MEX(EXCEPTION_FLT_UNDERFLOW)
		MEX(EXCEPTION_INT_DIVIDE_BY_ZERO)
		MEX(EXCEPTION_INT_OVERFLOW)
		MEX(EXCEPTION_PRIV_INSTRUCTION)
		MEX(EXCEPTION_IN_PAGE_ERROR)
		MEX(EXCEPTION_ILLEGAL_INSTRUCTION)
		MEX(EXCEPTION_NONCONTINUABLE_EXCEPTION)
		MEX(EXCEPTION_STACK_OVERFLOW)
		MEX(EXCEPTION_INVALID_DISPOSITION)
		MEX(EXCEPTION_GUARD_PAGE)
		MEX(EXCEPTION_INVALID_HANDLE)
	default:
		text << Format("Exception 0x%08x", (int)code);
	}
	text << '\n' << Fname(address, r) << '\n';
	while(!in.IsEof())
		in % stack.Add();
	text << "\nRecognized stack dwords:\n";
	for(i = 0; i < stack.GetCount(); i++) {
		String s = Fname(stack[i], r);
		if(r) text << "\t" << s << '\n';
	}
	text << "\nComplete stack:\n";
	for(i = 0; i < stack.GetCount(); i++)
		text << "\t" << Fname(stack[i], r) << '\n';
	return text;
}

class CrashViewer : public TopWindow {
public:
	typedef CrashViewer CLASSNAME;
	CrashViewer();

	void Run();

	bool OpenFile();

	virtual bool Key(dword key, int repcnt);

	void LoadCrashFile();

private:
	Splitter splitter;
	ArrayCtrl crashfiles;
	LineEdit crashdump;
};

CrashViewer::CrashViewer()
{
	Sizeable().Zoomable();
	splitter.Horz(crashfiles, crashdump);
	splitter.SetPos(3000);
	Add(splitter.SizePos());
	crashfiles.AutoHideSb().NoHeader();
	crashfiles.AddColumn();
	crashfiles.WhenCursor = THISBACK(LoadCrashFile);
	crashdump.SetFont(Courier(14));
}

void CrashViewer::Run()
{
	if(!OpenFile())
		return;
	LoadCrashFile();
	TopWindow::Run();
}

bool CrashViewer::Key(dword key, int repcnt)
{
	if(key == K_CTRL_O) {
		OpenFile();
		return true;
	}
	return TopWindow::Key(key, repcnt);
}

bool CrashViewer::OpenFile()
{
	FileSelector fs;
	LoadFromFile(fs);
	fs.Type("Crash file", "*.crash");
	fs.DefaultExt("crash");
	fs.Multi();
	if(!fs.ExecuteOpen()) return false;
	StoreToFile(fs);
	int newln = -1;
	for(int i = 0; i < fs.GetCount(); i++) {
		String path = fs[i];
		int f = crashfiles.Find(path);
		if(f < 0) {
			newln = crashfiles.GetCount();
			crashfiles.Add(path);
		}
	}
	if(newln >= 0)
		crashfiles.SetCursor(newln);
	return true;
}

void CrashViewer::LoadCrashFile()
{
	String fn = Null;
	if(crashfiles.IsCursor())
		fn = crashfiles.Get(0);
	String title = fn;
	if(!IsNull(title))
		title << " - crash analyzer";
	else
		title << "Crash analyzer";
	Title(title);
	if(IsNull(fn))
		crashdump <<= Null;
	else
		crashdump <<= ::LoadCrashFile(fn);
}

GUI_APP_MAIN
{
	SetDefaultCharset(CHARSET_WIN1250);
	CrashViewer viewer;
	viewer.Run();
/*
	FileSelector fs;
	LoadFromFile(fs);
	fs.Type("Crash file", "*.crash");
	if(!fs.ExecuteOpen()) return;
	String text = LoadCrashFile(~fs);
	TopWindow win;
	win.Title("Crash analyzer - " + ~fs).Sizeable().Zoomable();
	win.Icon(Image::Icon(100, true), Image::Icon(100, false));
	LineEdit edit;
	edit.SetFont(Courier(14));
	edit <<= text;
	win.Add(edit.SizePos());
	win.Run();
*/
//	StoreToFile(fs);
}
