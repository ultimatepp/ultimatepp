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

bool GetIdScope(String& os, const String& scope, const String& id, Index<String>& done)
{
	if(done.Find(scope) >= 0)
		return Null;
	done.Add(scope);
	Vector<String> tparam;
	String n = ParseTemplatedType(scope, tparam);
	int q = CodeBase().Find(n);
	if(q < 0)
		return Null;
	const Array<CppItem>& m = CodeBase()[q];
	Vector<String> r;
	if(FindName(m, id) >= 0) {
		os = n;
		return true;
	}
	for(int i = 0; i < m.GetCount(); i++) {
		const CppItem& im = m[i];
		if(im.IsType()) {
			Vector<String> b = Split(im.qptype, ';');
			ResolveTParam(b, tparam);
			for(int i = 0; i < b.GetCount(); i++) {
				if(GetIdScope(os, b[i], id, done))
					return true;
			}
		}
	}
	return false;
}

bool GetIdScope(String& os, const String& scope, const String& id)
{
	Index<String> done;
	return GetIdScope(os, scope, id, done);
}

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
		if(pos >= h || findarg(editor[pos], ';', '{', '}') >= 0)
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

String RemoveTemplateParams(const String& s)
{
	Vector<String> dummy;
	return ParseTemplatedType(s, dummy);
}

void Ide::ContextGoto0(int pos)
{
	if(designer)
		return;
	int li = editor.GetLine(pos);
	String l = editor.GetUtf8Line(li);
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
		editor.SetCursor(editor.GetPos(li));
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
		editor.SetCursor(editor.GetPos(li));
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
	CParser p(l);
	if(p.Char('#') && p.Id("include")) {
		String path = FindIncludeFile(p.GetPtr(), GetFileFolder(editfile));
		if(!IsNull(path)) {
			AddHistory();
			EditFile(path);
			editor.SetCursor(0);
			AddHistory();
		}
		return;
	}
	int q = pos;
	while(iscid(editor.Ch(q - 1)))
		q--;
	String tp;
	Vector<String> xp = editor.ReadBack(q); // try to load expression like "x[i]." or "ptr->"
	Index<String> type;
	Parser parser;
	int ci = pos;
	for(;;) {
		int c = editor.Ch(ci);
		if(c == '{' && editor.Ch(ci + 1)) {
			ci++;
			break;
		}
		if(c == '}' || c == 0 || c == ';')
			break;
		ci++;
	}
	editor.Context(parser, ci);
	if(xp.GetCount()) {
		type = editor.EvaluateExpressionType(parser, xp);
		if(type.GetCount() == 0)
			return;
	}
	
	String id = editor.GetWord(pos);
	if(id.GetCount() == 0)
		return;
	
	String qual; // Try to load type qualification like Foo::Bar, Vector<String>::Iterator
	while(editor.Ch(q - 1) == ' ')
		q--;
	if(editor.Ch(q - 1) == ':' && editor.Ch(q - 2) == ':') {
		q -= 3;
		while(q >= 0) {
			int c = editor.Ch(q);
			if(iscid(c) || findarg(c, '<', '>', ':', ',', ' ') >= 0) {
				if(c != ' ')
					qual = (char)c + qual;
				q--;
			}
			else
				break;
		}
		if(qual.GetCount() == 0)
			qual = ":";
	}

	Vector<String> scope;
	Vector<bool> istype; // prefer type (e.g. struct Foo) over constructor (Foo::Foo())

	for(int i = 0; i < type.GetCount(); i++) { // 'x.attr'
		Index<String> done;
		String r;
		if(GetIdScope(r, type[i], id, done)) {
			Vector<String> todo;
			todo.Add(r);
			while(scope.GetCount() < 100 && todo.GetCount()) { // Add base classes
				String t = todo[0];
				todo.Remove(0);
				if(t.EndsWith("::"))
					t.Trim(t.GetCount() - 2);
				if(t.GetCount()) {
					scope.Add(t);
					istype.Add(false);
					ScopeInfo f(CodeBase(), t); // Try base classes too!
					todo.Append(f.GetBases());
				}
			}
		}
	}
	
	Vector<String> ns = parser.GetNamespaces();

	if(qual.GetCount() > 2 && qual.StartsWith("::"))
		qual = qual.Mid(2);
	if(qual.GetCount()) { // Ctrl::MOUSELEFT, Vector<String>::Iterator
		Vector<String> todo;
		String qa = Qualify(CodeBase(), parser.current_scope, *qual == ':' ? id : qual + "::" + id,
		                    parser.context.namespace_using);
		qa = RemoveTemplateParams(qa);
		if(CodeBase().Find(qa) < 0) { // Upp::FileTabs::RenameFile
			int q = qa.ReverseFind("::");
			if(q > 0) {
				String h = qa.Mid(0, q);
				if(CodeBase().Find(h) >= 0) {
					scope.Add(h);
					istype.Add(false);
				}
			}
			else {
				scope.Add(Null); // Add global namespace
				istype.Add(false);
			}
		}
		todo.Add(qa);
		while(scope.GetCount() < 100 && todo.GetCount()) {
			String t = todo[0];
			if(t.EndsWith("::"))
				t.Trim(t.GetCount() - 2);
			todo.Remove(0);
			if(CodeBase().Find(t) >= 0) { // Ctrl::MOUSELEFT
				scope.Add(t);
				istype.Add(true);
			}
			String tt = t;
			tt << "::" << id;
			if(CodeBase().Find(tt) >= 0) { // Vector<String>::Iterator
				scope.Add(tt);
				istype.Add(true);
			}
			ScopeInfo f(CodeBase(), t); // Try base classes too!
			todo.Append(f.GetBases());
		}
	}
	else {
		Vector<String> todo;
		todo.Add(parser.current_scope);
		while(scope.GetCount() < 100 && todo.GetCount()) { // Add base classes
			String t = todo[0];
			todo.Remove(0);
			if(t.EndsWith("::"))
				t.Trim(t.GetCount() - 2);
			if(t.GetCount()) {
				scope.Add(t);
				istype.Add(false);
				ScopeInfo f(CodeBase(), t); // Try base classes too!
				todo.Append(f.GetBases());
			}
		}
		if(xp.GetCount() == 0) {
			q = parser.local.Find(id);
			if(q >= 0) { // Try locals
				AddHistory();
				editor.SetCursor(editor.GetPos(parser.local[q].line - 1));
				FindId(id);
				return;
			}
		}
		// Can be unqualified type name like 'String'
		String t = RemoveTemplateParams(Qualify(CodeBase(), parser.current_scope, id, parser.context.namespace_using));
		for(int i = 0; i < ns.GetCount(); i++) {
			String tt = Merge("::", ns[i], t);
			if(CodeBase().Find(tt) >= 0) {
				scope.Add(tt);
				istype.Add(true);
			}
		}
	}
	
	Vector<String> usings = Split(parser.context.namespace_using, ';');
	usings.Add(""); // Add global namespace too
	
	Index<String> done;
	for(int i = 0; i < ns.GetCount(); i++) {
		String r;
		if(GetIdScope(r, ns[i], id, done)) {
			scope.Add(r);
			istype.Add(false);
		}
	}

	for(int j = 0; j < scope.GetCount(); j++) {
		q = CodeBase().Find(scope[j]);
		if(q >= 0) {
			int ii = -1;
			const Array<CppItem>& n = CodeBase()[q];
			for(int i = 0; i < n.GetCount(); i++) {
				const CppItem& m = n[i];
				if(n[i].name == id) {
					if(ii < 0)
						ii = i;
					else {
						const CppItem& mm = n[ii];
						if(CombineCompare(findarg(mm.kind, CONSTRUCTOR) < 0, findarg(m.kind, CONSTRUCTOR) < 0)
						                 (!istype[j] || mm.IsType(), !istype[j] || m.IsType())
							             (findarg(mm.filetype, FILE_CPP, FILE_C) >= 0,
							              findarg(m.filetype, FILE_CPP, FILE_C) >= 0)
							             (mm.line, m.line) < 0)
							ii = i;
					}
				}
			}
			if(ii >= 0) {
				JumpToDefinition(n, ii, scope[j]);
				FindId(id);
				return;
			}
		}
	}
}

