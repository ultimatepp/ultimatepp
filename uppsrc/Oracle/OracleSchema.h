#define INT(x)                     COLUMN("number", int, x, 0, 0)
#define INT_ARRAY(x, items)        COLUMN_ARRAY("number", int, x, 0, 0, items)
#define INT_(x)                    COLUMN_("number", int, x, 0, 0)
#define INT_ARRAY_(x, items)       COLUMN_ARRAY("number", int, x, 0, 0, items)

#define INT64(x)                   COLUMN("number", int64, x, 0, 0)
#define INT64_ARRAY(x, items)      COLUMN_ARRAY("number", int64, x, 0, 0, items)
#define INT64_(x)                  COLUMN_("number", int64, x, 0, 0)
#define INT64_ARRAY_(x, items)     COLUMN_ARRAY("number", int64, x, 0, 0, items)

#define DOUBLE(x)                  COLUMN("number", double, x, 0, 0)
#define DOUBLE_ARRAY(x, items)     COLUMN_ARRAY(number, double, x, 0, 0, items)
#define DOUBLE_(x)                 COLUMN_("number", double, x, 0, 0)
#define DOUBLE_ARRAY_(x, items)    COLUMN_ARRAY_("number", double, x, 0, 0, items)

#define STRING(x, n)               COLUMN("varchar2(" #n ")", String, x, n, 0)
#define STRING_ARRAY(x, n, items)  COLUMN_ARRAY("varchar2(" #n ")", String, x, n, 0, items)
#define STRING_(x, n)              COLUMN_("varchar2(" #n ")", String, x, n, 0)
#define STRING_ARRAY_(x, n, items) COLUMN_ARRAY_("varchar2(" #n ")", String, x, n, 0, items)

#define DATE(x)                    COLUMN("date", Date, x, 0, 0)
#define DATE_ARRAY(x, items)       COLUMN_ARRAY("date", Date, x, 0, 0, items)
#define DATE_(x)                   COLUMN_("date", Date, x, 0, 0)
#define DATE_ARRAY_(x, items)      COLUMN_ARRAY_("date", Date, x, 0, 0, items)

#define TIME(x)                    COLUMN("date", Time, x, 0, 0)
#define TIME_ARRAY(x, items)       COLUMN_ARRAY("date", Time, x, 0, 0, items)
#define TIME_(x)                   COLUMN_("date", Time, x, 0, 0)
#define TIME_ARRAY_(x, items)      COLUMN_ARRAY_("date", Time, x, 0, 0, items)

#define BOOL(x)                    COLUMN("varchar2(1)", bool, x, 0, 0)
#define BOOL_ARRAY(x, items)       COLUMN_ARRAY("varchar2(1)", bool, x, 0, 0, items)
#define BOOL_(x)                   COLUMN_("varchar2(1)", bool, x, 0, 0)
#define BOOL_ARRAY_(x, items)      COLUMN_ARRAY_("varchar2(1)", bool, x, 0, 0, items)

#define LONG(x)                    COLUMN("long", String, x, 0, 0)
#define LONG_(x)                   COLUMN_("long", String, x, 0, 0)

#define LONGRAW(x)                 COLUMN("long raw", String, x, 0, 0)
#define LONGRAW_(x)                COLUMN_("long raw", String, x, 0, 0)

#define BLOB(x)                    COLUMN("blob", String, x, INT_MAX/2, 0)
#define BLOB_(x)                   COLUMN_("blob", String, x, INT_MAX/2, 0)

#define CLOB(x)                    COLUMN("clob", String, x, INT_MAX/2, 0)
#define CLOB_(x)                   COLUMN_("clob", String, x, INT_MAX/2, 0)

#define SEQUENCE(x)                SCHEMA("create sequence " #x " start with 1;",\
                                          "drop sequence " #x ";") \
                                   UPGRADE("create sequence " #x " start with 1;")
#define SEQUENCE_(x)               DOID(x) SEQUENCE(x)

#ifndef PRIMARY_KEY
#define PRIMARY_KEY                INLINE_ATTRIBUTE("primary key")
#endif

#define NOT_NULL                   INLINE_ATTRIBUTE("not null")
#define UNIQUE                     INLINE_ATTRIBUTE("unique")
#define SQLDEFAULT(v)              INLINE_ATTRIBUTE("default " v)

#define INDEX                      ATTRIBUTE("create index IDX_@x on @t(@c);", \
                                             "drop index IDX_@x;")
#ifndef REFERENCES
#define REFERENCES(x)              ATTRIBUTE("alter table @t add (constraint FK_@x foreign key "\
                                             "(@c) references " #x ");",\
                                             "alter table @t drop constraint FK_@x;")
#endif

#ifndef REFERENCES_CASCADE
#define REFERENCES_CASCADE(x)      ATTRIBUTE("alter table @t add (constraint FK_@x foreign key "\
                                             "(@c) references " #x " on delete cascade);",\
                                             "alter table @t drop constraint FK_@x;")
#endif

#ifndef REFERENCES_
#define REFERENCES_(n, x)          ATTRIBUTE("alter table @t add (constraint FK_@x$" #n " foreign key "\
                                             "(@c) references " #x ");",\
                                             "alter table @t drop constraint FK_@x$" #n ";")
#endif

#ifndef REFERENCES_CASCADE_
#define REFERENCES_CASCADE_(n, x)  ATTRIBUTE("alter table @t add (constraint FK_@x$" #n " foreign key "\
                                             "(@c) references " #x " on delete cascade);",\
                                             "alter table @t drop constraint FK_@x$" #n ";")
#endif

#define DUAL_PRIMARY_KEY(k1, k2)   INLINE_ATTRIBUTE(", primary key (" #k1 ", " #k2 ")")

#define DUAL_UNIQUE(k1, k2)        ATTRIBUTE("alter table @t add constraint DQ_@t unique "\
                                             "(" #k1 ", " #k2 ");",\
                                             "alter table @t drop constraint DQ_@t;")

#define UNIQUE_LIST(u, l)          ATTRIBUTE("alter table @t add constraint UQ_@t$" #u " unique "\
                                             "(" l ");",\
                                             "alter table @t drop constraint UQ_@t$" #u ";")

#define SQLCHECK(n, ct)            ATTRIBUTE("alter table @t add constraint CHK_@t$" #n " check "\
                                             "(" ct ");",\
                                             "alter table @t drop constraint CHK_@t$" #n ";")

#include <Sql/sch_model.h>


#undef INT
#undef INT_ARRAY
#undef INT_
#undef INT_ARRAY_

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

#undef PRIMARY_KEY
#undef NOT_NULL
#undef INDEX
#undef UNIQUE
#undef SQLDEFAULT
#undef REFERENCES
#undef REFERENCES_
#undef REFERENCES_CASCADE
#undef REFERENCES_CASCADE_
#undef DUAL_PRIMARY_KEY
#undef DUAL_UNIQUE
#undef UNIQUE_LIST
#undef SQLCHECK
