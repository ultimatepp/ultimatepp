#define INT(x)                     COLUMN("integer", int, x, 0, 0)
#define INT_ARRAY(x, items)        COLUMN_ARRAY("integer", int, x, 0, 0, items)
#define INT_(x)                    COLUMN_("integer", int, x, 0, 0)
#define INT_ARRAY_(x, items)       COLUMN_ARRAY_("integer", int, x, 0, 0, items)

#define INT64(x)                   COLUMN("bigint", int64, x, 0, 0)
#define INT64_ARRAY(x, items)      COLUMN_ARRAY("bigint", int64, x, 0, 0, items)
#define INT64_(x)                  COLUMN_("bigint", int64, x, 0, 0)
#define INT64_ARRAY_(x, items)     COLUMN_ARRAY_("bigint", int64, x, 0, 0, items)

#define SERIAL(x)                  COLUMN("serial", int64, x, 0, 0) //int is not enough, as it is unsigned..
#define SERIAL_ARRAY(x, items)     COLUMN_ARRAY("serial", int64, x, 0, 0, items)
#define SERIAL_(x)                 COLUMN_("serial", int64, x, 0, 0)
#define SERIAL_ARRAY_(x, items)    COLUMN_ARRAY_("serial", int64, x, 0, 0, items)

#define ISERIAL(x)                 COLUMN("serial", int, x, 0, 0) //..but we like to choose
#define ISERIAL_ARRAY(x, items)    COLUMN_ARRAY("serial", int, x, 0, 0, items)
#define ISERIAL_(x)                COLUMN_("serial", int, x, 0, 0)
#define ISERIAL_ARRAY_(x, items)   COLUMN_ARRAY_("serial", int, x, 0, 0, items)

#define BIGSERIAL(x)               COLUMN("bigserial", int64, x, 0, 0)
#define BIGSERIAL_ARRAY(x, items)  COLUMN_ARRAY("bigserial", int64, x, 0, 0, items)
#define BIGSERIAL_(x)              COLUMN_("bigserial", int64, x, 0, 0)
#define BIGSERIAL_ARRAY_(x, items) COLUMN_ARRAY_("bigserial", int64, x, 0, 0, items)

#define DOUBLE(x)                  COLUMN("double precision", double, x, 0, 0)
#define DOUBLE_ARRAY(x, items)     COLUMN_ARRAY("double precision", double, x, 0, 0, items)
#define DOUBLE_(x)                 COLUMN_("double precision", double, x, 0, 0)
#define DOUBLE_ARRAY_(x, items)    COLUMN_ARRAY_("double precision", double, x, 0, 0, items)

#define STRING(x, n)               COLUMN("varchar(" #n ")", String, x, n, 0)
#define STRING_ARRAY(x, n, items)  COLUMN_ARRAY("varchar(" #n ")", String, x, n, 0, items)
#define STRING_(x, n)              COLUMN_("varchar(" #n ")", String, x, n, 0)
#define STRING_ARRAY_(x, n, items) COLUMN_ARRAY_("varchar(" #n ")", String, x, n, 0, items)

#define DATE(x)                    COLUMN("date", Date, x, 0, 0)
#define DATE_ARRAY(x, items)       COLUMN_ARRAY("date", Date, x, 0, 0, items)
#define DATE_(x)                   COLUMN_("date", Date, x, 0, 0)
#define DATE_ARRAY_(x, items)      COLUMN_ARRAY_("date", Date, x, 0, 0, items)

#define TIME(x)                    COLUMN("timestamp", Time, x, 0, 0)
#define TIME_ARRAY(x, items)       COLUMN_ARRAY("timestamp", Time, x, 0, 0, items)
#define TIME_(x)                   COLUMN_("timestamp", Time, x, 0, 0)
#define TIME_ARRAY_(x, items)      COLUMN_ARRAY_("timestamp", Time, x, 0, 0, items)

