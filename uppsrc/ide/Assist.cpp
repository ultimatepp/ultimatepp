#include "ide.h"

#define LDUMP(x)    //DDUMP(x)
#define LDUMPC(x)   //DUMPC(x)
#define LLOG(x)     //DLOG(x)

class IndexSeparatorFrameCls : public CtrlFrame {
	virtual void FrameLayout(Rect& r)                   { r.right -= 1; }
	virtual void FramePaint(Draw& w, const Rect& r) {
		w.DrawRect(r.right - 1, r.top, 1, r.Height(), SColorShadow);
	}
	virtual void FrameAddSize(Size& sz) { sz.cx += 2; }
};

AssistEditor::AssistEditor()
{
	assist.NoHeader();
	assist.NoGrid();
	assist.AddColumn().Margin(0).SetDisplay(Single<CppItemInfoDisplay>());
	assist.WhenLeftClick = THISBACK(AssistInsert);
	type.NoHeader();
	type.NoGrid();
	type.AddColumn();
	type.WhenCursor = THISBACK(SyncAssist);
	type.NoWantFocus();
	popup.Horz(type, assist);
	popup.SetPos(2000);
	auto_assist = true;
	commentdp = false;

	InsertFrame(1, indexframe);
	indexframe.Left(indexpane, HorzLayoutZoom(140));
	int q = indexpane.GetStdSize().cy;
	indexpane.Add(searchindex.HSizePos().TopPos(0, q));
	indexpane.Add(index.HSizePos().VSizePos(q, 0));
	index.AddColumn("").Margin(2);
	index.AddIndex();
	index.NoHeader().AutoHideSb().NoGrid();
	index.SetFrame(Single<IndexSeparatorFrameCls>());
	index.WhenLeftClick = index.WhenLeftDouble = THISBACK(IndexClick);
	searchindex.SetFilter(CharFilterAlphaToUpper);
	searchindex <<= THISBACK(SearchIndex);
	showindex = true;
}

Vector<String> TemplatePars(const String& type)
{
	Vector<String> r;
	int q = type.Find('<');
	if(q >= 0) {
		const char *s = ~type + q + 1;
		int lvl = 0;
		String t;
		while(*s && (lvl || *s != '>')) {
			if(*s == '<')
				lvl++;
			if(*s == '>')
				lvl--;
			if(*s == ',') {
				r.Add(t);
				t.Clear();
				s++;
			}
			else
				t.Cat(*s++);
		}
		r.Add(t);
	}
	return r;
}

void SubstituteTpars(Vector<String>& type, const String& tname)
{
	Vector<String> tp = TemplatePars(tname);
	for(int i = 0; i < type.GetCount(); i++)
		if(IsDigit(type[i][0])) {
			int q = atoi(type[i]);
			if(q >= 0 && q < tp.GetCount()) {
				LLOG(type[i] << " -> " << tp[q]);
				type[i] = tp[q];
			}
		}
}

bool AssistEditor::NestId(const CppNest& n, const String& id, Vector<String>& type, bool& code,
                          String& t)
{
	int q = n.name.Find(NoTemplatePars(id));
	if(q >= 0) {
		do {
			const CppItem& m = n[q];
			const String& qt = m.qtype;
			if(!IsNull(qt) && FindIndex(type, qt) < 0 && (m.IsCode() || m.IsData())) {
				type.Add(qt);
				if(m.IsCode())
					code = true;
			}
			q = n.name.FindNext(q);
		}
		while(q >= 0);
		return true;
	}
	q = n.key.Find("class");
	if(q < 0)
		q = n.key.Find("struct");
	if(q < 0)
		q = n.key.Find("typedef");
	LDUMP(q);
	if(q >= 0) {
		Vector<String> base = Split(n[q].qptype, ';');
		LDUMP(id);
		LDUMPC(base);
		SubstituteTpars(base, t);
		for(int i = 0; i < base.GetCount(); i++) {
			q = BrowserBase().Find(NoTemplatePars(base[i]));
			String h = base[i];
			if(q >= 0 && NestId(BrowserBase()[q], id, type, code, h)) {
				t = h;
				return true;
			}
		}
	}
	return false;
}

void AssistScanError(int line, const String& text)
{
#ifdef _DEBUG
	PutVerbose(String().Cat() << "(" << line << "): " << text);
#endif
}

