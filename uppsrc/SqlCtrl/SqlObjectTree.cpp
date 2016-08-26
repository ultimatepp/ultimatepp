#include "SqlCtrl.h"
#include "SqlDlg.h"

namespace Upp {

class DlgSqlExport : public WithSqlExportLayout<TopWindow>
{
public:
	typedef DlgSqlExport CLASSNAME;
	DlgSqlExport();

	void    Run(Sql& cursor, String command, String tablename);
	void    Serialize(Stream& stream);

private:
	void    SyncUI();
	void    Toggle();

private:
	ArrayOption exp;
	enum {
		FMT_TEXT,
		FMT_SQL,
	};
	enum {
		DELIM_TAB,
		DELIM_SEMICOLON,
	};
	String recent_file;
};

void RunDlgSqlExport(Sql& cursor, String command, String tablename)
{
	DlgSqlExport().Run(cursor, command, tablename);
}

DlgSqlExport::DlgSqlExport()
{
	CtrlLayoutOKCancel(*this, "");
	columns.AddColumn(t_("Column name"));
	columns.AddColumn(t_("Type"));
	columns.AddColumn(t_("Width"));
	HeaderCtrl::Column& hc = exp.AddColumn(columns, "").Margin(0).HeaderTab();
	hc.SetMargin(0).Fixed(18).SetAlign(ALIGN_CENTER).SetImage(CtrlImg::smallcheck());
	hc.WhenAction = THISBACK(Toggle);
	format <<= FMT_TEXT;
	delimiters <<= DELIM_SEMICOLON;
	quote <<= true;
	format <<= THISBACK(SyncUI);
}

void DlgSqlExport::SyncUI()
{
	int f = ~format;
	delimiters.Enable(f == FMT_TEXT);
	quote.Enable(f == FMT_TEXT);
	identity_insert.Enable(f == FMT_SQL);
	object_name.Enable(f == FMT_SQL);
}

void DlgSqlExport::Toggle()
{
	bool state = true;
	if(columns.IsCursor())
		state = !(bool)columns.Get(3);
	for(int i = 0; i < columns.GetCount(); i++)
		columns.Set(i, 3, state);
}

void DlgSqlExport::Run(Sql& cursor, String command, String tablename)
{
	Title(Nvl(tablename, t_("SQL query")) + t_(" export"));
	object_name <<= tablename;
	if(!cursor.Execute(command)) {
		Exclamation(NFormat(t_("Error executing [* \1%s\1]: \1%s"), command, cursor.GetLastError()));
		return;
	}
	for(int i = 0; i < cursor.GetColumns(); i++) {
		const SqlColumnInfo& sci = cursor.GetColumnInfo(i);
		String type;
		switch(sci.type) {
			case BOOL_V:
			case INT_V: type = t_("integer"); break;
			case DOUBLE_V: type = t_("real number"); break;
			case STRING_V:
			case WSTRING_V: type = t_("string"); break;
			case DATE_V: type = t_("date"); break;
			case TIME_V: type = t_("date/time"); break;
			case /*ORA_BLOB_V*/-1: type = t_("BLOB"); break;
			case /*ORA_CLOB_V*/-2: type = t_("CLOB"); break;
			default: type = FormatInt(sci.type); break;
		}
		columns.Add(sci.name, sci.type, sci.width, 1);
	}
	static String cfg;
	LoadFromString(*this, cfg);
	SyncUI();
	while(TopWindow::Run() == IDOK)
		try {
			String out_table = ~object_name;
			String delim;
			switch((int)~delimiters) {
				case DELIM_TAB: delim = "\t"; break;
				case DELIM_SEMICOLON: delim = ";"; break;
			}
			Vector<int> out;
			String colstr;
			String title;
			for(int i = 0; i < columns.GetCount(); i++)
				if(columns.Get(i, 3)) {
					out.Add(i);
					String cname = cursor.GetColumnInfo(i).name;
					colstr << (i ? ", " : "") << cname;
					if(i) title << delim;
					title << cname;
				}
			if(out.IsEmpty()) {
				throw Exc(t_("No columns selected!"));
				continue;
			}
			String rowbegin, rowend;
			int fmt = ~format;
			FileSel fsel;
			String ext;
			switch(fmt) {
				case FMT_TEXT: {
					rowend = "";
					ext = ".txt";
					fsel.Type(t_("Text files (*.txt)"), "*.txt");
					break;
				}
				case FMT_SQL: {
					if(identity_insert)
						rowbegin << "set identity_insert " << out_table << " on ";
					rowbegin << "insert into " << out_table << "(" << colstr << ") values (";
					rowend = ");";
					ext = ".sql";
					fsel.Type(t_("SQL scripts (*.sql)"), "*.sql");
					break;
				}
			}
			fsel.AllFilesType().DefaultExt(ext.Mid(1));
			if(!IsNull(recent_file))
				fsel <<= ForceExt(recent_file, ext);
			if(!fsel.ExecuteSaveAs(t_("Save export as")))
				continue;
			recent_file = ~fsel;
			FileOut fo;
			if(!fo.Open(recent_file)) {
				Exclamation(NFormat(t_("Error creating file [* \1%s\1]."), recent_file));
				continue;
			}
			if(fmt == FMT_TEXT)
				fo.PutLine(title);
			Progress progress(t_("Exporting row %d"));
			while(cursor.Fetch()) {
				String script = rowbegin;
				for(int i = 0; i < out.GetCount(); i++) {
					Value v = cursor[out[i]];
					switch(fmt) {
						case FMT_TEXT: {
							if(i)
								script.Cat(delim);
							if(IsString(v) && quote) {
								String s = v;
								script << '\"';
								for(const char *p = s, *e = s.End(); p < e; p++)
									if(*p == '\"')
										script.Cat("\"\"");
									else
										script.Cat(*p);
								script << '\"';
							}
							else
								script << StdFormat(v);
							break;
						}
						case FMT_SQL: {
							if(i) script.Cat(", ");
							script << SqlCompile(cursor.GetDialect(), SqlFormat(v));
							break;
						}
					}
				}
				script << rowend;
				fo.PutLine(script);
/*
				if(autocommit && --left <= 0) {
					fo.PutLine("commit;");
					left = autocommit;
				}
*/
				if(progress.StepCanceled()) {
					Exclamation(t_("Export aborted!"));
					return;
				}
			}
			fo.Close();
			if(fo.IsError())
				throw Exc(NFormat(t_("Error writing file %s."), recent_file));
			break;
		}
		catch(Exc e) {
			ShowExc(e);
		}

	cfg = StoreAsString(*this);
}

void DlgSqlExport::Serialize(Stream& stream)
{
	int version = 1;
	stream / version % format % recent_file;
}

class SqlObjectTree : public TopWindow {
public:
	typedef SqlObjectTree CLASSNAME;
	SqlObjectTree(SqlSession& session);

