#include "SqlCtrl.h"
#include <Report/Report.h>

NAMESPACE_UPP

#define IMAGEFILE <SqlCtrl/SqlCtrl.iml>
#define IMAGECLASS SqlConsoleImg
#include <Draw/iml.h>

static bool scmpw(const byte *s, const char *w) {
	for(;;) {
		if(*w == '\0')
			return *s == ' ' || *s == '\t';
		if(ToUpper(*s++) != ToUpper(*w++))
			return false;
	}
}

static const int MAX_NESTING_LEVEL = 10;
static const int PROGRESS_RANGE = 1000000000;

bool SqlRunScript(int dialect, Stream& script_stream, 
	Gate1<String> executor, Gate2<int, int> progress,
	Index<String>& script_file_names, int progress_min, int progress_max)
{
	int line_number = 1;
	for(int c; (c = script_stream.Term()) >= 0;) {
		if(c <= ' ') {
			if(c == '\n')
				line_number++;
			script_stream.Get();
		}
		else if(c == '@') {
			script_stream.Get();
			int64 start = script_stream.GetPos() - 1;
			bool rel = false;
			if(script_stream.Term() == '@') {
				script_stream.Get();
				rel = true;
			}
			StringBuffer subscript_fn;
			while((c = script_stream.Get()) >= 0 && c != '\n')
				subscript_fn.Cat((char)c);
			int e = subscript_fn.GetLength();
			while(e > 0 && subscript_fn[e - 1] <= ' ')
				e--;
			String fn(~subscript_fn, e);
			fn = NormalizePath(fn, GetFileDirectory(script_file_names[rel ? script_file_names.GetCount() - 1 : 0]));
			String norm = fn;
#if !PLATFORM_PATH_HAS_CASE
			norm = ToLower(norm);
#endif
			if(script_file_names.Find(norm) >= 0)
				throw Exc(NFormat(t_("circular script inclusion of %s at %s:%d"),
					fn, script_file_names.Top(), line_number));
			if(script_file_names.GetCount() >= MAX_NESTING_LEVEL)
				throw Exc(NFormat(t_("script nesting too deep (%d levels)"), script_file_names.GetCount()));
			int64 end = script_stream.GetPos();
			
			int new_min = progress_min + (int)((progress_max - progress_min) * start / script_stream.GetSize());
			int new_max = progress_min + (int)((progress_max - progress_min) * end / script_stream.GetSize());
			
			FileIn fi;
			if(!fi.Open(fn))
				throw Exc(NFormat(t_("error opening script file '%s'"), fn));
			
			script_file_names.Add(norm);
			if(!SqlRunScript(dialect, fi, executor, progress, script_file_names, new_min, new_max))
				return false;
			script_file_names.Drop();
		}
		else {
			StringBuffer statement;
			bool exec = false;
			bool body = false;
			bool chr = false;

			while(!script_stream.IsEof() && !exec) {
				String line = script_stream.GetLine();
				line_number++;
				
				int l = line.GetLength();
				if(l && line[0] == '/' && !body && !chr)
					exec = true;
				else
				if(l && line[0] == '.' && body && !chr)
					body = false;
				else {
					if(statement.GetLength() && !chr)
						statement.Cat(' ');
					bool spc = true;
					bool create = false;
					for(const byte *s = (const byte *)(const char *)line; *s; s++) {
						if(s[0] == '-' && s[1] == '-' && !chr) break;
						if(*s == '\'') chr = !chr;
						if(!chr && spc && scmpw(s, "create"))
							create = true;
						if(!chr && spc && (scmpw(s, "begin") || scmpw(s, "declare") ||
						   create && (scmpw(s, "procedure") || scmpw(s, "function") || scmpw(s, "trigger"))
						))
							body = true;
						if(*s == ';' && !chr && !body) {
							exec = true;
							break;
						}
						if(*s > ' ' || chr) {
							statement.Cat(*s);
							spc = false;
						}
						else
						if(*s == '\t' || *s == ' ') {
							if(!spc) statement.Cat(' ');
							spc = true;
						}
						else
							spc = false;
					}
					if(chr) statement.Cat("\r\n");
				}
			}
			if(progress((int)(progress_min + script_stream.GetPos()
				* (progress_max - progress_min) / script_stream.GetSize()), PROGRESS_RANGE))
				throw AbortExc();
			if(!executor(statement))
				return false;
		}
	}
	return true;
}