void AssistEditor::Context(Parser& parser, int pos)
{
	theide->ScanFile();
	String s = Get(0, pos);
	StringStream ss(s);
	parser.dobody = true;
	parser.Do(ss, IgnoreList(), BrowserBase(), Null, callback(AssistScanError));
	QualifyTypes(BrowserBase(), parser.current_nest, parser.current);
	inbody = parser.IsInBody();
#ifdef _DEBUG
	PutVerbose("body: " + AsString(inbody));
	PutVerbose("nest: " + AsString(parser.current_nest));
#endif
}

void AssistEditor::TypeOf(const String& id, Vector<String>& r, bool& code)
{
	Parser parser;
	Context(parser, GetCursor());
	code = false;
	if(id == "this") {
		r.Add(parser.current_nest);
		return;
	}
	if(IsNull(id)) {
		r.Add(parser.current_nest);
		if(parser.current_nest != "::")
			r.Add("::");
		return;
	}
	int q = parser.local.FindLast(id);
	if(q >= 0) {
		r.Add(parser.local[q]);
		return;
	}
	Vector<String> p = Split(parser.current.pname, ';', false);
	q = FindIndex(p, id);
	if(q >= 0) {
		p = Split(parser.current.qptype, ';');
		if(q < p.GetCount())
			r.Add(p[q]);
		return;
	}
	String n = parser.current_nest;
	for(;;) {
		String type = n + "::" + id;
		if(BrowserBase().Find(type)) {
			code = true;
			r.Add(type);
		}
		int q = n.ReverseFind(':');
		if(q < 1 || n[q - 1] != ':')
			break;
		n.Trim(q - 1);
	}
	q = BrowserBase().Find(parser.current_nest);
	String dummy;
	if(q >= 0 && NestId(BrowserBase()[q], id, r, code, dummy))
		return;
	q = BrowserBase().Find("::");
	if(q >= 0)
		NestId(BrowserBase()[q], id, r, code, dummy);
}

Vector<String> AssistEditor::Operator(const char *oper, const Vector<String>& type)
{
	Vector<String> nt;
	bool d;
	for(int i = 0; i < type.GetCount(); i++) {
		String t = type[i];
		int q = BrowserBase().Find(NoTemplatePars(t));
		if(q < 0 || !NestId(BrowserBase()[q], oper, nt, d, t))
			nt.Add(type[i]);
		SubstituteTpars(nt, t);
	}
	return nt;
}

Vector<String> AssistEditor::TypeOf(const Vector<String>& xp, const String& tp)
{
	Vector<String> type;
	bool code;
	if(!IsNull(tp)) {
		Parser parser;
		Context(parser, GetCursor());
		type.Add(Qualify(BrowserBase(), parser.current_nest, tp));
	}
	else {
		if(xp.GetCount() == 0 || !iscid(xp[0][0]))
			return type;
		TypeOf(xp[0], type, code);
	}
	LDUMPC(xp);
	LDUMPC(type);
	int q = 1;
	if(code && xp.GetCount() > 1 && xp[1][0] == '(')
		q++;
	while(q < xp.GetCount() && type.GetCount()) {
		int c = xp[q][0];
		LDUMP(xp[q]);
		if(iscid(c)) {
			Vector<String> nt;
			bool code = false;
			for(int i = 0; i < type.GetCount(); i++) {
				String t = type[i];
				int j = BrowserBase().Find(NoTemplatePars(t));
				if(j >= 0)
					NestId(BrowserBase()[j], xp[q], nt, code, t);
				LDUMPC(nt);
				SubstituteTpars(nt, t);
			}
			if(code && q + 1 < xp.GetCount() && xp[q + 1][0] == '(')
				q++;
			type = nt;
		}
		if(c == '(')
			type = Operator("operator()", type);
		if(c == '[')
			type = Operator("operator[]", type);
		if(c == '-')
			type = Operator("operator->", type);
		q++;
	}
	LDUMPC(type);
	return type;
}

int CharFilterT(int c)
{
	return c >= '0' && c <= '9' ? "TUVWXYZMNO"[c - '0'] : c;
}

