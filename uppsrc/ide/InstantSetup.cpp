#include "ide.h"

#ifdef PLATFORM_WIN32_NO

class DirFinder {
	Vector<String> dirs;
	VectorMap<String, bool> entry;
	
	Progress pi;
	
	static String Normz(const String& p);
	
	bool Has(const String& d, const Vector<String>& m, bool isfile);
	bool Contains(const String& d, const Vector<String>& m);
	void ScanDirs(const char *dir);

public:
	void Dir(const String& d) { dirs.Add(d); }

	String ScanForDir(const String& dir, const char *ccontains, const char *cfiles, const char *csubdirs);
};

String DirFinder::Normz(const String& p)
{
	String h = ToLower(p);
	h.Replace("\\", "/");
	return h;
}

void DirFinder::ScanDirs(const char *dir)
{
	String h = Normz(dir);
	if(pi.Canceled() || entry.Find(h) >= 0)
		return;
	entry.GetAdd(Normz(dir)) = false;
	pi.SetText("Scanning " + GetFileName(dir));
	for(FindFile ff(AppendFileName(dir, "*.*")); ff; ff.Next()) {
		String p = ff.GetPath();
		if(ff.IsFolder())
			ScanDirs(p);
		else
		if(ff.IsFile())
			entry.GetAdd(Normz(p)) = true;
	}
}

bool DirFinder::Has(const String& d, const Vector<String>& m, bool isfile)
{
	for(int i = 0; i < m.GetCount(); i++) {
		String dd = d + '/' + m[i];
		int q = entry.Find(dd);
		if(q < 0 || entry[q] != isfile)
			return false;
	}
	return true;
}

int CharFilterNotDigit(int c)
{
	return IsDigit(c) ? 0 : c;
}

bool DirFinder::Contains(const String& d, const Vector<String>& m)
{
	for(int i = 0; i < m.GetCount(); i++)
		if(d.Find(m[i]) < 0)
			return false;
	return true;
}

String DirFinder::ScanForDir(const String& dir, const char *ccontains, const char *cfiles, const char *csubdirs)
{
	for(;;) {
		Vector<String> files = Split(cfiles, ';');
		Vector<String> subdirs = Split(csubdirs, ';');
		Vector<String> contains = Split(ccontains, ';');
		Vector<int> max;
		String r;
		for(int i = 0; i < entry.GetCount(); i++) {
			String d = entry.GetKey(i);
			if(entry.GetKey(i).EndsWith(dir) && Has(d, files, true) && Has(d, subdirs, false) && Contains(d, contains)) {
				Vector<String> m0 = Split(d, CharFilterNotDigit);
				Vector<int> m;
				for(int i = 0; i < m0.GetCount(); i++)
					m.Add(atoi(m0[i]));
				if(m > max) {
					max = pick(m);
					r = d;
				}
			}
		}
		if(r.GetCount() || dirs.GetCount() == 0)
			return r;
		ScanDirs(dirs[0]);
		dirs.Remove(0);
	}
}

CONSOLE_APP_MAIN
{
	DirFinder df;
	String pf = GetProgramsFolderX86();
	df.Dir(pf + "/microsoft visual studio 14.0/vc/bin");
	df.Dir(pf + "/windows kits/10");
	df.Dir(pf + "/windows kits");
	df.Dir(pf + "/microsoft visual studio 14.0");
	df.Dir(pf);
//	DDUMPM(df.entry);
	DDUMP(df.ScanForDir("/vc/bin", "", "bin/link.exe;cl.exe;mspdb140.dll", "1033"));
	DDUMP(df.ScanForDir("/vc/bin", "", "link.exe;cl.exe", "1033"));
	DDUMP(df.ScanForDir("", "/windows kits/", "", "um;ucrt;shared"));
//	h = "c:/program files (x86)/microsoft visual studio 14.0/vc/bin";
//	DDUMP(h);
//	DDUMP(Split(h, CharFilterNotDigit));
}

bool CheckDirs(const Vector<String>& d, int count)
{
	if(d.GetCount() < count)
		return false;
	for(int i = 0; i < count; i++)
		if(!DirectoryExists(d[i]))
			return false;
	return true;
}

void InstantSetup()
{
	DirFinder df;
	String pf = GetProgramsFolderX86();
	df.Dir(pf + "/microsoft visual studio 14.0/vc/bin");
	df.Dir(pf + "/windows kits/10");
	df.Dir(pf + "/windows kits");
	df.Dir(pf + "/microsoft visual studio 14.0");
	df.Dir(pf);
	df.Dir(GetProgramsFolder());
	
	String method = "MSC15Test"
	String vc, bin, inc, lib;

	VectorMap<String, String> bm = GetMethodVars(method);
	Vector<String> path = Split(bm.Get("PATH", ""), ';');
	Vector<String> include = Split(bm.Get("INCLUDE", ""), ';');
	Vector<String> lib = Split(bm.Get("LIB", ""), ';');
	if(CheckDirs(path, 2) && CheckDirs(include, 2) && CheckDirs(lib, 2))
		return;
	
	String vc = df.ScanForDir("/vc", "", "bin/link.exe;bin/cl.exe;bin/mspdb140.dll", "bin/1033")
	bin = df.ScanForDir("bin/x86", "/windows kits/", "makecat.exe;accevent.exe", "");
	inc = df.ScanForDir("", "/windows kits/", "um/adhoc.h", "um;ucrt;shared");
	lib = df.ScanForDir("", "/windows kits/", "um/adhoc.h", "um;ucrt;shared");

	if(vc.GetCount() * sdkbin.GetCount() * sdkinc.GetCount() * sdklib.GetCount() == 0)
		return;
	
	path.At(0) = vc + "/bin";
	path.At(1) = bin + "/x86";
	
	include.At(0) = vc + "/include";
	include.At(1) = inc + "/um";
	include.At(2) = inc + "/ucrt";
	include.At(3) = inc + "/shared";
}

#endif
