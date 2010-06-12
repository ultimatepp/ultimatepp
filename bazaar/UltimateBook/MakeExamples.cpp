#include <Core/Core.h>
#include <ide/Browser/Browser.h>
#include <Functions4U/Functions4U.h>

using namespace Upp;

String CppAsQtf(const String& str)
{
	static const char *cpp[] = {
		"__asm", "else", "struct",
		"enum", "switch", "auto", "__except", "template",
		"explicit", "this",
		"bool", "extern", "mutable", "thread",
		"break", "false", "throw",
		"case", "__fastcall", "namespace", "true",
		"catch", "__finally", "new", "try",
		"__cdecl", "float", "__try",
		"char", "for", "operator", "typedef",
		"class", "friend", "private", "typeid",
		"const", "goto", "protected", "typename",
		"const_cast", "if", "public", "union",
		"continue", "inline", "register", "unsigned",
		"__declspec", "__inline", "reinterpret_cast", "using",
		"using", "default", "int", "return",
		"delete", "__int8", "short", "__uuidof",
		"dllexport", "__int16", "signed", "virtual",
		"dllimport", "__int32", "sizeof", "void",
		"do", "__int64", "static", "volatile",
		"double", "__leave", "static_cast",
		"dynamic_cast", "long", "__stdcall", "while",
		"include", "define", "ifdef", "ifndef", "endif",
		NULL
	};
	static Index<String> keyword;
	if(keyword.GetCount() == 0) 
		for(const char **q = cpp; *q; q++)
			keyword.Add(*q);

	const char *s = str;
	StringBuffer qtf;
	qtf << "[l0r0<C1 ";
	while(*s)
	{
		if(iscib(*s)) {
			const char *b = s;
			while(iscid(*s))
				s++;
			String id(b, s);
			if(keyword.Find(id) >= 0)
				qtf << "[@B " << DeQtf(id) << "]";
			else
				qtf << DeQtf(id);
		}
		else {
			if(*s == '\t')
				qtf << "____";
			else
			if(*s == '\n')
				qtf << '&';
			else
			if((byte)*s >= 32)
				if(strchr("!+-*^/%~&|=[]:?<>.#", *s))
					qtf << "[@B `" << *s << "]";
				else
					qtf << '`' << *s;
			s++;
		}
	}
	return qtf;
}

void AddFiles(String& qtf, const String& dir, const char* ext, bool& b)
{
	FindFile ff(AppendFileName(dir, "*." + String(ext)));
	while(ff) {
		qtf << "[A4* " << DeQtf(ff.GetName()) << "]&&"
		    << CppAsQtf(LoadFile(AppendFileName(dir, ff.GetName())))
		    << "&&&";
		ff.Next();
		b = true;
	}
}

struct Isort {
	bool operator()(const String& a, const String& b) const
	{
		return ToUpper(a) < ToUpper(b);
	}
};

String MakeExamples(const char *webdir, const char *dir, const char *www, String language, GatherTpp &tt)
{
	String ttxt;
	FindFile ff(AppendFileName(dir, "*.*"));
	ttxt << "{{1:3 ";
	bool next = false;
	Vector<String> ls;
	while(ff) {
		if(ff.IsFolder())
			ls.Add(ff.GetName());
		ff.Next();
	}
	Sort(ls, Isort());
	for(int i = 0; i < ls.GetCount(); i++) {
		String name = ls[i];
		String link = String().Cat() << www << '$' << name << "$" << language << ".html";
		Topic& topic = tt.AddTopic(link);
		topic.title = name;
		String fn = AppendFileName(
						AppendFileName(
							webdir,
							String(www) + ".tpp"
						),
						topic.title + "$" + language  + ".tpp"
					);
		String h = ReadTopic(LoadFile(fn)).text;
		Package p;
		p.Load(AppendFileName(AppendFileName(dir, name), name + ".upp"));
		topic.text << "[R6* " << name << "]&&" << DeQtf(p.description) << "&";
		if(h.GetCount())
			topic.text << h;
		topic.text << "[A2<l0r0 &&";
		String d = AppendFileName(dir, name);
		bool b = false;
		AddFiles(topic.text, d, "h", b);
		AddFiles(topic.text, d, "hpp", b);
		AddFiles(topic.text, d, "cpp", b);
		AddFiles(topic.text, d, "usc", b);
		AddFiles(topic.text, d, "lay", b);
		AddFiles(topic.text, d, "key", b);
		AddFiles(topic.text, d, "brc", b);
		AddFiles(topic.text, d, "sch", b);
		AddFiles(topic.text, d, "xml", b);
		if(b) {
			if(next)
				ttxt << "\n::^ ";
			ttxt << "[^" << link << "^ " << DeQtf(topic.title) << "]::^ "
			     << DeQtf(p.description);
			next = true;
		}
	}
	ttxt << "}}&&";
	return ttxt;
}