void AssistEditor::GatherItems(const String& type, bool nom, Index<String>& in_types, bool tp)
{
	LLOG("GatherItems " << type);
	if(in_types.Find(type) >= 0) {
		LLOG("-> recursion, exiting");
		return;
	}
	in_types.Add(type);
	String ntp = NoTemplatePars(type);
	int q = BrowserBase().Find(ntp);
	LDUMP(q);
	if(q < 0)
		return;
	if(tp) {
		if(ntp != "::")
			ntp << "::";
		int typei = assist_type.FindAdd("<types>");
		for(int i = 0; i < BrowserBase().GetCount(); i++) {
			String n = BrowserBase().GetKey(i);
			if(n.GetLength() > ntp.GetLength() && memcmp(~ntp, ~n, ntp.GetLength()) == 0) {
				LDUMP(n);
				CppNest& m = BrowserBase()[i];
				for(int i = 0; i < m.GetCount(); i++) {
					const CppItem& im = m[i];
					String name = m.name[i];
					if(im.IsType()) {
						CppItemInfo& f = assist_item.Add(name);
						f.name = name;
						f.typei = typei;
						(CppSimpleItem&)f = im;
						break;
					}
				}
			}
		}
	}
	const CppNest& m = BrowserBase()[q];
	String base;
	int typei = assist_type.FindAdd(ntp);
	for(int i = 0; i < m.GetCount(); i++) {
		const CppItem& im = m[i];
		String name = m.name[i];
		if(im.IsType())
			base = im.qptype;
		LDUMP(name);
		if((im.IsCode() || im.IsData() || im.IsMacro() && type == "::")
		   && (nom || im.access == PUBLIC)) {
			if(im.IsCode()) {
				int q = assist_item.Find(name);
				while(q >= 0) {
					if(assist_item[q].typei != typei)
						assist_item[q].over = true;
					q = assist_item.FindNext(q);
				}
			}
			CppItemInfo& f = assist_item.Add(name);
			f.name = name;
			f.typei = typei;
			(CppSimpleItem&)f = im;
		}
	}
	Vector<String> b = Split(base, ';');
	SubstituteTpars(b, type);
	for(int i = 0; i < b.GetCount(); i++)
		GatherItems(b[i], nom, in_types, tp);
	in_types.Drop();
}

struct CppItemInfoOrder {
	bool operator()(const CppItemInfo& a, const CppItemInfo& b) const {
		return CombineCompare(a.name, b.name)(a.natural, b.natural) < 0;
	}
};

void AssistEditor::CloseAssist()
{
	if(popup.IsOpen())
		popup.Close();
	assist_item.Clear();
}

String AssistEditor::ReadIdBack(int q)
{
	String id;
	while(q > 0 && iscid(GetChar(q - 1)))
		q--;
	while(q < GetLength() && iscid(GetChar(q)))
		id << (char)GetChar(q++);
	return id;
}

int AssistEditor::Ch(int i)
{
	return i >= 0 && i < GetLength() ? GetChar(i) : 0;
}

int AssistEditor::ParsBack(int q)
{
	int level = 1;
	--q;
	while(q > 0) {
		if(isrbrkt(Ch(q)))
			level++;
		if(islbrkt(Ch(q)))
			if(--level <= 0)
				break;
		--q;
	}
	return max(0, q);
}

bool IsSpc(int c)
{
	return c > 0 && c <= 32;
}

void AssistEditor::SkipSpcBack(int& q)
{
	while(q > 0 && IsSpc(Ch(q - 1)))
		q--;
}

String AssistEditor::IdBack(int& qq)
{
	int q = qq;
	while(iscid(Ch(q - 1)))
		q--;
	if(!iscib(Ch(q)))
		return Null;
	String r;
	qq = q;
	while(q < GetLength() && iscid(Ch(q)))
		r.Cat(Ch(q++));
	return r;
}

