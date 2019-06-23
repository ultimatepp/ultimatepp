#include <Sql/Sql.h>

using namespace Upp;

#define MODEL <SqlExp/SqlExp.sch>
#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#include <Sql/sch_header.h>
#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>

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
SQLID(B)
SQLID(C)
SQLID(OBEXPBI)
SQLID(USERNAME)
SQLID(EMAIL)
SQLID(USERS)

SQLID(TN)
SQLID(TNSIZE)
SQLID(PARENT_ID)
SQLID(TREENODE)

void Exp0(int dialect, const char *dialectn, const char *sqlexp, SqlStatement x)
{
	LOG("\tTEST(" << dialectn << ",\n\t\t" << sqlexp << ",\n\t\t"
	    << AsCString(x.Get(dialect)) << "); // " << x.Get(dialect));
}

void Exp(const char *sqlexp, SqlStatement x)
{
	Exp0(MY_SQL, "MY_SQL", sqlexp, x);
	Exp0(SQLITE3, "SQLITE3", sqlexp, x);
	Exp0(ORACLE, "ORACLE", sqlexp, x);
	Exp0(MSSQL, "MSSQL", sqlexp, x);
	Exp0(PGSQL, "PGSQL", sqlexp, x);
	Exp0(FIREBIRD, "FIREBIRD", sqlexp, x);
	Exp0(DB2, "DB2", sqlexp, x);
	LOG("// ---------------------------------");
}

#define EXP(x) Exp(#x, x)

int NoQuotes(int c)
{
	return c == '\"' || c == '`' ? 0 : c;
}

bool IsCid(int c)
{
	return IsAlNum(c) || c == '_' || c == '$';
}

void Test(int dialect, const char *dialectn, SqlStatement s, const char *q)
{
	String compiled = s.Get(dialect);
	bool quote = false;
	for(int i = 0; i < compiled.GetCount(); i++) {
		if(compiled[i] == '\"' || compiled[i] == '`')
			quote = !quote;
		else
		if(quote && !IsCid(compiled[i])) {
			LOG("---------------------");
			LOG("Dialect  " << dialectn);
			LOG("Etalon   " << q);
			LOG("Compiled " << s.Get(dialect));
			LOG("FAILED QUOTES");
		#ifndef flagNOSTOP
			NEVER();
		#endif
		}
	}
	if(compiled != q) {
		LOG("---------------------");
		LOG("Dialect  " << dialectn);
		LOG("Etalon   " << q);
		LOG("Compiled " << s.Get(dialect));
		LOG("FAILED");
	#ifndef flagNOSTOP
		NEVER();
	#endif
	}
}

