#include "Debuggers.h"

void SkipGdbInfo(CParser& p)
{
	int level = 1;
	while(level > 0)
		if(p.Char('<'))
			level++;
		else
		if(p.Char('>'))
			level--;
		else
			p.GetChar();
}

String DataClean(CParser& p)
{
	String r;
	if(p.Char('{')) {
		while(!p.IsEof() && !p.Char('}')) {
			p.Id("static");
			if(p.Char('<')) {
				SkipGdbInfo(p);
				p.Char('=');
				String q = DataClean(p);
				if(!q.IsEmpty()) {
					if(!r.IsEmpty())
						r << ", ";
					r << q;
				}
			}
			else
			if(p.IsId()) {
				String id = p.ReadId();
				p.Char('=');
				String q = DataClean(p);
				if(!q.IsEmpty()) {
					if(!r.IsEmpty())
						r << ", ";
					r << id << " = " << q;
				}
			}
			else
				p.GetChar();
		}
		if(!r.IsEmpty() && (*r != '{' || *r.Last() != '}'))
			return "{ " + r + " }";
		return r;
	}
	int lvl = 0;
	for(;;) {
		bool sp = p.Spaces();
		if(lvl == 0 && (p.IsChar('}') || p.IsChar(',')) || p.IsEof())
			break;
		if(sp)
			r << ' ';
		if(p.IsString())
			r << AsCString(p.ReadString());
		else
		if(p.Char('<') && lvl == 0)
			SkipGdbInfo(p);
		else {
			if(p.IsChar('('))
				lvl++;
			if(p.IsChar(')'))
				lvl--;
			r.Cat(p.GetChar());
		}
	}
	return r;
}

String DataClean(const char *s)
{
	try {
		CParser p(s);
		return DataClean(p);
	}
	catch(CParser::Error) {}
	return Null;
}

String GdbData(const String& exp)
{
	int q = exp.Find('{');
	return q < 0 ? exp : TrimBoth(exp.Mid(q));
}

void Gdb::Locals()
{
	VectorMap<String, String> prev = DataMap(locals);
	locals.Clear();
	String s = FastCmd("info locals");
	StringStream ss(s);
	while(!ss.IsEof()) {
		String ln = ss.GetLine();
		const char *s = ln;
		const char *q = strchr(s, '=');
		if(!q) break;
		const char *e = q;
		while(e > s && e[-1] == ' ')
			e--;
		q++;
		while(*q == ' ')
			q++;
		String h = GdbData(q);
		locals.Add(String(s, e), DataClean(h), h);
	}
	MarkChanged(prev, locals);
}

String Gdb::Print0(const String& exp)
{
	String q = FastCmd("print " + exp);
	StringStream ss(q);
	String ln = ss.GetLine();
	const char *s = strchr(ln, '=');
	if(s) {
		s++;
		while(*s == ' ')
			s++;
		return GdbData(s);
	}
	else
		return GdbData(ln);
}

String Gdb::Print(const String& exp)
{
	int q = expression_cache.Find(exp);
	if(q >= 0)
		return expression_cache[q];
	String r = Print0(exp);
	expression_cache.Add(exp, r);
	return r;
}

void Gdb::Watches()
{
	VectorMap<String, String> prev = DataMap(watches);
	for(int i = 0; i < watches.GetCount(); i++) {
		String h = Print(watches.Get(i, 0));
		watches.Set(i, 1, DataClean(h));
		watches.Set(i, 2, h);
	}
	MarkChanged(prev, watches);
}

void Gdb::TryAuto(Index<String>& tested, const String& exp)
{
	if(tested.Find(exp) < 0 && findarg(exp, "true", "false") < 0) {
		tested.Add(exp);
		String val = Print(exp);
		if(!IsNull(val) && !val.EndsWith(")}") && !val.EndsWith(")>") &&
		   (!IsAlpha(*val) || findarg(val, "true", "false") >= 0))
			autos.Add(exp, DataClean(val), val);
	}
}

void Gdb::Autos()
{
	VectorMap<String, String> prev = DataMap(autos);
	autos.Clear();
	Index<String> tested;
	try {
		CParser p(autoline);
		while(!p.IsEof())
			if(p.IsId()) {
				String exp = p.ReadId();
				TryAuto(tested, exp);
				for(;;) {
					if(p.Char('.') && p.IsId())
						exp << '.';
					else
					if(p.Char2('-', '>') && p.IsId())
						exp << "->";
					else
						break;
					exp << p.ReadId();
					TryAuto(tested, exp);
				}
			}
			else
				p.SkipTerm();
	}
	catch(CParser::Error) {}
	autos.Sort();
	MarkChanged(prev, autos);
}

