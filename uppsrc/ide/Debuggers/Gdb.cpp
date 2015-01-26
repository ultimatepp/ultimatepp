#include "Debuggers.h"

#define PROMPT "<wksuiherk" "bnuiohnsfoptmb>"

bool Gdb::Result(String& result, const String& s)
{
	int l = result.GetLength();
	result.Cat(s);
	const char *q = FindTag(~result + max(l - 5, 0), PROMPT);
	if(q) {
		result.Trim((int)(q - ~result));
		return true;
	}
	return false;
}

void Gdb::DebugBar(Bar& bar)
{
	bar.Add("Stop debugging", DbgImg::StopDebug(), THISBACK(Stop))
	   .Key(K_SHIFT_F5);
	bar.Separator();
	bool b = !IdeIsDebugLock();
	bar.Add(b, "Step into", DbgImg::StepInto(), THISBACK1(Step, disas.HasFocus() ? "stepi"
	                                                                             : "step"))
	   .Key(K_F11);
	bar.Add(b, "Step over", DbgImg::StepOver(), THISBACK1(Step, disas.HasFocus() ? "nexti"
	                                                                             : "next"))
	   .Key(K_F10);
	bar.Add(b, "Step out", DbgImg::StepOut(), THISBACK1(Step, "finish"))
	   .Key(K_SHIFT_F11);
	bar.Add(b, "Run to", DbgImg::RunTo(), THISBACK(DoRunTo))
	   .Key(K_CTRL_F10);
	bar.Add(b, "Run", DbgImg::Run(), THISBACK(Run))
	   .Key(K_F5);
	bar.MenuSeparator();
	bar.Add(b, "Quick watch", THISBACK(QuickWatch))
	   	.Key(K_CTRL_Q);
	bar.MenuSeparator();
	bar.Add(b, "Copy backtrace", THISBACK(CopyStack));
	bar.Add(b, "Copy dissassembly", THISBACK(CopyDisas));
}

void Gdb::CopyStack()
{
	DropFrames();
	String s;
	for(int i = 0; i < frame.GetCount(); i++)
		s << frame.GetValue(i) << "\n";
	WriteClipboardText(s);
}

void Gdb::CopyDisas()
{
	disas.WriteClipboard();
}

int CharFilterReSlash(int c)
{
	return c == '\\' ? '/' : c;
}

String Bpoint(Host& host, const String& file, int line)
{
	return String().Cat() << Filter(host.GetHostPath(NormalizePath(file)), CharFilterReSlash) << ":" << line + 1;
}

bool Gdb::TryBreak(const char *text)
{
	return FindTag(FastCmd(text), "Breakpoint");
}

bool Gdb::SetBreakpoint(const String& filename, int line, const String& bp)
{
	String bi = Bpoint(*host, filename, line);
	if(bp.IsEmpty())
		FastCmd("clear " + bi);
	else if(bp[0]==0xe)
		FastCmd("b " + bi);
	else
		FastCmd("b " + bi + " if " + bp);
	return true;
}

void Gdb::SetDisas(const String& text)
{
	disas.Clear();
	StringStream ss(text);
	while(!ss.IsEof()) {
		String ln = ss.GetLine();
		const char *s = ln;
		while(*s && !IsDigit(*s))
			s++;
		adr_t adr = 0;
		String code, args;
		if(s[0] == '0' && ToLower(s[1]) == 'x')
			adr = (adr_t)ScanInt64(s + 2, NULL, 16);
		int q = ln.Find(">:");
		if(q >= 0) {
			s = ~ln + q + 2;
			while(IsSpace(*s))
				s++;
			while(*s && !IsSpace(*s))
				code.Cat(*s++);
			while(IsSpace(*s))
				s++;
			args = s;
			q = args.Find("0x");			
			if(q >= 0)
				disas.AddT(ScanInt(~args + q + 2, NULL, 16));
			disas.Add(adr, code, args);
		}
	}
}

void Gdb::SyncDisas(bool fr)
{
	if(!disas.IsVisible())
		return;
	if(!disas.InRange(addr))
		SetDisas(FastCmd("disas"));
	disas.SetCursor(addr);
	disas.SetIp(addr, fr ? DbgImg::FrameLinePtr() : DbgImg::IpLinePtr());
	String s = FastCmd("info registers");
	StringStream ss(s);
	for(int i = 0; i < reglbl.GetCount(); i++)
		reglbl[i]->SetInk(SColorText);
	while(!ss.IsEof()) {
		String ln = ss.GetLine();
		CParser p(ln);
		try {
			String name;
			if(p.IsId()) {
				name = p.ReadId();
				if(p.Char2('0', 'x')) {
					String n = Sprintf("%08X", p.ReadNumber64(16));
					for(int i = 0; i < reglbl.GetCount(); i++) {
						if(regname[i] == name) {
							if(reglbl[i]->GetText() != n) {
								reglbl[i]->SetLabel(n);
								reglbl[i]->SetInk(LtRed);
							}
						}
					}
				}
			}
		}
		catch(CParser::Error) {}
	}
}

