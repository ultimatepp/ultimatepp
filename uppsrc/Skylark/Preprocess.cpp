#include "Skylark.h"

namespace Upp {

String GetFileOnPath1(const char *file, const char *path)
{
	String r = GetFileOnPath(file, path);
	return r;
}

String LoadTemplate(const char *file, const String& search_path, int lang)
{
	String f = file;
	String path = GetFileOnPath1(f + '.' + ToLower(LNGAsText(lang)) + ".witz", search_path);
	if(IsNull(path)) {
		path = GetFileOnPath1(f + ".witz", search_path);
		if(IsNull(path)) {
			path = GetFileOnPath1(f, search_path);
			if(IsNull(path))
				return Null;
		}
	}
	FileIn in(path);
	String r;
	while(in && !in.IsEof()) {
		String line = in.GetLine();
		CParser p(line);
		if(p.Char('#') && p.Id("include")) {
			String file = p.GetPtr();
			int q = file.Find(' ');
			if(q < 0)
				q = file.Find('\t');
			if(q >= 0)
				file = file.Mid(0, q);
			r << LoadTemplate(file, GetFileFolder(path) + ';' + search_path, lang);
		}
		else
			r << line;
		r << "\r\n";
	}
	return r;
}

VectorMap<String, String> GetTemplateDefs(const char *file, int lang)
{
	String s = LoadTemplate(file, SkylarkApp::Config().path, lang);
	VectorMap<String, String> def;
	int ti = def.FindAdd("MAIN");
	StringStream ss(s);
	while(!ss.IsEof()) {
		String line = ss.GetLine();
		CParser p(line);
		if(p.Char('#') && p.Id("define")) {
			String id = "MAIN";
			if(p.IsId())
				id = p.ReadId();
			ti = def.FindAdd(id);
			def[ti].Clear();
			def[ti] << p.GetPtr();
		}
		else
			def[ti] << line << "\r\n";		
	}
	return def;
}

int CharFilterIsCrLf(int c)
{
	return c == '\r' || c == '\n' ? c : 0;
}

String ReplaceVars(const String& src, const VectorMap<String, String>& def, int chr)
{
	Index<String> expanded;
	String r = src;
	bool again;
	do {
		again = false;
		String rr;
		const char *s = ~r;
		for(;;) {
			const char *q = strchr(s, chr);
			if(q) {
				rr.Cat(s, q);
				CParser p(q + 1);
				if(p.Char(chr) || !p.IsId())
					rr << (char)20;
				else {
					String id = p.ReadId();
					if(expanded.Find(id) >= 0)
						rr.Cat(q, p.GetSpacePtr());
					else {
						rr.Cat(def.Get(id, Null));
						expanded.Add(id);
						again = true;
					}
				}
				s = p.GetSpacePtr();
			}
			else {
				rr.Cat(s, r.End());
				break;
			}
		}
		r = rr;
	}
	while(again);
	return r;
}

int CharFilter20toHash(int c)
{
	return c == 20 ? '#' : c;
}

String GetPreprocessedTemplate(const String& name, int lang)
{
	String id = "MAIN";
	String file = name;
	int q = file.Find(':');
	if(q >= 0) {
		id = file.Mid(q + 1);
		file = file.Mid(0, q);
	}
	VectorMap<String, String> def = GetTemplateDefs(file, lang);
	String r = Filter(ReplaceVars(def.Get(id, Null), def, '#'), CharFilter20toHash);
	return Join(Split(r, CharFilterIsCrLf), "\r\n");
}

};