bool SqlRunScript(int dialect, Stream& script_stream, const String& file_name,
	Gate1<String> executor, Gate2<int, int> progress)
{
	Index<String> script_file_names;
	String fn = NormalizePath(file_name);
#if !PLATFORM_PASH_HAS_CASE
	fn = ToLower(fn);
#endif
	script_file_names.Add(fn);
	return SqlRunScript(dialect, script_stream, executor, progress, script_file_names, 0, PROGRESS_RANGE);
}

void RunDlgSqlExport(Sql& cursor, String command, String tablename);

class MacroSet {
	struct Macro {
		int            n;
		String         text;
		Vector<String> param;

		void           Serialize(Stream& s)         { s / n % text % param; }
	};
	ArrayMap<String, Macro> macro;

	static bool   Spaces(const char *&s);
	static String GetId(const char *&s);
	static bool   IsId(int c) { return c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c == '_'; }

	String Expand(const char *s, Index<int>& used) const;

public:
	void   Serialize(Stream& s);
	bool   Define(const char *s);
	String Expand(const char *s) const;
	int    GetCount() const                         { return macro.GetCount(); }
	String Get(int i) const;
};

bool   MacroSet::Spaces(const char *&s) {
	if(*s != ' ' && *s != '\t') return false;
	while(*s == ' ' || *s == '\t')
		s++;
	return true;
}

String MacroSet::GetId(const char *&s) {
	String result;
	while(*s >= 'A' && *s <= 'Z' || *s >= 'a' && *s <= 'z' || *s >= '0' && *s <= '9' || *s == '_')
		result.Cat(*s++);
	return result;
}

void MacroSet::Serialize(Stream& s) {
	int version = 0;
	s % version;
	int n = macro.GetCount();
	s / n;
	if(s.IsLoading()) {
		macro.Clear();
		for(int i = 0; i < n; i++) {
			String key;
			Macro  value;
			s % key;
			s % macro.Add(key);
		}
	}
	else
		for(int i = 0; i < n; i++) {
			String key;
			Macro  value;
			key = macro.GetKey(i);
			value = macro[i];
			s % key % value;
		}
}

bool MacroSet::Define(const char *s) {
	if(!IsId(*s)) return false;
	String name = GetId(s);
	Index<String> param;
	Spaces(s);
	if(*s == '(') {
		s++;
		Spaces(s);
		if(*s != ')')
			for(;;) {
				Spaces(s);
				if(!IsId(*s)) return false;
				if(param.GetCount() >= 16) return false;
				param.Add(GetId(s));
				Spaces(s);
				if(*s == ')') break;
				if(*s != ',') return false;
				s++;
			}
		s++;
	}
	Spaces(s);
	Macro& m = macro.GetAdd(name);
	m.n = param.GetCount();
	m.param <<= param.GetKeys();
	m.text.Clear();
	while(*s) {
		if(IsId(*s)) {
			String name = GetId(s);
			int i = param.Find(name);
			if(i >= 0)
				m.text.Cat(i + 16);
			else
				m.text.Cat(name);
		}
		else {
			if(*s >= 16 && *s < 32)
				m.text.Cat(' ');
			else
				m.text.Cat(*s);
			s++;
		}
	}
	return true;
}

