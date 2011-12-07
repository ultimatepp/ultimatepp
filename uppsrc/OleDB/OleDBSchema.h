#define BIT(x)                     COLUMN("bit", int, x, 0, 0)
#define BIT_ARRAY(x, items)        COLUMN_ARRAY("bit", int, x, 0, 0, items)
#define BIT_(x)                    COLUMN_("bit", int, x, 0, 0)
#define BIT_ARRAY_(x, items)       COLUMN_ARRAY_("bit", int, x, 0, 0, items)

#define BOOL(x)                    COLUMN("bit", int, x, 0, 0)
#define BOOL_ARRAY(x, items)       COLUMN_ARRAY("bit", int, x, 0, 0, items)
#define BOOL_(x)                   COLUMN_("bit", int, x, 0, 0)
#define BOOL_ARRAY_(x, items)      COLUMN_ARRAY_("bit", int, x, 0, 0, items)

#define INT(x)                     COLUMN("integer", int, x, 0, 0)
#define INT_ARRAY(x, items)        COLUMN_ARRAY("integer", int, x, 0, 0, items)
#define INT_(x)                    COLUMN_("integer", int, x, 0, 0)
#define INT_ARRAY_(x, items)       COLUMN_ARRAY_("integer", int, x, 0, 0, items)

#define BIGINT(x)                  COLUMN("bigint", int64, x, 0, 0)
#define BIGINT_ARRAY(x, items)     COLUMN_ARRAY("bigint", int64, x, 0, 0, items)
#define BIGINT_(x)                 COLUMN_("bigint", int64, x, 0, 0)
#define BIGINT_ARRAY_(x, items)    COLUMN_ARRAY_("bigint", int64, x, 0, 0, items)

#define DOUBLE(x)                  COLUMN("double precision", double, x, 0, 0)
#define DOUBLE_ARRAY(x, items)     COLUMN_ARRAY("double precision", double, x, 0, 0, items)
#define DOUBLE_(x)                 COLUMN_("double precision", double, x, 0, 0)
#define DOUBLE_ARRAY_(x, items)    COLUMN_ARRAY_("double precision", double, x, 0, 0, items)

#define DATE(x)                    COLUMN("datetime", Date, x, 0, 0)
#define DATE_ARRAY(x, items)       COLUMN_ARRAY("datetime", Date, x, 0, 0, items)
#define DATE_(x)                   COLUMN_("datetime", Date, x, 0, 0)
#define DATE_ARRAY_(x, items)      COLUMN_ARRAY_("datetime", Date, x, 0, 0, items)

#define TIME(x)                    COLUMN("datetime", Time, x, 0, 0)
#define TIME_ARRAY(x, items)       COLUMN_ARRAY("datetime", Time, x, 0, 0, items)
#define TIME_(x)                   COLUMN_("datetime", Time, x, 0, 0)
#define TIME_ARRAY_(x, items)      COLUMN_ARRAY_("datetime", Time, x, 0, 0, items)

#define STRING(x, n)               COLUMN(MSSQLTextType(n), String, x, n, 0)
#define STRING_ARRAY(x, n, items)  COLUMN_ARRAY(MSSQLTextType(n), String, x, n, 0, items)
#define STRING_(x, n)              COLUMN_(MSSQLTextType(n), String, x, n, 0)
#define STRING_ARRAY_(x, n, items) COLUMN_ARRAY_(MSSQLTextType(n), String, x, n, 0, items)

#define LONGRAW(x)                 COLUMN("varbinary(max)", String, x, 0, 0)
#define LONGRAW_(x)                COLUMN_("varbinary(max)", String, x, 0, 0)

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

#define INDEX                      ATTRIBUTE("create index IDX_@x on @t(@c);", \
                                             "drop index IDX_@x;")
#define UNIQUE                     ATTRIBUTE("create unique index UNQ_@x on @t(@c);", \
	                                         "drop index UNQ_@x;")
#define REFERENCES(x)              ATTRIBUTE("alter table @t add constraint FK_@x foreign key "\
                                             "(@c) references @s" #x ";",\
                                             "alter table @t drop constraint FK_@x;")

#define TIMESTAMP(ts)              SCHEMA("-- " ts "\n\n", NULL)

#define COMMENT(txt)               SCHEMA("-- " #txt "\n", NULL)

#include <Sql/sch_model.h>

#undef INT
#undef INT_ARRAY
#undef INT_
#undef INT_ARRAY_

#undef BIGINT
#undef BIGINT_ARRAY
#undef BIGINT_
#undef BIGINT_ARRAY_

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

#undef PRIMARY_KEY
#undef KEY
#undef NOT_NULL
#undef IDENTITY

#undef INDEX

#undef UNIQUE

#undef TIMESTAMP
#undef COMMENT
