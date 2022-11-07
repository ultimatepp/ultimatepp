#include "ide.h"

#if 0
#define LDUMP(x)     DDUMP(x)
#define LDUMPC(x)    DDUMPC(x)
#define LLOG(x)      DLOG(x)
#else
#define LDUMP(x)
#define LDUMPC(x)
#define LLOG(x)
#endif

bool IsPif(const String& l)
{
	return l.Find("#if") >= 0;
}

bool IsPelse(const String& l)
{
	return l.Find("#el") >= 0;
}

bool IsPendif(const String& l)
{
	return l.Find("#endif") >= 0;
}

bool Ide::OpenLink(const String& s, int pos)
{ // try to find link at cursor, either http, https or file
	auto IsLinkChar = [](int c) { return findarg(c, '\'', '\"', '\t', ' ', '\0') < 0; };
	int b = pos;
	while(b > 0 && IsLinkChar(s[b - 1]))
		b--;
	int e = pos;
	while(IsLinkChar(s[e]))
		e++;
	String link = s.Mid(b, e - b);
	if(link.StartsWith("http://") || link.StartsWith("https://"))
		LaunchWebBrowser(link);
	else
	if(FileExists(link))
		EditFile(link);
	else
		return false;
	return true;
}

String Ide::GetRefId(int pos, String& name, Point& ref_pos)
{
	int ci = 0;
	name = editor.ReadIdBack(pos);
	String ref_id;
	int lp = pos;
	int li = editor.GetLinePos(lp);
	for(int pass = 0; pass < 2 && IsNull(ref_id); pass++)
		for(const ReferenceItem& m : editor.references) {
			if(m.pos.y == li && m.pos.x <= lp && m.pos.x >= ci &&
			   (GetNameFromId(m.id) == name || pass == 1)) {
				ref_id = m.id;
				ref_pos = m.ref_pos;
				ci = m.pos.x;
			}
		}
	return ref_id;
}

