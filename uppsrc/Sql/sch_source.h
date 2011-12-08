// SqlId

#define DOID(x) SqlId ADD_SCHEMA_PREFIX_CPP(x)(#x);
//#define DOID(x)                 SqlId x(#x);

#include SCHEMADIALECT

// constructor

#define TYPE(x)  S_##x::S_##x() {
#define COLUMN(type, ctype, name, width, prec)               SqlSchemaInitClear(ADD_SCHEMA_PREFIX_CPP(name));
#define COLUMN_ARRAY(type, ctype, name, width, prec, items)  SqlSchemaInitClear(ADD_SCHEMA_PREFIX_CPP(name), items);
#define END_TYPE }

#include SCHEMADIALECT

// constructor, ColumnSet, TableName, SqlField

#define TYPECODE(x, y) \
const char S_##x::TableName[] = #x; \
const SqlSet& S_##x::ColumnSet() { \
	static SqlSet set = ColumnSet(""); \
	return set; \
} \
SqlSet S_##x::Of(SqlId id) { \
	return ColumnSet(id.ToString() + '.'); \
} \
SqlSet S_##x::ColumnSet(const String& prefix) { \
	SqlSet set; \
	y \

#define TYPE(x)                 TYPECODE(x, __NIL)

#define BASECODE(b) \
set.Cat(S_##b::ColumnSet(prefix));
#define TYPE_I(x, b)            TYPECODE(x, BASECODE(b))

#define BASE2CODE(b1, b2)\
set.Cat(S_##b1::ColumnSet(prefix)); \
set.Cat(S_##b2::ColumnSet(prefix));
#define TYPE_II(x, b1, b2)      TYPECODE(x, BASE2CODE(b1, b2))

#define BASE3CODE(b1, b2, b3)\
set.Cat(S_##b1::ColumnSet(prefix)); \
set.Cat(S_##b2::ColumnSet(prefix)); \
set.Cat(S_##b3::ColumnSet(prefix));
#define TYPE_III(x, b1, b2, b3) TYPECODE(x, BASE3CODE(b1, b2, b3))

#define VAR(type, x)             td_var(set, prefix, #x, &S_##type::ColumnSet);

#define COLUMN(type, ctype, name, width, prec)               td_scalar(set, prefix, #name);
#define COLUMN_ARRAY(type, ctype, name, width, prec, items)  td_array(set, prefix, #name, items);

#define END_TYPE                 return set; }

#include SCHEMADIALECT

#undef  TYPECODE
#undef  BASECODE
#undef  BASE2CODE
#undef  BASE3CODE

// Clear

#define TYPE(x)\
void S_##x::Clear() {

#define TYPE_I(x, b)\
void S_##x::Clear() { S_##b::Clear();

#define TYPE_II(x, b1, b2)\
void S_##x::Clear() { S_##b1::Clear(); S_##b2::Clear();

#define TYPE_III(x, b1, b2, b3)\
void S_##x::Clear() { S_##b1::Clear(); S_##b2::Clear(); S_##b3::Clear();

#define VAR(type, x)             x.Clear();

#define COLUMN(type, ctype, name, width, prec)               SqlSchemaClear(ADD_SCHEMA_PREFIX_CPP(name));
#define COLUMN_ARRAY(type, ctype, name, width, prec, items)  SqlSchemaClear(ADD_SCHEMA_PREFIX_CPP(name), items);

#define END_TYPE    }

#include SCHEMADIALECT

// FieldLayout

#define TYPE(x) \
void S_##x::FieldLayout(FieldOperator& fo) {\
	fo.Table(#x);\
	FieldLayoutRaw(fo);\
}

#include SCHEMADIALECT

// FieldLayoutRaw

#define TYPE(x) \
void S_##x::FieldLayoutRaw(FieldOperator& fo, const String& prefix) {

#define TYPE_I(x, b) \
void S_##x::FieldLayoutRaw(FieldOperator& fo, const String& prefix) {\
	S_##b::FieldLayoutRaw(fo, prefix);

#define TYPE_II(x, b1, b2) \
void S_##x::FieldLayoutRaw(FieldOperator& fo, const String& prefix) {\
	S_##b1::FieldLayoutRaw(fo, prefix);\
	S_##b2::FieldLayoutRaw(fo, prefix);

#define TYPE_III(x, b1, b2, b3) \
void S_##x::FieldLayoutRaw(FieldOperator& fo, const String& prefix) {\
	S_##b1::FieldLayoutRaw(fo, prefix);\
	S_##b2::FieldLayoutRaw(fo, prefix);\
	S_##b3::FieldLayoutRaw(fo, prefix);

#define COLUMN(type, ctype, name, width, prec)               fo(prefix + #name, ADD_SCHEMA_PREFIX_CPP(name));
#define COLUMN_ARRAY(type, ctype, name, width, prec, items) \
{ \
	for(int i = 0; i < items; i++)\
		fo(Format("%s%s%d", ~prefix, #name, i), ADD_SCHEMA_PREFIX_CPP(name)[i]); \
}

#define VAR(type, x)             x.FieldLayoutRaw(fo, prefix + #x + "$");

#define END_TYPE                 }

#include SCHEMADIALECT

// Introspection

#define TYPE(x)                   void SchDbInfo##x() {
#define TYPE_I(x, b)              void SchDbInfo##x() { SchDbInfo##b();
#define TYPE_II(x, b1, b2)        void SchDbInfo##x() { SchDbInfo##b1(); SchDbInfo##b2();
#define TYPE_III(x, b1, b2, b3)   void SchDbInfo##x() { SchDbInfo##b1(); SchDbInfo##b2(); SchDbInfo##b3();
#define COLUMN(type, ctype, name, width, prec)               SchDbInfoColumn(#name);
#define VAR(type, name)                                      SchDbInfoVar(SchDbInfo##type, #name);
#define REFERENCES(table)                                    SchDbInfoReferences(#table);
#define REFERENCES_CASCADE(table)                            SchDbInfoReferences(#table);
#define REFERENCES_(table, column)                           SchDbInfoReferences(#table, #column);
#define REFERENCES_CASCADE_(table, column)                   SchDbInfoReferences(#table, #column);
#define PRIMARY_KEY                                          SchDbInfoPrimaryKey();
#define COLUMN_ARRAY(type, ctype, name, width, prec, items)  SchDbInfoColumnArray(#name, items);
#define END_TYPE }

#include SCHEMADIALECT

#define TYPE(x)              struct SINS_##x##_ { SINS_##x##_(); } SINS_##x##__; SINS_##x##_::SINS_##x##_() {\
									SchDbInfoTable(#x); SchDbInfo##x();
#define TYPE_I(x, b)              TYPE(x)
#define TYPE_II(x, b1, b2)        TYPE(x)
#define TYPE_III(x, b1, b2, b3)   TYPE(x)
#define END_TYPE }

#include SCHEMADIALECT