void Gdb::ReadGdbValues(CParser& p, VectorMap<String, String>& val)
{
	if(p.Char('{')) {
		while(!p.Char('}') && !p.IsEof()) {
			if(p.IsChar('{')) {
				ReadGdbValues(p, val);
				continue;
			}
			String id;
			String value;
			const char *b = p.GetPtr();
			while(!p.IsEof())
				if(p.IsChar('=')) {
					id = TrimBoth(String(b, p.GetPtr()));
					p.Char('=');
					break;
				}
				else
					p.SkipTerm();
			b = p.GetPtr();
			int lvl = 0;
			while(!p.IsEof())
				if((p.IsChar(',') || p.IsChar('}')) && lvl == 0) {
					String v = TrimBoth(String(b, p.GetPtr()));
					if(*id == '<') // Base class
						ReadGdbValues(v, val);
					else
						val.Add(id, v);
					p.Char(',');
					break;
				}
				else
				if(p.Char('{'))
					lvl++;
				else
				if(p.Char('}'))
					lvl--;
				else
					p.SkipTerm();
		}
	}
}

void Gdb::ReadGdbValues(const String& h, VectorMap<String, String>& val)
{
	try {
		CParser p(h);
		ReadGdbValues(p, val);
	}
	catch(CParser::Error) {}
}

void Gdb::Self()
{
	VectorMap<String, String> prev = DataMap(locals);
	self.Clear();
	VectorMap<String, String> val;
	ReadGdbValues(Print("*this"), val);
	for(int i = 0; i < val.GetCount(); i++)
		self.Add(val.GetKey(i), DataClean(val[i]), val[i]);
	MarkChanged(prev, locals);
}

void Gdb::Cpu()
{
	String s = FastCmd("info registers");
	StringStream ss(s);
	cpu.Clear();
	bool even = false;
	while(!ss.IsEof()) {
		String ln = ss.GetLine();
		cpu.Add(AttrText(ln).SetFont(Courier(Ctrl::HorzLayoutZoom(12)))
		        .Paper(even ? Blend(SColorMark, SColorPaper, 220) : SColorPaper)
		        .Ink(SColorText));
		even = !even;
	}
}

void Gdb::Data()
{
	switch(tab.Get()) {
	case 0: Autos(); break;
	case 1: Locals(); break;
	case 2: Watches(); break;
	case 3: Self(); break;
	case 4: Cpu(); break;
	}
}

void Gdb::SetTab(int q)
{
	tab.Set(q);
	Data();
}

void Gdb::ClearWatches()
{
	watches.Clear();
	SetTab(2);
}

void Gdb::QuickWatch()
{
	quickwatch.expression << quickwatch.Breaker(999);
	FastCmd("set print pretty on");
	for(;;) {
		int q = quickwatch.Run();
		if(findarg(q, IDCANCEL, IDOK) >= 0) {
			FastCmd("set print pretty off");
			if(q == IDOK) {
				watches.Add(~quickwatch.expression);
				SetTab(2);
			}
			quickwatch.Close();
			return;
		}
		String s = FastCmd("p " + (String)~quickwatch.expression);
		const char *a = strchr(s, '=');
		if(a) {
			a++;
			while(*a == ' ')
				a++;
			quickwatch.value <<= a;
			quickwatch.expression.AddHistory();
		}
		else
			quickwatch.value <<= s;
	}
}

bool Gdb::Tip(const String& exp, CodeEditor::MouseTip& mt)
{
	if(IsNull(exp))
		return false;
	String val = DataClean(Print(exp));
	if(!IsNull(val) && !val.EndsWith(")}") && !IsAlpha(*val)) {
		if(val.GetCount() > 100) {
			val.Trim(100);
			val << "...";
		}
		mt.display = &StdDisplay();
		mt.value = val;
		mt.sz = mt.display->GetStdSize(String(mt.value) + "X");
		return true;
	}
	return false;
}

void Gdb::SetTree(ArrayCtrl *a)
{
	tree.Clear();
	if(a->IsCursor()) {
		String h = a->Get(2);
		tree.SetRoot(Null, h, String().Cat() << a->Get(0) << " = " << DataClean(h));
		tree.Open(0);
	}
}

void Gdb::TreeExpand(int node)
{
	if(tree.GetChildCount(node))
		return;
	VectorMap<String, String> val;
	ReadGdbValues(tree.Get(node), val);
	for(int i = 0; i < val.GetCount(); i++)
		tree.Add(node, Null, val[i], val.GetKey(i) + " = " + DataClean(val[i]), *val[i] == '{');
}
