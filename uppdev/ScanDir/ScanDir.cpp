#include <Core/Core.h>
#include <Sql/Sql.h>

using namespace Upp;

class DirFinder {
	Vector<String> dirs;
	VectorMap<String, bool> entry;
	
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
	if(entry.Find(h) >= 0)
		return;
	entry.GetAdd(Normz(dir)) = false;
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
		DDUMP(dirs[0]);
		ScanDirs(dirs[0]);
		DDUMP(entry.GetCount());
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
	DDUMP(df.ScanForDir("/vc/bin", "", "link.exe;cl.exe;mspdb140.dll", "1033"));
	DDUMP(df.ScanForDir("/vc/bin", "", "link.exe;cl.exe", "1033"));
	DDUMP(df.ScanForDir("", "/windows kits/", "", "um;ucrt;shared"));
//	h = "c:/program files (x86)/microsoft visual studio 14.0/vc/bin";
//	DDUMP(h);
//	DDUMP(Split(h, CharFilterNotDigit));
}