	void        Run();

private:
	enum OBJTYPE {
		OBJ_NULL,
		OBJ_SCHEMA,
		OBJ_SCHEMA_OBJECTS,
		OBJ_TABLES,
		OBJ_VIEWS,
		OBJ_SEQUENCES,
		OBJ_TABLE_COLUMNS,
		OBJ_SEQUENCE,
		OBJ_COLUMN,
		OBJ_PRIMARY_KEY,
		OBJ_ROWID,
	};
	struct Item {
		Item(OBJTYPE type = OBJ_NULL, String schema = Null, String object = Null)
		: type(type), schema(schema), object(object) {}
		Item(const Value& v) { if(IsTypeRaw<Item>(v)) *this = ValueTo<Item>(v); }
		operator Value() const { return RawToValue(*this); }

		int type;
		String schema;
		String object;
	};

	void        Open(int node);
	void        OpenSchema(int node);
	void        OpenTables(int node, const Item& item);
	void        OpenViews(int node, const Item& item);
	void        OpenSequences(int node, const Item& item);
	void        OpenTableColumns(int node, const Item& item);

	void        ToolLocal(Bar& bar);
	void        TableExport(String table_name);

private:
	TreeCtrl    schema;
	SqlSession& session;
};

void SQLObjectTree(SqlSession& session) { SqlObjectTree(session).Run(); }

SqlObjectTree::SqlObjectTree(SqlSession& sess)
: session(sess)
{
	Title(t_("SQL object tree"));
	Sizeable().MaximizeBox();
	Add(schema.SizePos());
	schema.WhenOpen = THISBACK(Open);
	schema.SetRoot(CtrlImg::Computer(), Item(OBJ_SCHEMA), t_("Schemas"));
	schema.WhenBar = THISBACK(ToolLocal);
}

void SqlObjectTree::Run() {
	TopWindow::Run();
}

void SqlObjectTree::Open(int node)
{
	const Item& item = ValueTo<Item>(schema[node]);
	switch(item.type) {
		case OBJ_SCHEMA:         OpenSchema(node); break;
		case OBJ_TABLES:         OpenTables(node, item); break;
		case OBJ_VIEWS:          OpenViews(node, item); break;
		case OBJ_SEQUENCES:      OpenSequences(node, item); break;
		case OBJ_TABLE_COLUMNS:  OpenTableColumns(node, item); break;
	}
}

void SqlObjectTree::OpenSchema(int node) {
	try {
		schema.RemoveChildren(node);
		Vector<String> schemas = session.EnumDatabases();
		Sort(schemas);
		for(int i = 0; i < schemas.GetCount(); i++) {
			String sname = schemas[i];
			int snode = schema.Add(node, CtrlImg::Dir(), Item(OBJ_SCHEMA_OBJECTS, sname), sname, true);
			schema.Add(snode, CtrlImg::Dir(), Item(OBJ_TABLES, sname), t_("Tables"), true);
			schema.Add(snode, CtrlImg::Dir(), Item(OBJ_VIEWS, sname), t_("Views"), true);
			schema.Add(snode, CtrlImg::Dir(), Item(OBJ_SEQUENCES, sname), t_("Sequences"), true);
		}
	}
	catch(Exc e) {
		ShowExc(e);
	}
}

void SqlObjectTree::OpenTables(int node, const Item& item)
{
	try {
		schema.RemoveChildren(node);
		Vector<String> tables = session.EnumTables(item.schema);
		Sort(tables);
		for(int i = 0; i < tables.GetCount(); i++)
			schema.Add(node, CtrlImg::File(),
				Item(OBJ_TABLE_COLUMNS, item.schema, tables[i]),
				tables[i], true);
	} catch(Exc e) { ShowExc(e); }
}

void SqlObjectTree::OpenViews(int node, const Item& item) {
	try {
		schema.RemoveChildren(node);
		Vector<String> views = session.EnumViews(item.schema);
		Sort(views);
		for(int i = 0; i < views.GetCount(); i++)
			schema.Add(node, CtrlImg::File(),
				Item(OBJ_TABLE_COLUMNS, item.schema, views[i]),
				views[i], true);
	} catch(Exc e) { ShowExc(e); }
}

void SqlObjectTree::OpenSequences(int node, const Item& item) {
	try {
		schema.RemoveChildren(node);
		Vector<String> sequences = session.EnumSequences(item.schema);
		Sort(sequences);
		for(int i = 0; i < sequences.GetCount(); i++)
			schema.Add(node, CtrlImg::File(),
				Item(OBJ_SEQUENCE, item.schema, sequences[i]),
				sequences[i], true);
	} catch(Exc e) { ShowExc(e); }
}

void SqlObjectTree::OpenTableColumns(int node, const Item& item)
{
	try {
		schema.RemoveChildren(node);
		Vector<SqlColumnInfo> columns = session.EnumColumns(item.schema, item.object);
		for(int i = 0; i < columns.GetCount(); i++)
			schema.Add(node, CtrlImg::Hd(), Item(OBJ_COLUMN, item.schema, item.object), (Value)columns[i].name);
		Vector<String> pk = session.EnumPrimaryKey(item.schema, item.object);
		if(!pk.IsEmpty()) {
			String pklist;
			for(int i = 0; i < pk.GetCount(); i++)
				pklist << (i ? "; " : t_("Primary key: ")) << pk[i];
			schema.Add(node, Null, Item(OBJ_PRIMARY_KEY, item.schema, item.object), (Value)pklist);
		}
		String rowid = session.EnumRowID(item.schema, item.object);
		if(!IsNull(rowid))
			schema.Add(node, Null, Item(OBJ_ROWID, item.schema, item.object), Value("RowID: " + rowid));
	} catch(Exc e) { ShowExc(e); }
}

void SqlObjectTree::ToolLocal(Bar& bar)
{
	if(schema.IsCursor()) {
		const Item& item = ValueTo<Item>(~schema);
		switch(item.type) {
			case OBJ_TABLE_COLUMNS: {
				bar.Add(t_("Export"), THISBACK1(TableExport, item.schema + "." + item.object));
			}
		}
	}
}

void SqlObjectTree::TableExport(String table_name)
{
	String schema, table = table_name;
	int f = table.Find('.');
	if(f >= 0) {
		schema = table.Left(f);
		table = table.Mid(f + 1);
	}
	Vector<SqlColumnInfo> info = session.EnumColumns(schema, table);
	Sql cursor(session);
	RunDlgSqlExport(cursor, "select * from " + schema + "." + table, table);
}

}