Vector<String> AssistEditor::ReadBack(int q, String& type)
{
	Vector<String> r;
	type.Clear();
	bool wasid = true;
	for(;;) {
		if(r.GetCount() > 200) {
			r.Clear();
			type.Clear();
			break;
		}
		SkipSpcBack(q);
		int c = Ch(q - 1);
		int c1 = Ch(q - 2);
		if(c == ':' && c1 == ':') {
			for(;;) {
				q -= 2;
				SkipSpcBack(q);
				String id = IdBack(q);
				if(IsNull(id))
					break;
				type = id + type;
				SkipSpcBack(q);
				if(Ch(q - 1) != ':' || Ch(q - 2) != ':')
					break;
				type = "::" + type;
			}
			break;
		}
		if(iscid(c)) {
			if(wasid)
				break;
			String id = IdBack(q);
			if(IsNull(id))
				break;
			r.Add() = id;
			wasid = true;
			continue;
		}
		else {
			if(c == ']') {
				if(wasid)
					break;
				r.Add("[]");
				q = ParsBack(q - 1);
				wasid = false;
				continue;
			}
			else
			if(c == ')') {
				if(wasid)
					break;
				r.Add("()");
				q = ParsBack(q - 1);
				wasid = false;
				continue;
			}
			wasid = false;
			c = Ch(q - 1);
			if(c == '>' && Ch(q - 2) == '-') {
				r.Add("->");
				q -= 2;
				continue;
			}
			if(c == '.') {
				r.Add(".");
				q--;
				continue;
			}
		}
		break;
	}
	Reverse(r);
	return r;
}

int memcmp_i(const char *s, const char *t, int n)
{
	while(n--) {
		int q = ToUpper(*s++) - ToUpper(*t++);
		if(q)
			return q;
	}
	return 0;
}

void AssistEditor::SyncAssist()
{
	String name;
	name = ReadIdBack(GetCursor());
	assist.Clear();
	int typei = type.GetCursor() - 1;
	VectorMap<String, int> over;
	for(int p = 0; p < 2; p++)
		for(int i = 0; i < assist_item.GetCount(); i++) {
			const CppItemInfo& m = assist_item[i];
			if((typei < 0 || m.typei == typei) &&
			   (p ? memcmp_i(name, m.name, name.GetCount()) == 0
			        && memcmp(name, m.name, name.GetCount())
			      : memcmp(name, m.name, name.GetCount()) == 0)) {
				int q = over.Find(m.name);
				if(q < 0 || over[q] == m.typei) {
					assist.Add(RawToValue(m));
					if(q < 0)
						over.Add(m.name, m.typei);
				}
			}
		}
}

void AssistEditor::Assist()
{
	if(!assist_active)
		return;
	CloseAssist();
	int q = GetCursor();
	assist_cursor = q;
	while(iscid(Ch(q - 1)))
		q--;
	String tp;
	Vector<String> h = ReadBack(q, tp);
	Vector<String> type;
	bool d;
	if(h.GetCount() == 0 && IsNull(tp))
		TypeOf(Null, type, d);
	else {
		type = TypeOf(h, tp);
		if(type.GetCount() == 0)
			return;
	}
	assist_type.Clear();
	assist_item.Clear();
	for(int i = 0; i < type.GetCount(); i++) {
		Index<String> in_types;
		GatherItems(type[i], h.GetCount() == 0, in_types, !IsNull(tp) || h.GetCount() == 0);
	}
	if(assist_item.GetCount() == 0)
		return;
	Sort(assist_item, CppItemInfoOrder());
	PopUpAssist();
}

void AssistEditor::PopUpAssist(bool auto_insert)
{
	int lcy = max(16, BrowserFont().Info().GetHeight());
	type.Clear();
	type.Add(AttrText("<all>").Ink(SColorHighlight()));
	for(int i = 0; i < assist_type.GetCount(); i++) {
		String s = assist_type[i];
		if(s[0] == ':' && s[1] == ':')
			s = s.Mid(2);
		s = Nvl(s, "<globals>");
		if(s[0] == '<')
			type.Add(AttrText(s).Ink(SColorMark()));
		else
			type.Add(Nvl(s, "<globals>"));
	}
	type.SetCursor(0);
	if(!assist.GetCount())
		return;
	int cy = min(300, lcy * max(type.GetCount(), assist.GetCount()));
	cy += 4;
	cy += HeaderCtrl::GetStdHeight();
	assist.SetLineCy(lcy);
	Point p = GetCaretPoint() + GetScreenView().TopLeft();
	Rect wa = GetWorkArea();
	int cx = min(wa.Width() - 100, HorzLayoutZoom(600));
	Rect r = RectC(p.x, p.y, cx, cy);
	if(p.x + cx > wa.right)
		p.x = wa.right - cx;
	if(p.y + cy + GetFontSize().cy < wa.bottom)
		popup.SetRect(p.x, p.y + GetFontSize().cy, cx, cy);
	else
		popup.SetRect(p.x, p.y - cy, cx, cy);
	popup.BackPaint();
	if(auto_insert && assist.GetCount() == 1) {
		assist.GoBegin();
		AssistInsert();
	}
	else
		popup.Ctrl::PopUp(this, false, false, true);
}