String MacroSet::Expand(const char *s, Index<int>& used) const {
	String text;
	while(*s)
		if(IsId(*s)) {
			String name = GetId(s);
			int i = macro.Find(name);
			if(i >= 0 && used.Find(i) < 0) {
				const Macro& m = macro[i];
				Vector<String> param;
				const char *t = s;
				Spaces(s);
				if(*s == '(') {
					int level = 0;
					String p;
					for(;;) {
						s++;
						if((*s == ',' || *s == ')') && level == 0) {
							p = TrimRight(TrimLeft(p));
//							p.TrimLeft();
//							p.TrimRight();
							param.Add(Expand(p));
							p.Clear();
						}
						else
							p.Cat(*s);
						if(*s == '(') level++;
						if(*s == ')') {
							if(level <= 0) {
								s++;
								break;
							}
							level--;
						}
					}
				}
				else
					s = t;
				param.SetCount(16);
				Index<int> newused(used, 1);
				newused.Add(i);
				String mt = Expand(m.text, newused);
				for(const char *q = mt; *q; q++)
					if(*q >= 16 && *q < 32)
						text.Cat(param[*q - 16]);
					else
						text.Cat(*q);
			}
			else
				text.Cat(name);
		}
		else
			text.Cat(*s++);
	return text;
}

String MacroSet::Expand(const char *s) const {
	Index<int> temp;
	return Expand(s, temp);
}

String MacroSet::Get(int i) const {
	const Macro& m = macro[i];
	String s = macro.GetKey(i);
	if(m.param.GetCount()) {
		s.Cat('(');
		for(i = 0; i < m.param.GetCount(); i++) {
			if(i) s.Cat(", ");
			s.Cat(m.param[i]);
		}
		s.Cat(')');
	}
	s.Cat("   ");
	s.Cat(Expand(s));
	return s;
}

class SqlConsole : public TopWindow {
public:
	virtual bool Key(dword key, int count);
	virtual void Serialize(Stream& s);

	void ListMenu(Bar& bar);
	void ListPrintRow();
	void ListPrintList();
	void ListExport();

protected:
	Sql                        cursor;
	String                     lastquery;
	Splitter                   lires, vsplit;
	ArrayCtrl                  list;
	StaticRect                 rec_err;
	DocEdit                    errortext;
	ArrayCtrl                  record;
	ArrayCtrl                  trace;
	WithDropChoice<EditString> command;
	Button                     schema;
//	CallbackSet                hide;
	Vector<int>                cw;
	Vector<bool>               visible;
	Vector<bool>               lob;
	String                     LastDir;
	MacroSet                   macroset;

	enum {
		NORMAL,
		RERUN,
		SCRIPT,
		QUIET,
	};

	void    ColSize();
	void    Hide(int i);
	void    Record();
	void    Execute(int type = NORMAL);
	void    TraceToCommand();
	void    TraceToExecute();
	void    ListToCommand();
	void    SaveTrace();
	void    RunScript(bool quiet);
	void    TraceMenu(Bar& menu);
	void    MacroList();
	void    ObjectTree() { SQLObjectTree(cursor.GetSession()); }

	class Exec;
	friend class Exec;
	class Exec : public StatementExecutor {
	public:
		typedef Exec CLASSNAME;
		Exec(bool quiet) : quiet(quiet) {}
		SqlConsole *me;
		bool quiet;
		virtual bool Execute(const String& stmt) {
			me->command <<= stmt; me->Sync(); me->Execute(quiet ? QUIET : SCRIPT); return true;
		}
		bool GateExec(String stmt) { return Execute(stmt); }
		operator Gate1<String> () { return THISBACK(GateExec); }
		
	};

public:
	typedef SqlConsole CLASSNAME;

	void    Perform();

	SqlConsole(SqlSession& session);
};

void SqlConsole::MacroList() {
	list.Reset();
	record.Clear();
	list.AddColumn(t_("Defined macros"));
	for(int i = 0; i < macroset.GetCount(); i++)
		list.Add('#' + macroset.Get(i));
}