void Ide::ContextGoto0(int pos)
{
	if(designer)
		return;
	int lp = pos;
	int li = editor.GetLinePos(lp);
	String l = editor.GetUtf8Line(li);
	if(OpenLink(l, lp))
		return;
	if(IsPif(l) || IsPelse(l)) {
		int lvl = 0;
		while(li + 1 < editor.GetLineCount()) {
			l = editor.GetUtf8Line(++li);
			if(IsPif(l))
				lvl++;
			if(IsPelse(l) && lvl == 0)
				break;
			if(IsPendif(l)) {
				if(lvl == 0) break;
				lvl--;
			}
		}
		AddHistory();
		editor.SetCursor(editor.GetPos64(li));
		return;
	}
	if(IsPendif(l)) {
		int lvl = 0;
		while(li - 1 >= 0) {
			l = editor.GetUtf8Line(--li);
			if(IsPif(l)) {
				if(lvl == 0) break;
				lvl--;
			}
			if(IsPendif(l))
				lvl++;
		}
		AddHistory();
		editor.SetCursor(editor.GetPos64(li));
		return;
	}
	int cr = editor.Ch(pos);
	int cl = editor.Ch(pos - 1);
	if(!IsAlNum(cr)) {
		if(islbrkt(cr)) {
			AddHistory();
			editor.MoveNextBrk(false);
			return;
		}
		if(isrbrkt(cr)) {
			AddHistory();
			editor.MovePrevBrk(false);
			return;
		}
		if(islbrkt(cl)) {
			AddHistory();
			editor.MoveNextBrk(false);
			return;
		}
		if(isrbrkt(cl)) {
			AddHistory();
			editor.MovePrevBrk(false);
			return;
		}
	}
	try {
		CParser p(l);
		if(p.Char('#') && p.Id("include")) {
			String path = Hdepend::FindIncludeFile(p.GetPtr(), GetFileFolder(editfile), SplitDirs(GetIncludePath()));
			if(!IsNull(path)) {
				AddHistory();
				EditFile(path);
				editor.SetCursor(0);
				AddHistory();
			}
			return;
		}
	}
	catch(CParser::Error) {}
	
	if(!editor.WaitCurrentFile())
		return;

	String name;
	Point  ref_pos;
	String ref_id = GetRefId(pos, name, ref_pos);
	
	PutAssist("ref_id: " + ref_id);
	
	if(ref_id.GetCount()) {
		String found_path;
		Point  found_pos(INT_MAX, INT_MAX);
		bool   found_definition = false;
		String found_name;
		String found_nest;
		
		AnnotationItem cm = editor.FindCurrentAnnotation(); // what function body are we in?
		PutAssist("Context: " + cm.id);
		if(IsFunction(cm.kind)) { // do local variables
			for(const AnnotationItem& m : editor.locals) {
				int ppy = -1;
				if(m.id == ref_id && m.pos.y >= cm.pos.y && m.pos.y <= li && m.pos.y > ppy) {
					ppy = m.pos.y;
					found_path = editfile;
					found_pos = m.pos;
					found_definition = m.definition;
					found_name = m.name;
					found_nest = m.nest;
					PutAssist("Found Local: " + AsString(m.pos));
					if(ref_pos == m.pos)
						break;
				}
			}
		}
		
		if(IsNull(found_path))
			for(const auto& f : ~CodeIndex())
				for(const AnnotationItem& m : f.value.items)
					if(m.id == ref_id &&
					   (IsNull(found_path) ||
					    CombineCompare(found_definition, m.definition)(f.key, found_path)
					                  (m.pos.y, found_pos.y)(m.pos.x, found_pos.x) < 0)) {
						found_path = f.key;
						found_pos = m.pos;
						found_definition = m.definition;
						found_name = m.name;
						found_nest = m.nest;
						PutAssist("Found Global: " + found_path);
					}
		
		if(found_path.GetCount()) {
			AddHistory();
			EditFile(found_path);
			LayDesigner *l = dynamic_cast<LayDesigner *>(~designer);
			if(l) {
				String cls = found_nest;
				int q = cls.ReverseFind(':');
				if(q >= 0)
					cls = cls.Mid(q + 1);
				if(cls.TrimStart("With")) {
					l->FindLayout(cls, found_name);
					return;
				}
				else
				if(found_name.TrimStart("SetLayout_")) {
					l->FindLayout(found_name, Null);
					return;
				}
				DoEditAsText(found_path);
			}
			IdeIconDes *k = dynamic_cast<IdeIconDes *>(~designer);
			if(k) {
				k->FindId(found_name);
				return;
			}
			GotoPos(found_pos);
			AddHistory();
		}
	}
}

void Ide::ContextGoto()
{
	ContextGoto0(editor.GetCursor());
}

void Ide::CtrlClick(int64 pos)
{
	if(pos < INT_MAX)
		ContextGoto0((int)pos);
}

void Ide::FindDesignerItemReferences(const String& id, const String& name)
{
	String path = NormalizePath(editfile);
	int q = CodeIndex().Find(path);
	if(q >= 0) {
		AnnotationItem cm;
		for(const AnnotationItem& m : CodeIndex()[q].items)
			if(m.id.EndsWith(id) &&
			   (m.id.GetCount() <= id.GetCount() || !iscid(m.id[m.id.GetCount() - id.GetCount() - 1]))) {
				cm = m;
				break;
			}
		if(cm.id.GetCount()) {
			Vector<Tuple<String, Point>> set;
			for(const auto& f : ~CodeIndex()) {
				if(f.key != path) {
					for(const AnnotationItem& m : f.value.items)
						if(FindId(m.type, cm.id) >= 0 || FindId(m.bases, cm.id) >= 0 || FindId(m.pretty, cm.id) >= 0)
							set.Add({ f.key, m.pos });
					for(const ReferenceItem& m : f.value.refs)
						if(FindId(m.id, cm.id) >= 0)
							set.Add({ f.key, m.pos });
				}
			}
			if(set.GetCount()) {
				if(set.GetCount() > 1) {
					SetFFound(ffoundi_next);
					FFound().Clear();
					Index<String> unique;
					for(auto& m : set)
						AddReferenceLine(m.a, m.b, name, unique);
					SortByKey(CodeIndex());
					FFoundFinish();
				}
				else
					GotoPos(set[0].a, set[0].b);
				return;
			}
		}
	}
	Exclamation("No usage has been found.");
}