void AssistEditor::Complete()
{
	CloseAssist();
	Index<String> id;
	int l = GetCursorLine() - 1;
	while(l >= 0) {
		String x = GetUtf8Line(l);
		CParser p(x);
		while(!p.IsEof())
			if(p.IsId())
				id.FindAdd(p.ReadId());
			else
				p.SkipTerm();
		l--;
	}
	for(int i = 0; i < id.GetCount(); i++) {
		CppItemInfo& f = assist_item.Add(id[i]);
		f.name = id[i];
		f.natural = id[i];
		f.access = 0;
		f.kind = 100;
	}
	PopUpAssist(true);
}

void AssistEditor::AssistInsert()
{
	if(assist.IsCursor()) {
		const CppItemInfo& f = ValueTo<CppItemInfo>(assist.Get(0));
		String txt = f.name;
		int l = txt.GetCount();
		int pl = txt.GetCount();
		if(f.kind >= FUNCTION && f.kind <= INLINEFRIEND) {
			txt << '(';
			pl = l = txt.GetCount();
			if(IsNull(f.param))
				l++;
			else {
				Vector<String> p = Split(f.param, ';', false);
				for(int i = 0; i < p.GetCount(); i++) {
					if(i) {
						txt << (inbody ? (char)184 : ',');
						txt << ' ';
					}
					txt << p[i];
					if(i == 0)
						pl = txt.GetCount();
				}
			}
			txt << ')';
		}
		int cl = GetCursor();
		int ch = cl;
		while(iscid(Ch(cl - 1)))
			cl--;
		while(iscid(Ch(ch)))
			ch++;
		Remove(cl, ch - cl);
		SetCursor(cl);
		int n = Paste(ToUnicode(txt, CHARSET_WIN1250));
		if(!inbody)
			SetCursor(cl + n);
		else
		if(pl > l)
			SetSelection(cl + l, cl + pl);
		else
			SetCursor(cl + l);
	}
	CloseAssist();
	IgnoreMouseUp();
}

bool AssistEditor::InCode()
{
	int pos = GetCursor();
	int line = GetLinePos(pos);
	SyntaxState st = ScanSyntax(line);
	WString l = GetWLine(line);
	st.ScanSyntax(l, ~l + pos);
	return !st.comment && !st.string && !st.linecomment;
}

bool AssistEditor::Key(dword key, int count)
{
	if(searchindex.HasFocus())
		return IndexKey(key);
	if(popup.IsOpen()) {
		int k = key & ~K_CTRL;
		ArrayCtrl& kt = key & K_CTRL ? type : assist;
		if(k == K_UP || k == K_PAGEUP || k == K_CTRL_PAGEUP || k == K_CTRL_END)
			if(kt.IsCursor())
				return kt.Key(k, count);
			else {
				kt.SetCursor(kt.GetCount() - 1);
				return true;
			}
		if(k == K_DOWN || k == K_PAGEDOWN || k == K_CTRL_PAGEDOWN || k == K_CTRL_HOME)
			if(kt.IsCursor())
				return kt.Key(k, count);
			else {
				kt.SetCursor(0);
				return true;
			}
		if(key == K_ENTER && assist.IsCursor()) {
			AssistInsert();
			return true;
		}
		if(key == K_TAB && !assist.IsCursor() && assist.GetCount()) {
			assist.GoBegin();
			AssistInsert();
			return true;
		}
	}
#ifdef _DEBUG
	if(key == K_ALT_F12) {
		PutConsole("Type is");
		String tp;
		Vector<String> h = ReadBack(GetCursor(), tp);
		Vector<String> r = TypeOf(h, tp);
		PutConsole(tp);
		for(int i = 0; i < r.GetCount(); i++)
			PutConsole(r[i]);
	}
#endif
	int c = GetCursor();
	int cc = GetChar(c);
	int bcc = c > 0 ? GetChar(c - 1) : 0;
	bool b = CodeEditor::Key(key, count);
	if(assist.IsOpen()) {
		if(!iscid(key) &&
		   !(iscid(cc) && (key == K_DELETE || key == K_RIGHT)) &&
		   !(iscid(bcc) && (key == K_LEFT || key == K_BACKSPACE))) {
			if(b) {
				CloseAssist();
				if(key == '.')
					Assist();
			}
		}
		else
			SyncAssist();
	}
	else {
		if(auto_assist &&
		   (key == '.' || key == '>' && Ch(GetCursor() - 2) == '-' ||
		    key == ':' && Ch(GetCursor() - 2) == ':') &&
		    InCode())
				Assist();

	}
	if(key == ',') {
		if(Ch(GetCursor()) == 184) {
			int q = GetCursor() + 1;
			int lvl = 0;
			while(q < GetLength()) {
				int ch = Ch(q);
				if(ch == '\n')
					return b;
				if(ch == 184 || lvl <= 0 && ch == ')') {
					SetSelection(GetCursor(), q);
					return b;
				}
				if(ch == '(')
					lvl++;
				if(ch == '(')
					lvl--;
				q++;
			}
		}
	}
	return b;
}