void SqlConsole::Execute(int type) {
	list.Reset();
	visible.Clear();
	lob.Clear();
	record.Clear();
	String s = command.GetText().ToString();
	if(*s == '#') {
		if(s[1] == '\0') {
			MacroList();
			command.Clear();
			return;
		}
		list.AddColumn(t_("Macro definition"));
		if(macroset.Define(~s + 1)) {
			list.Add("OK");
			trace.Add(s, t_("Macro OK"), "");
			if(type == NORMAL)
				command.AddHistory();
			command.Clear();
		}
		else {
			list.Add(t_("Invalid macro"));
			trace.Add(s, t_("Invalid"), "");
		}
		return;
	}
	s = TrimRight(macroset.Expand(s));
//	s.TrimRight();
	while(*s.Last() == ';')
		s.Trim(s.GetLength() - 1);
	int ms0 = GetTickCount();
	cursor.ClearError();
	lastquery = s;
	if(!cursor.Execute(s)) {
	error:
		record.Hide();
		errortext.Show();
		list.AddColumn(t_("Error"));
		String err = cursor.GetLastError();
		errortext <<= err;
		list.Add(err);
		trace.Add(s, err, "");
		trace.GoEnd();
		Title((s + " - " + err).ToWString());
		return;
	}
	if(type == QUIET)
		return;
	bool onecol = cursor.GetColumns() == 1;
	record.Show(!onecol);
	errortext.Show(onecol);
	int ms1 = GetTickCount();
	cw.SetCount(cursor.GetColumns());
	visible.SetCount(cw.GetCount(), true);
	for(int i = 0; i < cursor.GetColumns(); i++) {
		const SqlColumnInfo& ci = cursor.GetColumnInfo(i);
		list.AddColumn(ci.name);
		list.HeaderTab(i).WhenAction = THISBACK1(Hide, i);
		cw[i] = GetTextSize(ci.name, StdFont()).cx + 2 * list.HeaderTab(i).GetMargin();
		record.Add(ci.name, Null);
		lob.Add(ci.type == -1 || ci.type == -2); // !! BLOB / CLOB hack
	}
	Progress pi;
	pi.SetText(t_("Fetched %d line(s)"));
	while(cursor.Fetch()) {
		Vector<Value> row = cursor.GetRow();
		for(int i = 0; i < cursor.GetColumns(); i++)
		{
			if(lob[i])
			{
				String temp;
				cursor.GetColumn(i, temp);
				row[i] = temp;
			}
			cw[i] = max(cw[i], GetTextSize(StdFormat(row[i]), StdFont()).cx +
				2 * list.HeaderTab(i).GetMargin());
		}
		list.Add(row);
		if(pi.StepCanceled()) break;
	}
	if(cursor.GetColumns() && cursor.WasError()) {
		list.Reset();
		goto error;
	}
	visible.SetCount(cursor.GetColumns(), true);
	ColSize();
	if(list.GetCount() > 0)
		list.SetCursor(0);
	Title(NFormat(t_("%s (%d rows)"), s, pi.GetPos()));
	String rrows = Format(t_("%d rows"), max(list.GetCount(), cursor.GetRowsProcessed()));
	String rms = Format(t_("%d ms"), ms1 - ms0);
	if(type == RERUN && trace.IsCursor()) {
		trace.Set(1, rrows);
		trace.Set(2, rms);
	}
	else {
		trace.Add(s, rrows, rms);
		trace.GoEnd();
	}
	if(type == NORMAL)
		command.AddHistory();
	command.Clear();
}

void SqlConsole::ColSize() {
	int maxw = 18 * StdFont().Info().GetAveWidth();
	int maxx = list.GetSize().cx;
	int wx = 0;
	for(int i = 0; i < cursor.GetColumns(); i++)
		if(visible[i]) {
			int w = min(maxw, cw[i]);
			wx += w;
			if(wx < maxx) {
				list.HeaderObject().SetTabRatio(i, w);
				list.HeaderObject().ShowTab(i);
			}
			else
				list.HeaderObject().HideTab(i);
		}
		else
			list.HeaderObject().HideTab(i);
}

void SqlConsole::Hide(int i) {
	if(i < visible.GetCount())
		visible[i] = false;
	ColSize();
}

