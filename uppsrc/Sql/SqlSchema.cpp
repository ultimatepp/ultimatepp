#include "Sql.h"

namespace Upp {

void SqlSchema::FlushColumn() {
	if(column.IsEmpty()) return;
	for(int i = items ? 0 : Null; items ? i < items : IsNull(i); i++) {
		String cn = Format("\t%-20s ", ~Expand("@c", i)) + CurrentType();
		if(dialect == SQLITE3)
			cn = Format(" %s ", ~Expand("@c", i)) + CurrentType();
		String attr = Expand(attribute, i);
		String cd = cn + attr;
		if(firstcolumn) {
			Schema() << Expand("create table @t (\n");
			SchemaDrop() << Expand("drop table @t;\n");
			if(dialect == SQLITE3)
				Upgrade() << Expand("create table @t ( ") << cd << " )" << table_suffix << ";\n";
			else
				Upgrade() << Expand("create table @t (\n") << cd << "\n)" << table_suffix << ";\n\n";
		}
		else
		{
			if(dialect == MSSQL) {
				Upgrade() << Expand("alter table @t add ") << cn << ";\n";
				Upgrade() << Expand("alter table @t alter column ") << cd << ";\n";
			}
			else
			if(dialect == MY_SQL) {
				Upgrade() << Expand("alter table @t add ") << cn << ";\n";
				Upgrade() << Expand("alter table @t modify ") << cd << ";\n";
			}
			else
			if (dialect == SQLITE3 || dialect == MY_SQL)
				Upgrade() << Expand("alter table @t add ") << cd << ";\n";
			else
			if (dialect == PGSQL)
				Upgrade() << Expand("alter table @t add \n") << cd << "\n;\n\n";
			else
				Upgrade() << Expand("alter table @t add (\n") << cd << "\n);\n\n";
			Schema() << ",\n";
		}
		Schema() << cd;
		firstcolumn = false;
	}
	column.Clear();
}

void SqlSchema::FlushTable() {
	FlushColumn();
	if(!table.IsEmpty())
		Schema() << "\n)" << table_suffix << ";\n\n";
	table.Clear();
}

void SqlSchema::Object(const char *text, const char *drop) {
	FlushTable();
	Schema() << text << '\n';
	SchemaDrop() << drop << '\n';
}

void SqlSchema::Upgrade(const char *text) {
	Upgrade() << text << '\n';
}

void SqlSchema::Table(const char *name) {
	FlushTable();
	table = name;
	table_suffix = Null;
	Attributes() << '\n';
	AttributesDrop() << '\n';
	firstcolumn = true;
}

void SqlSchema::TableSuffix(const char *suffix)
{
	table_suffix << suffix;
}

void SqlSchema::Column(const char *_type, const char *name) {
	FlushColumn();

	items = 0;
	type = _type;
	column = prefix + name;
	attribute.Clear();
}

void SqlSchema::SqlName(const char *name)
{
	(firstcolumn ? table : column) = prefix + name;
}

void SqlSchema::ColumnArray(const char *type, const char *name, int _items) {
	Column(type, name);
	items = _items;
}

void SqlSchema::Attribute(const char *attr, const char *drop) {
	for(int i = items ? 0 : Null; items ? i < items : IsNull(i); i++) {
		Attributes() << Expand(attr, i) << '\n';
		AttributesDrop() << Expand(drop, i) << '\n';
	}
}

void SqlSchema::InlineAttribute(const char *attr) {
	attribute << (*attr == ',' ? "" : " ") << Expand(attr);
}

void SqlSchema::Config(const char *config, const char *drop) {
	Config() << config;
	ConfigDrop() << drop;
}

String SqlSchema::CurrentTable() const
{
	return IsNull(schemaname) ? table : schemaname + '.' + table;
}

String SqlSchema::Expand(const char *txt, int i) const
{
	String n, r;
	if(!IsNull(i))
		n = Format("%d", i);
	String c = CurrentColumn() + n;
	String v = table + '$' + c;
	v = v.Mid(max(0, min(v.GetLength(), v.GetLength() - maxidlen)));
	while(*txt) {
		if(*txt == '@' && txt[1]) {
			switch(*++txt) {
			case 'i': r.Cat(n); break;
			case 't': r.Cat(CurrentTable()); break;
			case 'u': r.Cat(table); break;
			case 'c': r.Cat(c); break;
			case 'C': r.Cat(CurrentColumn()); break;
			case 'T': r.Cat(CurrentType()); break;
			case 'x': r.Cat(v); break;
			case 's': if(!IsNull(schemaname)) { r.Cat(schemaname); r.Cat('.'); } break;
			default:  r.Cat(*txt); break;
			}
		}
		else
			r.Cat(*txt);
		txt++;
	}
	return r;
}

void SqlSchema::Var(void (*type)(SqlSchema& s), const char *name) {
	String p = prefix;
	prefix << name << '$';
	(*type)(*this);
	prefix = p;
}

String& SqlSchema::Script(int si)   { return script.At(si); }
String  SqlSchema::Script(int si) const  { return si < script.GetCount() ? script[si] : String(); }
String& SqlSchema::Schema()         { return Script(SCHEMA); }
String& SqlSchema::SchemaDrop()     { return Script(DROPSCHEMA); }
String& SqlSchema::Attributes()     { return Script(ATTRIBUTES); }
String& SqlSchema::AttributesDrop() { return Script(DROPATTRIBUTES); }
String& SqlSchema::Upgrade()        { return Script(UPGRADE); }
String& SqlSchema::UpgradeDrop()    { return Script(DROPUPGRADE); }
String& SqlSchema::Config()         { return Script(CONFIG); }
String& SqlSchema::ConfigDrop()     { return Script(DROPCONFIG); }

String SqlSchema::NormalFileName(int i, const char *dir, const char *name) const {
	ASSERT(i >= 0 && i <= DROPCONFIG);
	String n;
	if(name)
		n = name;
	else
#ifdef PLATFORM_WIN32
		n = ForceExt(GetFileName(GetExeFilePath()), ".sql");
#endif
#ifdef PLATFORM_POSIX
		n = "script.sql";
#endif
	const char *pfx[] = {
		"S_", "SD_", "A_", "AD_", "U_", "UD_", "C_", "CD_"
	};
	return dir ? AppendFileName(dir, pfx[i] + n) : ConfigFile(pfx[i] + n);
}

bool SqlSchema::ScriptChanged(int i, const char *dir, const char *name) const {
	String fn = NormalFileName(i, dir, name);
	return LoadFile(fn) != Script(i);
}

bool SqlSchema::UpdateNormalFile(int i, const char *dir, const char *name) const {
	String fn = NormalFileName(i, dir, name);
	if(LoadFile(fn) != Script(i)) {
		DeleteFile(fn);
		SaveFile(fn, Script(i));
		return true;
	}
	return false;
}

void SqlSchema::SaveNormal(const char *dir, const char *name) const {
	for(int i = SCHEMA; i <= DROPCONFIG; i++)
		UpdateNormalFile(i, dir, name);
}

SqlSchema::SqlSchema(int dialect_)
{
	dialect = dialect_;
	maxidlen = 63;
	if(dialect == MSSQL)
		maxidlen = 128;
	if(dialect == ORACLE)
		maxidlen = 24;
}

void operator*(SqlSchema& schema, const SqlInsert& insert) {
	schema.Config() << SqlStatement(insert).Get(schema.GetDialect()) << ";\n";
	schema.ConfigDrop() << SqlStatement(Delete(insert.GetTable())
		                                .Where(insert.GetKeyColumn() == insert.GetKeyValue()))
		                   .Get(schema.GetDialect())
	                    << ";\n";
}

VectorMap<String, String>& sSqlRename()
{
	static VectorMap<String, String> x;
	return x;
}

const char *RegSqlName__;

void SqlRename__(const char *name)
{
	static auto& rename = sSqlRename();
	if(RegSqlName__)
		rename.Add(RegSqlName__, name);
	RegSqlName__ = NULL;
}

const char *SqlResolveId__(const char *id)
{
	static auto& rename = sSqlRename();
	int q;
	if(rename.GetCount() && (q = rename.Find(id)) >= 0)
		return rename[q];
	return id; // cannot be conditional expression as we are returning const char * !
}

};