void AssistEditor::MouseWheel(Point p, int zdelta, dword keyflags)
{
	if(assist.IsOpen())
		assist.MouseWheel(p, zdelta, keyflags);
	else
		CodeEditor::MouseWheel(p, zdelta, keyflags);
}

void AssistEditor::LeftDown(Point p, dword keyflags)
{
	CloseAssist();
	CodeEditor::LeftDown(p, keyflags);
}

void AssistEditor::LostFocus()
{
	CloseAssist();
}

String AssistEditor::RemoveDefPar(const char *s)
{
	String r;
	int lvl = 0;
	bool dp = true;
	while(*s) {
		byte c = *s++;
		if(c == '(')
			lvl++;
		if(lvl == 0) {
			if(c == '=') {
				dp = false;
				if(commentdp)
					r.Cat("/* ");
				else
					while(r.GetCount() && *r.Last() == ' ')
						r.Trim(r.GetCount() - 1);
			}
			if(c == ')') {
				if(!dp && commentdp)
					r.Cat("*/");
				r.Cat(')');
				if(CParser(s).Char('='))
					break;
				r.Cat(s);
				break;
			}
			if(c == ',') {
				if(!dp && commentdp)
					r.Cat("*/");
				dp = true;
			}
		}
		else
		if(c == ')')
			lvl--;
		if(dp || commentdp)
			r.Cat(c);
	}
	return r;
}

String AssistEditor::MakeDefinition(const String& cls, const String& _n)
{
	String n = TrimLeft(_n);
	CParser p(n);
	bool dest = false;
	const char *beg = n;
	while(!p.IsEof()) {
		const char *b = p.GetPtr();
		if(p.Id("operator"))
			return NormalizeSpaces(String(beg, b) + ' ' + cls + "::" + b);
		if(p.Char('~')) {
			beg = p.GetPtr();
			dest = true;
		}
		else
		if(p.IsId()) {
			String id = p.ReadId();
			if(p.Char('(')) {
				String rp = RemoveDefPar(p.GetPtr());
				if(dest)
					return NormalizeSpaces(String(beg, b) + cls + "::~" + id + '(' + rp);
				else
					return NormalizeSpaces(String(beg, b) + ' ' + cls + "::" + id + '(' + rp);
			}
		}
		else
			p.SkipTerm();
	}
	return n;
}