void SqlConsole::Record() {
	if(list.GetIndexCount() == 1)
		errortext <<= StdFormat(list.Get(0));
	for(int i = 0; i < list.GetIndexCount(); i++)
		record.Set(i, 1, list.Get(i));
}

bool SqlConsole::Key(dword key, int count) {
	switch(key) {
	case K_ENTER:
		Execute();
		return true;
	case K_CTRL_R:
		RunScript(false);
		return true;
	case K_CTRL_Q:
		RunScript(true);
		return true;
	case K_CTRL_S:
		SaveTrace();
		return true;
	case K_CTRL_M:
		MacroList();
		return true;
	}
	if(key >= ' ' && key < 256 || key == K_RIGHT || key == K_LEFT || key == K_ALT_DOWN) {
		command.SetFocus();
		command.Key(key, count);
		return true;
	}
	return TopWindow::Key(key, count);
}

void SqlConsole::Serialize(Stream& s) {
	int version = 2;
	s / version;
	s.Magic();
	Rect r = GetRect();
	s % r;
	SetRect(r);
	vsplit.Serialize(s);
	record.SerializeHeader(s);
	lires.Serialize(s);
	command.SerializeList(s);
	trace.SerializeHeader(s);
	if(version >= 1)
		s % LastDir;
	if(version >= 2)
		s % macroset;
}

void SqlConsole::Perform() {
	const char cfg[] = "SqlConsole.cfg";
	LoadFromFile(*this, cfg);
	Title(t_("SQL Commander"));
	Icon(SqlConsoleImg::SqlConsoleIconSmall(), SqlConsoleImg::SqlConsoleIconLarge());
	Sizeable();
	Zoomable();
	ActiveFocus(command);
	Run();
	cursor.ClearError();
	StoreToFile(*this, cfg);
}

void SqlConsole::TraceToCommand() {
	if(trace.IsCursor())
		command.SetData(trace.Get(0));
}

void SqlConsole::ListToCommand() {
	int c = list.GetEditColumn();
	if(list.IsCursor() && c >= 0 && c < list.GetIndexCount())
		command.Insert(StdFormat(list.Get(c)).ToWString());
	command.SetFocus();
}

void SqlConsole::TraceToExecute() {
	Execute(RERUN);
}

void SqlConsole::SaveTrace() {
	FileSel fsel;
	fsel.ActiveDir(LastDir);
	fsel.DefaultExt("sql");
	fsel.Type(t_("SQL scripts (*.sql)"), "*.sql");
	fsel.AllFilesType();
	if(!fsel.ExecuteSaveAs(t_("Save trace as"))) return;
	FileOut out(~fsel);
	if(!out) return;
	LastDir = GetFileDirectory(~fsel);
	for(int i = 0; i < trace.GetCount(); i++) {
		out.Put((String)trace.Get(i, 0));
		out.Put(";\n");
	}
}

void SqlConsole::RunScript(bool quiet) {
//	UPP::RunScript runscript = cursor.GetSession().GetRunScript();
//	if(!runscript) {
//		Exclamation(t_("Database connection doesn't support running scripts."));
//		return;
//	}
	FileSel fsel;
	fsel.ActiveDir(LastDir);
	fsel.DefaultExt("sql");
	fsel.Type(t_("SQL scripts (*.sql)"), "*.sql");
	fsel.AllFilesType();
	if(!fsel.ExecuteOpen(t_("Run script"))) return;
	Exec exec(quiet);
	exec.me = this;
	LastDir = GetFileDirectory(~fsel);
	Progress progress(t_("Executing script"));
	FileIn fi;
	if(!fi.Open(~fsel)) {
		Exclamation(NFormat(t_("Cannot open file [* \1%s\1]."), ~fsel));
		return;
	}
	try {
		SqlRunScript(cursor.GetDialect(), fi, ~fsel, exec, progress);
	}
	catch(Exc e) {
		ShowExc(e);
	}
}

