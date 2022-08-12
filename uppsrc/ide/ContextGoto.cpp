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

void Ide::FindId(const String& id)
{
	int pos = editor.GetCursor();
	int h = min(editor.GetLength(), pos + 4000);
	for(;;) {
		if(pos >= h || editor[pos] == ';')
			break;
		if(iscib(editor[pos])) {
			int p0 = pos;
			String tid;
			while(pos < h && iscid(editor[pos])) {
				tid.Cat(editor[pos]);
				pos++;
			}
			if(tid == id) {
				editor.SetCursor(p0);
				return;
			}
		}
		else
			pos++;
	}
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

	String ref_id;
	int ci = 0;
	String id = editor.ReadIdBack(pos);
	for(int pass = 0; pass < 2; pass++)
		for(const ReferenceItem& m : editor.references) {
			if(m.pos.y == li && m.pos.x <= lp && m.pos.x >= ci &&
			   (m.id.Mid(max(m.id.ReverseFind(':'), 0)) == id || pass == 1)) {
				ref_id = m.id;
				ci = m.pos.x;
			}
		}

	if(ref_id.GetCount()) {
		String found_path;
		Point  found_pos(INT_MAX, INT_MAX);
		bool   found_definition = false;
		
		for(const auto& f : ~CodeIndex())
			for(const AnnotationItem& m : f.value.items) {
				if(m.id == ref_id &&
				   (IsNull(found_path) ||
				    CombineCompare(found_definition, m.definition)(f.key, found_path)
				                  (m.pos.y, found_pos.y)(m.pos.x, found_pos.x) < 0)) {
					found_path = f.key;
					found_pos = m.pos;
					found_definition = m.definition;
				}
			}
		if(found_path.GetCount())
			GotoPos(found_path, found_pos);
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

bool Ide::GotoDesignerFile(const String& path, const String& scope, const String& name, int line)
{
	if(ToLower(GetFileExt(path)) == ".lay") {
		AddHistory();
		EditFile(path);
		LayDesigner *l = dynamic_cast<LayDesigner *>(~designer);
		if(l) {
			if(scope.StartsWith("With"))
				l->FindLayout(scope.Mid(4), name);
			else
			if(name.StartsWith("SetLayout_"))
				l->FindLayout(name.Mid(10), Null);
		}
		else {
			editor.SetCursor(editor.GetPos64(line - 1));
			editor.TopCursor(4);
			editor.SetFocus();
		}
		AddHistory();
		return true;
	}
	else
	if(ToLower(GetFileExt(path)) == ".iml") {
		AddHistory();
		EditFile(path);
		IdeIconDes *l = dynamic_cast<IdeIconDes *>(~designer);
		if(l)
			l->FindId(name);
		else
			editor.SetFocus();
		AddHistory();
		return true;
	}
	return false;
}

void Ide::GotoFileAndId(const String& path, const String& id)
{
	AddHistory();
	EditFile(path);
	WString wid = id.ToWString();
	if(editor.GetLength64() < 100000) {
		for(int i = 0; i < editor.GetLineCount(); i++) {
			WString ln = editor.GetWLine(i);
			int q = ln.Find(wid);
			while(q >= 0) {
				if(q == 0 || !iscid(ln[q - 1]) && !iscid(ln[q + wid.GetCount()])) {
					editor.SetCursor(editor.GetPos64(i, q));
					editor.CenterCursor();
					return;
				}
				if(q + 1 >= ln.GetCount())
					break;
				q = ln.Find(wid, q + 1);
			}
		}
	}
	AddHistory();
}