#define TEST(dialect, s, e) Test(dialect, #dialect, s, e)

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	SqlId::UseQuotes();

	Date date = Date(2011, 12, 31);
	EXP(Select(A, B).From(TABLE1, TABLE2).OrderBy(B, C));
	EXP(Select(A, B).From(TABLE1, TABLE2).Where(A == 10).OrderBy(B, C));
	EXP(Select(A, B).From(SqlSet(TABLE1, TABLE2)).Where(A == 10).OrderBy(SqlSet(B, C)));
	EXP(Select(PH_SEQ, SqlSum(CASTKA)).From(PH_POLOZKA).Where(DATUM <= date).GroupBy(PH_SEQ).Having(C == B));
	EXP(Insert(TABLE1)(A, 1)(B, 2));
	EXP(Insert(PLAT_DTM)
				(PLAT_DTM_SEQ, NextVal(SEQ_POPLATEK))
				(PRIPAD_SEQ, 1111)
				(DATUM)
				(CASTKA)
				(TXT)
			  .From(PLAT_DTM).Where(PRIPAD_SEQ == 2222));
	EXP(Select(KATASTR_SEQ).From(ADRESA)
				.StartWith(PAR_ADRESA_SEQ == 1111)
				.ConnectBy(Prior(ADRESA_SEQ) == PAR_ADRESA_SEQ)
			| Select(KATASTR_SEQ).From(ADRESA)
				.StartWith(ADRESA_SEQ == 2222)
				.ConnectBy(Prior(PAR_ADRESA_SEQ) == ADRESA_SEQ));
	EXP(SqlUpdate(KONTO)(ROK, 1111)(OBEXPBI, 2222).Where(KONTO_CON == 3333));
	EXP(Select(23 * (ROK + 1)).From(TABLE1));
	EXP(Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2));
	EXP(Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2));
	EXP(Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2));

	EXP(Select(COL).From(TABLE1));
	EXP(Select(COL.Of(TABLE1)).From(TABLE1));
	EXP(Select(COL.As(A)).From(TABLE1));
	EXP(Select(COL&A).From(TABLE1));
	EXP(Select(COL[3]).From(TABLE1));
	EXP(Select(COLUMN1 % COLUMN2).From(TABLE1));
	EXP(Select(COLUMN1 | COLUMN2).From(TABLE1));
	EXP(Select(SqlFunc("any_fn", COL, 2)).From(TABLE1));
	EXP(Select(Distinct(COL)).From(TABLE1));
	EXP(Select(Distinct(SqlSet(COLUMN1, COLUMN2))).From(TABLE1));
	EXP(Select(All(COL)).From(TABLE1));
	EXP(Select(Count(COL)).From(TABLE1));
	EXP(Select(SqlAll()).From(TABLE1));
	EXP(Select(SqlCountRows()).From(TABLE1));
	EXP(Select(COL).From(TABLE1).OrderBy(Descending(COL)));
	EXP(Select(SqlMax(COL)).From(TABLE1));
	EXP(Select(SqlMin(COL)).From(TABLE1));
	EXP(Select(SqlSum(COL)).From(TABLE1));
	EXP(Select(Avg(COL)).From(TABLE1));
	EXP(Select(Stddev(COL)).From(TABLE1));
	EXP(Select(Variance(COL)).From(TABLE1));
	EXP(Select(Greatest(COLUMN1, COLUMN2)).From(TABLE1));
	EXP(Select(Least(COLUMN1, COLUMN2)).From(TABLE1));
	EXP(Select(Upper(COL)).From(TABLE1));
	EXP(Select(Lower(COL)).From(TABLE1));
	EXP(Select(Instr(COL, "hello")).From(TABLE1));
	EXP(Select(SqlNvl(COLUMN1, COLUMN2)).From(TABLE1));
	EXP(Select(NextVal(SEQ)).Get());
	EXP(Select(CurrVal(SEQ)).Get());
	EXP(Select(SqlArg()).From(TABLE1));
	EXP(Select(Coalesce(COLUMN1, "_")).From(TABLE1));
	EXP(Select(Coalesce(COLUMN1, "_").As("H")).From(TABLE1));
	EXP(Select(Coalesce(COLUMN1, "_").As(COLUMN2)).From(TABLE1));
	EXP(Select(Decode(COL, SqlSet("1", 2, 3))).From(TABLE1));
	EXP(Select(Length(COL)).From(TABLE1));
	EXP(Select(Substr(COL, 2, 3)).From(TABLE1));
	EXP(Select(SqlDate(COL, SEQ, A)).From(TABLE1));

	EXP(Select(COL).From(TABLE1).Where(COL / 2 > 1 && COLUMN1 == "A" || COLUMN2 == Date(2006, 1, 1)));
	EXP(Select(COL).From(TABLE1).Where(!(COL == 1)));
	EXP(Select(COL).From(TABLE1).Where((COLUMN1 == 1) - (COLUMN2 == 1)));
	EXP(Select(COL).From(TABLE1).Where(IsNull(COLUMN1)));
	EXP(Select(COL).From(TABLE1).Where(NotNull(COLUMN1)));
	EXP(Select(COL).From(TABLE1).Where(Like(COLUMN1, Wild("A*"))));
	EXP(Select(COL).From(TABLE1).Where(NotLike(COLUMN1, Wild("A*"))));
	EXP(Select(COL).From(TABLE1).Where(In(COL, Select(COL).From(TABLE1))));
	EXP(Select(COL).From(TABLE1).Where(COL == Select(COL).From(TABLE1)));
	EXP(Select(COL).From(TABLE1).Where(COL == Select(COL).From(TABLE1).AsValue()));
	EXP(Select(COL).From(TABLE1).Where(NotIn(COL, Select(COL).From(TABLE1))));
	EXP(Select(COL).From(TABLE1).Where(COL != Select(COL).From(TABLE1)));
	EXP(Select(COL).From(TABLE1).Where(Exists(Select(COL).From(TABLE1))));
	EXP(Select(COL).From(TABLE1).Where(NotExists(Select(COL).From(TABLE1))));

	EXP(Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2))));
	EXP(Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2))));
	EXP(Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2))));
	EXP(Select(COL).From(TABLE1).Where(COL == 0).GroupBy(COL).Having(COL == 0).OrderBy(Descending(COL)));
	EXP(Select(COL, Count(Select(SqlAll()).From(TABLE1))).From(TABLE1).Where(COL == 0).GroupBy(COL)
	    .Having(COL == 0).OrderBy(Descending(COL)));
	EXP(Select(COL).From(TABLE1).Limit(100));
	EXP(Select(COL).From(TABLE1).Limit(100, 10));
	EXP(Select(COL).From(TABLE1).Offset(20));
	EXP(Select(25 * 34).Get());
	EXP(Select(COL).From(TABLE1).Hint("hint"));
	EXP(Select(COL).From(TABLE1).InnerJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)));
	EXP(Select(COL).From(TABLE1).LeftJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)));
	EXP(Select(COL).From(TABLE1).RightJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)));
	EXP(Select(COL).From(TABLE1).FullJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)));

	EXP(Delete(TABLE1).Where(COL < 0));

	EXP(Insert(TABLE1)(COLUMN1, 12)(COLUMN2, "hello")(COL, Date(2007, 1, 1)));
	EXP(Insert(TABLE1)(COLUMN1, 12)(COLUMN2)(COL).From(TABLE1).Where(COL >= 0));

	EXP(Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1));
	EXP(Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1).Having(COLUMN2 > 10));

	EXP(Insert(TABLE1)(COLUMN1, 1).Where(NotExists(Select(ID).From(TABLE1).Where(COLUMN1 == 1))));

	EXP(Update(TABLE1)(COLUMN1, 13)(COLUMN2, "world").Where(COL > Date(2007, 1, 1)));
	
	EXP(Insert(TABLE1)(COL, Select(COLUMN1).From(TABLE1).Where(COLUMN2 == 21).AsValue()));

	EXP(Select(COL).From(Select(COL).From(TABLE1)));
	EXP(Select(COL).From(Select(COL).From(TABLE1).AsTable(TABLE2)));
	EXP(Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)));
	EXP(Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1).AsTable(TABLE2)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)));

	EXP(Select(TABLE1(COL, COLUMN1)).From(TABLE1));

	EXP(Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).Where(BDATE == Date(2011, 12, 9)));
	EXP(Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).On(IsNull(BDATE)).Where(BDATE == Date(2011, 12, 9)));
	
	EXP(Select(SqlAll().Of(TABLE1)).From(TABLE1));

	EXP(Select(SqlTxt("TEST.*")).From(TABLE1));

	EXP(Select(SqlTxt("123")).From(TABLE1));

	VectorMap<int, String> m;
	for(int i = 0; i < 10; i++)
		m.Add(i, AsString(i));
	
	EXP(Select(ID).From(TABLE1).Where(ID == SqlSetFrom(m)));
	
	EXP(SelectAll().From(ID).InnerJoin(
		SelectAll().From(ID).Where(ID >= Time(2014, 1, 1, 0, 0, 0)).GroupBy(ID).AsTable(ID)));

	EXP(
		WithRecursive(TN)(ID, NAME, PARENT_ID)
			.As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID))
			    +
			    Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN)
			    .Where(TREENODE(PARENT_ID) == TN(ID)))
		.With(TNSIZE)
		    .As(Select(PARENT_ID,
		               Select(NAME).From(TREENODE).Where(ID == TN(PARENT_ID)).AsValue(),
		               SqlCountRows())
		        .From(TN)
		        .GroupBy(PARENT_ID))
		(SelectAll().From(TNSIZE))
	);

	SqlId COUNT("COUNT");
	EXP(
		WithRecursive(TN)(ID, NAME, PARENT_ID)
			.As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID))
			    +
			    Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN)
			    .Where(TREENODE(PARENT_ID) == TN(ID)))
		.With(TNSIZE)(NAME, COUNT)
		    .As(Select(Select(NAME).From(TREENODE).Where(ID == TN(PARENT_ID)).AsValue(),
		               SqlCountRows())
		        .From(TN)
		        .GroupBy(PARENT_ID))
		(Insert(TABLE1)(NAME, NAME)(COL, COUNT).From(TNSIZE))
	);
	
	EXP(
		WithRecursive(TN)(ID, NAME, PARENT_ID)
			.As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID))
			    +
			    Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN)
			    .Where(TREENODE(PARENT_ID) == TN(ID)))
		.With(TNSIZE)(ID, COUNT)
		    .As(Select(ID,
		               SqlCountRows())
		        .From(TN)
		        .GroupBy(PARENT_ID))
		(Delete(TREENODE).Where(ID == Select(ID).From(TNSIZE).Where(COUNT == 0)))
	);

	EXP(
		WithRecursive(TN)(ID, NAME, PARENT_ID)
			.As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID))
			    +
			    Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN)
			    .Where(TREENODE(PARENT_ID) == TN(ID)))
		.With(TNSIZE)(ID, COUNT)
		    .As(Select(ID,
		               SqlCountRows())
		        .From(TN)
		        .GroupBy(PARENT_ID))
		(Update(TABLE1)(COL, Select(COUNT).From(TNSIZE).Where(TNSIZE(ID) == TABLE1(ID)).AsValue()))
	);
	
	EXP(Temporary("TT").As(Select(NAME, LASTNAME).From(TABLE1).Where(COLUMN1 == 12)));

	EXP(Update(TABLE1)(COLUMN1, TABLE2(COLUMN2)).From(TABLE2).On(TABLE1(ID) == TABLE2(TABLE1_ID)));
	
	EXP(
		Select(A(SqlAll())).From(USERS.As(A))
		.InnerJoin(
			Select(Count(SqlAll()), USERNAME, EMAIL).From(USERS)
			.GroupBy(USERNAME, EMAIL).Having(Count(SqlAll()) > 1)
			.AsTable(B)
		)
		.On(A(USERNAME) == B(USERNAME) && A(EMAIL) == B(EMAIL))
	);

	LOG("\n\n\n=========================================================================");
	#include "Test.i"
	
	LOG("================== OK");
}
