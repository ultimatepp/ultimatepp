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
	void SqlName(const char *name);
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


template<> inline void SqlSchemaInitClear(int& a)    { a = Null; }
template<> inline void SqlSchemaInitClear(double& a) { a = Null; }
template<> inline void SqlSchemaInitClear(int64& a)  { a = Null; }
template<> inline void SqlSchemaInitClear(bool& a)   { a = false; }

template <class T>
inline void SqlSchemaClear(T& a)          { a = Null; }

inline void SqlSchemaClear(bool& a)       { a = false; }

template <class T>
inline void SqlSchemaClear(T *a, int n) {
	while(n--)
		SqlSchemaClear(*a++);
}

void SchDbInfoTable(const char *table);
void SchDbInfoType(const char *table);
void SchDbInfoColumn(const char *column);
void SchDbInfoColumnArray(const char *name, int items);
void SchDbInfoVar(void (*fn)(), const char *name);
void SchDbInfoPrimaryKey();
void SchDbInfoReferences(const char *table);
void SchDbInfoReferences(const char *table, const char *column);

SqlBool FindSchJoin(const String& tables);

Vector<String>   GetSchTables();
Vector<String>   GetSchColumns(const String& table);

struct SchColumnInfo {
	String name;
	String references;
	String foreign_key;
	
	String ToString() const { return name; }
};

ArrayMap< String, Array<SchColumnInfo> > GetSchAll();

String ExportSch(SqlSession& session, const String& database);
String ExportIds(SqlSession& session, const String& database);

#ifndef NOAPPSQL
String ExportSch(const String& database);
String ExportIds(const String& database);
#endif

struct S_info {
	struct Column : Moveable<Column> {
		intptr_t    offset;
		RefManager *manager;
		int         width;
	};
	VectorMap<String, Column> column;
	SqlSet        set;
	Vector<SqlId> ids;
	
	int      GetCount() const                               { return column.GetCount(); }

	SqlId    GetId(int i) const                             { return column.GetKey(i); }
	int      GetIndex(const String& id) const               { return column.Find(id); }
	int      GetIndex(const SqlId& id) const                { return column.Find(~id); }
	
	int      GetWidth(int i) const                          { return column[i].width; }
	int      GetWidth(const SqlId& id) const;

	Ref      GetRef(const void *s, int i) const;
	Ref      GetRef(const void *s, const SqlId& id) const;
	Value    Get(const void *s, const SqlId& id) const;
	Value    Get(const void *s, int i) const;
	ValueMap Get(const void *s) const;
	
	void     Set(const void *s, int i, const Value& v) const;
	void     Set(const void *s, const SqlId& id, const Value& v) const;
	void     Set(const void *s, const ValueMap& m) const;
	
	SqlSet   GetSet(const String& prefix) const;
	SqlSet   GetOf(const SqlId& table) const;
	
	void     Init();
};

struct S_info_maker : FieldOperator {
	S_info&  info;
	void    *s;

	virtual void Field(const char *name, Ref f, bool *b);
	virtual void Width(int width);

	S_info_maker(S_info& f, void *s) : info(f), s(s) {}
};

#if 0 // For documentation only, 'type' is a placeholder for the .sch TABLE or TYPE name
struct S_type {
	static const char           TableName[];
	static const SqlSet&        ColumnSet();
	static SqlSet               ColumnSet(const String& prefix);
	static SqlSet               Of(SqlId table);
	static const Vector<SqlId>& GetColumnIds();
	
	void                        Clear();

	void                        FieldLayoutRaw(FieldOperator& f, const String& prefix = String());
	void                        FieldLayout(FieldOperator& f);
	operator                    Fields();

	bool                        operator==(const S_type& x) const;
	bool                        operator!=(const S_type& x) const;
	String                      ToString() const;

	static int                  GetCount();
	static SqlId                GetId(int i);
	static int                  GetIndex(const String& id);
	static int                  GetIndex(const SqlId& id);
	static int                  GetWidth(int i);
	static int                  GetWidth(const SqlId& id);

	Ref                         GetRef(int i);
	Ref                         GetRef(const SqlId& id);
	Value                       Get(const SqlId& id) const;
	Value                       Get(int i) const;
	ValueMap                    Get() const;
	void                        Set(int i, const Value& v);
	void                        Set(const SqlId& id, const Value& v);
	void                        Set(const ValueMap& m);

	S_type();
	S_type(const ValueMap& m);
};
#endif

// support for SQL_RENAME

extern const char *RegSqlName__;

struct RegSqlId__ {
	RegSqlId__(const char *name) { RegSqlName__ = name; }
};

void SqlRename__(const char *name);

const char *SqlResolveId__(const char *id);
