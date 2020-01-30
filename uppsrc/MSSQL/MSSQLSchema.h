#define BIT(x)                    COLUMN("bit", int, x, 0, 0)
#define BIT_ARRAY(x, items)       COLUMN_ARRAY("bit", int, x, 0, 0, items)
#define BIT_(x)                   COLUMN_("bit", int, x, 0, 0)
#define BIT_ARRAY_(x, items)      COLUMN_ARRAY_("bit", int, x, 0, 0, items)

#define BOOL(x)                    COLUMN("char(1)", bool, x, 0, 0)
#define BOOL_ARRAY(x, items)       COLUMN_ARRAY("char(1)", bool, x, 0, 0, items)
#define BOOL_(x)                   COLUMN_("char(1)", bool, x, 0, 0)
#define BOOL_ARRAY_(x, items)      COLUMN_ARRAY_("char(1)", bool, x, 0, 0, items)

#define INT64(x)                   COLUMN("bigint", int64, x, 0, 0)
#define INT64_ARRAY(x, items)      COLUMN_ARRAY("bigint", int64, x, 0, 0, items)
#define INT64_(x)                  COLUMN_("bigint", int64, x, 0, 0)
#define INT64_ARRAY_(x, items)     COLUMN_ARRAY_("bigint", int64, x, 0, 0, items)

#define INT(x)                     COLUMN("integer", int, x, 0, 0)
#define INT_ARRAY(x, items)        COLUMN_ARRAY("integer", int, x, 0, 0, items)
#define INT_(x)                    COLUMN_("integer", int, x, 0, 0)
#define INT_ARRAY_(x, items)       COLUMN_ARRAY_("integer", int, x, 0, 0, items)

#define DOUBLE(x)                  COLUMN("float", double, x, 0, 0)
#define DOUBLE_ARRAY(x, items)     COLUMN_ARRAY("float", double, x, 0, 0, items)
#define DOUBLE_(x)                 COLUMN_("float", double, x, 0, 0)
#define DOUBLE_ARRAY_(x, items)    COLUMN_ARRAY_("float", double, x, 0, 0, items)

#define DATE(x)                    COLUMN("date", Date, x, 0, 0)
#define DATE_ARRAY(x, items)       COLUMN_ARRAY("date", Date, x, 0, 0, items)
#define DATE_(x)                   COLUMN_("date", Date, x, 0, 0)
#define DATE_ARRAY_(x, items)      COLUMN_ARRAY_("date", Date, x, 0, 0, items)

#define TIME(x)                    COLUMN("datetime", Time, x, 0, 0)
#define TIME_ARRAY(x, items)       COLUMN_ARRAY("datetime", Time, x, 0, 0, items)
#define TIME_(x)                   COLUMN_("datetime", Time, x, 0, 0)
#define TIME_ARRAY_(x, items)      COLUMN_ARRAY_("datetime", Time, x, 0, 0, items)

#define STRING(x, n)               COLUMN(MsSqlTextType(n), String, x, n, 0)
#define STRING_ARRAY(x, n, items)  COLUMN_ARRAY(MsSqlTextType(n), String, x, n, 0, items)
#define STRING_(x, n)              COLUMN_(MsSqlTextType(n), String, x, n, 0)
#define STRING_ARRAY_(x, n, items) COLUMN_ARRAY_(MsSqlTextType(n), String, x, n, 0, items)

#define LONGRAW(x)                 COLUMN("varbinary(max)", String, x, 0, 0)
#define LONGRAW_(x)                COLUMN_("varbinary(max)", String, x, 0, 0)

#define BLOB(x)                    COLUMN("varbinary(max)", String, x, 0, 0)
#define BLOB_(x)                   COLUMN_("varbinary(max)", String, x, 0, 0)

#ifndef PRIMARY_KEY
#define PRIMARY_KEY                INLINE_ATTRIBUTE("primary key")\
                                   ATTRIBUTE("alter table @t add constraint PK_@x primary key "\
                                             "(@c)",\
                                             "alter table @t drop constraint PK_@x;")\
                                   ATTRIBUTE("create index PKX_@x on @t(@c);", \
                                             "drop index PKX_@x;")
#endif

#define KEY                        INLINE_ATTRIBUTE("key")
#define NOT_NULL                   INLINE_ATTRIBUTE("not null")
#define IDENTITY                   INLINE_ATTRIBUTE("identity")
#define AUTO_INCREMENT             INLINE_ATTRIBUTE("identity")

#define SQLDEFAULT(v)              ATTRIBUTE("alter table @t add constraint DF_@x default "#v " for @c;",\
                                             "alter table @t drop constraint DF_@x;")

#define INDEX                      ATTRIBUTE("create index IDX_@x on @t(@c);", \
                                             "drop index IDX_@x;")
#define UNIQUE                     ATTRIBUTE("create unique index UNQ_@x on @t(@c);", \
	                                         "drop index UNQ_@x;")
	                                         
#ifndef REFERENCES
#define REFERENCES(x)              ATTRIBUTE("alter table @t add constraint FK_@x foreign key "\
                                             "(@c) references @s" #x ";",\
                                             "alter table @t drop constraint FK_@x;")
#endif

#define TIMESTAMP(ts)              SCHEMA("-- " ts "\n\n", NULL)

#define COMMENT(txt)               SCHEMA("-- " #txt "\n", NULL)

#define SEQUENCE(x)                SCHEMA("create sequence " #x " start with 1;",\
                                          "drop sequence " #x ";") \
                                   UPGRADE("create sequence " #x " start with 1;")
#define SEQUENCE_(x)               DOID(x) SEQUENCE(x)

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

#undef BIT
#undef BIT_ARRAY
#undef BIT_
#undef BIT_ARRAY_

#undef BOOL
#undef BOOL_ARRAY
#undef BOOL_
#undef BOOL_ARRAY_

#undef INT
#undef INT_ARRAY
#undef INT_
#undef INT_ARRAY_

#undef DOUBLE
#undef DOUBLE_ARRAY
#undef DOUBLE_
#undef DOUBLE_ARRAY_

#undef DATE
#undef DATE_ARRAY
#undef DATE_
#undef DATE_ARRAY_

#undef TIME
#undef TIME_ARRAY
#undef TIME_
#undef TIME_ARRAY_

#undef STRING
#undef STRING_ARRAY
#undef STRING_
#undef STRING_ARRAY_

#undef LONGRAW
#undef LONGRAW_

#undef PRIMARY_KEY
#undef KEY
#undef NOT_NULL
#undef IDENTITY

#undef INDEX

#undef UNIQUE

#undef REFERENCES

#undef TIMESTAMP
#undef COMMENT

#undef SEQUENCE
#undef SEQUENCE_

#undef DUAL_PRIMARY_KEY
#undef DUAL_UNIQUE
#undef UNIQUE_LIST
#undef SQLCHECK
