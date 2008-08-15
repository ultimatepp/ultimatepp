#include <Sql/Sql.h>

using namespace Upp;

SQLID(COLUMN)
SQLID(COLUMN1)
SQLID(COLUMN2)
SQLID(TABLE)
SQLID(TABLE1)
SQLID(TABLE2)
SQLID(SEQ)
SQLID(A)
SQLID(B)
SQLID(C)

SQLID(PH_SEQ)
SQLID(CASTKA)
SQLID(PH_POLOZKA)
SQLID(DATUM)
SQLID(PLAT_DTM)
SQLID(PLAT_DTM_SEQ)
SQLID(SEQ_POPLATEK)
SQLID(PRIPAD_SEQ)
SQLID(TXT)
SQLID(KATASTR_SEQ)
SQLID(ADRESA)
SQLID(ADRESA_SEQ)
SQLID(PAR_ADRESA_SEQ)
SQLID(KONTO)
SQLID(ROK)
SQLID(OBDOBI)
SQLID(KONTO_CON)

void DO(SqlStatement s)
{
	LOG(s.Get(ORACLE));
}

void EXP(SqlStatement s, const char *q)
{
	ASSERT(s.Get(PGSQL) == q);
}

CONSOLE_APP_MAIN
{
	DO(Select(A, B).From(TABLE1, TABLE2).OrderBy(B, C));
	DO(Select(A, B).From(TABLE1, TABLE2).Where(A == 10).OrderBy(B, C));
	DO(Select(A, B).From(SqlSet(TABLE1, TABLE2)).Where(A == 10).OrderBy(SqlSet(B, C)));
	DO(Select(PH_SEQ, SqlSum(CASTKA)).From(PH_POLOZKA).Where(DATUM <= GetSysDate()).GroupBy(PH_SEQ).Having(C == B));
	DO(Insert(TABLE1)(A, 1)(B, 2));
	DO(Insert(PLAT_DTM)
				(PLAT_DTM_SEQ, NextVal(SEQ_POPLATEK))
				(PRIPAD_SEQ, 1111)
				(DATUM)
				(CASTKA)
				(TXT)
			  .From(PLAT_DTM).Where(PRIPAD_SEQ == 2222));
	DO(Select(KATASTR_SEQ).From(ADRESA)
				.StartWith(PAR_ADRESA_SEQ == 1111)
				.ConnectBy(Prior(ADRESA_SEQ) == PAR_ADRESA_SEQ)
			| Select(KATASTR_SEQ).From(ADRESA)
				.StartWith(ADRESA_SEQ == 2222)
				.ConnectBy(Prior(PAR_ADRESA_SEQ) == ADRESA_SEQ));
	DO(SqlUpdate(KONTO)(ROK, 1111)(OBDOBI, 2222).Where(KONTO_CON == 3333));


	EXP(Select(COLUMN).From(TABLE),
	    "select COLUMN from TABLE");
	EXP(Select(COLUMN.Of(TABLE)).From(TABLE),
	    "select TABLE.COLUMN from TABLE");
	EXP(Select(COLUMN.As(A)).From(TABLE),
	    "select COLUMN as A from TABLE");
	EXP(Select(COLUMN&A).From(TABLE),
	    "select COLUMN$A from TABLE");
	EXP(Select(COLUMN[3]).From(TABLE),
	    "select COLUMN3 from TABLE");
	EXP(Select(COLUMN1 % COLUMN2).From(TABLE),
	    "select mod(COLUMN1, COLUMN2) from TABLE");
	EXP(Select(COLUMN1 | COLUMN2).From(TABLE),
	    "select COLUMN1 || COLUMN2 from TABLE");
	EXP(Select(SqlFunc("any_fn", COLUMN, 2)).From(TABLE),
	    "select any_fn(COLUMN, 2) from TABLE");
	EXP(Select(Distinct(COLUMN)).From(TABLE),
	    "select distinct COLUMN from TABLE");
	EXP(Select(Distinct(SqlSet(COLUMN1, COLUMN2))).From(TABLE),
	    "select distinct COLUMN1, COLUMN2 from TABLE");
	EXP(Select(All(COLUMN)).From(TABLE),
	    "select all COLUMN from TABLE");
	EXP(Select(Count(COLUMN)).From(TABLE),
	    "select count(COLUMN) from TABLE");
	EXP(Select(SqlAll()).From(TABLE),
	    "select * from TABLE");
	EXP(Select(SqlCountRows()).From(TABLE),
	    "select count(*) from TABLE");
	EXP(Select(COLUMN).From(TABLE).OrderBy(Descending(COLUMN)),
	    "select COLUMN from TABLE order by COLUMN desc");
	EXP(Select(SqlMax(COLUMN)).From(TABLE),
	    "select max(COLUMN) from TABLE");
	EXP(Select(SqlMin(COLUMN)).From(TABLE),
	    "select min(COLUMN) from TABLE");
	EXP(Select(SqlSum(COLUMN)).From(TABLE),
	    "select sum(COLUMN) from TABLE");
	EXP(Select(Avg(COLUMN)).From(TABLE),
	    "select avg(COLUMN) from TABLE");
	EXP(Select(Stddev(COLUMN)).From(TABLE),
	    "select stddev(COLUMN) from TABLE");
	EXP(Select(Variance(COLUMN)).From(TABLE),
	    "select variance(COLUMN) from TABLE");
	EXP(Select(Greatest(COLUMN1, COLUMN2)).From(TABLE),
	    "select greatest(COLUMN1, COLUMN2) from TABLE");
	EXP(Select(Least(COLUMN1, COLUMN2)).From(TABLE),
	    "select least(COLUMN1, COLUMN2) from TABLE");
	EXP(Select(Upper(COLUMN)).From(TABLE),
	    "select upper(COLUMN) from TABLE");
	EXP(Select(Lower(COLUMN)).From(TABLE),
	    "select lower(COLUMN) from TABLE");
	EXP(Select(Substr(COLUMN, 1)).From(TABLE),
	    "select SUBSTR(COLUMN, 1) from TABLE");
	EXP(Select(Substr(COLUMN, 2, 1)).From(TABLE),
	    "select SUBSTR(COLUMN, 2, 1) from TABLE");
	EXP(Select(Instr(COLUMN, "hello")).From(TABLE),
	    "select INSTR(COLUMN, 'hello') from TABLE");
	EXP(Select(SqlNvl(COLUMN1, COLUMN2)).From(TABLE),
	    "select coalesce(COLUMN1, COLUMN2) from TABLE");
	EXP(Select(NextVal(SEQ)).Get(),
	    "select nextval('SEQ')");
	EXP(Select(CurrVal(SEQ)).Get(),
	    "select currval('SEQ')");
	EXP(Select(SqlArg()).From(TABLE),
	    "select ? from TABLE");
	EXP(Select(COLUMN).From(TABLE).Where(COLUMN / 2 > 1 && COLUMN1 == "A" || COLUMN2 == Date(2006, 1, 1)),
	    "select COLUMN from TABLE where COLUMN / 2 > 1 and COLUMN1 = 'A' or COLUMN2 = '2006-01-01'");
	EXP(Select(COLUMN).From(TABLE).Where(!(COLUMN == 1)),
	    "select COLUMN from TABLE where  not (COLUMN = 1)");
	EXP(Select(COLUMN).From(TABLE).Where((COLUMN1 == 1) - (COLUMN2 == 1)),
	    "select COLUMN from TABLE where COLUMN1 = 1 and not COLUMN2 = 1");
	EXP(Select(COLUMN).From(TABLE).Where(IsNull(COLUMN1)),
	    "select COLUMN from TABLE where COLUMN1 is NULL");
	EXP(Select(COLUMN).From(TABLE).Where(NotNull(COLUMN1)),
	    "select COLUMN from TABLE where COLUMN1 is not NULL");
	EXP(Select(COLUMN).From(TABLE).Where(Like(COLUMN1, Wild("A*"))),
	    "select COLUMN from TABLE where COLUMN1 ilike 'A%%'");
	EXP(Select(COLUMN).From(TABLE).Where(NotLike(COLUMN1, Wild("A*"))),
	    "select COLUMN from TABLE where COLUMN1 not ilike 'A%%'");
	EXP(Select(COLUMN).From(TABLE).Where(In(COLUMN, Select(COLUMN).From(TABLE1))),
	    "select COLUMN from TABLE where COLUMN in (select COLUMN from TABLE1)");
	EXP(Select(COLUMN).From(TABLE).Where(COLUMN == Select(COLUMN).From(TABLE1)),
	    "select COLUMN from TABLE where COLUMN in (select COLUMN from TABLE1)");
	EXP(Select(COLUMN).From(TABLE).Where(NotIn(COLUMN, Select(COLUMN).From(TABLE1))),
	    "select COLUMN from TABLE where COLUMN not in (select COLUMN from TABLE1)");
	EXP(Select(COLUMN).From(TABLE).Where(COLUMN != Select(COLUMN).From(TABLE1)),
	    "select COLUMN from TABLE where COLUMN not in (select COLUMN from TABLE1)");
	EXP(Select(COLUMN).From(TABLE).Where(Exists(Select(COLUMN).From(TABLE1))),
	    "select COLUMN from TABLE where exists (select COLUMN from TABLE1)");
	EXP(Select(COLUMN).From(TABLE).Where(NotExists(Select(COLUMN).From(TABLE1))),
	    "select COLUMN from TABLE where not exists (select COLUMN from TABLE1)");
	EXP(Select(COLUMN).From(TABLE).Where(COLUMN == (Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2))),
	    "select COLUMN from TABLE where COLUMN in (select COLUMN1 from TABLE1 union (select COLUMN2 from TABLE2))");
	EXP(Select(COLUMN).From(TABLE).Where(COLUMN == (Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2))),
	    "select COLUMN from TABLE where COLUMN in (select COLUMN1 from TABLE1 intersect (select COLUMN2 from TABLE2))");
	EXP(Select(COLUMN).From(TABLE).Where(COLUMN == (Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2))),
	    "select COLUMN from TABLE where COLUMN in (select COLUMN1 from TABLE1 minus (select COLUMN2 from TABLE2))");
	EXP(Select(COLUMN).From(TABLE).Where(COLUMN == 0).GroupBy(COLUMN).Having(COLUMN == 0).OrderBy(Descending(COLUMN)),
	    "select COLUMN from TABLE where COLUMN = 0 group by COLUMN having COLUMN = 0 order by COLUMN desc");
	EXP(Select(COLUMN).From(TABLE).Limit(100),
	    "select COLUMN from TABLE limit 100");
	EXP(Select(COLUMN).From(TABLE).Limit(100, 10),
	    "select COLUMN from TABLE limit 100, 10");
	EXP(Select(COLUMN).From(TABLE).Offset(20),
	    "select COLUMN from TABLE offset 20");
	EXP(Select(25 * 34).Get(),
	    "select 850");
	EXP(Select(COLUMN).From(TABLE).Hint("hint"),
	    "/*+ hint */ select COLUMN from TABLE");
	EXP(Select(COLUMN).From(TABLE).InnerJoin(TABLE1).On(COLUMN.Of(TABLE) == COLUMN1.Of(TABLE1)),
	    "select COLUMN from TABLE inner join TABLE1 on TABLE.COLUMN = TABLE1.COLUMN1");
	EXP(Select(COLUMN).From(TABLE).LeftJoin(TABLE1).On(COLUMN.Of(TABLE) == COLUMN1.Of(TABLE1)),
	    "select COLUMN from TABLE left outer join TABLE1 on TABLE.COLUMN = TABLE1.COLUMN1");
	EXP(Select(COLUMN).From(TABLE).RightJoin(TABLE1).On(COLUMN.Of(TABLE) == COLUMN1.Of(TABLE1)),
	    "select COLUMN from TABLE right outer join TABLE1 on TABLE.COLUMN = TABLE1.COLUMN1");
	EXP(Select(COLUMN).From(TABLE).FullJoin(TABLE1).On(COLUMN.Of(TABLE) == COLUMN1.Of(TABLE1)),
	    "select COLUMN from TABLE full outer join TABLE1 on TABLE.COLUMN = TABLE1.COLUMN1");
	EXP(Delete(TABLE).Where(COLUMN < 0),
	    "delete from TABLE where COLUMN < 0");
	EXP(Insert(TABLE)(COLUMN1, 12)(COLUMN2, "hello")(COLUMN, Date(2007, 1, 1)),
	    "insert into TABLE(COLUMN1, COLUMN2, COLUMN) values (12, 'hello', '2007-01-01')");
	EXP(Insert(TABLE)(COLUMN1, 12)(COLUMN2)(COLUMN).From(TABLE1).Where(COLUMN >= 0),
	    "insert into TABLE(COLUMN1, COLUMN2, COLUMN) select 12, COLUMN2, COLUMN from TABLE1 where COLUMN >= 0");
	EXP(Update(TABLE)(COLUMN1, 13)(COLUMN2, "world").Where(COLUMN > Date(2007, 1, 1)),
	    "update TABLE set COLUMN1 = 13, COLUMN2 = 'world' where COLUMN > '2007-01-01'");
}