#define TIME_SEC(x)                COLUMN("timestamp(0)", Time, x, 0, 0)
#define TIME_SEC_ARRAY(x, items)   COLUMN_ARRAY("timestamp(0)", Time, x, 0, 0, items)
#define TIME_SEC_(x)               COLUMN_("timestamp(0)", Time, x, 0, 0)
#define TIME_SEC_ARRAY_(x, items)  COLUMN_ARRAY_("timestamp(0)", Time, x, 0, 0, items)

#define BOOL(x)                    COLUMN("char(1)", bool, x, 0, 0)
#define BOOL_ARRAY(x, items)       COLUMN_ARRAY("char(1)", bool, x, 0, 0, items)
#define BOOL_(x)                   COLUMN_("char(1)", bool, x, 0, 0)
#define BOOL_ARRAY_(x, items)      COLUMN_ARRAY_("char(1)", bool, x, 0, 0, items)

#define LONG(x)                    COLUMN("text", String, x, 0, 0)
#define LONG_(x)                   COLUMN_("text", String, x, 0, 0)

#define LONGRAW(x)                 COLUMN("bytea", String, x, 0, 0)
#define LONGRAW_(x)                COLUMN_("bytea", String, x, 0, 0)

#define BLOB(x)                    COLUMN("bytea", String, x, INT_MAX/2, 0)
#define BLOB_(x)                   COLUMN_("bytea", String, x, INT_MAX/2, 0)

#define CLOB(x)                    COLUMN("text", String, x, INT_MAX/2, 0)
#define CLOB_(x)                   COLUMN_("text", String, x, INT_MAX/2, 0)

#define SEQUENCE(x)                SCHEMA("create sequence " #x " start with 1;",\
                                          "drop sequence " #x ";") \
                                   UPGRADE("create sequence " #x " start with 1;")
#define SEQUENCE_(x)               DOID(x) SEQUENCE(x)

#ifndef PRIMARY_KEY
#define PRIMARY_KEY                INLINE_ATTRIBUTE("primary key")
#endif
#define NOT_NULL                   INLINE_ATTRIBUTE("not null")
#define UNIQUE                     INLINE_ATTRIBUTE("unique")
#define SQLDEFAULT(v)              INLINE_ATTRIBUTE("default " #v)

#define INDEX                      ATTRIBUTE("create index IDX_@x on @t(@c);", \
                                             "drop index IDX_@x;")

#define PARTIAL_INDEX(cond)        ATTRIBUTE("create index IDX_@x on @t(@c) where " cond ";", \
                                             "drop index IDX_@x;")
#define PARTIAL_INDEX_(name, cond) ATTRIBUTE("create index IDXP_@x" #name " on @t(@c) where " cond ";", \
                                             "drop index IDXP_@x" #name ";")
#ifndef REFERENCES
#define REFERENCES(x)              ATTRIBUTE("alter table @t add constraint FK_@x foreign key "\
                                             "(@c) references " #x ";",\
                                             "alter table @t drop constraint FK_@x;")
#endif

#ifndef REFERENCES_CASCADE
#define REFERENCES_CASCADE(x)      ATTRIBUTE("alter table @t add constraint FK_@x foreign key "\
                                             "(@c) references " #x " on delete cascade;",\
                                             "alter table @t drop constraint FK_@x;")
#endif

#ifndef REFERENCES_
#define REFERENCES_(n, x)          ATTRIBUTE("alter table @t add constraint FK_@x$" #n " foreign key "\
                                             "(@c) references " #x ";",\
                                             "alter table @t drop constraint FK_@x$" #n ";")
#endif

#ifndef REFERENCES_CASCADE_
#define REFERENCES_CASCADE_(n, x)  ATTRIBUTE("alter table @t add constraint FK_@x$" #n " foreign key "\
                                             "(@c) references " #x " on delete cascade;",\
                                             "alter table @t drop constraint FK_@x$" #n ";")
#endif

#define DUAL_PRIMARY_KEY(k1, k2)   ATTRIBUTE("alter table @t add constraint DK_@t primary key "\
                                             "(" #k1 ", " #k2 ");",\
                                             "alter table @t drop constraint DK_@t;")