void Ide::ContextGoto()
{
	ContextGoto0(editor.GetCursor());
}

void Ide::CtrlClick(int pos)
{
	ContextGoto0(pos);
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
		}
		else {
			editor.SetCursor(editor.GetPos(line - 1));
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

void Ide::JumpToDefinition(const Array<CppItem>& n, int q, const String& scope)
{
	String qitem = n[q].qitem;
	int i = q;
	int qml = 0;
	int qcpp = -1;
	int qcppml = 0;
	int qimpl = -1;
	int qimplml = 0;
	String currentfile = editfile;
	while(i < n.GetCount() && n[i].qitem == qitem) {
		const CppItem& m = n[i];
		int ml = GetMatchLen(editfile, GetSourceFilePath(m.file));
		if(m.impl && ml > qimplml) {
			qimplml = ml;
			qimpl = i;
		}
		if((m.filetype == FILE_CPP || m.filetype == FILE_C) && ml > qcppml) {
			qcpp = i;
			qcppml = ml;
		}
		if(ml > qml) {
			q = i;
			qml = ml;
		}
		i++;
	}
	const CppItem& pos = n[qimpl >= 0 ? qimpl : qcpp >= 0 ? qcpp : q];
	String path = GetSourceFilePath(pos.file);
	editastext.RemoveKey(path);
	editashex.RemoveKey(path);
	if(!GotoDesignerFile(path, scope, pos.name, pos.line))
		GotoCpp(pos);
}

void Ide::GotoFileAndId(const String& path, const String& id)
{
	AddHistory();
	EditFile(path);
	WString wid = id.ToWString();
	if(editor.GetLength() < 100000) {
		for(int i = 0; i < editor.GetLineCount(); i++) {
			WString ln = editor.GetWLine(i);
			int q = ln.Find(wid);
			while(q >= 0) {
				if(q == 0 || !iscid(ln[q - 1]) && !iscid(ln[q + wid.GetCount()])) {
					editor.SetCursor(editor.GetPos(i, q));
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

void IdeGotoFileAndId(const String& path, const String& id)
{
	Ide *ide = dynamic_cast<Ide *>(TheIde());
	if(ide)
		ide->GotoFileAndId(path, id);
}
