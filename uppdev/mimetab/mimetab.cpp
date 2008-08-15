#include <Core/Core.h>

using namespace Upp;

struct ExtToMime {
	Index<String> major;
	Index<String> minor;
	VectorMap<String, dword> map;

	void Load(const char *file);
	void Refresh();
	bool GetMime(const String& ext, String& maj, String& min);
};

void ExtToMime::Load(const char *fn)
{
	FileIn in(fn);
	if(in)
		while(!in.IsEof()) {
			String ln = TrimLeft(TrimRight(in.GetLine()));
			if(ln[0] != '#') {
				int q = ln.Find(':');
				if(q >= 0) {
					String h = ln.Mid(0, q);
					int w = h.Find('/');
					if(w >= 0) {
						int x = ln.Find("*.", q);
						if(x >= 0) {
							String ext = ln.Mid(x + 2);
							if(ext.GetCount() && map.Find(ext) < 0)
								map.Add(ext, MAKELONG(minor.FindAdd(h.Mid(w + 1)), major.FindAdd(h.Mid(0, w))));
						}
					}
				}
			}
		}
}

void ExtToMime::Refresh()
{
	major.Clear();
	minor.Clear();
	map.Clear();
	Load("/usr/local/share/mime/globs");
	Load("/usr/share/mime/globs");
}

bool ExtToMime::GetMime(const String& ext, String& maj, String& min)
{
	ONCELOCK {
		Refresh();
	}
	int q = map.Find(ext);
	if(q < 0)
		return false;
	dword x = map[q];
	maj = major[HIWORD(x)];
	min = minor[LOWORD(x)];
}

String GetMime(const String& ext)
{
	String a, b;
	if(Single<ExtToMime>().GetMime(ext, a, b))
		return a + '/' + b;
	else
		return Null;
}

CONSOLE_APP_MAIN
{
	DUMP(GetMime("skr"));
	DUMP(GetMime("ai"));
	DUMP(GetMime("jpg"));
	DUMP(GetMime("bcpio"));
	DUMP(GetMime("jpeg"));
	DUMP(GetMime("zip"));
	DUMP(GetMime("xyz"));
	DUMP(GetMime("basdf"));
}
