#include <RichEdit/RichEdit.h>
#include <Sql/Sql.h>

using namespace Upp;

#ifdef flagQTF
#define GENERATE_QTF
#endif

#define MODEL <SqlExp/SqlExp.sch>
#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#include <Sql/sch_header.h>
#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>

#define EXP(s) Exp(#s, s)

#ifdef GENERATE_QTF
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
	qtf << "\n:: ";
	Put(s, ".From;.Where;.OrderBy");
	qtf << "\n:: ";
	Put(sql.Get(PGSQL), " from; where; order by");
}
#else
void Exp(const char *s, SqlStatement sql)
{
	LOG(s);
	LOG('\t' << sql.Get(PGSQL));
}
#endif

GUI_APP_MAIN
{
#ifdef GENERATE_QTF
	qtf << "[C1 {{1:1 SqlExp C`+`+:: PostgreSQL";
#else
	StdLogSetup(LOG_FILE|LOG_COUT);
#endif
	EXP(Select(COL).From(TABLE1));
	EXP(Select(COL.Of(TABLE1)).From(TABLE1));
	EXP(Select(COL.As(A)).From(TABLE1));
	EXP(Select(TABLE1(COL, A.As("T_A"), SEQ)).From(TABLE1));
	EXP(Select(COL&A).From(TABLE1));
	EXP(Select(COL[3]).From(TABLE1));
	EXP(Select(COLUMN1 % COLUMN2).From(TABLE1));
	EXP(Select(COLUMN1 | COLUMN2).From(TABLE1));
	EXP(Select(SqlFunc("any_fn", COL, 2)).From(TABLE1));
	EXP(Select(SqlTxt("current_time")).From(TABLE1));
	EXP(Select(Distinct(COL)).From(TABLE1));
	EXP(Select(Distinct(SqlSet(COLUMN1, COLUMN2))).From(TABLE1));
	EXP(Select(All(COL)).From(TABLE1));
	EXP(Select(Count(COL)).From(TABLE1));
	EXP(Select(SqlAll()).From(TABLE1));
	EXP(SelectAll().From(TABLE1));
	EXP(Select(SqlCountRows()).From(TABLE1));
	EXP(Select(COL).From(TABLE1).OrderBy(Descending(COL)));
	EXP(Select(SqlMax(COL)).From(TABLE1));
	EXP(Select(SqlMin(COL)).From(TABLE1));
	EXP(Select(SqlSum(COL)).From(TABLE1));
	EXP(Select(Avg(COL)).From(TABLE1));
	EXP(Select(Abs(COL)).From(TABLE1));
	EXP(Select(Stddev(COL)).From(TABLE1));
	EXP(Select(Variance(COL)).From(TABLE1));
	EXP(Select(Greatest(COLUMN1, COLUMN2)).From(TABLE1));
	EXP(Select(Least(COLUMN1, COLUMN2)).From(TABLE1));
	EXP(Select(Upper(COL)).From(TABLE1));
	EXP(Select(Lower(COL)).From(TABLE1));
	EXP(Select(Length(COL)).From(TABLE1));
	EXP(Select(Substr(COL, 2, 1)).From(TABLE1));
	EXP(Select(Instr(COL, "hello")).From(TABLE1));
	EXP(Select(SqlNvl(COLUMN1, COLUMN2)).From(TABLE1));
	EXP(Select(NextVal(SEQ)).Get());
	EXP(Select(CurrVal(SEQ)).Get());
	EXP(Select(SqlArg()).From(TABLE1));
	EXP(Select(Case(COL == 1, "one")(COL == 2, "two")("?")).From(TABLE1));
	EXP(Select((COL > 10).AsValue()).From(TABLE2));
	EXP(Select((COL > 10).As("GT10")).From(TABLE2));

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

	EXP(Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2));
	EXP(Select(COLUMN1).From(TABLE1) + Select(COLUMN2).From(TABLE2));
	EXP(Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2));
	EXP(Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2));
	EXP(Select(COL).From(TABLE1).Where(COL == 0).GroupBy(COL).Having(COL == 0).OrderBy(Descending(COL)));
	EXP(Select(COL, Count(Select(SqlAll()).From(TABLE1))).From(TABLE1).Where(COL == 0).GroupBy(COL)
	    .Having(COL == 0).OrderBy(Descending(COL)));
	EXP(Select(ID, Avg(COL)).From(TABLE1).GroupBy(ID).Having(SqlBoolFunc("bool_and", COLUMN1 > 0)));
	EXP(Select(COL).From(TABLE1).Limit(100));
	EXP(Select(COL).From(TABLE1).Limit(100, 10));
	EXP(Select(COL).From(TABLE1).Offset(20));
	EXP(Select(25 * 34).Get());
	EXP(Select(COL).From(TABLE1).Hint("hint"));
	EXP(Select(COL).From(TABLE1).InnerJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)));
	EXP(Select(COL).From(TABLE1).LeftJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)));
	EXP(Select(COL).From(TABLE1).RightJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)));
	EXP(Select(COL).From(TABLE1).FullJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)));

	EXP(Insert(TABLE1)(COLUMN1, 12)(COLUMN2, "hello")(COL, Date(2007, 1, 1)));
	EXP(Insert(TABLE1)(COLUMN1, 12)(COLUMN2)(COL).From(TABLE1).Where(COL >= 0));
	EXP(Insert(TABLE1)(COL, Select(COLUMN1).From(TABLE1).Where(COLUMN2 == 21).AsValue()));
	EXP(Insert(TABLE1)(COLUMN1, "unique_value").Where(NotExists(Select(ID).From(TABLE1).Where(COLUMN1 == "unique_value"))));
	EXP(Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1).Having(COLUMN2 > 10));
	EXP(Insert(TABLE1)(COLUMN1, 12)(COLUMN2, "hello")(COL, Date(2007, 1, 1)).Returning(ID));

	EXP(Update(TABLE1)(COLUMN1, 13)(COLUMN2, "world").Where(COL > Date(2007, 1, 1)));
	
	{
	ValueMap m;
	m(COLUMN1, 1)(COLUMN2, "hello");
	EXP(Insert(TABLE1)(m));
	EXP(Update(TABLE2)(m).Where(ID == 123));
	}
	
	EXP(Delete(TABLE1).Where(COL < 0));

	EXP(Select(COL).From(Select(COL).From(TABLE1)));
	EXP(Select(COL).From(Select(COL).From(TABLE1).AsTable(TABLE2)));
	EXP(Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)));
	EXP(Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1).AsTable(TABLE2)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)));

	EXP(Select(TABLE1(COL, COLUMN1)).From(TABLE1));

	EXP(Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).Where(BDATE == Date(2011, 12, 9)));
	EXP(Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).On(IsNull(BDATE)).Where(BDATE == Date(2011, 12, 9)));
	
	EXP(Select(TABLE1(SqlAll())).From(TABLE1));
	EXP(Select(SqlAll().Of(TABLE1)).From(TABLE1));
	EXP(Select(TABLE1(S_TABLE2())).From(TABLE1));
	
	Vector<int> m;
	for(int i = 0; i < 10; i++)
		m.Add(i);
	
	EXP(Select(ID).From(TABLE1).Where(ID == SqlSetFrom(m)));

	SqlId TN("TN");
	SqlId TNSIZE("TNSIZE");
	
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

#ifdef GENERATE_QTF
	qtf << "}}";

	RichEditWithToolBar edit;
	edit.SetQTF(qtf);
	TopWindow win;
	win.Add(edit.SizePos());
	win.Run();
#endif
}
