#include "CodeEditor.h"

namespace Upp {

ArrayMap<String, EditorSyntax::SyntaxDef>& EditorSyntax::defs()
{
	static ArrayMap<String, SyntaxDef> d;
	return d;
}

void EditorSyntax::Register(const char *id, Event<One<EditorSyntax>&> factory,
                            const char *exts, const char *description)
{
	SyntaxDef& f = defs().GetAdd(id);
	f.factory = factory;
	f.patterns = exts;
	f.description = description;
}

One<EditorSyntax> EditorSyntax::Create(const char *id)
{
	One<EditorSyntax> s;
	SyntaxDef *f = defs().FindPtr(id);
	if(f)
		f->factory(s);
	if(!s)
		s.Create();
	return s;
}

String EditorSyntax::GetSyntaxForFilename(const char *fn)
{
	ArrayMap<String, SyntaxDef>& d = defs();
	for(int i = 0; i < d.GetCount(); i++)
		if(PatternMatchMulti(d[i].patterns, fn))
			return d.GetKey(i);
	return Null;
}

String EditorSyntax::GetSyntaxDescription(int i)
{
	String d = defs()[i].description;
	String x = defs()[i].patterns;
	if(x.GetCount()) {
		x.Replace("*", "");
		d << " (" << TrimBoth(x) << ")";
	}
	return d;
}

}
