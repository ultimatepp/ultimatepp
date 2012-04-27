#include <RichText/RichText.h>
#include <Sql/Sql.h>

using namespace Upp;

SQLID(COLUMN);
SQLID(COLUMN1);
SQLID(COLUMN2);
SQLID(TABLE);
SQLID(TABLE1);
SQLID(TABLE2);
SQLID(SEQ);
SQLID(A);

#define EXP(s) Exp(#s, s)

String qtf;

void Put(String s, const char *splits)
{
	Vector<String> split = Split(splits, ';');
	bool was = false;
	for(int i = 0; i < split.GetCount(); i++) {
		int q = s.Find(split[i]);
		if(q >= 0) {
			if(was)
				qtf << "&\n";
			qtf << DeQtf(s.Mid(0, q));
			s = s.Mid(q);
			was = true;
		}
	}
	if(was)
		qtf << "&\n";
	qtf << DeQtf(s);
}

void Exp(const char *s, SqlStatement sql)
{
	DDUMP(s);
	DDUMP(sql.Get(PGSQL));

	qtf << "\n:: ";
	Put(s, ".From;.Where;.OrderBy");
	qtf << "\n:: ";
	Put(SqlCompile(PGSQL, sql.Get(PGSQL)), " from; where; order by");
}

CONSOLE_APP_MAIN
{
	DLOG(SqlCompile(PGSQL, String().Cat() << '\t' << TABLE(COLUMN1, COLUMN2) << '\t' << " from \t" << TABLE << "\t"));
//	return;
	qtf << "[C1 {{1:1 SqlExp C`+`+:: PostgreSQL";
	EXP(Select(COLUMN).From(TABLE));
	EXP(Select(COLUMN.Of(TABLE)).From(TABLE));
	EXP(Select(COLUMN.As(A)).From(TABLE));
	EXP(Select(COLUMN&A).From(TABLE));
	EXP(Select(COLUMN[3]).From(TABLE));
	EXP(Select(COLUMN1 % COLUMN2).From(TABLE));
	EXP(Select(COLUMN1 | COLUMN2).From(TABLE));
	EXP(Select(SqlFunc("any_fn", COLUMN, 2)).From(TABLE));
	EXP(Select(Distinct(COLUMN)).From(TABLE));
	EXP(Select(Distinct(SqlSet(COLUMN1, COLUMN2))).From(TABLE));
	EXP(Select(All(COLUMN)).From(TABLE));
	EXP(Select(Count(COLUMN)).From(TABLE));
	EXP(Select(SqlAll()).From(TABLE));
	EXP(Select(SqlCountRows()).From(TABLE));
	EXP(Select(COLUMN).From(TABLE).OrderBy(Descending(COLUMN)));
	EXP(Select(SqlMax(COLUMN)).From(TABLE));
	EXP(Select(SqlMin(COLUMN)).From(TABLE));
	EXP(Select(SqlSum(COLUMN)).From(TABLE));
	EXP(Select(Avg(COLUMN)).From(TABLE));
	EXP(Select(Stddev(COLUMN)).From(TABLE));
	EXP(Select(Variance(COLUMN)).From(TABLE));
	EXP(Select(Greatest(COLUMN1, COLUMN2)).From(TABLE));
	EXP(Select(Least(COLUMN1, COLUMN2)).From(TABLE));
	EXP(Select(Upper(COLUMN)).From(TABLE));
	EXP(Select(Lower(COLUMN)).From(TABLE));
	EXP(Select(Substr(COLUMN, 1)).From(TABLE));
	EXP(Select(Substr(COLUMN, 2, 1)).From(TABLE));
	EXP(Select(Instr(COLUMN, "hello")).From(TABLE));
	EXP(Select(SqlNvl(COLUMN1, COLUMN2)).From(TABLE));
	EXP(Select(NextVal(SEQ)).Get());
	EXP(Select(CurrVal(SEQ)).Get());
	EXP(Select(SqlArg()).From(TABLE));

	EXP(Select(COLUMN).From(TABLE).Where(COLUMN / 2 > 1 && COLUMN1 == "A" || COLUMN2 == Date(2006, 1, 1)));
	EXP(Select(COLUMN).From(TABLE).Where(!(COLUMN == 1)));
	EXP(Select(COLUMN).From(TABLE).Where((COLUMN1 == 1) - (COLUMN2 == 1)));
	EXP(Select(COLUMN).From(TABLE).Where(IsNull(COLUMN1)));
	EXP(Select(COLUMN).From(TABLE).Where(NotNull(COLUMN1)));
	EXP(Select(COLUMN).From(TABLE).Where(Like(COLUMN1, Wild("A*"))));
	EXP(Select(COLUMN).From(TABLE).Where(NotLike(COLUMN1, Wild("A*"))));
	EXP(Select(COLUMN).From(TABLE).Where(In(COLUMN, Select(COLUMN).From(TABLE1))));
	EXP(Select(COLUMN).From(TABLE).Where(COLUMN == Select(COLUMN).From(TABLE1)));
	EXP(Select(COLUMN).From(TABLE).Where(NotIn(COLUMN, Select(COLUMN).From(TABLE1))));
	EXP(Select(COLUMN).From(TABLE).Where(COLUMN != Select(COLUMN).From(TABLE1)));
	EXP(Select(COLUMN).From(TABLE).Where(Exists(Select(COLUMN).From(TABLE1))));
	EXP(Select(COLUMN).From(TABLE).Where(NotExists(Select(COLUMN).From(TABLE1))));

	EXP(Select(COLUMN).From(TABLE).Where(COLUMN == (Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2))));
	EXP(Select(COLUMN).From(TABLE).Where(COLUMN == (Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2))));
	EXP(Select(COLUMN).From(TABLE).Where(COLUMN == (Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2))));
	EXP(Select(COLUMN).From(TABLE).Where(COLUMN == 0).GroupBy(COLUMN).Having(COLUMN == 0).OrderBy(Descending(COLUMN)));
	EXP(Select(COLUMN).From(TABLE).Limit(100));
	EXP(Select(COLUMN).From(TABLE).Limit(100, 10));
	EXP(Select(COLUMN).From(TABLE).Offset(20));
	EXP(Select(25 * 34).Get());
	EXP(Select(COLUMN).From(TABLE).Hint("hint"));
	EXP(Select(COLUMN).From(TABLE).InnerJoin(TABLE1).On(COLUMN.Of(TABLE) == COLUMN1.Of(TABLE1)));
	EXP(Select(COLUMN).From(TABLE).LeftJoin(TABLE1).On(COLUMN.Of(TABLE) == COLUMN1.Of(TABLE1)));
	EXP(Select(COLUMN).From(TABLE).RightJoin(TABLE1).On(COLUMN.Of(TABLE) == COLUMN1.Of(TABLE1)));
	EXP(Select(COLUMN).From(TABLE).FullJoin(TABLE1).On(COLUMN.Of(TABLE) == COLUMN1.Of(TABLE1)));

	EXP(Delete(TABLE).Where(COLUMN < 0));

	EXP(Insert(TABLE)(COLUMN1, 12)(COLUMN2, "hello")(COLUMN, Date(2007, 1, 1)));
	EXP(Insert(TABLE)(COLUMN1, 12)(COLUMN2)(COLUMN).From(TABLE1).Where(COLUMN >= 0));

	EXP(Update(TABLE)(COLUMN1, 13)(COLUMN2, "world").Where(COLUMN > Date(2007, 1, 1)));

	qtf << "}}";

	ParseQTF(qtf);
	SaveFile(ConfigFile("sqlexp.qtf"), qtf);
}
