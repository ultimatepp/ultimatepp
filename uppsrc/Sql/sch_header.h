// struct

#define CODETYPE(Table, b) \
struct S_##Table b { \
private: \
	S_##Table(int) {} \
	static const S_info&        GetInfo(); \
	static const S_info        *info; \
\
public: \
	static Upp::String          TableName; \
	static const Upp::SqlSet&   ColumnSet()                           { return GetInfo().set; } \
	static Upp::SqlSet          ColumnSet(const Upp::String& prefix)  { return GetInfo().GetSet(prefix); } \
	static Upp::SqlSet          Of(Upp::SqlId table)                  { return GetInfo().GetOf(table); } \
	static const Upp::Vector<Upp::SqlId>& GetColumnIds()              { return GetInfo().ids; } \
	\
	void                        Clear(); \
	void                        FieldLayoutRaw(Upp::FieldOperator& f, const Upp::String& prefix = Upp::String()); \
	void                        FieldLayout(Upp::FieldOperator& f); \
	operator                    Upp::Fields()                         { return callback(this, &S_##Table::FieldLayout); } \
	bool                        operator==(const S_##Table& x) const  { return Upp::EqualFields(const_cast<S_##Table&>(*this), const_cast<S_##Table&>(x)); } \
	bool                        operator!=(const S_##Table& x) const  { return !operator==(x); } \
	Upp::String                 ToString() const                      { return Upp::AsString((Upp::Fields)const_cast<S_##Table&>(*this)); } \
	\
	static int                  GetCount()                            { return GetInfo().GetCount(); } \
	static SqlId                GetId(int i)                          { return GetInfo().GetId(i); } \
	static int                  GetIndex(const Upp::String& id)       { return GetInfo().GetIndex(id); } \
	static int                  GetIndex(const Upp::SqlId& id)        { return GetInfo().GetIndex(id); } \
	static int                  GetWidth(int i)                       { return GetInfo().GetWidth(i); } \
	static int                  GetWidth(const Upp::SqlId& id)        { return GetInfo().GetWidth(id); } \
	\
	Upp::Ref                    GetRef(int i)                         { return info->GetRef(this, i); } \
	Upp::Ref                    GetRef(const Upp::SqlId& id)          { return info->GetRef(this, id); } \
	Upp::Value                  Get(const Upp::SqlId& id) const       { return info->Get(this, id); } \
	Upp::Value                  Get(int i) const                      { return info->Get(this, i); } \
	Upp::ValueMap               Get() const                           { return info->Get(this); } \
	void                        Set(int i, const Upp::Value& v)       { return info->Set(this, i, v); } \
	void                        Set(const Upp::SqlId& id, const Upp::Value& v)  { return info->Set(this, id, v); } \
	void                        Set(const Upp::ValueMap& m)                     { return info->Set(this, m); } \
	\
	operator                    const S_info&() const                 { return *info; } \
	\
	S_##Table(); \
	S_##Table(const Upp::ValueMap& m);

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
static Upp::SqlId colid_##name;

#define COLUMN_ARRAY(type, ctype, name, width, prec, items) \
enum { ADD_SCHEMA_PREFIX_CPP2(name,_WIDTH) = width, ADD_SCHEMA_PREFIX_CPP2(name,_PRECISION) = prec }; \
ctype ADD_SCHEMA_PREFIX_CPP(name)[items]; \
static Upp::SqlId colid_##name;
 
#define END_TYPE                 };

#define SQL_NAME(x)

#include SCHEMADIALECT

#undef CODETYPE
#undef TWO_BASES
#undef THREE_BASES

// SqlId

#define DOID(x) extern Upp::SqlId ADD_SCHEMA_PREFIX_CPP(x);

#define SQL_NAME(x)

#include SCHEMADIALECT
