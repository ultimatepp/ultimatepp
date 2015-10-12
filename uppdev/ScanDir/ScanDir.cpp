#include <Core/Core.h>
#include <Sql/Sql.h>

using namespace Upp;

#if 0

bool   IsTheDir(const char *dir, const Vector<String>& files, const Vector<String>& subdirs)
{
	DDUMP(dir);
	if(!DirectoryExists(dir)) {
		DLOG("NODIR");
		return false;
	}
	for(int i = 0; i < files.GetCount(); i++)
		if(!FileExists(AppendFileName(dir, files[i]))) {
			DDUMP(files[i]);
			return false;
		}
	for(int i = 0; i < subdirs.GetCount(); i++)
		if(!DirectoryExists(AppendFileName(dir, subdirs[i]))) {
			DDUMP(subdirs[i]);
			return false;
		}
	return true;
}

String ScanForDir(const char *path, const char *dir, const Vector<String>& files, const Vector<String>& subdirs)
{
	DLOG("Testing " << path << ", dir: " << dir);
	String h = AppendFileName(path, dir);
	if(IsTheDir(h, files, subdirs))
		return h;
	FindFile ff(AppendFileName(path, "*.*"));
	while(ff) {
		if(ff.IsFolder()) {
			String h = ScanForDir(ff.GetPath(), dir, files, subdirs);
			if(h.GetCount())
				return h;
		}
		ff.Next();
	}
	return Null;
}

String ScanForDir(const char *tip,
                  const char *start,
                  const char *dir,
                  const char *cfiles,
                  const char *csubdirs)
{
	Vector<String> files = Split(cfiles, ';');
	Vector<String> subdirs = Split(csubdirs, ';');
	String h = AppendFileName(tip, dir);
	if(IsTheDir(h, files, subdirs))
		return h;
	return ScanForDir(start, dir, files, subdirs);
}

#endif

struct DirFinder {
	VectorMap<String, bool> entry;
	
	static String Normz(const String& p);
	
	bool Has(const String& d, const Vector<String>& m, bool isfile);
	void ScanDirs(const char *dir);
	Vector<String> ScanForDir(const String& dir, const char *cfiles, const char *csubdirs);
};

String DirFinder::Normz(const String& p)
{
	String h = ToLower(p);
	h.Replace("\\", "/");
	return h;
}

void DirFinder::ScanDirs(const char *dir)
{
	entry.GetAdd(Normz(dir)) = false;
	for(FindFile ff(AppendFileName(dir, "*.*")); ff; ff.Next()) {
		if(ff.IsFolder())
			ScanDirs(ff.GetPath());
		else
		if(ff.IsFile())
			entry.GetAdd(Normz(dir)) = true;
	}
}

bool DirFinder::Has(const String& d, const Vector<String>& m, bool isfile)
{
	for(int i = 0; i < m.GetCount(); i++) {
		String dd = d + '/' + m[i];
		int q = entry.Find(dd);
		DDUMP(dd);
		DDUMP(q);
		if(q >= 0)
			DDUMP(entry[q]);
		if(q < 0 || entry[q] != isfile)
			return false;
	}
	return true;
}

Vector<String> DirFinder::ScanForDir(const String& dir, const char *cfiles, const char *csubdirs)
{
	Vector<String> r;
	Vector<String> files = Split(cfiles, ';');
	Vector<String> subdirs = Split(csubdirs, ';');
	for(int i = 0; i < entry.GetCount(); i++) {
		String d = entry.GetKey(i);
		if(entry.GetKey(i).EndsWith(dir) && Has(d, files, true) && Has(d, subdirs, false))
			r.Add(entry.GetKey(i));
	}
	return r;
}

CONSOLE_APP_MAIN
{
	DirFinder df;
	df.ScanDirs(GetProgramsFolderX86());
	DDUMPM(df.entry);
	DDUMP(df.ScanForDir("vc/bin", "link.exe;cl.exe;mspdb140.dll", "1033"));
}
