// SqlId

#define DOID(x) SqlId ADD_SCHEMA_PREFIX_CPP(x)(#x);
//#define DOID(x)                 SqlId x(#x);

#include SCHEMADIALECT

// constructor

#define TYPE(x)  const S_info *S_##x::info; S_##x::S_##x()   { ONCELOCK { info = &GetInfo(); }
#define COLUMN(type, ctype, name, width, prec)                 SqlSchemaInitClear(ADD_SCHEMA_PREFIX_CPP(name));
#define COLUMN_ARRAY(type, ctype, name, width, prec, items)    SqlSchemaInitClear(ADD_SCHEMA_PREFIX_CPP(name), items);
#define END_TYPE                                             }

#include SCHEMADIALECT

// constructor from ValueMap

#define TYPE(x)  S_##x::S_##x(const ValueMap& m)             { ONCELOCK { info = &GetInfo(); }
#define COLUMN(type, ctype, name, width, prec)                 SqlSchemaInitClear(ADD_SCHEMA_PREFIX_CPP(name));
#define COLUMN_ARRAY(type, ctype, name, width, prec, items)    SqlSchemaInitClear(ADD_SCHEMA_PREFIX_CPP(name), items);
#define END_TYPE                                               Set(m); }

#include SCHEMADIALECT

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

// TableName, FieldLayout and GetInfo

#define TYPE(x) \
const char S_##x::TableName[] = #x; \
\
void S_##x::FieldLayout(FieldOperator& fo) {\
	fo.Table(#x);\
	FieldLayoutRaw(fo);\
} \
\
const S_info& S_##x::GetInfo() {\
	static S_info *info; \
	ONCELOCK { \
		static S_info f; \
		S_##x prot(0); \
		S_info_maker m(f, &prot); \
		prot.FieldLayoutRaw(m); \
		f.Init(); \
		info = &f; \
	} \
	return *info; \
} \

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

#define COLUMN(type, ctype, name, width, prec)               fo(prefix + #name, ADD_SCHEMA_PREFIX_CPP(name)), fo.Width(width);
#define COLUMN_ARRAY(type, ctype, name, width, prec, items) \
{ \
	for(int i = 0; i < items; i++)\
		fo(Format("%s%s%d", ~prefix, #name, i), ADD_SCHEMA_PREFIX_CPP(name)[i]), fo.Width(width); \
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
									SchDbInfoType(#x); SchDbInfo##x();
#define TYPE_I(x, b)              TYPE(x)
#define TYPE_II(x, b1, b2)        TYPE(x)
#define TYPE_III(x, b1, b2, b3)   TYPE(x)

#define TABLE(x)              struct SINS_##x##_ { SINS_##x##_(); } SINS_##x##__; SINS_##x##_::SINS_##x##_() {\
									SchDbInfoTable(#x); SchDbInfo##x();
#define TABLE_I(x, b)              TABLE(x)
#define TABLE_II(x, b1, b2)        TABLE(x)
#define TABLE_III(x, b1, b2, b3)   TABLE(x)

#define END_TYPE }

#include SCHEMADIALECT