void SqlConsole::TraceMenu(Bar& menu) {
	menu.Add(t_("Save as script.."), THISBACK(SaveTrace)).Key(K_CTRL_S);
	menu.Add(t_("Run script.."), THISBACK1(RunScript, false)).Key(K_CTRL_R);
	menu.Add(t_("Run script quietly.."), THISBACK1(RunScript, true)).Key(K_CTRL_Q);
	menu.Add(t_("List macros.."), THISBACK(MacroList)).Key(K_CTRL_M);
}

void SqlConsole::ListMenu(Bar& bar)
{
	bar.Add(t_("Print record"), THISBACK(ListPrintRow));
	bar.Add(t_("Print list"), THISBACK(ListPrintList));
	bar.Add(t_("Export..."), THISBACK(ListExport));
}

void SqlConsole::ListPrintRow()
{
	String qtf;
	qtf << "[A1 ";
	if(!IsNull(lastquery))
		qtf << "[4* \1" << lastquery << "\1]&&";
	for(int i = 0; i < record.GetCount(); i++) {
		qtf << (i ? "--" : "++") << "::10@(240.240.240) \1" << StdFormat(record.Get(i, 0)) << "\1"
			"||::30@(255.255.255) \1" << StdFormat(record.Get(i, 1)) << "\1";
	}
	qtf << "++";
	Report report;
	report << qtf;
	UPP::Perform(report);
}

void SqlConsole::ListPrintList()
{
	String qtf;
	qtf << "[A1 ";
	if(!IsNull(lastquery))
		qtf << "[4* \1" << lastquery << "\1]&&";
	for(int i = 0; i < record.GetCount(); i++)
		qtf << (i ? "||" : "++") << "::@(240.240.240) [* \1"
		<< StdFormat(record.Get(i, 0)) << "\1]";
	for(int i = 0; i < list.GetCount(); i++)
		for(int j = 0; j < list.GetIndexCount(); j++)
			qtf << (j ? "||" : "--") << "::@(255.255.255) \1" << StdFormat(list.Get(i, j)) << '\1';
	qtf << "++";
	Report report;
	report << qtf;
	UPP::Perform(report);
}

void SqlConsole::ListExport()
{
	RunDlgSqlExport(cursor, lastquery, Null);
}

SqlConsole::SqlConsole(SqlSession& session)
: cursor(session)
{
	int ecy = EditField::GetStdHeight();
	lires.Horz(list, rec_err);
	rec_err << record.SizePos() << errortext.SizePos();
	errortext.SetReadOnly();
	errortext.SetFont(Courier(12));
	errortext.Hide();
	vsplit.Vert(lires, trace);
	record.NoCursor();
	record.AddColumn(t_("Column"), 5);
	record.AddColumn(t_("Value"), 10);
	trace.AddColumn(t_("Command"), 8);
	trace.AddColumn(t_("Result"), 1);
	trace.AddColumn(t_("Duration"), 1);
	trace.WhenLeftClick = THISBACK(TraceToCommand);
	trace.WhenLeftDouble = THISBACK(TraceToExecute);
	trace.WhenBar = THISBACK(TraceMenu);
	list.WhenEnterRow = THISBACK(Record);
	list.WhenLeftDouble = THISBACK(ListToCommand);
	list.WhenBar = THISBACK(ListMenu);
	Add(vsplit.VSizePos(0, ecy + 4).HSizePos(0, 0));
	Add(command.BottomPos(0, ecy).HSizePos(0, 90));
	Add(schema.BottomPos(0, ecy).RightPos(0, 80));
	schema.SetLabel(t_("&Schema"));
	schema <<= THISBACK(ObjectTree);
//	hide = callback(this, &SqlConsole::Hide);
//	Size sz = Stock.GetScreenSize();
//	SetRect((sz.cx - 600) / 2, (sz.cy - 400) / 2, 600, 400);
	ActiveFocus(command);
}

void SQLCommander(SqlSession& session) {
	SqlConsole con(session);
	con.Perform();
}

END_UPP_NAMESPACE
