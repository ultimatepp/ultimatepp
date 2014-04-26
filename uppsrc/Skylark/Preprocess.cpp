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
			if(IsNull(path)) {
#ifdef _DEBUG
				return "<hr/><h1>Missing template file " + EscapeHtml(String(file)) + "</h1>$set()<hr/>";
#else
				throw Exc("missing template " + String(file));
#endif
			}
		}
	}
	SKYLARKLOG("Loading template file " << path);
	FileIn in(path);
	String r;
	int lineno = 0;
	while(in && !in.IsEof()) {
		String line = in.GetLine();
		lineno++;
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
			r << CParser::LineInfoComment(path, lineno) << line;
		r << "\n";
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
			def[ti] << p.GetLineInfoComment() << p.GetPtr();
		}
		else
			def[ti] << line << "\n";
	}
	return def;
}

String ReplaceVars(const String& src, const VectorMap<String, String>& def, int chr, Index<String>& expanded)
{
	String r;
	const char *b = src;
	const char *s = b;
	for(;;) {
		s = strchr(s, chr);
		if(s) {
			r.Cat(b, s);
			if(s[1] == chr) {
				r.Cat(chr);
				b = s = s + 2;
			}
			else {
				b = s;
				CParser p(++s);
				if(p.IsId()) {
					String id = p.ReadId();
					int q = def.Find(id);
					if(q >= 0 && expanded.Find(id) < 0) {
						int ii = expanded.GetCount();
						expanded.Add(id); // prevent the same macro to be expanded again
						if(expanded.GetCount() < 20) // maximum number of nested expansion - recursion limit
							r.Cat(ReplaceVars(def[q], def, chr, expanded));
						b = s = p.GetSpacePtr();
						expanded.Trim(ii);
					}
				}
			}
		}
		else {
			r.Cat(b, src.End());
			return r;
		}
	}
}

String ReplaceVars(const String& src, const VectorMap<String, String>& def, int chr)
{
	Index<String> expanded;
	return ReplaceVars(src, def, chr, expanded);
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
	String r = ReplaceVars(def.Get(id, Null), def, '#');
	return Join(Split(r, '\n', false), "\r\n");
}

};