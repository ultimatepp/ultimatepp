// struct

#define CODETYPE(Table, b) \
struct S_##Table b { \
public:\
	void Shrink(); \
	void Clear(); \
	static const char         TableName[]; \
	static const SqlSet&      ColumnSet(); \
	static SqlSet             ColumnSet(const String& prefix); \
	static SqlSet             Of(SqlId table); \
	void FieldLayoutRaw(FieldOperator& f, const String& prefix = String()); \
	void FieldLayout(FieldOperator& f); \
	operator Fields()              { return callback(this, &S_##Table::FieldLayout); } \
	bool operator==(const S_##Table& x) const  { return EqualFields(const_cast<S_##Table&>(*this), const_cast<S_##Table&>(x)); } \
	bool operator!=(const S_##Table& x) const  { return !EqualFields(const_cast<S_##Table&>(*this), const_cast<S_##Table&>(x)); } \
	S_##Table();

#define TYPE(Table)                  CODETYPE(Table, __NIL)
#define TYPE_I(Table, b)             CODETYPE(Table, : public S_##b)
#define TWO_BASES(b1, b2)            : public S_##b1, public S_##b2
#define TYPE_II(Table, b1, b2)       CODETYPE(Table, TWO_BASES(b1, b2));
#define THREE_BASES(b1, b2, b3)      : public S_##b1, public S_##b2, public S_##b3
#define TYPE_III(Table, b1, b2, b3)  CODETYPE(Table, THREE_BASES(b1, b2, b3));

#define VAR(type, x)             S_##type x;

#define COLUMN(type, ctype, name, width, prec) \
enum { ADD_SCHEMA_PREFIX_CPP2(name,_WIDTH) = width, ADD_SCHEMA_PREFIX_CPP2(name,_PRECISION) = prec }; ctype ADD_SCHEMA_PREFIX_CPP(name);

#define COLUMN_ARRAY(type, ctype, name, width, prec, items) \
enum { ADD_SCHEMA_PREFIX_CPP2(name,_WIDTH) = width, ADD_SCHEMA_PREFIX_CPP2(name,_PRECISION) = prec }; ctype ADD_SCHEMA_PREFIX_CPP(name)[items];
 
#define END_TYPE                 };

#include SCHEMADIALECT

#undef CODETYPE
#undef TWO_BASES
#undef THREE_BASES

// SqlId

#define DOID(x) extern SqlId ADD_SCHEMA_PREFIX_CPP(x);            
//#define DOID(x)                  extern SqlId x;

#include SCHEMADIALECT
