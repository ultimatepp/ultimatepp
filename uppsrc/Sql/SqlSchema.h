// attribute macros
//
// @i      array index (or "" when no array)
// @t      table name
// @c      column name (with index)
// @C      column name (without index)
// @T      column type
// @x      combined column+table up to maxidlen characters (default 24) (for constraint names)

class SqlSchema {
protected:
	Vector<String> script;
	String         schemaname;
	String         prefix;
	String         table;
	String         table_suffix;
	String         type;
	String         column;
	bool           firstcolumn;
	String         attribute;
	int            items;
	int            dialect;

	int            maxidlen;

public:
	virtual void FlushColumn();
	virtual void FlushTable();

	void Object(const char *text, const char *drop);

	void Table(const char *name);
	void TableSuffix(const char *suffix);
	void Column(const char *type, const char *name);
	void ColumnArray(const char *type, const char *name, int items);
	void Attribute(const char *attr, const char *drop);
	void InlineAttribute(const char *attr);
	void EndTable()                                        { FlushTable(); }

	void Config(const char *config, const char *drop);
	void Upgrade(const char *text);

	String  CurrentTable() const;
	String  CurrentColumn() const                          { return column; }
	String  CurrentType() const                            { return Expand(type); }
	String  Expand(const char *txt, int i = Null) const;

	enum {
		SCHEMA, DROPSCHEMA, ATTRIBUTES, DROPATTRIBUTES, UPGRADE, DROPUPGRADE, CONFIG, DROPCONFIG
	};

	String& Script(int si);
	String  Script(int si) const;

	String& Schema();
	String& SchemaDrop();
	String& Attributes();
	String& AttributesDrop();
	String& Upgrade();
	String& UpgradeDrop();
	String& Config();
	String& ConfigDrop();

	String  NormalFileName(int i, const char *dir = NULL, const char *name = NULL) const;
	bool    ScriptChanged(int i, const char *dir = NULL, const char *name = NULL) const;
	bool    UpdateNormalFile(int i, const char *dir = NULL, const char *name = NULL) const;
	void    SaveNormal(const char *dir = NULL, const char *name = NULL) const;

	void Var(void (*type)(SqlSchema& s), const char *name);

	SqlSchema& SchemaName(String sch)                          { schemaname = sch; return *this; }
	SqlSchema& MaxIDLen(int n)                                 { maxidlen = n; return *this; }
	
	int        GetDialect() const                              { return dialect; }

#ifndef NOAPPSQL
	SqlSchema(int dialect = SQL.GetDialect());
#else
	SqlSchema(int dialect);
#endif
	virtual ~SqlSchema() {}
};

void operator*(SqlSchema& schema, const SqlInsert& insert);

template <class T>
inline void SqlSchemaInitClear(T& a) {}

template <class T>
inline void SqlSchemaInitClear(T *a, int n) {
	while(n--)
		SqlSchemaInitClear(*a++);
}

inline void SqlSchemaInitClear(int& a)    { a = Null; }
inline void SqlSchemaInitClear(double& a) { a = Null; }
inline void SqlSchemaInitClear(int64& a)  { a = Null; }
inline void SqlSchemaInitClear(bool& a)   { a = false; }

template <class T>
inline void SqlSchemaClear(T& a)          { a = Null; }

inline void SqlSchemaClear(bool& a)       { a = false; }

template <class T>
inline void SqlSchemaClear(T *a, int n) {
	while(n--)
		SqlSchemaClear(*a++);
}

void SchDbInfoTable(const char *table);
void SchDbInfoColumn(const char *column);
void SchDbInfoColumnArray(const char *name, int items);
void SchDbInfoVar(void (*fn)(), const char *name);
void SchDbInfoPrimaryKey();
void SchDbInfoReferences(const char *table);
void SchDbInfoReferences(const char *table, const char *column);

SqlBool FindSchJoin(const String& tables);

Vector<String>  GetSchColumns(const String& table);

String ExportSch(SqlSession& session, const String& database);
String ExportIds(SqlSession& session, const String& database);

#ifndef NOAPPSQL
String ExportSch(const String& database);
String ExportIds(const String& database);
#endif
