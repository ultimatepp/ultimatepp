#define __NIL

#ifndef TIMESTAMP
#define TIMESTAMP(x)
#endif

#ifndef UPGRADE
#define UPGRADE(x)
#endif

#ifndef CHECK_IN
#define CHECK_IN(s)   INLINE_ATTRIBUTE("check (@c in (" s "))")
#endif

#ifndef CHECK_COND
#define CHECK_COND(s) INLINE_ATTRIBUTE("check (" s ")")
#endif


#ifdef TEXT
#undef TEXT
#endif

#ifndef DOC
#define DOC(x)
#endif

#ifndef SCHEMA_PREFIX_CPP
#define SCHEMA_PREFIX_CPP
#endif

#ifndef ADD_SCHEMA_PREFIX_CPP
#define ADD_SCHEMA_PREFIX_CPP__(_prefix_cpp_, x) _prefix_cpp_##x
#define ADD_SCHEMA_PREFIX_CPP_(_prefix_cpp_, x) ADD_SCHEMA_PREFIX_CPP__(_prefix_cpp_, x)
#define ADD_SCHEMA_PREFIX_CPP(x) ADD_SCHEMA_PREFIX_CPP_(SCHEMA_PREFIX_CPP,x)
#define ADD_SCHEMA_PREFIX_CPP2__(_prefix_cpp_, x, y) _prefix_cpp_##x##y
#define ADD_SCHEMA_PREFIX_CPP2_(_prefix_cpp_, x, y) ADD_SCHEMA_PREFIX_CPP2__(_prefix_cpp_, x, y)
#define ADD_SCHEMA_PREFIX_CPP2(x,y) ADD_SCHEMA_PREFIX_CPP2_(SCHEMA_PREFIX_CPP,x, y)
#endif

#ifndef DOID
#define DOID(x)
#endif

#ifndef TYPE
#define TYPE(x)
#endif

#ifndef TYPE_
#define TYPE_(x)                    DOID(x) TYPE(x)
#endif

#ifndef END_TYPE
#define                             END_TYPE
#endif

#ifndef TYPE_I
#define TYPE_I(x, b)                TYPE(x)
#endif

#ifndef TYPE_I_
#define TYPE_I_(x, b)               DOID(x) TYPE_I(x, b)
#endif

#ifndef TYPE_II
#define TYPE_II(x, b1, b2)          TYPE(x)
#endif

#ifndef TYPE_II_
#define TYPE_II_(x, b1, b2)         DOID(x) TYPE_II(x, b1, b2)
#endif

#ifndef TYPE_III
#define TYPE_III(x, b1, b2, b3)     TYPE(x)
#endif

#ifndef TYPE_III_
#define TYPE_III_(x, b1, b2, b3)    DOID(x) TYPE_III(x, b1, b2, b3)
#endif

#ifndef TABLE
#define TABLE(x)                    TYPE(x)
#endif

#ifndef TABLE_
#define TABLE_(x)                   DOID(x) TABLE(x)
#endif

#ifndef END_TABLE
#define END_TABLE                   END_TYPE
#endif

#ifndef TABLE_I
#define TABLE_I(x, b)               TYPE_I(x, b)
#endif

#ifndef TABLE_I_
#define TABLE_I_(x, b)              DOID(x) TABLE_I(x, b)
#endif

#ifndef TABLE_II
#define TABLE_II(x, b1, b2)         TYPE_II(x, b1, b2)
#endif

#ifndef TABLE_II_
#define TABLE_II_(x, b1, b2)        DOID(x) TABLE_II(x, b1, b2)
#endif

#ifndef TABLE_III
#define TABLE_III(x, b1, b2, b3)    TYPE_III(x, b1, b2, b3)
#endif

#ifndef TABLE_III_
#define TABLE_III_(x, b1, b2, b3)   DOID(x) TABLE_III(x, b1, b2, b3)
#endif

#ifndef VAR
#define VAR(type, x)
#endif

#ifndef VAR_
#define VAR_(type, x)               DOID(x) VAR(type, x)
#endif

#ifndef COLUMN
#define COLUMN(type, ctype, name, width, prec)
#endif

#ifndef COLUMN_
#define COLUMN_(type, ctype, name, width, prec)   DOID(name) COLUMN(type, ctype, name, width, prec)
#endif

#ifndef COLUMN_ARRAY
#define COLUMN_ARRAY(type, ctype, name, width, prec, items)
#endif

#ifndef COLUMN_ARRAY_
#define COLUMN_ARRAY_(type, ctype, name, width, prec, items) \
        DOID(name) COLUMN_ARRAY(type, ctype, name, width, prec, items)
#endif

#ifndef INLINE_ATTRIBUTE
#define INLINE_ATTRIBUTE(x)
#endif

#ifndef ATTRIBUTE
#define ATTRIBUTE(x, d)
#endif

#ifndef TABLE_SUFFIX
#define TABLE_SUFFIX(s)
#endif

#ifndef SCHEMA
#define SCHEMA(x, d)
#endif

#ifndef CONFIG
#define CONFIG(x, d)
#endif

#ifndef SQL_NAME
#define SQL_NAME(x)
#endif

#include MODEL

#undef SQL_NAME

#undef __NIL

#undef STAMP

#undef DOID
#undef DOC
#undef TYPE
#undef TYPE_
#undef END_TYPE
#undef TYPE_I
#undef TYPE_I_
#undef TYPE_II
#undef TYPE_II_
#undef TYPE_III
#undef TYPE_III_

#undef TABLE
#undef TABLE_
#undef END_TABLE
#undef TABLE_I
#undef TABLE_I_
#undef TABLE_II
#undef TABLE_II_
#undef TABLE_III
#undef TABLE_III_
#undef VAR
#undef VAR_
#undef COLUMN
#undef COLUMN_ARRAY
#undef ATTRIBUTE
#undef INLINE_ATTRIBUTE
#undef TABLE_SUFFIX
#undef SCHEMA
#undef CONFIG
#undef UPGRADE
#undef TIMESTAMP