String FormatFrame(const char *s)
{
	if(*s++ != '#')
		return Null;
	while(IsDigit(*s))
		s++;
	while(*s == ' ')
		s++;
	if(s[0] == '0' && ToUpper(s[1]) == 'X') {
		s += 2;
		while(IsXDigit(*s))
			s++;
		while(*s == ' ')
			s++;
		if(s[0] != 'i' && s[1] != 'n')
			return Null;
		s += 2;
		while(*s == ' ')
			s++;
	}
	if(!IsAlpha(*s))
		return Null;
	const char *w = strchr(s, '\r');
	if(w)
		return String(s, w);
	w = strchr(s, '\n');
	if(w)
		return String(s, w);
	return s;
}

bool ParsePos(const String& s, String& fn, int& line, adr_t & adr)
{
	const char *q = FindTag(s, "\x1a\x1a");
	if(!q) return false;
	q += 2;
	Vector<String> p = Split(q + 2, ':');
	p.SetCount(5);
	fn = String(q, q + 2) + p[0];
	line = atoi(p[1]);
	CParser pa(p[4]);
	try {
		pa.Char2('0', 'x');
		if(pa.IsNumber(16))
			adr = (adr_t)pa.ReadNumber64(16);
	}
	catch(CParser::Error) {}
	return true;
}

void Gdb::CheckEnd(const char *s)
{
	if(!dbg) {
		Stop();
		return;
	}
	if(FindTag(s, "Program exited normally.")) {
		Stop();
		return;
	}
	const char *q = FindTag(s, "Program exited with code ");
	if(q) {
		PutConsole(q);
		Stop();
		return;
	}
}

String Gdb::Cmdp(const char *cmdline, bool fr)
{
	String s = Cmd(cmdline);
	if(ParsePos(s, file, line, addr)) {
		IdeSetDebugPos(GetLocalPath(file), line - 1, fr ? DbgImg::FrameLinePtr()
		                                                : DbgImg::IpLinePtr(), 0);
		IdeSetDebugPos(GetLocalPath(file), line - 1,
		               disas.HasFocus() ? fr ? DbgImg::FrameLinePtr() : DbgImg::IpLinePtr()
		                                : Image(), 1);
		SyncDisas(fr);
		autoline = IdeGetLine(line - 2) + ' ' + IdeGetLine(line - 1) + ' ' + IdeGetLine(line);
	}
	else {
		file = Null;
		CParser pa(s);
		try {
			pa.Char2('0', 'x');
			if(pa.IsNumber(16))
				addr = (adr_t)pa.ReadNumber64(16);
		}
		catch(CParser::Error) {}
		SyncDisas(fr);
	}
	frame.Clear();
	frame.Add(0, FormatFrame(Cmd("frame")));
	frame <<= 0;
	Data();
	return s;
}

bool Gdb::RunTo()
{
	String bi;
	bool df = disas.HasFocus();
	if(df) {
		if(!disas.GetCursor())
			return false;
		bi = Sprintf("*0x%X", disas.GetCursor());
	}
	else
		bi = Bpoint(*host, IdeGetFileName(), IdeGetFileLine());
	if(!TryBreak("b " + bi)) {
		Exclamation("No code at chosen location !");
		return false;
	}
	String e = Cmdp(firstrun ? "run" : "continue");
	firstrun = false;
	FastCmd("clear " + bi);
	if(df)
		disas.SetFocus();
	CheckEnd(e);
	IdeActivateBottom();
	return true;
}

void Gdb::Run()
{
	CheckEnd(Cmdp(firstrun ? "run" : "continue"));
	firstrun = false;
	IdeActivateBottom();
}

void Gdb::Step(const char *cmd)
{
	bool b = disas.HasFocus();
	String s = Cmdp(cmd);
	if(b) disas.SetFocus();
	CheckEnd(s);
	IdeActivateBottom();
}

void Gdb::DisasCursor()
{
	if(!disas.HasFocus())
		return;
	int line;
	String file;
	adr_t addr;
	if(ParsePos(FastCmd(Sprintf("info line *0x%X", disas.GetCursor())), file, line, addr))
		IdeSetDebugPos(file, line - 1, DbgImg::DisasPtr(), 1);
	disas.SetFocus();
}

void Gdb::DisasFocus()
{
//	if(!disas.HasFocus())
//		IdeSetDebugPos(file, 0, Null, 1);
}

void Gdb::DropFrames()
{
	int i = 0;
	int q = ~frame;
	frame.Clear();
	for(;;) {
		String s = FormatFrame(FastCmd(Sprintf("frame %d", i)));
		if(IsNull(s)) break;
		frame.Add(i++, s);
	}
	frame <<= q;
}

void Gdb::ShowFrame()
{
	int i = (int)~frame;
	Cmdp(Sprintf("frame %d", i), i);
}