#define DUAL_UNIQUE(k1, k2)        ATTRIBUTE("alter table @t add constraint DQ_@t$" #k1 "$" #k2 " unique "\
                                             "(" #k1 ", " #k2 ");",\
                                             "alter table @t drop constraint DQ_@t$" #k1 "$" #k2 ";")

#define SQLCHECK(name, chk)        ATTRIBUTE("alter table @t add constraint CHK_@t$" #name " check "\
                                             "(" chk ");",\
                                             "alter table @t drop constraint CHK_@t$" #name ";")

#define PRIMARY_KEY_LIST(name, keys) \
ATTRIBUTE("alter table @t add constraint PKL_@t$" #name " primary key (" keys ");", \
          "alter table @t drop constraint PKL_@t$" #name  ";")

#define REFERENCES_LIST(name, fks, tab, keys) \
ATTRIBUTE("alter table @t add constraint FKL_@t$" #name " foreign key " \
          "(" fks ") references " #tab "(" keys ");", \
          "alter table @t drop constraint FKL_@t$" #name ";")

#define INDEX_LIST(name, keys) \
ATTRIBUTE("create index IDXL_@t$" #name " on @t(" keys ");", \
          "drop index IDXL_@t$" #name ";");

#define PARTIAL_INDEX_LIST(name, keys, where) \
ATTRIBUTE("create index IDXL_@t$" #name " on @t(" keys ") where " where ";", \
          "drop index IDXL_@t$" #name ";");

#define UNIQUE_LIST(name, list) \
ATTRIBUTE("alter table @t add constraint UQ_@t$" #name " unique (" list ");", \
          "alter table @t drop constraint UQ_@t$" #name ";")

#include <Sql/sch_model.h>

#undef INT
#undef INT_ARRAY
#undef INT_
#undef INT_ARRAY_
#undef INT64
#undef INT64_ARRAY
#undef INT64_
#undef INT64_ARRAY_
#undef SERIAL
#undef SERIAL_ARRAY
#undef SERIAL_
#undef SERIAL_ARRAY_
#undef ISERIAL
#undef ISERIAL_ARRAY
#undef ISERIAL_
#undef ISERIAL_ARRAY_
#undef BIGSERIAL
#undef BIGSERIAL_ARRAY
#undef BIGSERIAL_
#undef BIGSERIAL_ARRAY_
#undef DOUBLE
#undef DOUBLE_ARRAY
#undef DOUBLE_
#undef DOUBLE_ARRAY_
#undef STRING
#undef STRING_ARRAY
#undef STRING_
#undef STRING_ARRAY_
#undef DATE
#undef DATE_ARRAY
#undef DATE_
#undef DATE_ARRAY_
#undef TIME
#undef TIME_ARRAY
#undef TIME_
#undef TIME_ARRAY_
#undef TIME_SEC
#undef TIME_SEC_ARRAY
#undef TIME_SEC_
#undef TIME_SEC_ARRAY_
#undef BOOL
#undef BOOL_ARRAY
#undef BOOL_
#undef BOOL_ARRAY_
#undef LONG
#undef LONG_
#undef LONGRAW
#undef LONGRAW_
#undef BLOB
#undef BLOB_
#undef CLOB
#undef CLOB_
#undef SEQUENCE
#undef SEQUENCE_
#undef PRIMARY_KEY
#undef NOT_NULL
#undef UNIQUE
#undef SQLDEFAULT
#undef INDEX
#undef PARTIAL_INDEX
#undef PARTIAL_INDEX_
#undef REFERENCES
#undef REFERENCES_CASCADE
#undef REFERENCES_
#undef REFERENCES_CASCADE_
#undef DUAL_PRIMARY_KEY
#undef DUAL_UNIQUE
#undef SQLCHECK
#undef PRIMARY_KEY_LIST
#undef REFERENCES_LIST
#undef INDEX_LIST
#undef PARTIAL_INDEX_LIST
#undef UNIQUE_LIST
