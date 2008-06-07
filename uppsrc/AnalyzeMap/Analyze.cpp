#include <CtrlLib/CtrlLib.h>
#include <winver.h>
#include <imagehlp.h>

using namespace Upp;

#define DLLFILENAME "imagehlp.dll"
#define DLIHEADER   "imagehlp.dli"
#define DLIMODULE   Imagehlp
#include <Core/dli.h>

String AnalyzeMap(String fn)
{
	Vector<unsigned>  len;
	Vector<String>    name;
	String map = LoadFile(fn);
	if(IsNull(map))
		return "can't open " + fn;
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
				if(len.GetCount())
					len.Top() = addr - len.Top();
				len.Add(addr);
				name.Add(String(b, e));
			}

		}
//	if(len.GetCount())
//		len.Top() -= addr;
	IndexSort(len, name, StdGreater<unsigned>());
	String r;
	for(int i = 0; i < len.GetCount(); i++) {
		r << Sprintf("%5d: ", len[i]);
		if(Imagehlp()) {
			char nm[1024];
			Imagehlp().UnDecorateSymbolName(name[i], nm, 1024, UNDNAME_NO_ACCESS_SPECIFIERS|
				                            UNDNAME_NO_MEMBER_TYPE|UNDNAME_NO_MS_KEYWORDS);
			r << nm;
		}
		else
			r << name[i];
		r << '\n';
	}
	return r;
}

class MapAnalyzer : public TopWindow {
public:
	typedef MapAnalyzer CLASSNAME;
	MapAnalyzer();

	void Run();

	bool OpenFile();

	virtual bool Key(dword key, int repcnt);

	void LoadCrashFile();

private:
	LineEdit map;
};

MapAnalyzer::MapAnalyzer()
{
	Sizeable().Zoomable();
	Add(map.SizePos());
}

void MapAnalyzer::Run()
{
	if(!OpenFile())
		return;
	TopWindow::Run();
}

bool MapAnalyzer::Key(dword key, int repcnt)
{
	if(key == K_CTRL_O) {
		OpenFile();
		return true;
	}
	return TopWindow::Key(key, repcnt);
}

bool MapAnalyzer::OpenFile()
{
	FileSel fs;
	LoadFromFile(fs);
	fs.Type("Map file", "*.map");
	fs.DefaultExt("map");
	fs.Multi();
	if(!fs.ExecuteOpen()) return false;
	StoreToFile(fs);
	map <<= AnalyzeMap(~fs);
	return true;
}

GUI_APP_MAIN
{
	SetDefaultCharset(CHARSET_WIN1250);
	MapAnalyzer viewer;
	viewer.Run();
}