String DataClean(CParser& p)
{
	String r;
	if(p.Char('{')) {
		while(!p.IsEof() && !p.Char('}')) {
			p.Id("static");
			if(p.Char('<')) {
				int level = 1;
				while(level > 0)
					if(p.Char('<'))
						level++;
					else
					if(p.Char('>'))
						level--;
					else
						p.GetChar();
				p.Spaces();
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
					r << id << "=" << q;
				}
			}
			else {
				p.GetChar();
				p.Spaces();
			}
		}
		if(!r.IsEmpty() && (*r != '{' || *r.Last() != '}'))

			return '{' + r + '}';
		return r;
	}
	p.Spaces();
	for(;;) {
		bool sp = p.Spaces();
		if(p.IsChar('}') || p.IsChar(',') || p.IsEof())
			break;
		if(sp)
			r << ' ';
		if(p.IsString())
			r << AsCString(p.ReadString());
		else
			r.Cat(p.GetChar());
	}
	return r;
}

String DataClean(const char *s)
{
	CParser p(s);
	try {
		return DataClean(p);
	}
	catch(CParser::Error) {}
	return Null;
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
		locals.Add(String(s, e), DataClean(q));
	}
	MarkChanged(prev, locals);
}

String Gdb::Print(const String& exp)
{
	String q = FastCmd("print " + exp);
	StringStream ss(q);
	String ln = ss.GetLine();
	const char *s = strchr(ln, '=');
	if(s) {
		s++;
		while(*s == ' ')
			s++;
		return DataClean(s);
	}
	else
		return DataClean(ln);
}

void Gdb::Watches()
{
	VectorMap<String, String> prev = DataMap(watches);
	for(int i = 0; i < watches.GetCount(); i++)
		watches.Set(i, 1, Print(watches.Get(i, 0)));
	MarkChanged(prev, watches);
}

void Gdb::Autos()
{
	VectorMap<String, String> prev = DataMap(autos);
	autos.Clear();
	CParser p(autoline);
	try {
		while(!p.IsEof()) {
			if(p.IsId()) {
				String exp = p.ReadId();
				for(;;) {
					if(p.Char('.') && p.IsId())
						exp << '.';
					else
					if(p.Char2('-', '>') && p.IsId())
						exp << "->";
					else
						break;
					exp << p.ReadId();
				}
				if(autos.Find(exp) < 0) {
					String val = Print(exp);
					if(!IsNull(val) && val.Find('(') < 0)
						autos.Add(exp, val);
				}
			}
			p.SkipTerm();
		}
	}
	catch(CParser::Error) {}
	autos.Sort();
	MarkChanged(prev, autos);
}

void Gdb::QuickWatch()
{
	for(;;) {
		int q = quickwatch.Run();
		if(q == IDCANCEL)
			break;
		FastCmd("set print pretty on");
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
		FastCmd("set print pretty off");
	}
	quickwatch.Close();
}

void Gdb::Data()
{
	switch(tab.Get()) {
	case 0: Watches(); break;
	case 1: Locals(); break;
	case 2: Autos(); break;
	}
}

bool Gdb::Key(dword key, int count)
{
	if(key >= 32 && key < 65535 && tab.Get() == 2) {
		watches.DoInsertAfter();
		Ctrl* f = GetFocusCtrl();
		if(f && watches.HasChildDeep(f))
			f->Key(key, count);
		return true;
	}
	return Ctrl::Key(key, count);
}

bool Gdb::Create(One<Host> rval_ _host, const String& exefile, const String& cmdline, bool console)
{
	host = pick(_host);
	dbg = host->StartProcess(GdbCommand(console) + GetHostPath(exefile));
	if(!dbg) {
		Exclamation("Error invoking gdb !");
		return false;
	}
	IdeSetBottom(*this);
	IdeSetRight(disas);

	disas.AddFrame(regs);
	disas.WhenCursor = THISBACK(DisasCursor);
	disas.WhenFocus = THISBACK(DisasFocus);
	frame.WhenDrop = THISBACK(DropFrames);
	frame <<= THISBACK(ShowFrame);

	watches.WhenAcceptEdit = THISBACK(Data);
	tab <<= THISBACK(Data);

	Cmd("set prompt " PROMPT);
	Cmd("set disassembly-flavor intel");
	Cmd("set exec-done-display off");
	Cmd("set annotate 1");
	Cmd("set height 0");
	Cmd("set width 0");
	Cmd("set confirm off");
	Cmd("set print asm-demangle");

	if(!IsNull(cmdline))
		Cmd("set args " + cmdline);


	firstrun = true;

	return true;
}

Gdb::~Gdb()
{
	IdeRemoveBottom(*this);
	IdeRemoveRight(disas);
	KillDebugTTY();
}

void Gdb::Periodic()
{
	if(TTYQuit())
		Stop();
}

Gdb::Gdb()
{
	periodic.Set(-50, THISBACK(Periodic));
}

One<Debugger> GdbCreate(One<Host> rval_ host, const String& exefile, const String& cmdline, bool console)
{
	Gdb *dbg = new Gdb;
	if(!dbg->Create(pick(host), exefile, cmdline, console)) {
		delete dbg;
		return NULL;
	}
	return dbg;
}
