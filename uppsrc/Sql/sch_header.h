// struct

#define CODETYPE(Table, b) \
struct S_##Table b { \
private: \
	S_##Table(int) {} \
	static const S_info&        GetInfo(); \
	static const S_info        *info; \
\
public: \
	static const char           TableName[]; \
	static const SqlSet&        ColumnSet()                       { return GetInfo().set; } \
	static SqlSet               ColumnSet(const String& prefix)   { return GetInfo().GetSet(prefix); } \
	static SqlSet               Of(SqlId table)                   { return GetInfo().GetOf(table); } \
	static const Vector<SqlId>& GetColumnIds()                    { return GetInfo().ids; } \
	\
	void                        Clear(); \
	void                        FieldLayoutRaw(FieldOperator& f, const String& prefix = String()); \
	void                        FieldLayout(FieldOperator& f); \
	operator                    Fields()                              { return callback(this, &S_##Table::FieldLayout); } \
	bool                        operator==(const S_##Table& x) const  { return EqualFields(const_cast<S_##Table&>(*this), const_cast<S_##Table&>(x)); } \
	bool                        operator!=(const S_##Table& x) const  { return !operator==(x); } \
	String                      ToString() const                      { return AsString((Fields)const_cast<S_##Table&>(*this)); } \
	\
	static int                  GetCount()                            { return GetInfo().GetCount(); } \
	static SqlId                GetId(int i)                          { return GetInfo().GetId(i); } \
	static int                  GetIndex(const String& id)            { return GetInfo().GetIndex(id); } \
	static int                  GetIndex(const SqlId& id)             { return GetInfo().GetIndex(id); } \
	static int                  GetWidth(int i)                       { return GetInfo().GetWidth(i); } \
	static int                  GetWidth(const SqlId& id)             { return GetInfo().GetWidth(id); } \
	\
	Ref                         GetRef(int i)                         { return info->GetRef(this, i); } \
	Ref                         GetRef(const SqlId& id)               { return info->GetRef(this, id); } \
	Value                       Get(const SqlId& id) const            { return info->Get(this, id); } \
	Value                       Get(int i) const                      { return info->Get(this, i); } \
	ValueMap                    Get() const                           { return info->Get(this); } \
	void                        Set(int i, const Value& v)            { return info->Set(this, i, v); } \
	void                        Set(const SqlId& id, const Value& v)  { return info->Set(this, id, v); } \
	void                        Set(const ValueMap& m)                { return info->Set(this, m); } \
	\
	operator                    const S_info&() const                 { return *info; } \
	\
	S_##Table(); \
	S_##Table(const ValueMap& m);

#define TYPE(Table)                  CODETYPE(Table, __NIL)
#define TYPE_I(Table, b)             CODETYPE(Table, : public S_##b)
#define TWO_BASES(b1, b2)            : public S_##b1, public S_##b2
#define TYPE_II(Table, b1, b2)       CODETYPE(Table, TWO_BASES(b1, b2));
#define THREE_BASES(b1, b2, b3)      : public S_##b1, public S_##b2, public S_##b3
#define TYPE_III(Table, b1, b2, b3)  CODETYPE(Table, THREE_BASES(b1, b2, b3));

#define VAR(type, x)             S_##type x;

#define COLUMN(type, ctype, name, width, prec) \
enum { ADD_SCHEMA_PREFIX_CPP2(name,_WIDTH) = width, ADD_SCHEMA_PREFIX_CPP2(name,_PRECISION) = prec }; \
ctype ADD_SCHEMA_PREFIX_CPP(name); \
static SqlId colid_##name;

#define COLUMN_ARRAY(type, ctype, name, width, prec, items) \
enum { ADD_SCHEMA_PREFIX_CPP2(name,_WIDTH) = width, ADD_SCHEMA_PREFIX_CPP2(name,_PRECISION) = prec }; \
ctype ADD_SCHEMA_PREFIX_CPP(name)[items]; \
static SqlId colid_##name;
 
#define END_TYPE                 };

#include SCHEMADIALECT

#undef CODETYPE
#undef TWO_BASES
#undef THREE_BASES

// SqlId

#define DOID(x) extern SqlId ADD_SCHEMA_PREFIX_CPP(x);            
//#define DOID(x)                  extern SqlId x;

#include SCHEMADIALECT