void AssistEditor::DCopy()
{
	String r;
	int l, h;
	if(!GetSelection(l, h)) {
		int i = GetLine(GetCursor());
		l = GetPos(i);
		h = l + line[i].GetLength() + 1;
	}
	Parser ctx;
	Context(ctx, l);
	String txt = Get(l, h - l);
	StringStream ss(txt);
	String cls = ctx.current_nest.Mid(ctx.current_namespacel);
	if(cls[0] == ':' && cls[1] == ':')
		cls = cls.Mid(2);
	CppBase cpp;
	Parser parser;
	parser.Do(ss, IgnoreList(), cpp, Null, CNULL, Split(cls, ':'));
	for(int i = 0; i < cpp.GetCount(); i++) {
		const CppNest& nest = cpp[i];
		for(int j = 0; j < nest.GetCount(); j++) {
			const CppItem& m = nest[j];
			if(m.IsCode())
				if(cls.GetCount())
					r << MakeDefinition(cls, m.natural) << "\n{\n}\n\n";
				else {
					if(cpp.IsType(i))
					   r << '\t';
					r << m.natural << ";\n";
				}
			if(m.IsData())
				r << "extern " << m.natural << ";\n";
		}
	}
	WriteClipboardText(r);
}

String GetIdNest(const String& nest, const String& id, Index<String>& done)
{
	if(done.Find(nest) >= 0)
		return Null;
	done.Add(nest);
	String n = NoTemplatePars(nest);
	int q = BrowserBase().Find(n);
	if(q < 0)
		return Null;
	CppNest& m = BrowserBase()[q];
	Vector<String> r;
	if(m.name.Find(id) >= 0)
		return n;
	for(int i = 0; i < m.GetCount(); i++) {
		const CppItem& im = m[i];
		if(im.IsType()) {
			Vector<String> b = Split(im.qptype, ';');
			SubstituteTpars(b, nest);
			for(int i = 0; i < b.GetCount(); i++) {
				String r = GetIdNest(b[i], id, done);
				if(!IsNull(r))
					return r;
			}
		}
	}
	return Null;
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

void Ide::JumpS()
{
	if(designer)
		return;
	int li = editor.GetCursorLine();
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
	int q = l.Find("#include");
	if(q >= 0) {
		String path = FindIncludeFile(~l + q + 8, GetFileFolder(editfile));
		if(!IsNull(path)) {
			AddHistory();
			EditFile(path);
		}
		return;
	}
	if(!editor.assist_active)
		return;
	q = editor.GetCursor();
	while(iscid(editor.Ch(q - 1)))
		q--;
	String tp;
	Vector<String> h = editor.ReadBack(q, tp);
	Vector<String> type;
	Parser parser;
	editor.Context(parser, editor.GetCursor());
	if(h.GetCount() == 0 && IsNull(tp)) {
		type.Add(parser.current_nest);
	}
	else {
		type = editor.TypeOf(h, tp);
		if(type.GetCount() == 0)
			return;
	}
	String id = editor.GetWord();
	Vector<String> nest;
	for(int i = 0; i < type.GetCount(); i++) {
		Index<String> done;
		String r = GetIdNest(type[i], id, done);
		if(!IsNull(r))
			nest.Add(r);
	}

	if(nest.GetCount() == 0) {
		Index<String> done;
		String r = GetIdNest("::", id, done);
		if(IsNull(r)) {
			String t = Qualify(BrowserBase(), parser.current_nest, id);
			if(BrowserBase().Find(t) < 0)
				return;
			nest.Add(t);
		}
		else
			nest.Add(r);
	}

	if(nest.GetCount() == 0)
		return;

	q = BrowserBase().Find(nest[0]);
	if(q < 0)
		return;
	const CppNest& n = BrowserBase()[q];
	q = n.name.Find(id);
	if(q >= 0)
		JumpToDefinition(n[q]);
}

void Ide::JumpToDefinition(const CppItem& m)
{
	if(m.pos.GetCount() == 0)
		return;
	int ai = 0;
	for(int i = 0; i < m.pos.GetCount(); i++) {
		const CppPos& fp = m.pos[i];
		if(fp.impl) {
			GotoCpp(fp);
			return;
		}
		String ext = ToLower(GetFileExt(fp.GetFile()));
		if(ext == ".cpp" || ext == ".c" || ext == ".cc")
			ai = i;
	}
	GotoCpp(m.pos[ai]);
}

void Ide::IdeGotoLink(String link)
{
	LLOG("IdeGotoLink " << link);
	String nest, key;
	if(!SplitNestKey(link, nest, key))
		return;
	int q = BrowserBase().Find(nest);
	if(q < 0)
		return;
	const CppNest& n = BrowserBase()[q];
	q = n.key.Find(key);
	if(q >= 0)
		JumpToDefinition(n[q]);
}
