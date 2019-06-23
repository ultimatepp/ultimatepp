	TEST(MY_SQL,
		Select(A, B).From(TABLE1, TABLE2).OrderBy(B, C),
		"select `A`, `B` from `TABLE1`, `TABLE2` order by `B`, `C`"); // select `A`, `B` from `TABLE1`, `TABLE2` order by `B`, `C`
	TEST(SQLITE3,
		Select(A, B).From(TABLE1, TABLE2).OrderBy(B, C),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" order by "B", "C"
	TEST(ORACLE,
		Select(A, B).From(TABLE1, TABLE2).OrderBy(B, C),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" order by "B", "C"
	TEST(MSSQL,
		Select(A, B).From(TABLE1, TABLE2).OrderBy(B, C),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" order by "B", "C"
	TEST(PGSQL,
		Select(A, B).From(TABLE1, TABLE2).OrderBy(B, C),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" order by "B", "C"
	TEST(FIREBIRD,
		Select(A, B).From(TABLE1, TABLE2).OrderBy(B, C),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" order by "B", "C"
	TEST(DB2,
		Select(A, B).From(TABLE1, TABLE2).OrderBy(B, C),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" order by "B", "C"
// ---------------------------------
	TEST(MY_SQL,
		Select(A, B).From(TABLE1, TABLE2).Where(A == 10).OrderBy(B, C),
		"select `A`, `B` from `TABLE1`, `TABLE2` where `A` = 10 order by `B`, `C`"); // select `A`, `B` from `TABLE1`, `TABLE2` where `A` = 10 order by `B`, `C`
	TEST(SQLITE3,
		Select(A, B).From(TABLE1, TABLE2).Where(A == 10).OrderBy(B, C),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" where \"A\" = 10 order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" where "A" = 10 order by "B", "C"
	TEST(ORACLE,
		Select(A, B).From(TABLE1, TABLE2).Where(A == 10).OrderBy(B, C),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" where \"A\" = 10 order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" where "A" = 10 order by "B", "C"
	TEST(MSSQL,
		Select(A, B).From(TABLE1, TABLE2).Where(A == 10).OrderBy(B, C),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" where \"A\" = 10 order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" where "A" = 10 order by "B", "C"
	TEST(PGSQL,
		Select(A, B).From(TABLE1, TABLE2).Where(A == 10).OrderBy(B, C),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" where \"A\" = 10 order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" where "A" = 10 order by "B", "C"
	TEST(FIREBIRD,
		Select(A, B).From(TABLE1, TABLE2).Where(A == 10).OrderBy(B, C),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" where \"A\" = 10 order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" where "A" = 10 order by "B", "C"
	TEST(DB2,
		Select(A, B).From(TABLE1, TABLE2).Where(A == 10).OrderBy(B, C),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" where \"A\" = 10 order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" where "A" = 10 order by "B", "C"
// ---------------------------------
	TEST(MY_SQL,
		Select(A, B).From(SqlSet(TABLE1, TABLE2)).Where(A == 10).OrderBy(SqlSet(B, C)),
		"select `A`, `B` from `TABLE1`, `TABLE2` where `A` = 10 order by `B`, `C`"); // select `A`, `B` from `TABLE1`, `TABLE2` where `A` = 10 order by `B`, `C`
	TEST(SQLITE3,
		Select(A, B).From(SqlSet(TABLE1, TABLE2)).Where(A == 10).OrderBy(SqlSet(B, C)),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" where \"A\" = 10 order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" where "A" = 10 order by "B", "C"
	TEST(ORACLE,
		Select(A, B).From(SqlSet(TABLE1, TABLE2)).Where(A == 10).OrderBy(SqlSet(B, C)),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" where \"A\" = 10 order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" where "A" = 10 order by "B", "C"
	TEST(MSSQL,
		Select(A, B).From(SqlSet(TABLE1, TABLE2)).Where(A == 10).OrderBy(SqlSet(B, C)),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" where \"A\" = 10 order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" where "A" = 10 order by "B", "C"
	TEST(PGSQL,
		Select(A, B).From(SqlSet(TABLE1, TABLE2)).Where(A == 10).OrderBy(SqlSet(B, C)),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" where \"A\" = 10 order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" where "A" = 10 order by "B", "C"
	TEST(FIREBIRD,
		Select(A, B).From(SqlSet(TABLE1, TABLE2)).Where(A == 10).OrderBy(SqlSet(B, C)),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" where \"A\" = 10 order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" where "A" = 10 order by "B", "C"
	TEST(DB2,
		Select(A, B).From(SqlSet(TABLE1, TABLE2)).Where(A == 10).OrderBy(SqlSet(B, C)),
		"select \"A\", \"B\" from \"TABLE1\", \"TABLE2\" where \"A\" = 10 order by \"B\", \"C\""); // select "A", "B" from "TABLE1", "TABLE2" where "A" = 10 order by "B", "C"
// ---------------------------------
	TEST(MY_SQL,
		Select(PH_SEQ, SqlSum(CASTKA)).From(PH_POLOZKA).Where(DATUM <= date).GroupBy(PH_SEQ).Having(C == B),
		"select `PH_SEQ`, sum(`CASTKA`) from `PH_POLOZKA` where `DATUM` <= '2011-12-31' group by `PH_SEQ` having `C` = `B`"); // select `PH_SEQ`, sum(`CASTKA`) from `PH_POLOZKA` where `DATUM` <= '2011-12-31' group by `PH_SEQ` having `C` = `B`
	TEST(SQLITE3,
		Select(PH_SEQ, SqlSum(CASTKA)).From(PH_POLOZKA).Where(DATUM <= date).GroupBy(PH_SEQ).Having(C == B),
		"select \"PH_SEQ\", sum(\"CASTKA\") from \"PH_POLOZKA\" where \"DATUM\" <= '2011-12-31' group by \"PH_SEQ\" having \"C\" = \"B\""); // select "PH_SEQ", sum("CASTKA") from "PH_POLOZKA" where "DATUM" <= '2011-12-31' group by "PH_SEQ" having "C" = "B"
	TEST(ORACLE,
		Select(PH_SEQ, SqlSum(CASTKA)).From(PH_POLOZKA).Where(DATUM <= date).GroupBy(PH_SEQ).Having(C == B),
		"select \"PH_SEQ\", sum(\"CASTKA\") from \"PH_POLOZKA\" where \"DATUM\" <= to_date('2011/12/31', 'SYYYY/MM/DD') group by \"PH_SEQ\" having \"C\" = \"B\""); // select "PH_SEQ", sum("CASTKA") from "PH_POLOZKA" where "DATUM" <= to_date('2011/12/31', 'SYYYY/MM/DD') group by "PH_SEQ" having "C" = "B"
	TEST(MSSQL,
		Select(PH_SEQ, SqlSum(CASTKA)).From(PH_POLOZKA).Where(DATUM <= date).GroupBy(PH_SEQ).Having(C == B),
		"select \"PH_SEQ\", sum(\"CASTKA\") from \"PH_POLOZKA\" where \"DATUM\" <= convert(datetime, '2011/12/31', 120) group by \"PH_SEQ\" having \"C\" = \"B\""); // select "PH_SEQ", sum("CASTKA") from "PH_POLOZKA" where "DATUM" <= convert(datetime, '2011/12/31', 120) group by "PH_SEQ" having "C" = "B"
	TEST(PGSQL,
		Select(PH_SEQ, SqlSum(CASTKA)).From(PH_POLOZKA).Where(DATUM <= date).GroupBy(PH_SEQ).Having(C == B),
		"select \"PH_SEQ\", sum(\"CASTKA\") from \"PH_POLOZKA\" where \"DATUM\" <= date '2011-12-31' group by \"PH_SEQ\" having \"C\" = \"B\""); // select "PH_SEQ", sum("CASTKA") from "PH_POLOZKA" where "DATUM" <= date '2011-12-31' group by "PH_SEQ" having "C" = "B"
	TEST(FIREBIRD,
		Select(PH_SEQ, SqlSum(CASTKA)).From(PH_POLOZKA).Where(DATUM <= date).GroupBy(PH_SEQ).Having(C == B),
		"select \"PH_SEQ\", sum(\"CASTKA\") from \"PH_POLOZKA\" where \"DATUM\" <= '2011-12-31' group by \"PH_SEQ\" having \"C\" = \"B\""); // select "PH_SEQ", sum("CASTKA") from "PH_POLOZKA" where "DATUM" <= '2011-12-31' group by "PH_SEQ" having "C" = "B"
	TEST(DB2,
		Select(PH_SEQ, SqlSum(CASTKA)).From(PH_POLOZKA).Where(DATUM <= date).GroupBy(PH_SEQ).Having(C == B),
		"select \"PH_SEQ\", sum(\"CASTKA\") from \"PH_POLOZKA\" where \"DATUM\" <= '2011-12-31' group by \"PH_SEQ\" having \"C\" = \"B\""); // select "PH_SEQ", sum("CASTKA") from "PH_POLOZKA" where "DATUM" <= '2011-12-31' group by "PH_SEQ" having "C" = "B"
// ---------------------------------
	TEST(MY_SQL,
		Insert(TABLE1)(A, 1)(B, 2),
		"insert into `TABLE1`(`A`, `B`) values (1, 2)"); // insert into `TABLE1`(`A`, `B`) values (1, 2)
	TEST(SQLITE3,
		Insert(TABLE1)(A, 1)(B, 2),
		"insert into \"TABLE1\"(\"A\", \"B\") values (1, 2)"); // insert into "TABLE1"("A", "B") values (1, 2)
	TEST(ORACLE,
		Insert(TABLE1)(A, 1)(B, 2),
		"insert into \"TABLE1\"(\"A\", \"B\") values (1, 2)"); // insert into "TABLE1"("A", "B") values (1, 2)
	TEST(MSSQL,
		Insert(TABLE1)(A, 1)(B, 2),
		"insert into \"TABLE1\"(\"A\", \"B\") values (1, 2)"); // insert into "TABLE1"("A", "B") values (1, 2)
	TEST(PGSQL,
		Insert(TABLE1)(A, 1)(B, 2),
		"insert into \"TABLE1\"(\"A\", \"B\") values (1, 2)"); // insert into "TABLE1"("A", "B") values (1, 2)
	TEST(FIREBIRD,
		Insert(TABLE1)(A, 1)(B, 2),
		"insert into \"TABLE1\"(\"A\", \"B\") values (1, 2)"); // insert into "TABLE1"("A", "B") values (1, 2)
	TEST(DB2,
		Insert(TABLE1)(A, 1)(B, 2),
		"insert into \"TABLE1\"(\"A\", \"B\") values (1, 2)"); // insert into "TABLE1"("A", "B") values (1, 2)
// ---------------------------------
	TEST(MY_SQL,
		Insert(PLAT_DTM) (PLAT_DTM_SEQ, NextVal(SEQ_POPLATEK)) (PRIPAD_SEQ, 1111) (DATUM) (CASTKA) (TXT) .From(PLAT_DTM).Where(PRIPAD_SEQ == 2222),
		"insert into `PLAT_DTM`(`PLAT_DTM_SEQ`, `PRIPAD_SEQ`, `DATUM`, `CASTKA`, `TXT`) select `SEQ_POPLATEK`.NEXTVAL, 1111, `DATUM`, `CASTKA`, `TXT` from `PLAT_DTM` where `PRIPAD_SEQ` = 2222"); // insert into `PLAT_DTM`(`PLAT_DTM_SEQ`, `PRIPAD_SEQ`, `DATUM`, `CASTKA`, `TXT`) select `SEQ_POPLATEK`.NEXTVAL, 1111, `DATUM`, `CASTKA`, `TXT` from `PLAT_DTM` where `PRIPAD_SEQ` = 2222
	TEST(SQLITE3,
		Insert(PLAT_DTM) (PLAT_DTM_SEQ, NextVal(SEQ_POPLATEK)) (PRIPAD_SEQ, 1111) (DATUM) (CASTKA) (TXT) .From(PLAT_DTM).Where(PRIPAD_SEQ == 2222),
		"insert into \"PLAT_DTM\"(\"PLAT_DTM_SEQ\", \"PRIPAD_SEQ\", \"DATUM\", \"CASTKA\", \"TXT\") select \"SEQ_POPLATEK\".NEXTVAL, 1111, \"DATUM\", \"CASTKA\", \"TXT\" from \"PLAT_DTM\" where \"PRIPAD_SEQ\" = 2222"); // insert into "PLAT_DTM"("PLAT_DTM_SEQ", "PRIPAD_SEQ", "DATUM", "CASTKA", "TXT") select "SEQ_POPLATEK".NEXTVAL, 1111, "DATUM", "CASTKA", "TXT" from "PLAT_DTM" where "PRIPAD_SEQ" = 2222
	TEST(ORACLE,
		Insert(PLAT_DTM) (PLAT_DTM_SEQ, NextVal(SEQ_POPLATEK)) (PRIPAD_SEQ, 1111) (DATUM) (CASTKA) (TXT) .From(PLAT_DTM).Where(PRIPAD_SEQ == 2222),
		"insert into \"PLAT_DTM\"(\"PLAT_DTM_SEQ\", \"PRIPAD_SEQ\", \"DATUM\", \"CASTKA\", \"TXT\") select \"SEQ_POPLATEK\".NEXTVAL, 1111, \"DATUM\", \"CASTKA\", \"TXT\" from \"PLAT_DTM\" where \"PRIPAD_SEQ\" = 2222"); // insert into "PLAT_DTM"("PLAT_DTM_SEQ", "PRIPAD_SEQ", "DATUM", "CASTKA", "TXT") select "SEQ_POPLATEK".NEXTVAL, 1111, "DATUM", "CASTKA", "TXT" from "PLAT_DTM" where "PRIPAD_SEQ" = 2222
	TEST(MSSQL,
		Insert(PLAT_DTM) (PLAT_DTM_SEQ, NextVal(SEQ_POPLATEK)) (PRIPAD_SEQ, 1111) (DATUM) (CASTKA) (TXT) .From(PLAT_DTM).Where(PRIPAD_SEQ == 2222),
		"insert into \"PLAT_DTM\"(\"PLAT_DTM_SEQ\", \"PRIPAD_SEQ\", \"DATUM\", \"CASTKA\", \"TXT\") select next value for \"SEQ_POPLATEK\", 1111, \"DATUM\", \"CASTKA\", \"TXT\" from \"PLAT_DTM\" where \"PRIPAD_SEQ\" = 2222"); // insert into "PLAT_DTM"("PLAT_DTM_SEQ", "PRIPAD_SEQ", "DATUM", "CASTKA", "TXT") select next value for "SEQ_POPLATEK", 1111, "DATUM", "CASTKA", "TXT" from "PLAT_DTM" where "PRIPAD_SEQ" = 2222
	TEST(PGSQL,
		Insert(PLAT_DTM) (PLAT_DTM_SEQ, NextVal(SEQ_POPLATEK)) (PRIPAD_SEQ, 1111) (DATUM) (CASTKA) (TXT) .From(PLAT_DTM).Where(PRIPAD_SEQ == 2222),
		"insert into \"PLAT_DTM\"(\"PLAT_DTM_SEQ\", \"PRIPAD_SEQ\", \"DATUM\", \"CASTKA\", \"TXT\") select nextval('SEQ_POPLATEK'), 1111, \"DATUM\", \"CASTKA\", \"TXT\" from \"PLAT_DTM\" where \"PRIPAD_SEQ\" = 2222"); // insert into "PLAT_DTM"("PLAT_DTM_SEQ", "PRIPAD_SEQ", "DATUM", "CASTKA", "TXT") select nextval('SEQ_POPLATEK'), 1111, "DATUM", "CASTKA", "TXT" from "PLAT_DTM" where "PRIPAD_SEQ" = 2222
	TEST(FIREBIRD,
		Insert(PLAT_DTM) (PLAT_DTM_SEQ, NextVal(SEQ_POPLATEK)) (PRIPAD_SEQ, 1111) (DATUM) (CASTKA) (TXT) .From(PLAT_DTM).Where(PRIPAD_SEQ == 2222),
		"insert into \"PLAT_DTM\"(\"PLAT_DTM_SEQ\", \"PRIPAD_SEQ\", \"DATUM\", \"CASTKA\", \"TXT\") select \"SEQ_POPLATEK\".NEXTVAL, 1111, \"DATUM\", \"CASTKA\", \"TXT\" from \"PLAT_DTM\" where \"PRIPAD_SEQ\" = 2222"); // insert into "PLAT_DTM"("PLAT_DTM_SEQ", "PRIPAD_SEQ", "DATUM", "CASTKA", "TXT") select "SEQ_POPLATEK".NEXTVAL, 1111, "DATUM", "CASTKA", "TXT" from "PLAT_DTM" where "PRIPAD_SEQ" = 2222
	TEST(DB2,
		Insert(PLAT_DTM) (PLAT_DTM_SEQ, NextVal(SEQ_POPLATEK)) (PRIPAD_SEQ, 1111) (DATUM) (CASTKA) (TXT) .From(PLAT_DTM).Where(PRIPAD_SEQ == 2222),
		"insert into \"PLAT_DTM\"(\"PLAT_DTM_SEQ\", \"PRIPAD_SEQ\", \"DATUM\", \"CASTKA\", \"TXT\") select \"SEQ_POPLATEK\".NEXTVAL, 1111, \"DATUM\", \"CASTKA\", \"TXT\" from \"PLAT_DTM\" where \"PRIPAD_SEQ\" = 2222"); // insert into "PLAT_DTM"("PLAT_DTM_SEQ", "PRIPAD_SEQ", "DATUM", "CASTKA", "TXT") select "SEQ_POPLATEK".NEXTVAL, 1111, "DATUM", "CASTKA", "TXT" from "PLAT_DTM" where "PRIPAD_SEQ" = 2222
// ---------------------------------
	TEST(MY_SQL,
		Select(KATASTR_SEQ).From(ADRESA) .StartWith(PAR_ADRESA_SEQ == 1111) .ConnectBy(Prior(ADRESA_SEQ) == PAR_ADRESA_SEQ) | Select(KATASTR_SEQ).From(ADRESA) .StartWith(ADRESA_SEQ == 2222) .ConnectBy(Prior(PAR_ADRESA_SEQ) == ADRESA_SEQ),
		"((select `KATASTR_SEQ` from `ADRESA` start with `PAR_ADRESA_SEQ` = 1111 connect by prior `ADRESA_SEQ` = `PAR_ADRESA_SEQ`) union (select `KATASTR_SEQ` from `ADRESA` start with `ADRESA_SEQ` = 2222 connect by prior `PAR_ADRESA_SEQ` = `ADRESA_SEQ`))"); // ((select `KATASTR_SEQ` from `ADRESA` start with `PAR_ADRESA_SEQ` = 1111 connect by prior `ADRESA_SEQ` = `PAR_ADRESA_SEQ`) union (select `KATASTR_SEQ` from `ADRESA` start with `ADRESA_SEQ` = 2222 connect by prior `PAR_ADRESA_SEQ` = `ADRESA_SEQ`))
	TEST(SQLITE3,
		Select(KATASTR_SEQ).From(ADRESA) .StartWith(PAR_ADRESA_SEQ == 1111) .ConnectBy(Prior(ADRESA_SEQ) == PAR_ADRESA_SEQ) | Select(KATASTR_SEQ).From(ADRESA) .StartWith(ADRESA_SEQ == 2222) .ConnectBy(Prior(PAR_ADRESA_SEQ) == ADRESA_SEQ),
		"select \"KATASTR_SEQ\" from \"ADRESA\" start with \"PAR_ADRESA_SEQ\" = 1111 connect by prior \"ADRESA_SEQ\" = \"PAR_ADRESA_SEQ\" union select \"KATASTR_SEQ\" from \"ADRESA\" start with \"ADRESA_SEQ\" = 2222 connect by prior \"PAR_ADRESA_SEQ\" = \"ADRESA_SEQ\""); // select "KATASTR_SEQ" from "ADRESA" start with "PAR_ADRESA_SEQ" = 1111 connect by prior "ADRESA_SEQ" = "PAR_ADRESA_SEQ" union select "KATASTR_SEQ" from "ADRESA" start with "ADRESA_SEQ" = 2222 connect by prior "PAR_ADRESA_SEQ" = "ADRESA_SEQ"
	TEST(ORACLE,
		Select(KATASTR_SEQ).From(ADRESA) .StartWith(PAR_ADRESA_SEQ == 1111) .ConnectBy(Prior(ADRESA_SEQ) == PAR_ADRESA_SEQ) | Select(KATASTR_SEQ).From(ADRESA) .StartWith(ADRESA_SEQ == 2222) .ConnectBy(Prior(PAR_ADRESA_SEQ) == ADRESA_SEQ),
		"((select \"KATASTR_SEQ\" from \"ADRESA\" start with \"PAR_ADRESA_SEQ\" = 1111 connect by prior \"ADRESA_SEQ\" = \"PAR_ADRESA_SEQ\") union (select \"KATASTR_SEQ\" from \"ADRESA\" start with \"ADRESA_SEQ\" = 2222 connect by prior \"PAR_ADRESA_SEQ\" = \"ADRESA_SEQ\"))"); // ((select "KATASTR_SEQ" from "ADRESA" start with "PAR_ADRESA_SEQ" = 1111 connect by prior "ADRESA_SEQ" = "PAR_ADRESA_SEQ") union (select "KATASTR_SEQ" from "ADRESA" start with "ADRESA_SEQ" = 2222 connect by prior "PAR_ADRESA_SEQ" = "ADRESA_SEQ"))
	TEST(MSSQL,
		Select(KATASTR_SEQ).From(ADRESA) .StartWith(PAR_ADRESA_SEQ == 1111) .ConnectBy(Prior(ADRESA_SEQ) == PAR_ADRESA_SEQ) | Select(KATASTR_SEQ).From(ADRESA) .StartWith(ADRESA_SEQ == 2222) .ConnectBy(Prior(PAR_ADRESA_SEQ) == ADRESA_SEQ),
		"((select \"KATASTR_SEQ\" from \"ADRESA\" start with \"PAR_ADRESA_SEQ\" = 1111 connect by prior \"ADRESA_SEQ\" = \"PAR_ADRESA_SEQ\") union (select \"KATASTR_SEQ\" from \"ADRESA\" start with \"ADRESA_SEQ\" = 2222 connect by prior \"PAR_ADRESA_SEQ\" = \"ADRESA_SEQ\"))"); // ((select "KATASTR_SEQ" from "ADRESA" start with "PAR_ADRESA_SEQ" = 1111 connect by prior "ADRESA_SEQ" = "PAR_ADRESA_SEQ") union (select "KATASTR_SEQ" from "ADRESA" start with "ADRESA_SEQ" = 2222 connect by prior "PAR_ADRESA_SEQ" = "ADRESA_SEQ"))
	TEST(PGSQL,
		Select(KATASTR_SEQ).From(ADRESA) .StartWith(PAR_ADRESA_SEQ == 1111) .ConnectBy(Prior(ADRESA_SEQ) == PAR_ADRESA_SEQ) | Select(KATASTR_SEQ).From(ADRESA) .StartWith(ADRESA_SEQ == 2222) .ConnectBy(Prior(PAR_ADRESA_SEQ) == ADRESA_SEQ),
		"((select \"KATASTR_SEQ\" from \"ADRESA\" start with \"PAR_ADRESA_SEQ\" = 1111 connect by prior \"ADRESA_SEQ\" = \"PAR_ADRESA_SEQ\") union (select \"KATASTR_SEQ\" from \"ADRESA\" start with \"ADRESA_SEQ\" = 2222 connect by prior \"PAR_ADRESA_SEQ\" = \"ADRESA_SEQ\"))"); // ((select "KATASTR_SEQ" from "ADRESA" start with "PAR_ADRESA_SEQ" = 1111 connect by prior "ADRESA_SEQ" = "PAR_ADRESA_SEQ") union (select "KATASTR_SEQ" from "ADRESA" start with "ADRESA_SEQ" = 2222 connect by prior "PAR_ADRESA_SEQ" = "ADRESA_SEQ"))
	TEST(FIREBIRD,
		Select(KATASTR_SEQ).From(ADRESA) .StartWith(PAR_ADRESA_SEQ == 1111) .ConnectBy(Prior(ADRESA_SEQ) == PAR_ADRESA_SEQ) | Select(KATASTR_SEQ).From(ADRESA) .StartWith(ADRESA_SEQ == 2222) .ConnectBy(Prior(PAR_ADRESA_SEQ) == ADRESA_SEQ),
		"((select \"KATASTR_SEQ\" from \"ADRESA\" start with \"PAR_ADRESA_SEQ\" = 1111 connect by prior \"ADRESA_SEQ\" = \"PAR_ADRESA_SEQ\") union (select \"KATASTR_SEQ\" from \"ADRESA\" start with \"ADRESA_SEQ\" = 2222 connect by prior \"PAR_ADRESA_SEQ\" = \"ADRESA_SEQ\"))"); // ((select "KATASTR_SEQ" from "ADRESA" start with "PAR_ADRESA_SEQ" = 1111 connect by prior "ADRESA_SEQ" = "PAR_ADRESA_SEQ") union (select "KATASTR_SEQ" from "ADRESA" start with "ADRESA_SEQ" = 2222 connect by prior "PAR_ADRESA_SEQ" = "ADRESA_SEQ"))
	TEST(DB2,
		Select(KATASTR_SEQ).From(ADRESA) .StartWith(PAR_ADRESA_SEQ == 1111) .ConnectBy(Prior(ADRESA_SEQ) == PAR_ADRESA_SEQ) | Select(KATASTR_SEQ).From(ADRESA) .StartWith(ADRESA_SEQ == 2222) .ConnectBy(Prior(PAR_ADRESA_SEQ) == ADRESA_SEQ),
		"((select \"KATASTR_SEQ\" from \"ADRESA\" start with \"PAR_ADRESA_SEQ\" = 1111 connect by prior \"ADRESA_SEQ\" = \"PAR_ADRESA_SEQ\") union (select \"KATASTR_SEQ\" from \"ADRESA\" start with \"ADRESA_SEQ\" = 2222 connect by prior \"PAR_ADRESA_SEQ\" = \"ADRESA_SEQ\"))"); // ((select "KATASTR_SEQ" from "ADRESA" start with "PAR_ADRESA_SEQ" = 1111 connect by prior "ADRESA_SEQ" = "PAR_ADRESA_SEQ") union (select "KATASTR_SEQ" from "ADRESA" start with "ADRESA_SEQ" = 2222 connect by prior "PAR_ADRESA_SEQ" = "ADRESA_SEQ"))
// ---------------------------------
	TEST(MY_SQL,
		SqlUpdate(KONTO)(ROK, 1111)(OBEXPBI, 2222).Where(KONTO_CON == 3333),
		"update `KONTO` set `ROK` = 1111, `OBEXPBI` = 2222 where `KONTO_CON` = 3333"); // update `KONTO` set `ROK` = 1111, `OBEXPBI` = 2222 where `KONTO_CON` = 3333
	TEST(SQLITE3,
		SqlUpdate(KONTO)(ROK, 1111)(OBEXPBI, 2222).Where(KONTO_CON == 3333),
		"update \"KONTO\" set \"ROK\" = 1111, \"OBEXPBI\" = 2222 where \"KONTO_CON\" = 3333"); // update "KONTO" set "ROK" = 1111, "OBEXPBI" = 2222 where "KONTO_CON" = 3333
	TEST(ORACLE,
		SqlUpdate(KONTO)(ROK, 1111)(OBEXPBI, 2222).Where(KONTO_CON == 3333),
		"update \"KONTO\" set \"ROK\" = 1111, \"OBEXPBI\" = 2222 where \"KONTO_CON\" = 3333"); // update "KONTO" set "ROK" = 1111, "OBEXPBI" = 2222 where "KONTO_CON" = 3333
	TEST(MSSQL,
		SqlUpdate(KONTO)(ROK, 1111)(OBEXPBI, 2222).Where(KONTO_CON == 3333),
		"update \"KONTO\" set \"ROK\" = 1111, \"OBEXPBI\" = 2222 where \"KONTO_CON\" = 3333"); // update "KONTO" set "ROK" = 1111, "OBEXPBI" = 2222 where "KONTO_CON" = 3333
	TEST(PGSQL,
		SqlUpdate(KONTO)(ROK, 1111)(OBEXPBI, 2222).Where(KONTO_CON == 3333),
		"update \"KONTO\" set \"ROK\" = 1111, \"OBEXPBI\" = 2222 where \"KONTO_CON\" = 3333"); // update "KONTO" set "ROK" = 1111, "OBEXPBI" = 2222 where "KONTO_CON" = 3333
	TEST(FIREBIRD,
		SqlUpdate(KONTO)(ROK, 1111)(OBEXPBI, 2222).Where(KONTO_CON == 3333),
		"update \"KONTO\" set \"ROK\" = 1111, \"OBEXPBI\" = 2222 where \"KONTO_CON\" = 3333"); // update "KONTO" set "ROK" = 1111, "OBEXPBI" = 2222 where "KONTO_CON" = 3333
	TEST(DB2,
		SqlUpdate(KONTO)(ROK, 1111)(OBEXPBI, 2222).Where(KONTO_CON == 3333),
		"update \"KONTO\" set \"ROK\" = 1111, \"OBEXPBI\" = 2222 where \"KONTO_CON\" = 3333"); // update "KONTO" set "ROK" = 1111, "OBEXPBI" = 2222 where "KONTO_CON" = 3333
// ---------------------------------
	TEST(MY_SQL,
		Select(23 * (ROK + 1)).From(TABLE1),
		"select 23 * (`ROK` + 1) from `TABLE1`"); // select 23 * (`ROK` + 1) from `TABLE1`
	TEST(SQLITE3,
		Select(23 * (ROK + 1)).From(TABLE1),
		"select 23 * (\"ROK\" + 1) from \"TABLE1\""); // select 23 * ("ROK" + 1) from "TABLE1"
	TEST(ORACLE,
		Select(23 * (ROK + 1)).From(TABLE1),
		"select 23 * (\"ROK\" + 1) from \"TABLE1\""); // select 23 * ("ROK" + 1) from "TABLE1"
	TEST(MSSQL,
		Select(23 * (ROK + 1)).From(TABLE1),
		"select 23 * (\"ROK\" + 1) from \"TABLE1\""); // select 23 * ("ROK" + 1) from "TABLE1"
	TEST(PGSQL,
		Select(23 * (ROK + 1)).From(TABLE1),
		"select 23 * (\"ROK\" + 1) from \"TABLE1\""); // select 23 * ("ROK" + 1) from "TABLE1"
	TEST(FIREBIRD,
		Select(23 * (ROK + 1)).From(TABLE1),
		"select 23 * (\"ROK\" + 1) from \"TABLE1\""); // select 23 * ("ROK" + 1) from "TABLE1"
	TEST(DB2,
		Select(23 * (ROK + 1)).From(TABLE1),
		"select 23 * (\"ROK\" + 1) from \"TABLE1\""); // select 23 * ("ROK" + 1) from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2),
		"((select `COLUMN1` from `TABLE1`) union (select `COLUMN2` from `TABLE2`))"); // ((select `COLUMN1` from `TABLE1`) union (select `COLUMN2` from `TABLE2`))
	TEST(SQLITE3,
		Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2),
		"select \"COLUMN1\" from \"TABLE1\" union select \"COLUMN2\" from \"TABLE2\""); // select "COLUMN1" from "TABLE1" union select "COLUMN2" from "TABLE2"
	TEST(ORACLE,
		Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2),
		"((select \"COLUMN1\" from \"TABLE1\") union (select \"COLUMN2\" from \"TABLE2\"))"); // ((select "COLUMN1" from "TABLE1") union (select "COLUMN2" from "TABLE2"))
	TEST(MSSQL,
		Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2),
		"((select \"COLUMN1\" from \"TABLE1\") union (select \"COLUMN2\" from \"TABLE2\"))"); // ((select "COLUMN1" from "TABLE1") union (select "COLUMN2" from "TABLE2"))
	TEST(PGSQL,
		Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2),
		"((select \"COLUMN1\" from \"TABLE1\") union (select \"COLUMN2\" from \"TABLE2\"))"); // ((select "COLUMN1" from "TABLE1") union (select "COLUMN2" from "TABLE2"))
	TEST(FIREBIRD,
		Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2),
		"((select \"COLUMN1\" from \"TABLE1\") union (select \"COLUMN2\" from \"TABLE2\"))"); // ((select "COLUMN1" from "TABLE1") union (select "COLUMN2" from "TABLE2"))
	TEST(DB2,
		Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2),
		"((select \"COLUMN1\" from \"TABLE1\") union (select \"COLUMN2\" from \"TABLE2\"))"); // ((select "COLUMN1" from "TABLE1") union (select "COLUMN2" from "TABLE2"))
// ---------------------------------
	TEST(MY_SQL,
		Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2),
		"((select `COLUMN1` from `TABLE1`) intersect (select `COLUMN2` from `TABLE2`))"); // ((select `COLUMN1` from `TABLE1`) intersect (select `COLUMN2` from `TABLE2`))
	TEST(SQLITE3,
		Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2),
		"select \"COLUMN1\" from \"TABLE1\" intersect select \"COLUMN2\" from \"TABLE2\""); // select "COLUMN1" from "TABLE1" intersect select "COLUMN2" from "TABLE2"
	TEST(ORACLE,
		Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2),
		"((select \"COLUMN1\" from \"TABLE1\") intersect (select \"COLUMN2\" from \"TABLE2\"))"); // ((select "COLUMN1" from "TABLE1") intersect (select "COLUMN2" from "TABLE2"))
	TEST(MSSQL,
		Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2),
		"((select \"COLUMN1\" from \"TABLE1\") intersect (select \"COLUMN2\" from \"TABLE2\"))"); // ((select "COLUMN1" from "TABLE1") intersect (select "COLUMN2" from "TABLE2"))
	TEST(PGSQL,
		Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2),
		"((select \"COLUMN1\" from \"TABLE1\") intersect (select \"COLUMN2\" from \"TABLE2\"))"); // ((select "COLUMN1" from "TABLE1") intersect (select "COLUMN2" from "TABLE2"))
	TEST(FIREBIRD,
		Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2),
		"((select \"COLUMN1\" from \"TABLE1\") intersect (select \"COLUMN2\" from \"TABLE2\"))"); // ((select "COLUMN1" from "TABLE1") intersect (select "COLUMN2" from "TABLE2"))
	TEST(DB2,
		Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2),
		"((select \"COLUMN1\" from \"TABLE1\") intersect (select \"COLUMN2\" from \"TABLE2\"))"); // ((select "COLUMN1" from "TABLE1") intersect (select "COLUMN2" from "TABLE2"))
// ---------------------------------
	TEST(MY_SQL,
		Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2),
		"((select `COLUMN1` from `TABLE1`) minus (select `COLUMN2` from `TABLE2`))"); // ((select `COLUMN1` from `TABLE1`) minus (select `COLUMN2` from `TABLE2`))
	TEST(SQLITE3,
		Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2),
		"select \"COLUMN1\" from \"TABLE1\" except select \"COLUMN2\" from \"TABLE2\""); // select "COLUMN1" from "TABLE1" except select "COLUMN2" from "TABLE2"
	TEST(ORACLE,
		Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2),
		"((select \"COLUMN1\" from \"TABLE1\") minus (select \"COLUMN2\" from \"TABLE2\"))"); // ((select "COLUMN1" from "TABLE1") minus (select "COLUMN2" from "TABLE2"))
	TEST(MSSQL,
		Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2),
		"((select \"COLUMN1\" from \"TABLE1\") except (select \"COLUMN2\" from \"TABLE2\"))"); // ((select "COLUMN1" from "TABLE1") except (select "COLUMN2" from "TABLE2"))
	TEST(PGSQL,
		Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2),
		"((select \"COLUMN1\" from \"TABLE1\") except (select \"COLUMN2\" from \"TABLE2\"))"); // ((select "COLUMN1" from "TABLE1") except (select "COLUMN2" from "TABLE2"))
	TEST(FIREBIRD,
		Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2),
		"((select \"COLUMN1\" from \"TABLE1\") minus (select \"COLUMN2\" from \"TABLE2\"))"); // ((select "COLUMN1" from "TABLE1") minus (select "COLUMN2" from "TABLE2"))
	TEST(DB2,
		Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2),
		"((select \"COLUMN1\" from \"TABLE1\") minus (select \"COLUMN2\" from \"TABLE2\"))"); // ((select "COLUMN1" from "TABLE1") minus (select "COLUMN2" from "TABLE2"))
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1),
		"select `COL` from `TABLE1`"); // select `COL` from `TABLE1`
	TEST(SQLITE3,
		Select(COL).From(TABLE1),
		"select \"COL\" from \"TABLE1\""); // select "COL" from "TABLE1"
	TEST(ORACLE,
		Select(COL).From(TABLE1),
		"select \"COL\" from \"TABLE1\""); // select "COL" from "TABLE1"
	TEST(MSSQL,
		Select(COL).From(TABLE1),
		"select \"COL\" from \"TABLE1\""); // select "COL" from "TABLE1"
	TEST(PGSQL,
		Select(COL).From(TABLE1),
		"select \"COL\" from \"TABLE1\""); // select "COL" from "TABLE1"
	TEST(FIREBIRD,
		Select(COL).From(TABLE1),
		"select \"COL\" from \"TABLE1\""); // select "COL" from "TABLE1"
	TEST(DB2,
		Select(COL).From(TABLE1),
		"select \"COL\" from \"TABLE1\""); // select "COL" from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(COL.Of(TABLE1)).From(TABLE1),
		"select `TABLE1`.`COL` from `TABLE1`"); // select `TABLE1`.`COL` from `TABLE1`
	TEST(SQLITE3,
		Select(COL.Of(TABLE1)).From(TABLE1),
		"select \"TABLE1\".\"COL\" from \"TABLE1\""); // select "TABLE1"."COL" from "TABLE1"
	TEST(ORACLE,
		Select(COL.Of(TABLE1)).From(TABLE1),
		"select \"TABLE1\".\"COL\" from \"TABLE1\""); // select "TABLE1"."COL" from "TABLE1"
	TEST(MSSQL,
		Select(COL.Of(TABLE1)).From(TABLE1),
		"select \"TABLE1\".\"COL\" from \"TABLE1\""); // select "TABLE1"."COL" from "TABLE1"
	TEST(PGSQL,
		Select(COL.Of(TABLE1)).From(TABLE1),
		"select \"TABLE1\".\"COL\" from \"TABLE1\""); // select "TABLE1"."COL" from "TABLE1"
	TEST(FIREBIRD,
		Select(COL.Of(TABLE1)).From(TABLE1),
		"select \"TABLE1\".\"COL\" from \"TABLE1\""); // select "TABLE1"."COL" from "TABLE1"
	TEST(DB2,
		Select(COL.Of(TABLE1)).From(TABLE1),
		"select \"TABLE1\".\"COL\" from \"TABLE1\""); // select "TABLE1"."COL" from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(COL.As(A)).From(TABLE1),
		"select `COL` `A` from `TABLE1`"); // select `COL` `A` from `TABLE1`
	TEST(SQLITE3,
		Select(COL.As(A)).From(TABLE1),
		"select \"COL\" \"A\" from \"TABLE1\""); // select "COL" "A" from "TABLE1"
	TEST(ORACLE,
		Select(COL.As(A)).From(TABLE1),
		"select \"COL\" \"A\" from \"TABLE1\""); // select "COL" "A" from "TABLE1"
	TEST(MSSQL,
		Select(COL.As(A)).From(TABLE1),
		"select \"COL\" as \"A\" from \"TABLE1\""); // select "COL" as "A" from "TABLE1"
	TEST(PGSQL,
		Select(COL.As(A)).From(TABLE1),
		"select \"COL\" as \"A\" from \"TABLE1\""); // select "COL" as "A" from "TABLE1"
	TEST(FIREBIRD,
		Select(COL.As(A)).From(TABLE1),
		"select \"COL\" \"A\" from \"TABLE1\""); // select "COL" "A" from "TABLE1"
	TEST(DB2,
		Select(COL.As(A)).From(TABLE1),
		"select \"COL\" \"A\" from \"TABLE1\""); // select "COL" "A" from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(COL&A).From(TABLE1),
		"select `COL$A` from `TABLE1`"); // select `COL$A` from `TABLE1`
	TEST(SQLITE3,
		Select(COL&A).From(TABLE1),
		"select \"COL$A\" from \"TABLE1\""); // select "COL$A" from "TABLE1"
	TEST(ORACLE,
		Select(COL&A).From(TABLE1),
		"select \"COL$A\" from \"TABLE1\""); // select "COL$A" from "TABLE1"
	TEST(MSSQL,
		Select(COL&A).From(TABLE1),
		"select \"COL$A\" from \"TABLE1\""); // select "COL$A" from "TABLE1"
	TEST(PGSQL,
		Select(COL&A).From(TABLE1),
		"select \"COL$A\" from \"TABLE1\""); // select "COL$A" from "TABLE1"
	TEST(FIREBIRD,
		Select(COL&A).From(TABLE1),
		"select \"COL$A\" from \"TABLE1\""); // select "COL$A" from "TABLE1"
	TEST(DB2,
		Select(COL&A).From(TABLE1),
		"select \"COL$A\" from \"TABLE1\""); // select "COL$A" from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(COL[3]).From(TABLE1),
		"select `COL3` from `TABLE1`"); // select `COL3` from `TABLE1`
	TEST(SQLITE3,
		Select(COL[3]).From(TABLE1),
		"select \"COL3\" from \"TABLE1\""); // select "COL3" from "TABLE1"
	TEST(ORACLE,
		Select(COL[3]).From(TABLE1),
		"select \"COL3\" from \"TABLE1\""); // select "COL3" from "TABLE1"
	TEST(MSSQL,
		Select(COL[3]).From(TABLE1),
		"select \"COL3\" from \"TABLE1\""); // select "COL3" from "TABLE1"
	TEST(PGSQL,
		Select(COL[3]).From(TABLE1),
		"select \"COL3\" from \"TABLE1\""); // select "COL3" from "TABLE1"
	TEST(FIREBIRD,
		Select(COL[3]).From(TABLE1),
		"select \"COL3\" from \"TABLE1\""); // select "COL3" from "TABLE1"
	TEST(DB2,
		Select(COL[3]).From(TABLE1),
		"select \"COL3\" from \"TABLE1\""); // select "COL3" from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(COLUMN1 % COLUMN2).From(TABLE1),
		"select mod(`COLUMN1`, `COLUMN2`) from `TABLE1`"); // select mod(`COLUMN1`, `COLUMN2`) from `TABLE1`
	TEST(SQLITE3,
		Select(COLUMN1 % COLUMN2).From(TABLE1),
		"select mod(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select mod("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(ORACLE,
		Select(COLUMN1 % COLUMN2).From(TABLE1),
		"select mod(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select mod("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(MSSQL,
		Select(COLUMN1 % COLUMN2).From(TABLE1),
		"select mod(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select mod("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(PGSQL,
		Select(COLUMN1 % COLUMN2).From(TABLE1),
		"select mod(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select mod("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(FIREBIRD,
		Select(COLUMN1 % COLUMN2).From(TABLE1),
		"select mod(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select mod("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(DB2,
		Select(COLUMN1 % COLUMN2).From(TABLE1),
		"select mod(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select mod("COLUMN1", "COLUMN2") from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(COLUMN1 | COLUMN2).From(TABLE1),
		"select `COLUMN1` + `COLUMN2` from `TABLE1`"); // select `COLUMN1` + `COLUMN2` from `TABLE1`
	TEST(SQLITE3,
		Select(COLUMN1 | COLUMN2).From(TABLE1),
		"select \"COLUMN1\"||\"COLUMN2\" from \"TABLE1\""); // select "COLUMN1"||"COLUMN2" from "TABLE1"
	TEST(ORACLE,
		Select(COLUMN1 | COLUMN2).From(TABLE1),
		"select \"COLUMN1\"||\"COLUMN2\" from \"TABLE1\""); // select "COLUMN1"||"COLUMN2" from "TABLE1"
	TEST(MSSQL,
		Select(COLUMN1 | COLUMN2).From(TABLE1),
		"select \"COLUMN1\" + \"COLUMN2\" from \"TABLE1\""); // select "COLUMN1" + "COLUMN2" from "TABLE1"
	TEST(PGSQL,
		Select(COLUMN1 | COLUMN2).From(TABLE1),
		"select \"COLUMN1\"||\"COLUMN2\" from \"TABLE1\""); // select "COLUMN1"||"COLUMN2" from "TABLE1"
	TEST(FIREBIRD,
		Select(COLUMN1 | COLUMN2).From(TABLE1),
		"select \"COLUMN1\" + \"COLUMN2\" from \"TABLE1\""); // select "COLUMN1" + "COLUMN2" from "TABLE1"
	TEST(DB2,
		Select(COLUMN1 | COLUMN2).From(TABLE1),
		"select \"COLUMN1\" + \"COLUMN2\" from \"TABLE1\""); // select "COLUMN1" + "COLUMN2" from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(SqlFunc("any_fn", COL, 2)).From(TABLE1),
		"select any_fn(`COL`, 2) from `TABLE1`"); // select any_fn(`COL`, 2) from `TABLE1`
	TEST(SQLITE3,
		Select(SqlFunc("any_fn", COL, 2)).From(TABLE1),
		"select any_fn(\"COL\", 2) from \"TABLE1\""); // select any_fn("COL", 2) from "TABLE1"
	TEST(ORACLE,
		Select(SqlFunc("any_fn", COL, 2)).From(TABLE1),
		"select any_fn(\"COL\", 2) from \"TABLE1\""); // select any_fn("COL", 2) from "TABLE1"
	TEST(MSSQL,
		Select(SqlFunc("any_fn", COL, 2)).From(TABLE1),
		"select any_fn(\"COL\", 2) from \"TABLE1\""); // select any_fn("COL", 2) from "TABLE1"
	TEST(PGSQL,
		Select(SqlFunc("any_fn", COL, 2)).From(TABLE1),
		"select any_fn(\"COL\", 2) from \"TABLE1\""); // select any_fn("COL", 2) from "TABLE1"
	TEST(FIREBIRD,
		Select(SqlFunc("any_fn", COL, 2)).From(TABLE1),
		"select any_fn(\"COL\", 2) from \"TABLE1\""); // select any_fn("COL", 2) from "TABLE1"
	TEST(DB2,
		Select(SqlFunc("any_fn", COL, 2)).From(TABLE1),
		"select any_fn(\"COL\", 2) from \"TABLE1\""); // select any_fn("COL", 2) from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Distinct(COL)).From(TABLE1),
		"select distinct `COL` from `TABLE1`"); // select distinct `COL` from `TABLE1`
	TEST(SQLITE3,
		Select(Distinct(COL)).From(TABLE1),
		"select distinct \"COL\" from \"TABLE1\""); // select distinct "COL" from "TABLE1"
	TEST(ORACLE,
		Select(Distinct(COL)).From(TABLE1),
		"select distinct \"COL\" from \"TABLE1\""); // select distinct "COL" from "TABLE1"
	TEST(MSSQL,
		Select(Distinct(COL)).From(TABLE1),
		"select distinct \"COL\" from \"TABLE1\""); // select distinct "COL" from "TABLE1"
	TEST(PGSQL,
		Select(Distinct(COL)).From(TABLE1),
		"select distinct \"COL\" from \"TABLE1\""); // select distinct "COL" from "TABLE1"
	TEST(FIREBIRD,
		Select(Distinct(COL)).From(TABLE1),
		"select distinct \"COL\" from \"TABLE1\""); // select distinct "COL" from "TABLE1"
	TEST(DB2,
		Select(Distinct(COL)).From(TABLE1),
		"select distinct \"COL\" from \"TABLE1\""); // select distinct "COL" from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Distinct(SqlSet(COLUMN1, COLUMN2))).From(TABLE1),
		"select distinct `COLUMN1`, `COLUMN2` from `TABLE1`"); // select distinct `COLUMN1`, `COLUMN2` from `TABLE1`
	TEST(SQLITE3,
		Select(Distinct(SqlSet(COLUMN1, COLUMN2))).From(TABLE1),
		"select distinct \"COLUMN1\", \"COLUMN2\" from \"TABLE1\""); // select distinct "COLUMN1", "COLUMN2" from "TABLE1"
	TEST(ORACLE,
		Select(Distinct(SqlSet(COLUMN1, COLUMN2))).From(TABLE1),
		"select distinct \"COLUMN1\", \"COLUMN2\" from \"TABLE1\""); // select distinct "COLUMN1", "COLUMN2" from "TABLE1"
	TEST(MSSQL,
		Select(Distinct(SqlSet(COLUMN1, COLUMN2))).From(TABLE1),
		"select distinct \"COLUMN1\", \"COLUMN2\" from \"TABLE1\""); // select distinct "COLUMN1", "COLUMN2" from "TABLE1"
	TEST(PGSQL,
		Select(Distinct(SqlSet(COLUMN1, COLUMN2))).From(TABLE1),
		"select distinct \"COLUMN1\", \"COLUMN2\" from \"TABLE1\""); // select distinct "COLUMN1", "COLUMN2" from "TABLE1"
	TEST(FIREBIRD,
		Select(Distinct(SqlSet(COLUMN1, COLUMN2))).From(TABLE1),
		"select distinct \"COLUMN1\", \"COLUMN2\" from \"TABLE1\""); // select distinct "COLUMN1", "COLUMN2" from "TABLE1"
	TEST(DB2,
		Select(Distinct(SqlSet(COLUMN1, COLUMN2))).From(TABLE1),
		"select distinct \"COLUMN1\", \"COLUMN2\" from \"TABLE1\""); // select distinct "COLUMN1", "COLUMN2" from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(All(COL)).From(TABLE1),
		"select all `COL` from `TABLE1`"); // select all `COL` from `TABLE1`
	TEST(SQLITE3,
		Select(All(COL)).From(TABLE1),
		"select all \"COL\" from \"TABLE1\""); // select all "COL" from "TABLE1"
	TEST(ORACLE,
		Select(All(COL)).From(TABLE1),
		"select all \"COL\" from \"TABLE1\""); // select all "COL" from "TABLE1"
	TEST(MSSQL,
		Select(All(COL)).From(TABLE1),
		"select all \"COL\" from \"TABLE1\""); // select all "COL" from "TABLE1"
	TEST(PGSQL,
		Select(All(COL)).From(TABLE1),
		"select all \"COL\" from \"TABLE1\""); // select all "COL" from "TABLE1"
	TEST(FIREBIRD,
		Select(All(COL)).From(TABLE1),
		"select all \"COL\" from \"TABLE1\""); // select all "COL" from "TABLE1"
	TEST(DB2,
		Select(All(COL)).From(TABLE1),
		"select all \"COL\" from \"TABLE1\""); // select all "COL" from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Count(COL)).From(TABLE1),
		"select count(`COL`) from `TABLE1`"); // select count(`COL`) from `TABLE1`
	TEST(SQLITE3,
		Select(Count(COL)).From(TABLE1),
		"select count(\"COL\") from \"TABLE1\""); // select count("COL") from "TABLE1"
	TEST(ORACLE,
		Select(Count(COL)).From(TABLE1),
		"select count(\"COL\") from \"TABLE1\""); // select count("COL") from "TABLE1"
	TEST(MSSQL,
		Select(Count(COL)).From(TABLE1),
		"select count(\"COL\") from \"TABLE1\""); // select count("COL") from "TABLE1"
	TEST(PGSQL,
		Select(Count(COL)).From(TABLE1),
		"select count(\"COL\") from \"TABLE1\""); // select count("COL") from "TABLE1"
	TEST(FIREBIRD,
		Select(Count(COL)).From(TABLE1),
		"select count(\"COL\") from \"TABLE1\""); // select count("COL") from "TABLE1"
	TEST(DB2,
		Select(Count(COL)).From(TABLE1),
		"select count(\"COL\") from \"TABLE1\""); // select count("COL") from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(SqlAll()).From(TABLE1),
		"select * from `TABLE1`"); // select * from `TABLE1`
	TEST(SQLITE3,
		Select(SqlAll()).From(TABLE1),
		"select * from \"TABLE1\""); // select * from "TABLE1"
	TEST(ORACLE,
		Select(SqlAll()).From(TABLE1),
		"select * from \"TABLE1\""); // select * from "TABLE1"
	TEST(MSSQL,
		Select(SqlAll()).From(TABLE1),
		"select * from \"TABLE1\""); // select * from "TABLE1"
	TEST(PGSQL,
		Select(SqlAll()).From(TABLE1),
		"select * from \"TABLE1\""); // select * from "TABLE1"
	TEST(FIREBIRD,
		Select(SqlAll()).From(TABLE1),
		"select * from \"TABLE1\""); // select * from "TABLE1"
	TEST(DB2,
		Select(SqlAll()).From(TABLE1),
		"select * from \"TABLE1\""); // select * from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(SqlCountRows()).From(TABLE1),
		"select count(*) from `TABLE1`"); // select count(*) from `TABLE1`
	TEST(SQLITE3,
		Select(SqlCountRows()).From(TABLE1),
		"select count(*) from \"TABLE1\""); // select count(*) from "TABLE1"
	TEST(ORACLE,
		Select(SqlCountRows()).From(TABLE1),
		"select count(*) from \"TABLE1\""); // select count(*) from "TABLE1"
	TEST(MSSQL,
		Select(SqlCountRows()).From(TABLE1),
		"select count(*) from \"TABLE1\""); // select count(*) from "TABLE1"
	TEST(PGSQL,
		Select(SqlCountRows()).From(TABLE1),
		"select count(*) from \"TABLE1\""); // select count(*) from "TABLE1"
	TEST(FIREBIRD,
		Select(SqlCountRows()).From(TABLE1),
		"select count(*) from \"TABLE1\""); // select count(*) from "TABLE1"
	TEST(DB2,
		Select(SqlCountRows()).From(TABLE1),
		"select count(*) from \"TABLE1\""); // select count(*) from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).OrderBy(Descending(COL)),
		"select `COL` from `TABLE1` order by `COL` desc"); // select `COL` from `TABLE1` order by `COL` desc
	TEST(SQLITE3,
		Select(COL).From(TABLE1).OrderBy(Descending(COL)),
		"select \"COL\" from \"TABLE1\" order by \"COL\" desc"); // select "COL" from "TABLE1" order by "COL" desc
	TEST(ORACLE,
		Select(COL).From(TABLE1).OrderBy(Descending(COL)),
		"select \"COL\" from \"TABLE1\" order by \"COL\" desc"); // select "COL" from "TABLE1" order by "COL" desc
	TEST(MSSQL,
		Select(COL).From(TABLE1).OrderBy(Descending(COL)),
		"select \"COL\" from \"TABLE1\" order by \"COL\" desc"); // select "COL" from "TABLE1" order by "COL" desc
	TEST(PGSQL,
		Select(COL).From(TABLE1).OrderBy(Descending(COL)),
		"select \"COL\" from \"TABLE1\" order by \"COL\" desc"); // select "COL" from "TABLE1" order by "COL" desc
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).OrderBy(Descending(COL)),
		"select \"COL\" from \"TABLE1\" order by \"COL\" desc"); // select "COL" from "TABLE1" order by "COL" desc
	TEST(DB2,
		Select(COL).From(TABLE1).OrderBy(Descending(COL)),
		"select \"COL\" from \"TABLE1\" order by \"COL\" desc"); // select "COL" from "TABLE1" order by "COL" desc
// ---------------------------------
	TEST(MY_SQL,
		Select(SqlMax(COL)).From(TABLE1),
		"select max(`COL`) from `TABLE1`"); // select max(`COL`) from `TABLE1`
	TEST(SQLITE3,
		Select(SqlMax(COL)).From(TABLE1),
		"select max(\"COL\") from \"TABLE1\""); // select max("COL") from "TABLE1"
	TEST(ORACLE,
		Select(SqlMax(COL)).From(TABLE1),
		"select max(\"COL\") from \"TABLE1\""); // select max("COL") from "TABLE1"
	TEST(MSSQL,
		Select(SqlMax(COL)).From(TABLE1),
		"select max(\"COL\") from \"TABLE1\""); // select max("COL") from "TABLE1"
	TEST(PGSQL,
		Select(SqlMax(COL)).From(TABLE1),
		"select max(\"COL\") from \"TABLE1\""); // select max("COL") from "TABLE1"
	TEST(FIREBIRD,
		Select(SqlMax(COL)).From(TABLE1),
		"select max(\"COL\") from \"TABLE1\""); // select max("COL") from "TABLE1"
	TEST(DB2,
		Select(SqlMax(COL)).From(TABLE1),
		"select max(\"COL\") from \"TABLE1\""); // select max("COL") from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(SqlMin(COL)).From(TABLE1),
		"select min(`COL`) from `TABLE1`"); // select min(`COL`) from `TABLE1`
	TEST(SQLITE3,
		Select(SqlMin(COL)).From(TABLE1),
		"select min(\"COL\") from \"TABLE1\""); // select min("COL") from "TABLE1"
	TEST(ORACLE,
		Select(SqlMin(COL)).From(TABLE1),
		"select min(\"COL\") from \"TABLE1\""); // select min("COL") from "TABLE1"
	TEST(MSSQL,
		Select(SqlMin(COL)).From(TABLE1),
		"select min(\"COL\") from \"TABLE1\""); // select min("COL") from "TABLE1"
	TEST(PGSQL,
		Select(SqlMin(COL)).From(TABLE1),
		"select min(\"COL\") from \"TABLE1\""); // select min("COL") from "TABLE1"
	TEST(FIREBIRD,
		Select(SqlMin(COL)).From(TABLE1),
		"select min(\"COL\") from \"TABLE1\""); // select min("COL") from "TABLE1"
	TEST(DB2,
		Select(SqlMin(COL)).From(TABLE1),
		"select min(\"COL\") from \"TABLE1\""); // select min("COL") from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(SqlSum(COL)).From(TABLE1),
		"select sum(`COL`) from `TABLE1`"); // select sum(`COL`) from `TABLE1`
	TEST(SQLITE3,
		Select(SqlSum(COL)).From(TABLE1),
		"select sum(\"COL\") from \"TABLE1\""); // select sum("COL") from "TABLE1"
	TEST(ORACLE,
		Select(SqlSum(COL)).From(TABLE1),
		"select sum(\"COL\") from \"TABLE1\""); // select sum("COL") from "TABLE1"
	TEST(MSSQL,
		Select(SqlSum(COL)).From(TABLE1),
		"select sum(\"COL\") from \"TABLE1\""); // select sum("COL") from "TABLE1"
	TEST(PGSQL,
		Select(SqlSum(COL)).From(TABLE1),
		"select sum(\"COL\") from \"TABLE1\""); // select sum("COL") from "TABLE1"
	TEST(FIREBIRD,
		Select(SqlSum(COL)).From(TABLE1),
		"select sum(\"COL\") from \"TABLE1\""); // select sum("COL") from "TABLE1"
	TEST(DB2,
		Select(SqlSum(COL)).From(TABLE1),
		"select sum(\"COL\") from \"TABLE1\""); // select sum("COL") from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Avg(COL)).From(TABLE1),
		"select avg(`COL`) from `TABLE1`"); // select avg(`COL`) from `TABLE1`
	TEST(SQLITE3,
		Select(Avg(COL)).From(TABLE1),
		"select avg(\"COL\") from \"TABLE1\""); // select avg("COL") from "TABLE1"
	TEST(ORACLE,
		Select(Avg(COL)).From(TABLE1),
		"select avg(\"COL\") from \"TABLE1\""); // select avg("COL") from "TABLE1"
	TEST(MSSQL,
		Select(Avg(COL)).From(TABLE1),
		"select avg(\"COL\") from \"TABLE1\""); // select avg("COL") from "TABLE1"
	TEST(PGSQL,
		Select(Avg(COL)).From(TABLE1),
		"select avg(\"COL\") from \"TABLE1\""); // select avg("COL") from "TABLE1"
	TEST(FIREBIRD,
		Select(Avg(COL)).From(TABLE1),
		"select avg(\"COL\") from \"TABLE1\""); // select avg("COL") from "TABLE1"
	TEST(DB2,
		Select(Avg(COL)).From(TABLE1),
		"select avg(\"COL\") from \"TABLE1\""); // select avg("COL") from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Stddev(COL)).From(TABLE1),
		"select stddev(`COL`) from `TABLE1`"); // select stddev(`COL`) from `TABLE1`
	TEST(SQLITE3,
		Select(Stddev(COL)).From(TABLE1),
		"select stddev(\"COL\") from \"TABLE1\""); // select stddev("COL") from "TABLE1"
	TEST(ORACLE,
		Select(Stddev(COL)).From(TABLE1),
		"select stddev(\"COL\") from \"TABLE1\""); // select stddev("COL") from "TABLE1"
	TEST(MSSQL,
		Select(Stddev(COL)).From(TABLE1),
		"select stddev(\"COL\") from \"TABLE1\""); // select stddev("COL") from "TABLE1"
	TEST(PGSQL,
		Select(Stddev(COL)).From(TABLE1),
		"select stddev(\"COL\") from \"TABLE1\""); // select stddev("COL") from "TABLE1"
	TEST(FIREBIRD,
		Select(Stddev(COL)).From(TABLE1),
		"select stddev(\"COL\") from \"TABLE1\""); // select stddev("COL") from "TABLE1"
	TEST(DB2,
		Select(Stddev(COL)).From(TABLE1),
		"select stddev(\"COL\") from \"TABLE1\""); // select stddev("COL") from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Variance(COL)).From(TABLE1),
		"select variance(`COL`) from `TABLE1`"); // select variance(`COL`) from `TABLE1`
	TEST(SQLITE3,
		Select(Variance(COL)).From(TABLE1),
		"select variance(\"COL\") from \"TABLE1\""); // select variance("COL") from "TABLE1"
	TEST(ORACLE,
		Select(Variance(COL)).From(TABLE1),
		"select variance(\"COL\") from \"TABLE1\""); // select variance("COL") from "TABLE1"
	TEST(MSSQL,
		Select(Variance(COL)).From(TABLE1),
		"select variance(\"COL\") from \"TABLE1\""); // select variance("COL") from "TABLE1"
	TEST(PGSQL,
		Select(Variance(COL)).From(TABLE1),
		"select variance(\"COL\") from \"TABLE1\""); // select variance("COL") from "TABLE1"
	TEST(FIREBIRD,
		Select(Variance(COL)).From(TABLE1),
		"select variance(\"COL\") from \"TABLE1\""); // select variance("COL") from "TABLE1"
	TEST(DB2,
		Select(Variance(COL)).From(TABLE1),
		"select variance(\"COL\") from \"TABLE1\""); // select variance("COL") from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Greatest(COLUMN1, COLUMN2)).From(TABLE1),
		"select greatest(`COLUMN1`, `COLUMN2`) from `TABLE1`"); // select greatest(`COLUMN1`, `COLUMN2`) from `TABLE1`
	TEST(SQLITE3,
		Select(Greatest(COLUMN1, COLUMN2)).From(TABLE1),
		"select max(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select max("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(ORACLE,
		Select(Greatest(COLUMN1, COLUMN2)).From(TABLE1),
		"select greatest(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select greatest("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(MSSQL,
		Select(Greatest(COLUMN1, COLUMN2)).From(TABLE1),
		"select (case when \"COLUMN1\" < \"COLUMN2\" then \"COLUMN2\" else \"COLUMN1\" end) from \"TABLE1\""); // select (case when "COLUMN1" < "COLUMN2" then "COLUMN2" else "COLUMN1" end) from "TABLE1"
	TEST(PGSQL,
		Select(Greatest(COLUMN1, COLUMN2)).From(TABLE1),
		"select greatest(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select greatest("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(FIREBIRD,
		Select(Greatest(COLUMN1, COLUMN2)).From(TABLE1),
		"select greatest(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select greatest("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(DB2,
		Select(Greatest(COLUMN1, COLUMN2)).From(TABLE1),
		"select greatest(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select greatest("COLUMN1", "COLUMN2") from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Least(COLUMN1, COLUMN2)).From(TABLE1),
		"select least(`COLUMN1`, `COLUMN2`) from `TABLE1`"); // select least(`COLUMN1`, `COLUMN2`) from `TABLE1`
	TEST(SQLITE3,
		Select(Least(COLUMN1, COLUMN2)).From(TABLE1),
		"select min(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select min("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(ORACLE,
		Select(Least(COLUMN1, COLUMN2)).From(TABLE1),
		"select least(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select least("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(MSSQL,
		Select(Least(COLUMN1, COLUMN2)).From(TABLE1),
		"select (case when \"COLUMN1\" > \"COLUMN2\" then \"COLUMN2\" else \"COLUMN1\" end) from \"TABLE1\""); // select (case when "COLUMN1" > "COLUMN2" then "COLUMN2" else "COLUMN1" end) from "TABLE1"
	TEST(PGSQL,
		Select(Least(COLUMN1, COLUMN2)).From(TABLE1),
		"select least(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select least("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(FIREBIRD,
		Select(Least(COLUMN1, COLUMN2)).From(TABLE1),
		"select least(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select least("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(DB2,
		Select(Least(COLUMN1, COLUMN2)).From(TABLE1),
		"select least(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select least("COLUMN1", "COLUMN2") from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Upper(COL)).From(TABLE1),
		"select upper(`COL`) from `TABLE1`"); // select upper(`COL`) from `TABLE1`
	TEST(SQLITE3,
		Select(Upper(COL)).From(TABLE1),
		"select upper(\"COL\") from \"TABLE1\""); // select upper("COL") from "TABLE1"
	TEST(ORACLE,
		Select(Upper(COL)).From(TABLE1),
		"select upper(\"COL\") from \"TABLE1\""); // select upper("COL") from "TABLE1"
	TEST(MSSQL,
		Select(Upper(COL)).From(TABLE1),
		"select upper(\"COL\") from \"TABLE1\""); // select upper("COL") from "TABLE1"
	TEST(PGSQL,
		Select(Upper(COL)).From(TABLE1),
		"select upper(\"COL\") from \"TABLE1\""); // select upper("COL") from "TABLE1"
	TEST(FIREBIRD,
		Select(Upper(COL)).From(TABLE1),
		"select upper(\"COL\") from \"TABLE1\""); // select upper("COL") from "TABLE1"
	TEST(DB2,
		Select(Upper(COL)).From(TABLE1),
		"select upper(\"COL\") from \"TABLE1\""); // select upper("COL") from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Lower(COL)).From(TABLE1),
		"select lower(`COL`) from `TABLE1`"); // select lower(`COL`) from `TABLE1`
	TEST(SQLITE3,
		Select(Lower(COL)).From(TABLE1),
		"select lower(\"COL\") from \"TABLE1\""); // select lower("COL") from "TABLE1"
	TEST(ORACLE,
		Select(Lower(COL)).From(TABLE1),
		"select lower(\"COL\") from \"TABLE1\""); // select lower("COL") from "TABLE1"
	TEST(MSSQL,
		Select(Lower(COL)).From(TABLE1),
		"select lower(\"COL\") from \"TABLE1\""); // select lower("COL") from "TABLE1"
	TEST(PGSQL,
		Select(Lower(COL)).From(TABLE1),
		"select lower(\"COL\") from \"TABLE1\""); // select lower("COL") from "TABLE1"
	TEST(FIREBIRD,
		Select(Lower(COL)).From(TABLE1),
		"select lower(\"COL\") from \"TABLE1\""); // select lower("COL") from "TABLE1"
	TEST(DB2,
		Select(Lower(COL)).From(TABLE1),
		"select lower(\"COL\") from \"TABLE1\""); // select lower("COL") from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Instr(COL, "hello")).From(TABLE1),
		"select INSTR(`COL`, 'hello') from `TABLE1`"); // select INSTR(`COL`, 'hello') from `TABLE1`
	TEST(SQLITE3,
		Select(Instr(COL, "hello")).From(TABLE1),
		"select INSTR(\"COL\", 'hello') from \"TABLE1\""); // select INSTR("COL", 'hello') from "TABLE1"
	TEST(ORACLE,
		Select(Instr(COL, "hello")).From(TABLE1),
		"select INSTR(\"COL\", 'hello') from \"TABLE1\""); // select INSTR("COL", 'hello') from "TABLE1"
	TEST(MSSQL,
		Select(Instr(COL, "hello")).From(TABLE1),
		"select INSTR(\"COL\", 'hello') from \"TABLE1\""); // select INSTR("COL", 'hello') from "TABLE1"
	TEST(PGSQL,
		Select(Instr(COL, "hello")).From(TABLE1),
		"select INSTR(\"COL\", E'hello') from \"TABLE1\""); // select INSTR("COL", E'hello') from "TABLE1"
	TEST(FIREBIRD,
		Select(Instr(COL, "hello")).From(TABLE1),
		"select INSTR(\"COL\", 'hello') from \"TABLE1\""); // select INSTR("COL", 'hello') from "TABLE1"
	TEST(DB2,
		Select(Instr(COL, "hello")).From(TABLE1),
		"select INSTR(\"COL\", 'hello') from \"TABLE1\""); // select INSTR("COL", 'hello') from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(SqlNvl(COLUMN1, COLUMN2)).From(TABLE1),
		"select ifnull(`COLUMN1`, `COLUMN2`) from `TABLE1`"); // select ifnull(`COLUMN1`, `COLUMN2`) from `TABLE1`
	TEST(SQLITE3,
		Select(SqlNvl(COLUMN1, COLUMN2)).From(TABLE1),
		"select ifnull(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select ifnull("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(ORACLE,
		Select(SqlNvl(COLUMN1, COLUMN2)).From(TABLE1),
		"select nvl(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select nvl("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(MSSQL,
		Select(SqlNvl(COLUMN1, COLUMN2)).From(TABLE1),
		"select isnull(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select isnull("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(PGSQL,
		Select(SqlNvl(COLUMN1, COLUMN2)).From(TABLE1),
		"select coalesce(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select coalesce("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(FIREBIRD,
		Select(SqlNvl(COLUMN1, COLUMN2)).From(TABLE1),
		"select nvl(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select nvl("COLUMN1", "COLUMN2") from "TABLE1"
	TEST(DB2,
		Select(SqlNvl(COLUMN1, COLUMN2)).From(TABLE1),
		"select nvl(\"COLUMN1\", \"COLUMN2\") from \"TABLE1\""); // select nvl("COLUMN1", "COLUMN2") from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(NextVal(SEQ)).Get(),
		"select `SEQ`.NEXTVAL"); // select `SEQ`.NEXTVAL
	TEST(SQLITE3,
		Select(NextVal(SEQ)).Get(),
		"select \"SEQ\".NEXTVAL"); // select "SEQ".NEXTVAL
	TEST(ORACLE,
		Select(NextVal(SEQ)).Get(),
		"select \"SEQ\".NEXTVAL from DUAL"); // select "SEQ".NEXTVAL from DUAL
	TEST(MSSQL,
		Select(NextVal(SEQ)).Get(),
		"select next value for \"SEQ\""); // select next value for "SEQ"
	TEST(PGSQL,
		Select(NextVal(SEQ)).Get(),
		"select nextval('SEQ')"); // select nextval('SEQ')
	TEST(FIREBIRD,
		Select(NextVal(SEQ)).Get(),
		"select \"SEQ\".NEXTVAL"); // select "SEQ".NEXTVAL
	TEST(DB2,
		Select(NextVal(SEQ)).Get(),
		"select \"SEQ\".NEXTVAL"); // select "SEQ".NEXTVAL
// ---------------------------------
	TEST(MY_SQL,
		Select(CurrVal(SEQ)).Get(),
		"select `SEQ`.CURRVAL"); // select `SEQ`.CURRVAL
	TEST(SQLITE3,
		Select(CurrVal(SEQ)).Get(),
		"select \"SEQ\".CURRVAL"); // select "SEQ".CURRVAL
	TEST(ORACLE,
		Select(CurrVal(SEQ)).Get(),
		"select \"SEQ\".CURRVAL from DUAL"); // select "SEQ".CURRVAL from DUAL
	TEST(MSSQL,
		Select(CurrVal(SEQ)).Get(),
		"select \"SEQ\".CURRVAL"); // select "SEQ".CURRVAL
	TEST(PGSQL,
		Select(CurrVal(SEQ)).Get(),
		"select currval('SEQ')"); // select currval('SEQ')
	TEST(FIREBIRD,
		Select(CurrVal(SEQ)).Get(),
		"select \"SEQ\".CURRVAL"); // select "SEQ".CURRVAL
	TEST(DB2,
		Select(CurrVal(SEQ)).Get(),
		"select \"SEQ\".CURRVAL"); // select "SEQ".CURRVAL
// ---------------------------------
	TEST(MY_SQL,
		Select(SqlArg()).From(TABLE1),
		"select ? from `TABLE1`"); // select ? from `TABLE1`
	TEST(SQLITE3,
		Select(SqlArg()).From(TABLE1),
		"select ? from \"TABLE1\""); // select ? from "TABLE1"
	TEST(ORACLE,
		Select(SqlArg()).From(TABLE1),
		"select ? from \"TABLE1\""); // select ? from "TABLE1"
	TEST(MSSQL,
		Select(SqlArg()).From(TABLE1),
		"select ? from \"TABLE1\""); // select ? from "TABLE1"
	TEST(PGSQL,
		Select(SqlArg()).From(TABLE1),
		"select ? from \"TABLE1\""); // select ? from "TABLE1"
	TEST(FIREBIRD,
		Select(SqlArg()).From(TABLE1),
		"select ? from \"TABLE1\""); // select ? from "TABLE1"
	TEST(DB2,
		Select(SqlArg()).From(TABLE1),
		"select ? from \"TABLE1\""); // select ? from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Coalesce(COLUMN1, "_")).From(TABLE1),
		"select coalesce(`COLUMN1`, '_') from `TABLE1`"); // select coalesce(`COLUMN1`, '_') from `TABLE1`
	TEST(SQLITE3,
		Select(Coalesce(COLUMN1, "_")).From(TABLE1),
		"select coalesce(\"COLUMN1\", '_') from \"TABLE1\""); // select coalesce("COLUMN1", '_') from "TABLE1"
	TEST(ORACLE,
		Select(Coalesce(COLUMN1, "_")).From(TABLE1),
		"select coalesce(\"COLUMN1\", '_') from \"TABLE1\""); // select coalesce("COLUMN1", '_') from "TABLE1"
	TEST(MSSQL,
		Select(Coalesce(COLUMN1, "_")).From(TABLE1),
		"select coalesce(\"COLUMN1\", '_') from \"TABLE1\""); // select coalesce("COLUMN1", '_') from "TABLE1"
	TEST(PGSQL,
		Select(Coalesce(COLUMN1, "_")).From(TABLE1),
		"select coalesce(\"COLUMN1\", E'_') from \"TABLE1\""); // select coalesce("COLUMN1", E'_') from "TABLE1"
	TEST(FIREBIRD,
		Select(Coalesce(COLUMN1, "_")).From(TABLE1),
		"select coalesce(\"COLUMN1\", '_') from \"TABLE1\""); // select coalesce("COLUMN1", '_') from "TABLE1"
	TEST(DB2,
		Select(Coalesce(COLUMN1, "_")).From(TABLE1),
		"select coalesce(\"COLUMN1\", '_') from \"TABLE1\""); // select coalesce("COLUMN1", '_') from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Coalesce(COLUMN1, "_").As("H")).From(TABLE1),
		"select coalesce(`COLUMN1`, '_') `H` from `TABLE1`"); // select coalesce(`COLUMN1`, '_') `H` from `TABLE1`
	TEST(SQLITE3,
		Select(Coalesce(COLUMN1, "_").As("H")).From(TABLE1),
		"select coalesce(\"COLUMN1\", '_') \"H\" from \"TABLE1\""); // select coalesce("COLUMN1", '_') "H" from "TABLE1"
	TEST(ORACLE,
		Select(Coalesce(COLUMN1, "_").As("H")).From(TABLE1),
		"select coalesce(\"COLUMN1\", '_') \"H\" from \"TABLE1\""); // select coalesce("COLUMN1", '_') "H" from "TABLE1"
	TEST(MSSQL,
		Select(Coalesce(COLUMN1, "_").As("H")).From(TABLE1),
		"select coalesce(\"COLUMN1\", '_') as \"H\" from \"TABLE1\""); // select coalesce("COLUMN1", '_') as "H" from "TABLE1"
	TEST(PGSQL,
		Select(Coalesce(COLUMN1, "_").As("H")).From(TABLE1),
		"select coalesce(\"COLUMN1\", E'_') as \"H\" from \"TABLE1\""); // select coalesce("COLUMN1", E'_') as "H" from "TABLE1"
	TEST(FIREBIRD,
		Select(Coalesce(COLUMN1, "_").As("H")).From(TABLE1),
		"select coalesce(\"COLUMN1\", '_') \"H\" from \"TABLE1\""); // select coalesce("COLUMN1", '_') "H" from "TABLE1"
	TEST(DB2,
		Select(Coalesce(COLUMN1, "_").As("H")).From(TABLE1),
		"select coalesce(\"COLUMN1\", '_') \"H\" from \"TABLE1\""); // select coalesce("COLUMN1", '_') "H" from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Coalesce(COLUMN1, "_").As(COLUMN2)).From(TABLE1),
		"select coalesce(`COLUMN1`, '_') `COLUMN2` from `TABLE1`"); // select coalesce(`COLUMN1`, '_') `COLUMN2` from `TABLE1`
	TEST(SQLITE3,
		Select(Coalesce(COLUMN1, "_").As(COLUMN2)).From(TABLE1),
		"select coalesce(\"COLUMN1\", '_') \"COLUMN2\" from \"TABLE1\""); // select coalesce("COLUMN1", '_') "COLUMN2" from "TABLE1"
	TEST(ORACLE,
		Select(Coalesce(COLUMN1, "_").As(COLUMN2)).From(TABLE1),
		"select coalesce(\"COLUMN1\", '_') \"COLUMN2\" from \"TABLE1\""); // select coalesce("COLUMN1", '_') "COLUMN2" from "TABLE1"
	TEST(MSSQL,
		Select(Coalesce(COLUMN1, "_").As(COLUMN2)).From(TABLE1),
		"select coalesce(\"COLUMN1\", '_') as \"COLUMN2\" from \"TABLE1\""); // select coalesce("COLUMN1", '_') as "COLUMN2" from "TABLE1"
	TEST(PGSQL,
		Select(Coalesce(COLUMN1, "_").As(COLUMN2)).From(TABLE1),
		"select coalesce(\"COLUMN1\", E'_') as \"COLUMN2\" from \"TABLE1\""); // select coalesce("COLUMN1", E'_') as "COLUMN2" from "TABLE1"
	TEST(FIREBIRD,
		Select(Coalesce(COLUMN1, "_").As(COLUMN2)).From(TABLE1),
		"select coalesce(\"COLUMN1\", '_') \"COLUMN2\" from \"TABLE1\""); // select coalesce("COLUMN1", '_') "COLUMN2" from "TABLE1"
	TEST(DB2,
		Select(Coalesce(COLUMN1, "_").As(COLUMN2)).From(TABLE1),
		"select coalesce(\"COLUMN1\", '_') \"COLUMN2\" from \"TABLE1\""); // select coalesce("COLUMN1", '_') "COLUMN2" from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Decode(COL, SqlSet("1", 2, 3))).From(TABLE1),
		"select (case when `COL` = '1' then 2 else 3 end) from `TABLE1`"); // select (case when `COL` = '1' then 2 else 3 end) from `TABLE1`
	TEST(SQLITE3,
		Select(Decode(COL, SqlSet("1", 2, 3))).From(TABLE1),
		"select (case when \"COL\" = '1' then 2 else 3 end) from \"TABLE1\""); // select (case when "COL" = '1' then 2 else 3 end) from "TABLE1"
	TEST(ORACLE,
		Select(Decode(COL, SqlSet("1", 2, 3))).From(TABLE1),
		"select decode(\"COL\", '1', 2, 3) from \"TABLE1\""); // select decode("COL", '1', 2, 3) from "TABLE1"
	TEST(MSSQL,
		Select(Decode(COL, SqlSet("1", 2, 3))).From(TABLE1),
		"select (case when \"COL\" = '1' then 2 else 3 end) from \"TABLE1\""); // select (case when "COL" = '1' then 2 else 3 end) from "TABLE1"
	TEST(PGSQL,
		Select(Decode(COL, SqlSet("1", 2, 3))).From(TABLE1),
		"select (case when \"COL\" = E'1' then 2 else 3 end) from \"TABLE1\""); // select (case when "COL" = E'1' then 2 else 3 end) from "TABLE1"
	TEST(FIREBIRD,
		Select(Decode(COL, SqlSet("1", 2, 3))).From(TABLE1),
		"select (case when \"COL\" = '1' then 2 else 3 end) from \"TABLE1\""); // select (case when "COL" = '1' then 2 else 3 end) from "TABLE1"
	TEST(DB2,
		Select(Decode(COL, SqlSet("1", 2, 3))).From(TABLE1),
		"select (case when \"COL\" = '1' then 2 else 3 end) from \"TABLE1\""); // select (case when "COL" = '1' then 2 else 3 end) from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Length(COL)).From(TABLE1),
		"select length(`COL`) from `TABLE1`"); // select length(`COL`) from `TABLE1`
	TEST(SQLITE3,
		Select(Length(COL)).From(TABLE1),
		"select length(\"COL\") from \"TABLE1\""); // select length("COL") from "TABLE1"
	TEST(ORACLE,
		Select(Length(COL)).From(TABLE1),
		"select length(\"COL\") from \"TABLE1\""); // select length("COL") from "TABLE1"
	TEST(MSSQL,
		Select(Length(COL)).From(TABLE1),
		"select len(\"COL\") from \"TABLE1\""); // select len("COL") from "TABLE1"
	TEST(PGSQL,
		Select(Length(COL)).From(TABLE1),
		"select length(\"COL\") from \"TABLE1\""); // select length("COL") from "TABLE1"
	TEST(FIREBIRD,
		Select(Length(COL)).From(TABLE1),
		"select length(\"COL\") from \"TABLE1\""); // select length("COL") from "TABLE1"
	TEST(DB2,
		Select(Length(COL)).From(TABLE1),
		"select length(\"COL\") from \"TABLE1\""); // select length("COL") from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(Substr(COL, 2, 3)).From(TABLE1),
		"select substr(`COL`, 2, 3) from `TABLE1`"); // select substr(`COL`, 2, 3) from `TABLE1`
	TEST(SQLITE3,
		Select(Substr(COL, 2, 3)).From(TABLE1),
		"select substr(\"COL\", 2, 3) from \"TABLE1\""); // select substr("COL", 2, 3) from "TABLE1"
	TEST(ORACLE,
		Select(Substr(COL, 2, 3)).From(TABLE1),
		"select substr(\"COL\", 2, 3) from \"TABLE1\""); // select substr("COL", 2, 3) from "TABLE1"
	TEST(MSSQL,
		Select(Substr(COL, 2, 3)).From(TABLE1),
		"select substring(\"COL\", 2, 3) from \"TABLE1\""); // select substring("COL", 2, 3) from "TABLE1"
	TEST(PGSQL,
		Select(Substr(COL, 2, 3)).From(TABLE1),
		"select substr(\"COL\", 2, 3) from \"TABLE1\""); // select substr("COL", 2, 3) from "TABLE1"
	TEST(FIREBIRD,
		Select(Substr(COL, 2, 3)).From(TABLE1),
		"select substr(\"COL\", 2, 3) from \"TABLE1\""); // select substr("COL", 2, 3) from "TABLE1"
	TEST(DB2,
		Select(Substr(COL, 2, 3)).From(TABLE1),
		"select substr(\"COL\", 2, 3) from \"TABLE1\""); // select substr("COL", 2, 3) from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(SqlDate(COL, SEQ, A)).From(TABLE1),
		"select to_date(to_char(`A`, '00') || to_char(`SEQ`, '00') || to_char(`COL`,'9999'), 'DDMMYYYY') from `TABLE1`"); // select to_date(to_char(`A`, '00') || to_char(`SEQ`, '00') || to_char(`COL`,'9999'), 'DDMMYYYY') from `TABLE1`
	TEST(SQLITE3,
		Select(SqlDate(COL, SEQ, A)).From(TABLE1),
		"select to_date(to_char(\"A\", '00') || to_char(\"SEQ\", '00') || to_char(\"COL\",'9999'), 'DDMMYYYY') from \"TABLE1\""); // select to_date(to_char("A", '00') || to_char("SEQ", '00') || to_char("COL",'9999'), 'DDMMYYYY') from "TABLE1"
	TEST(ORACLE,
		Select(SqlDate(COL, SEQ, A)).From(TABLE1),
		"select to_date(\"COL\"||'.'||\"SEQ\"||'.'||\"A\", 'SYYYY.MM.DD') from \"TABLE1\""); // select to_date("COL"||'.'||"SEQ"||'.'||"A", 'SYYYY.MM.DD') from "TABLE1"
	TEST(MSSQL,
		Select(SqlDate(COL, SEQ, A)).From(TABLE1),
		"select datefromparts(\"COL\", \"SEQ\", \"A\") from \"TABLE1\""); // select datefromparts("COL", "SEQ", "A") from "TABLE1"
	TEST(PGSQL,
		Select(SqlDate(COL, SEQ, A)).From(TABLE1),
		"select to_date(to_char(\"A\", '00') || to_char(\"SEQ\", '00') || to_char(\"COL\",'9999'), 'DDMMYYYY') from \"TABLE1\""); // select to_date(to_char("A", '00') || to_char("SEQ", '00') || to_char("COL",'9999'), 'DDMMYYYY') from "TABLE1"
	TEST(FIREBIRD,
		Select(SqlDate(COL, SEQ, A)).From(TABLE1),
		"select to_date(to_char(\"A\", '00') || to_char(\"SEQ\", '00') || to_char(\"COL\",'9999'), 'DDMMYYYY') from \"TABLE1\""); // select to_date(to_char("A", '00') || to_char("SEQ", '00') || to_char("COL",'9999'), 'DDMMYYYY') from "TABLE1"
	TEST(DB2,
		Select(SqlDate(COL, SEQ, A)).From(TABLE1),
		"select to_date(to_char(\"A\", '00') || to_char(\"SEQ\", '00') || to_char(\"COL\",'9999'), 'DDMMYYYY') from \"TABLE1\""); // select to_date(to_char("A", '00') || to_char("SEQ", '00') || to_char("COL",'9999'), 'DDMMYYYY') from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(COL / 2 > 1 && COLUMN1 == "A" || COLUMN2 == Date(2006, 1, 1)),
		"select `COL` from `TABLE1` where `COL` / 2 > 1 and `COLUMN1` = 'A' or `COLUMN2` = '2006-01-01'"); // select `COL` from `TABLE1` where `COL` / 2 > 1 and `COLUMN1` = 'A' or `COLUMN2` = '2006-01-01'
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(COL / 2 > 1 && COLUMN1 == "A" || COLUMN2 == Date(2006, 1, 1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" / 2 > 1 and \"COLUMN1\" = 'A' or \"COLUMN2\" = '2006-01-01'"); // select "COL" from "TABLE1" where "COL" / 2 > 1 and "COLUMN1" = 'A' or "COLUMN2" = '2006-01-01'
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(COL / 2 > 1 && COLUMN1 == "A" || COLUMN2 == Date(2006, 1, 1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" / 2 > 1 and \"COLUMN1\" = 'A' or \"COLUMN2\" = to_date('2006/1/1', 'SYYYY/MM/DD')"); // select "COL" from "TABLE1" where "COL" / 2 > 1 and "COLUMN1" = 'A' or "COLUMN2" = to_date('2006/1/1', 'SYYYY/MM/DD')
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(COL / 2 > 1 && COLUMN1 == "A" || COLUMN2 == Date(2006, 1, 1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" / 2 > 1 and \"COLUMN1\" = 'A' or \"COLUMN2\" = convert(datetime, '2006/1/1', 120)"); // select "COL" from "TABLE1" where "COL" / 2 > 1 and "COLUMN1" = 'A' or "COLUMN2" = convert(datetime, '2006/1/1', 120)
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(COL / 2 > 1 && COLUMN1 == "A" || COLUMN2 == Date(2006, 1, 1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" / 2 > 1 and \"COLUMN1\" = E'A' or \"COLUMN2\" = date '2006-01-01'"); // select "COL" from "TABLE1" where "COL" / 2 > 1 and "COLUMN1" = E'A' or "COLUMN2" = date '2006-01-01'
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(COL / 2 > 1 && COLUMN1 == "A" || COLUMN2 == Date(2006, 1, 1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" / 2 > 1 and \"COLUMN1\" = 'A' or \"COLUMN2\" = '2006-01-01'"); // select "COL" from "TABLE1" where "COL" / 2 > 1 and "COLUMN1" = 'A' or "COLUMN2" = '2006-01-01'
	TEST(DB2,
		Select(COL).From(TABLE1).Where(COL / 2 > 1 && COLUMN1 == "A" || COLUMN2 == Date(2006, 1, 1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" / 2 > 1 and \"COLUMN1\" = 'A' or \"COLUMN2\" = '2006-01-01'"); // select "COL" from "TABLE1" where "COL" / 2 > 1 and "COLUMN1" = 'A' or "COLUMN2" = '2006-01-01'
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(!(COL == 1)),
		"select `COL` from `TABLE1` where  not (`COL` = 1)"); // select `COL` from `TABLE1` where  not (`COL` = 1)
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(!(COL == 1)),
		"select \"COL\" from \"TABLE1\" where  not (\"COL\" = 1)"); // select "COL" from "TABLE1" where  not ("COL" = 1)
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(!(COL == 1)),
		"select \"COL\" from \"TABLE1\" where  not (\"COL\" = 1)"); // select "COL" from "TABLE1" where  not ("COL" = 1)
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(!(COL == 1)),
		"select \"COL\" from \"TABLE1\" where  not (\"COL\" = 1)"); // select "COL" from "TABLE1" where  not ("COL" = 1)
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(!(COL == 1)),
		"select \"COL\" from \"TABLE1\" where  not (\"COL\" = 1)"); // select "COL" from "TABLE1" where  not ("COL" = 1)
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(!(COL == 1)),
		"select \"COL\" from \"TABLE1\" where  not (\"COL\" = 1)"); // select "COL" from "TABLE1" where  not ("COL" = 1)
	TEST(DB2,
		Select(COL).From(TABLE1).Where(!(COL == 1)),
		"select \"COL\" from \"TABLE1\" where  not (\"COL\" = 1)"); // select "COL" from "TABLE1" where  not ("COL" = 1)
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where((COLUMN1 == 1) - (COLUMN2 == 1)),
		"select `COL` from `TABLE1` where `COLUMN1` = 1 and not `COLUMN2` = 1"); // select `COL` from `TABLE1` where `COLUMN1` = 1 and not `COLUMN2` = 1
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where((COLUMN1 == 1) - (COLUMN2 == 1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" = 1 and not \"COLUMN2\" = 1"); // select "COL" from "TABLE1" where "COLUMN1" = 1 and not "COLUMN2" = 1
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where((COLUMN1 == 1) - (COLUMN2 == 1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" = 1 and not \"COLUMN2\" = 1"); // select "COL" from "TABLE1" where "COLUMN1" = 1 and not "COLUMN2" = 1
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where((COLUMN1 == 1) - (COLUMN2 == 1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" = 1 and not \"COLUMN2\" = 1"); // select "COL" from "TABLE1" where "COLUMN1" = 1 and not "COLUMN2" = 1
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where((COLUMN1 == 1) - (COLUMN2 == 1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" = 1 and not \"COLUMN2\" = 1"); // select "COL" from "TABLE1" where "COLUMN1" = 1 and not "COLUMN2" = 1
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where((COLUMN1 == 1) - (COLUMN2 == 1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" = 1 and not \"COLUMN2\" = 1"); // select "COL" from "TABLE1" where "COLUMN1" = 1 and not "COLUMN2" = 1
	TEST(DB2,
		Select(COL).From(TABLE1).Where((COLUMN1 == 1) - (COLUMN2 == 1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" = 1 and not \"COLUMN2\" = 1"); // select "COL" from "TABLE1" where "COLUMN1" = 1 and not "COLUMN2" = 1
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(IsNull(COLUMN1)),
		"select `COL` from `TABLE1` where `COLUMN1` is NULL"); // select `COL` from `TABLE1` where `COLUMN1` is NULL
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(IsNull(COLUMN1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" is NULL"); // select "COL" from "TABLE1" where "COLUMN1" is NULL
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(IsNull(COLUMN1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" is NULL"); // select "COL" from "TABLE1" where "COLUMN1" is NULL
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(IsNull(COLUMN1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" is NULL"); // select "COL" from "TABLE1" where "COLUMN1" is NULL
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(IsNull(COLUMN1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" is NULL"); // select "COL" from "TABLE1" where "COLUMN1" is NULL
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(IsNull(COLUMN1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" is NULL"); // select "COL" from "TABLE1" where "COLUMN1" is NULL
	TEST(DB2,
		Select(COL).From(TABLE1).Where(IsNull(COLUMN1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" is NULL"); // select "COL" from "TABLE1" where "COLUMN1" is NULL
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(NotNull(COLUMN1)),
		"select `COL` from `TABLE1` where `COLUMN1` is not NULL"); // select `COL` from `TABLE1` where `COLUMN1` is not NULL
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(NotNull(COLUMN1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" is not NULL"); // select "COL" from "TABLE1" where "COLUMN1" is not NULL
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(NotNull(COLUMN1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" is not NULL"); // select "COL" from "TABLE1" where "COLUMN1" is not NULL
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(NotNull(COLUMN1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" is not NULL"); // select "COL" from "TABLE1" where "COLUMN1" is not NULL
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(NotNull(COLUMN1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" is not NULL"); // select "COL" from "TABLE1" where "COLUMN1" is not NULL
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(NotNull(COLUMN1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" is not NULL"); // select "COL" from "TABLE1" where "COLUMN1" is not NULL
	TEST(DB2,
		Select(COL).From(TABLE1).Where(NotNull(COLUMN1)),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" is not NULL"); // select "COL" from "TABLE1" where "COLUMN1" is not NULL
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(Like(COLUMN1, Wild("A*"))),
		"select `COL` from `TABLE1` where `COLUMN1` like binary 'A%%'"); // select `COL` from `TABLE1` where `COLUMN1` like binary 'A%%'
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(Like(COLUMN1, Wild("A*"))),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" like 'A%%'"); // select "COL" from "TABLE1" where "COLUMN1" like 'A%%'
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(Like(COLUMN1, Wild("A*"))),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" like 'A%%'"); // select "COL" from "TABLE1" where "COLUMN1" like 'A%%'
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(Like(COLUMN1, Wild("A*"))),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" like 'A%%'"); // select "COL" from "TABLE1" where "COLUMN1" like 'A%%'
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(Like(COLUMN1, Wild("A*"))),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" like E'A%%'"); // select "COL" from "TABLE1" where "COLUMN1" like E'A%%'
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(Like(COLUMN1, Wild("A*"))),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" like 'A%%'"); // select "COL" from "TABLE1" where "COLUMN1" like 'A%%'
	TEST(DB2,
		Select(COL).From(TABLE1).Where(Like(COLUMN1, Wild("A*"))),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" like 'A%%'"); // select "COL" from "TABLE1" where "COLUMN1" like 'A%%'
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(NotLike(COLUMN1, Wild("A*"))),
		"select `COL` from `TABLE1` where `COLUMN1` not like 'A%%'"); // select `COL` from `TABLE1` where `COLUMN1` not like 'A%%'
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(NotLike(COLUMN1, Wild("A*"))),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" not like 'A%%'"); // select "COL" from "TABLE1" where "COLUMN1" not like 'A%%'
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(NotLike(COLUMN1, Wild("A*"))),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" not like 'A%%'"); // select "COL" from "TABLE1" where "COLUMN1" not like 'A%%'
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(NotLike(COLUMN1, Wild("A*"))),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" not like 'A%%'"); // select "COL" from "TABLE1" where "COLUMN1" not like 'A%%'
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(NotLike(COLUMN1, Wild("A*"))),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" not like E'A%%'"); // select "COL" from "TABLE1" where "COLUMN1" not like E'A%%'
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(NotLike(COLUMN1, Wild("A*"))),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" not like 'A%%'"); // select "COL" from "TABLE1" where "COLUMN1" not like 'A%%'
	TEST(DB2,
		Select(COL).From(TABLE1).Where(NotLike(COLUMN1, Wild("A*"))),
		"select \"COL\" from \"TABLE1\" where \"COLUMN1\" not like 'A%%'"); // select "COL" from "TABLE1" where "COLUMN1" not like 'A%%'
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(In(COL, Select(COL).From(TABLE1))),
		"select `COL` from `TABLE1` where `COL` in (select `COL` from `TABLE1`)"); // select `COL` from `TABLE1` where `COL` in (select `COL` from `TABLE1`)
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(In(COL, Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" in (select "COL" from "TABLE1")
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(In(COL, Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" in (select "COL" from "TABLE1")
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(In(COL, Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" in (select "COL" from "TABLE1")
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(In(COL, Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" in (select "COL" from "TABLE1")
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(In(COL, Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" in (select "COL" from "TABLE1")
	TEST(DB2,
		Select(COL).From(TABLE1).Where(In(COL, Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" in (select "COL" from "TABLE1")
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(COL == Select(COL).From(TABLE1)),
		"select `COL` from `TABLE1` where `COL` in (select `COL` from `TABLE1`)"); // select `COL` from `TABLE1` where `COL` in (select `COL` from `TABLE1`)
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(COL == Select(COL).From(TABLE1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" in (select "COL" from "TABLE1")
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(COL == Select(COL).From(TABLE1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" in (select "COL" from "TABLE1")
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(COL == Select(COL).From(TABLE1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" in (select "COL" from "TABLE1")
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(COL == Select(COL).From(TABLE1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" in (select "COL" from "TABLE1")
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(COL == Select(COL).From(TABLE1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" in (select "COL" from "TABLE1")
	TEST(DB2,
		Select(COL).From(TABLE1).Where(COL == Select(COL).From(TABLE1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" in (select "COL" from "TABLE1")
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(COL == Select(COL).From(TABLE1).AsValue()),
		"select `COL` from `TABLE1` where `COL` = ((select `COL` from `TABLE1`))"); // select `COL` from `TABLE1` where `COL` = ((select `COL` from `TABLE1`))
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(COL == Select(COL).From(TABLE1).AsValue()),
		"select \"COL\" from \"TABLE1\" where \"COL\" = ((select \"COL\" from \"TABLE1\"))"); // select "COL" from "TABLE1" where "COL" = ((select "COL" from "TABLE1"))
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(COL == Select(COL).From(TABLE1).AsValue()),
		"select \"COL\" from \"TABLE1\" where \"COL\" = ((select \"COL\" from \"TABLE1\"))"); // select "COL" from "TABLE1" where "COL" = ((select "COL" from "TABLE1"))
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(COL == Select(COL).From(TABLE1).AsValue()),
		"select \"COL\" from \"TABLE1\" where \"COL\" = ((select \"COL\" from \"TABLE1\"))"); // select "COL" from "TABLE1" where "COL" = ((select "COL" from "TABLE1"))
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(COL == Select(COL).From(TABLE1).AsValue()),
		"select \"COL\" from \"TABLE1\" where \"COL\" = ((select \"COL\" from \"TABLE1\"))"); // select "COL" from "TABLE1" where "COL" = ((select "COL" from "TABLE1"))
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(COL == Select(COL).From(TABLE1).AsValue()),
		"select \"COL\" from \"TABLE1\" where \"COL\" = ((select \"COL\" from \"TABLE1\"))"); // select "COL" from "TABLE1" where "COL" = ((select "COL" from "TABLE1"))
	TEST(DB2,
		Select(COL).From(TABLE1).Where(COL == Select(COL).From(TABLE1).AsValue()),
		"select \"COL\" from \"TABLE1\" where \"COL\" = ((select \"COL\" from \"TABLE1\"))"); // select "COL" from "TABLE1" where "COL" = ((select "COL" from "TABLE1"))
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(NotIn(COL, Select(COL).From(TABLE1))),
		"select `COL` from `TABLE1` where `COL` not in (select `COL` from `TABLE1`)"); // select `COL` from `TABLE1` where `COL` not in (select `COL` from `TABLE1`)
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(NotIn(COL, Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where \"COL\" not in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" not in (select "COL" from "TABLE1")
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(NotIn(COL, Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where \"COL\" not in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" not in (select "COL" from "TABLE1")
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(NotIn(COL, Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where \"COL\" not in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" not in (select "COL" from "TABLE1")
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(NotIn(COL, Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where \"COL\" not in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" not in (select "COL" from "TABLE1")
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(NotIn(COL, Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where \"COL\" not in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" not in (select "COL" from "TABLE1")
	TEST(DB2,
		Select(COL).From(TABLE1).Where(NotIn(COL, Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where \"COL\" not in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" not in (select "COL" from "TABLE1")
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(COL != Select(COL).From(TABLE1)),
		"select `COL` from `TABLE1` where `COL` not in (select `COL` from `TABLE1`)"); // select `COL` from `TABLE1` where `COL` not in (select `COL` from `TABLE1`)
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(COL != Select(COL).From(TABLE1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" not in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" not in (select "COL" from "TABLE1")
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(COL != Select(COL).From(TABLE1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" not in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" not in (select "COL" from "TABLE1")
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(COL != Select(COL).From(TABLE1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" not in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" not in (select "COL" from "TABLE1")
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(COL != Select(COL).From(TABLE1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" not in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" not in (select "COL" from "TABLE1")
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(COL != Select(COL).From(TABLE1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" not in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" not in (select "COL" from "TABLE1")
	TEST(DB2,
		Select(COL).From(TABLE1).Where(COL != Select(COL).From(TABLE1)),
		"select \"COL\" from \"TABLE1\" where \"COL\" not in (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where "COL" not in (select "COL" from "TABLE1")
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(Exists(Select(COL).From(TABLE1))),
		"select `COL` from `TABLE1` where exists (select `COL` from `TABLE1`)"); // select `COL` from `TABLE1` where exists (select `COL` from `TABLE1`)
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(Exists(Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where exists (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where exists (select "COL" from "TABLE1")
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(Exists(Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where exists (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where exists (select "COL" from "TABLE1")
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(Exists(Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where exists (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where exists (select "COL" from "TABLE1")
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(Exists(Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where exists (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where exists (select "COL" from "TABLE1")
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(Exists(Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where exists (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where exists (select "COL" from "TABLE1")
	TEST(DB2,
		Select(COL).From(TABLE1).Where(Exists(Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where exists (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where exists (select "COL" from "TABLE1")
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(NotExists(Select(COL).From(TABLE1))),
		"select `COL` from `TABLE1` where not exists (select `COL` from `TABLE1`)"); // select `COL` from `TABLE1` where not exists (select `COL` from `TABLE1`)
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(NotExists(Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where not exists (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where not exists (select "COL" from "TABLE1")
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(NotExists(Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where not exists (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where not exists (select "COL" from "TABLE1")
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(NotExists(Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where not exists (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where not exists (select "COL" from "TABLE1")
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(NotExists(Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where not exists (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where not exists (select "COL" from "TABLE1")
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(NotExists(Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where not exists (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where not exists (select "COL" from "TABLE1")
	TEST(DB2,
		Select(COL).From(TABLE1).Where(NotExists(Select(COL).From(TABLE1))),
		"select \"COL\" from \"TABLE1\" where not exists (select \"COL\" from \"TABLE1\")"); // select "COL" from "TABLE1" where not exists (select "COL" from "TABLE1")
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2))),
		"select `COL` from `TABLE1` where `COL` in (((select `COLUMN1` from `TABLE1`) union (select `COLUMN2` from `TABLE2`)))"); // select `COL` from `TABLE1` where `COL` in (((select `COLUMN1` from `TABLE1`) union (select `COLUMN2` from `TABLE2`)))
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (select \"COLUMN1\" from \"TABLE1\" union select \"COLUMN2\" from \"TABLE2\")"); // select "COL" from "TABLE1" where "COL" in (select "COLUMN1" from "TABLE1" union select "COLUMN2" from "TABLE2")
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (((select \"COLUMN1\" from \"TABLE1\") union (select \"COLUMN2\" from \"TABLE2\")))"); // select "COL" from "TABLE1" where "COL" in (((select "COLUMN1" from "TABLE1") union (select "COLUMN2" from "TABLE2")))
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (((select \"COLUMN1\" from \"TABLE1\") union (select \"COLUMN2\" from \"TABLE2\")))"); // select "COL" from "TABLE1" where "COL" in (((select "COLUMN1" from "TABLE1") union (select "COLUMN2" from "TABLE2")))
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (((select \"COLUMN1\" from \"TABLE1\") union (select \"COLUMN2\" from \"TABLE2\")))"); // select "COL" from "TABLE1" where "COL" in (((select "COLUMN1" from "TABLE1") union (select "COLUMN2" from "TABLE2")))
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (((select \"COLUMN1\" from \"TABLE1\") union (select \"COLUMN2\" from \"TABLE2\")))"); // select "COL" from "TABLE1" where "COL" in (((select "COLUMN1" from "TABLE1") union (select "COLUMN2" from "TABLE2")))
	TEST(DB2,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) | Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (((select \"COLUMN1\" from \"TABLE1\") union (select \"COLUMN2\" from \"TABLE2\")))"); // select "COL" from "TABLE1" where "COL" in (((select "COLUMN1" from "TABLE1") union (select "COLUMN2" from "TABLE2")))
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2))),
		"select `COL` from `TABLE1` where `COL` in (((select `COLUMN1` from `TABLE1`) intersect (select `COLUMN2` from `TABLE2`)))"); // select `COL` from `TABLE1` where `COL` in (((select `COLUMN1` from `TABLE1`) intersect (select `COLUMN2` from `TABLE2`)))
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (select \"COLUMN1\" from \"TABLE1\" intersect select \"COLUMN2\" from \"TABLE2\")"); // select "COL" from "TABLE1" where "COL" in (select "COLUMN1" from "TABLE1" intersect select "COLUMN2" from "TABLE2")
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (((select \"COLUMN1\" from \"TABLE1\") intersect (select \"COLUMN2\" from \"TABLE2\")))"); // select "COL" from "TABLE1" where "COL" in (((select "COLUMN1" from "TABLE1") intersect (select "COLUMN2" from "TABLE2")))
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (((select \"COLUMN1\" from \"TABLE1\") intersect (select \"COLUMN2\" from \"TABLE2\")))"); // select "COL" from "TABLE1" where "COL" in (((select "COLUMN1" from "TABLE1") intersect (select "COLUMN2" from "TABLE2")))
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (((select \"COLUMN1\" from \"TABLE1\") intersect (select \"COLUMN2\" from \"TABLE2\")))"); // select "COL" from "TABLE1" where "COL" in (((select "COLUMN1" from "TABLE1") intersect (select "COLUMN2" from "TABLE2")))
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (((select \"COLUMN1\" from \"TABLE1\") intersect (select \"COLUMN2\" from \"TABLE2\")))"); // select "COL" from "TABLE1" where "COL" in (((select "COLUMN1" from "TABLE1") intersect (select "COLUMN2" from "TABLE2")))
	TEST(DB2,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) & Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (((select \"COLUMN1\" from \"TABLE1\") intersect (select \"COLUMN2\" from \"TABLE2\")))"); // select "COL" from "TABLE1" where "COL" in (((select "COLUMN1" from "TABLE1") intersect (select "COLUMN2" from "TABLE2")))
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2))),
		"select `COL` from `TABLE1` where `COL` in (((select `COLUMN1` from `TABLE1`) minus (select `COLUMN2` from `TABLE2`)))"); // select `COL` from `TABLE1` where `COL` in (((select `COLUMN1` from `TABLE1`) minus (select `COLUMN2` from `TABLE2`)))
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (select \"COLUMN1\" from \"TABLE1\" except select \"COLUMN2\" from \"TABLE2\")"); // select "COL" from "TABLE1" where "COL" in (select "COLUMN1" from "TABLE1" except select "COLUMN2" from "TABLE2")
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (((select \"COLUMN1\" from \"TABLE1\") minus (select \"COLUMN2\" from \"TABLE2\")))"); // select "COL" from "TABLE1" where "COL" in (((select "COLUMN1" from "TABLE1") minus (select "COLUMN2" from "TABLE2")))
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (((select \"COLUMN1\" from \"TABLE1\") except (select \"COLUMN2\" from \"TABLE2\")))"); // select "COL" from "TABLE1" where "COL" in (((select "COLUMN1" from "TABLE1") except (select "COLUMN2" from "TABLE2")))
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (((select \"COLUMN1\" from \"TABLE1\") except (select \"COLUMN2\" from \"TABLE2\")))"); // select "COL" from "TABLE1" where "COL" in (((select "COLUMN1" from "TABLE1") except (select "COLUMN2" from "TABLE2")))
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (((select \"COLUMN1\" from \"TABLE1\") minus (select \"COLUMN2\" from \"TABLE2\")))"); // select "COL" from "TABLE1" where "COL" in (((select "COLUMN1" from "TABLE1") minus (select "COLUMN2" from "TABLE2")))
	TEST(DB2,
		Select(COL).From(TABLE1).Where(COL == (Select(COLUMN1).From(TABLE1) - Select(COLUMN2).From(TABLE2))),
		"select \"COL\" from \"TABLE1\" where \"COL\" in (((select \"COLUMN1\" from \"TABLE1\") minus (select \"COLUMN2\" from \"TABLE2\")))"); // select "COL" from "TABLE1" where "COL" in (((select "COLUMN1" from "TABLE1") minus (select "COLUMN2" from "TABLE2")))
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Where(COL == 0).GroupBy(COL).Having(COL == 0).OrderBy(Descending(COL)),
		"select `COL` from `TABLE1` where `COL` = 0 group by `COL` having `COL` = 0 order by `COL` desc"); // select `COL` from `TABLE1` where `COL` = 0 group by `COL` having `COL` = 0 order by `COL` desc
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Where(COL == 0).GroupBy(COL).Having(COL == 0).OrderBy(Descending(COL)),
		"select \"COL\" from \"TABLE1\" where \"COL\" = 0 group by \"COL\" having \"COL\" = 0 order by \"COL\" desc"); // select "COL" from "TABLE1" where "COL" = 0 group by "COL" having "COL" = 0 order by "COL" desc
	TEST(ORACLE,
		Select(COL).From(TABLE1).Where(COL == 0).GroupBy(COL).Having(COL == 0).OrderBy(Descending(COL)),
		"select \"COL\" from \"TABLE1\" where \"COL\" = 0 group by \"COL\" having \"COL\" = 0 order by \"COL\" desc"); // select "COL" from "TABLE1" where "COL" = 0 group by "COL" having "COL" = 0 order by "COL" desc
	TEST(MSSQL,
		Select(COL).From(TABLE1).Where(COL == 0).GroupBy(COL).Having(COL == 0).OrderBy(Descending(COL)),
		"select \"COL\" from \"TABLE1\" where \"COL\" = 0 group by \"COL\" having \"COL\" = 0 order by \"COL\" desc"); // select "COL" from "TABLE1" where "COL" = 0 group by "COL" having "COL" = 0 order by "COL" desc
	TEST(PGSQL,
		Select(COL).From(TABLE1).Where(COL == 0).GroupBy(COL).Having(COL == 0).OrderBy(Descending(COL)),
		"select \"COL\" from \"TABLE1\" where \"COL\" = 0 group by \"COL\" having \"COL\" = 0 order by \"COL\" desc"); // select "COL" from "TABLE1" where "COL" = 0 group by "COL" having "COL" = 0 order by "COL" desc
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Where(COL == 0).GroupBy(COL).Having(COL == 0).OrderBy(Descending(COL)),
		"select \"COL\" from \"TABLE1\" where \"COL\" = 0 group by \"COL\" having \"COL\" = 0 order by \"COL\" desc"); // select "COL" from "TABLE1" where "COL" = 0 group by "COL" having "COL" = 0 order by "COL" desc
	TEST(DB2,
		Select(COL).From(TABLE1).Where(COL == 0).GroupBy(COL).Having(COL == 0).OrderBy(Descending(COL)),
		"select \"COL\" from \"TABLE1\" where \"COL\" = 0 group by \"COL\" having \"COL\" = 0 order by \"COL\" desc"); // select "COL" from "TABLE1" where "COL" = 0 group by "COL" having "COL" = 0 order by "COL" desc
// ---------------------------------
	TEST(MY_SQL,
		Select(COL, Count(Select(SqlAll()).From(TABLE1))).From(TABLE1).Where(COL == 0).GroupBy(COL) .Having(COL == 0).OrderBy(Descending(COL)),
		"select `COL`, count(select * from `TABLE1`) from `TABLE1` where `COL` = 0 group by `COL` having `COL` = 0 order by `COL` desc"); // select `COL`, count(select * from `TABLE1`) from `TABLE1` where `COL` = 0 group by `COL` having `COL` = 0 order by `COL` desc
	TEST(SQLITE3,
		Select(COL, Count(Select(SqlAll()).From(TABLE1))).From(TABLE1).Where(COL == 0).GroupBy(COL) .Having(COL == 0).OrderBy(Descending(COL)),
		"select \"COL\", count(select * from \"TABLE1\") from \"TABLE1\" where \"COL\" = 0 group by \"COL\" having \"COL\" = 0 order by \"COL\" desc"); // select "COL", count(select * from "TABLE1") from "TABLE1" where "COL" = 0 group by "COL" having "COL" = 0 order by "COL" desc
	TEST(ORACLE,
		Select(COL, Count(Select(SqlAll()).From(TABLE1))).From(TABLE1).Where(COL == 0).GroupBy(COL) .Having(COL == 0).OrderBy(Descending(COL)),
		"select \"COL\", count(select * from \"TABLE1\") from \"TABLE1\" where \"COL\" = 0 group by \"COL\" having \"COL\" = 0 order by \"COL\" desc"); // select "COL", count(select * from "TABLE1") from "TABLE1" where "COL" = 0 group by "COL" having "COL" = 0 order by "COL" desc
	TEST(MSSQL,
		Select(COL, Count(Select(SqlAll()).From(TABLE1))).From(TABLE1).Where(COL == 0).GroupBy(COL) .Having(COL == 0).OrderBy(Descending(COL)),
		"select \"COL\", count(select * from \"TABLE1\") from \"TABLE1\" where \"COL\" = 0 group by \"COL\" having \"COL\" = 0 order by \"COL\" desc"); // select "COL", count(select * from "TABLE1") from "TABLE1" where "COL" = 0 group by "COL" having "COL" = 0 order by "COL" desc
	TEST(PGSQL,
		Select(COL, Count(Select(SqlAll()).From(TABLE1))).From(TABLE1).Where(COL == 0).GroupBy(COL) .Having(COL == 0).OrderBy(Descending(COL)),
		"select \"COL\", count(select * from \"TABLE1\") from \"TABLE1\" where \"COL\" = 0 group by \"COL\" having \"COL\" = 0 order by \"COL\" desc"); // select "COL", count(select * from "TABLE1") from "TABLE1" where "COL" = 0 group by "COL" having "COL" = 0 order by "COL" desc
	TEST(FIREBIRD,
		Select(COL, Count(Select(SqlAll()).From(TABLE1))).From(TABLE1).Where(COL == 0).GroupBy(COL) .Having(COL == 0).OrderBy(Descending(COL)),
		"select \"COL\", count(select * from \"TABLE1\") from \"TABLE1\" where \"COL\" = 0 group by \"COL\" having \"COL\" = 0 order by \"COL\" desc"); // select "COL", count(select * from "TABLE1") from "TABLE1" where "COL" = 0 group by "COL" having "COL" = 0 order by "COL" desc
	TEST(DB2,
		Select(COL, Count(Select(SqlAll()).From(TABLE1))).From(TABLE1).Where(COL == 0).GroupBy(COL) .Having(COL == 0).OrderBy(Descending(COL)),
		"select \"COL\", count(select * from \"TABLE1\") from \"TABLE1\" where \"COL\" = 0 group by \"COL\" having \"COL\" = 0 order by \"COL\" desc"); // select "COL", count(select * from "TABLE1") from "TABLE1" where "COL" = 0 group by "COL" having "COL" = 0 order by "COL" desc
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Limit(100),
		"select `COL` from `TABLE1` limit 100"); // select `COL` from `TABLE1` limit 100
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Limit(100),
		"select \"COL\" from \"TABLE1\" limit 100"); // select "COL" from "TABLE1" limit 100
	TEST(ORACLE,
		Select(COL).From(TABLE1).Limit(100),
		"select \"COL\" from \"TABLE1\" limit 100"); // select "COL" from "TABLE1" limit 100
	TEST(MSSQL,
		Select(COL).From(TABLE1).Limit(100),
		"select top 100 \"COL\" from \"TABLE1\""); // select top 100 "COL" from "TABLE1"
	TEST(PGSQL,
		Select(COL).From(TABLE1).Limit(100),
		"select \"COL\" from \"TABLE1\" limit 100"); // select "COL" from "TABLE1" limit 100
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Limit(100),
		"select \"COL\" from \"TABLE1\" limit 100"); // select "COL" from "TABLE1" limit 100
	TEST(DB2,
		Select(COL).From(TABLE1).Limit(100),
		"select \"COL\" from \"TABLE1\" limit 100"); // select "COL" from "TABLE1" limit 100
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Limit(100, 10),
		"select `COL` from `TABLE1` limit 100, 10"); // select `COL` from `TABLE1` limit 100, 10
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Limit(100, 10),
		"select \"COL\" from \"TABLE1\" limit 100, 10"); // select "COL" from "TABLE1" limit 100, 10
	TEST(ORACLE,
		Select(COL).From(TABLE1).Limit(100, 10),
		"select \"COL\" from \"TABLE1\" limit 100, 10"); // select "COL" from "TABLE1" limit 100, 10
	TEST(MSSQL,
		Select(COL).From(TABLE1).Limit(100, 10),
		"select \"COL\" from \"TABLE1\" limit 100, 10"); // select "COL" from "TABLE1" limit 100, 10
	TEST(PGSQL,
		Select(COL).From(TABLE1).Limit(100, 10),
		"select \"COL\" from \"TABLE1\" limit 100, 10"); // select "COL" from "TABLE1" limit 100, 10
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Limit(100, 10),
		"select \"COL\" from \"TABLE1\" limit 100, 10"); // select "COL" from "TABLE1" limit 100, 10
	TEST(DB2,
		Select(COL).From(TABLE1).Limit(100, 10),
		"select \"COL\" from \"TABLE1\" limit 100, 10"); // select "COL" from "TABLE1" limit 100, 10
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Offset(20),
		"select `COL` from `TABLE1` offset 20"); // select `COL` from `TABLE1` offset 20
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Offset(20),
		"select \"COL\" from \"TABLE1\" offset 20"); // select "COL" from "TABLE1" offset 20
	TEST(ORACLE,
		Select(COL).From(TABLE1).Offset(20),
		"select \"COL\" from \"TABLE1\" offset 20"); // select "COL" from "TABLE1" offset 20
	TEST(MSSQL,
		Select(COL).From(TABLE1).Offset(20),
		"select \"COL\" from \"TABLE1\" offset 20"); // select "COL" from "TABLE1" offset 20
	TEST(PGSQL,
		Select(COL).From(TABLE1).Offset(20),
		"select \"COL\" from \"TABLE1\" offset 20"); // select "COL" from "TABLE1" offset 20
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Offset(20),
		"select \"COL\" from \"TABLE1\" offset 20"); // select "COL" from "TABLE1" offset 20
	TEST(DB2,
		Select(COL).From(TABLE1).Offset(20),
		"select \"COL\" from \"TABLE1\" offset 20"); // select "COL" from "TABLE1" offset 20
// ---------------------------------
	TEST(MY_SQL,
		Select(25 * 34).Get(),
		"select 850"); // select 850
	TEST(SQLITE3,
		Select(25 * 34).Get(),
		"select 850"); // select 850
	TEST(ORACLE,
		Select(25 * 34).Get(),
		"select 850 from DUAL"); // select 850 from DUAL
	TEST(MSSQL,
		Select(25 * 34).Get(),
		"select 850"); // select 850
	TEST(PGSQL,
		Select(25 * 34).Get(),
		"select 850"); // select 850
	TEST(FIREBIRD,
		Select(25 * 34).Get(),
		"select 850"); // select 850
	TEST(DB2,
		Select(25 * 34).Get(),
		"select 850"); // select 850
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).Hint("hint"),
		"/*+ hint */ select `COL` from `TABLE1`"); // /*+ hint */ select `COL` from `TABLE1`
	TEST(SQLITE3,
		Select(COL).From(TABLE1).Hint("hint"),
		"/*+ hint */ select \"COL\" from \"TABLE1\""); // /*+ hint */ select "COL" from "TABLE1"
	TEST(ORACLE,
		Select(COL).From(TABLE1).Hint("hint"),
		"/*+ hint */ select \"COL\" from \"TABLE1\""); // /*+ hint */ select "COL" from "TABLE1"
	TEST(MSSQL,
		Select(COL).From(TABLE1).Hint("hint"),
		"/*+ hint */ select \"COL\" from \"TABLE1\""); // /*+ hint */ select "COL" from "TABLE1"
	TEST(PGSQL,
		Select(COL).From(TABLE1).Hint("hint"),
		"/*+ hint */ select \"COL\" from \"TABLE1\""); // /*+ hint */ select "COL" from "TABLE1"
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).Hint("hint"),
		"/*+ hint */ select \"COL\" from \"TABLE1\""); // /*+ hint */ select "COL" from "TABLE1"
	TEST(DB2,
		Select(COL).From(TABLE1).Hint("hint"),
		"/*+ hint */ select \"COL\" from \"TABLE1\""); // /*+ hint */ select "COL" from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).InnerJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select `COL` from `TABLE1` inner join `TABLE1` on `TABLE1`.`COL` = `TABLE1`.`COLUMN1`"); // select `COL` from `TABLE1` inner join `TABLE1` on `TABLE1`.`COL` = `TABLE1`.`COLUMN1`
	TEST(SQLITE3,
		Select(COL).From(TABLE1).InnerJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" inner join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" inner join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(ORACLE,
		Select(COL).From(TABLE1).InnerJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" inner join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" inner join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(MSSQL,
		Select(COL).From(TABLE1).InnerJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" inner join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" inner join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(PGSQL,
		Select(COL).From(TABLE1).InnerJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" inner join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" inner join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).InnerJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" inner join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" inner join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(DB2,
		Select(COL).From(TABLE1).InnerJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" inner join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" inner join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).LeftJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select `COL` from `TABLE1` left outer join `TABLE1` on `TABLE1`.`COL` = `TABLE1`.`COLUMN1`"); // select `COL` from `TABLE1` left outer join `TABLE1` on `TABLE1`.`COL` = `TABLE1`.`COLUMN1`
	TEST(SQLITE3,
		Select(COL).From(TABLE1).LeftJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(ORACLE,
		Select(COL).From(TABLE1).LeftJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(MSSQL,
		Select(COL).From(TABLE1).LeftJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(PGSQL,
		Select(COL).From(TABLE1).LeftJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).LeftJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(DB2,
		Select(COL).From(TABLE1).LeftJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).RightJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select `COL` from `TABLE1` right outer join `TABLE1` on `TABLE1`.`COL` = `TABLE1`.`COLUMN1`"); // select `COL` from `TABLE1` right outer join `TABLE1` on `TABLE1`.`COL` = `TABLE1`.`COLUMN1`
	TEST(SQLITE3,
		Select(COL).From(TABLE1).RightJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" right outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" right outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(ORACLE,
		Select(COL).From(TABLE1).RightJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" right outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" right outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(MSSQL,
		Select(COL).From(TABLE1).RightJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" right outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" right outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(PGSQL,
		Select(COL).From(TABLE1).RightJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" right outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" right outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).RightJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" right outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" right outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(DB2,
		Select(COL).From(TABLE1).RightJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" right outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" right outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).FullJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select `COL` from `TABLE1` full outer join `TABLE1` on `TABLE1`.`COL` = `TABLE1`.`COLUMN1`"); // select `COL` from `TABLE1` full outer join `TABLE1` on `TABLE1`.`COL` = `TABLE1`.`COLUMN1`
	TEST(SQLITE3,
		Select(COL).From(TABLE1).FullJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" full outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" full outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(ORACLE,
		Select(COL).From(TABLE1).FullJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" full outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" full outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(MSSQL,
		Select(COL).From(TABLE1).FullJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" full outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" full outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(PGSQL,
		Select(COL).From(TABLE1).FullJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" full outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" full outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).FullJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" full outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" full outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(DB2,
		Select(COL).From(TABLE1).FullJoin(TABLE1).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" full outer join \"TABLE1\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" full outer join "TABLE1" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
// ---------------------------------
	TEST(MY_SQL,
		Delete(TABLE1).Where(COL < 0),
		"delete from `TABLE1` where `COL` < 0"); // delete from `TABLE1` where `COL` < 0
	TEST(SQLITE3,
		Delete(TABLE1).Where(COL < 0),
		"delete from \"TABLE1\" where \"COL\" < 0"); // delete from "TABLE1" where "COL" < 0
	TEST(ORACLE,
		Delete(TABLE1).Where(COL < 0),
		"delete from \"TABLE1\" where \"COL\" < 0"); // delete from "TABLE1" where "COL" < 0
	TEST(MSSQL,
		Delete(TABLE1).Where(COL < 0),
		"delete from \"TABLE1\" where \"COL\" < 0"); // delete from "TABLE1" where "COL" < 0
	TEST(PGSQL,
		Delete(TABLE1).Where(COL < 0),
		"delete from \"TABLE1\" where \"COL\" < 0"); // delete from "TABLE1" where "COL" < 0
	TEST(FIREBIRD,
		Delete(TABLE1).Where(COL < 0),
		"delete from \"TABLE1\" where \"COL\" < 0"); // delete from "TABLE1" where "COL" < 0
	TEST(DB2,
		Delete(TABLE1).Where(COL < 0),
		"delete from \"TABLE1\" where \"COL\" < 0"); // delete from "TABLE1" where "COL" < 0
// ---------------------------------
	TEST(MY_SQL,
		Insert(TABLE1)(COLUMN1, 12)(COLUMN2, "hello")(COL, Date(2007, 1, 1)),
		"insert into `TABLE1`(`COLUMN1`, `COLUMN2`, `COL`) values (12, 'hello', '2007-01-01')"); // insert into `TABLE1`(`COLUMN1`, `COLUMN2`, `COL`) values (12, 'hello', '2007-01-01')
	TEST(SQLITE3,
		Insert(TABLE1)(COLUMN1, 12)(COLUMN2, "hello")(COL, Date(2007, 1, 1)),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COLUMN2\", \"COL\") values (12, 'hello', '2007-01-01')"); // insert into "TABLE1"("COLUMN1", "COLUMN2", "COL") values (12, 'hello', '2007-01-01')
	TEST(ORACLE,
		Insert(TABLE1)(COLUMN1, 12)(COLUMN2, "hello")(COL, Date(2007, 1, 1)),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COLUMN2\", \"COL\") values (12, 'hello', to_date('2007/1/1', 'SYYYY/MM/DD'))"); // insert into "TABLE1"("COLUMN1", "COLUMN2", "COL") values (12, 'hello', to_date('2007/1/1', 'SYYYY/MM/DD'))
	TEST(MSSQL,
		Insert(TABLE1)(COLUMN1, 12)(COLUMN2, "hello")(COL, Date(2007, 1, 1)),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COLUMN2\", \"COL\") values (12, 'hello', convert(datetime, '2007/1/1', 120))"); // insert into "TABLE1"("COLUMN1", "COLUMN2", "COL") values (12, 'hello', convert(datetime, '2007/1/1', 120))
	TEST(PGSQL,
		Insert(TABLE1)(COLUMN1, 12)(COLUMN2, "hello")(COL, Date(2007, 1, 1)),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COLUMN2\", \"COL\") values (12, E'hello', date '2007-01-01')"); // insert into "TABLE1"("COLUMN1", "COLUMN2", "COL") values (12, E'hello', date '2007-01-01')
	TEST(FIREBIRD,
		Insert(TABLE1)(COLUMN1, 12)(COLUMN2, "hello")(COL, Date(2007, 1, 1)),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COLUMN2\", \"COL\") values (12, 'hello', '2007-01-01')"); // insert into "TABLE1"("COLUMN1", "COLUMN2", "COL") values (12, 'hello', '2007-01-01')
	TEST(DB2,
		Insert(TABLE1)(COLUMN1, 12)(COLUMN2, "hello")(COL, Date(2007, 1, 1)),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COLUMN2\", \"COL\") values (12, 'hello', '2007-01-01')"); // insert into "TABLE1"("COLUMN1", "COLUMN2", "COL") values (12, 'hello', '2007-01-01')
// ---------------------------------
	TEST(MY_SQL,
		Insert(TABLE1)(COLUMN1, 12)(COLUMN2)(COL).From(TABLE1).Where(COL >= 0),
		"insert into `TABLE1`(`COLUMN1`, `COLUMN2`, `COL`) select 12, `COLUMN2`, `COL` from `TABLE1` where `COL` >= 0"); // insert into `TABLE1`(`COLUMN1`, `COLUMN2`, `COL`) select 12, `COLUMN2`, `COL` from `TABLE1` where `COL` >= 0
	TEST(SQLITE3,
		Insert(TABLE1)(COLUMN1, 12)(COLUMN2)(COL).From(TABLE1).Where(COL >= 0),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COLUMN2\", \"COL\") select 12, \"COLUMN2\", \"COL\" from \"TABLE1\" where \"COL\" >= 0"); // insert into "TABLE1"("COLUMN1", "COLUMN2", "COL") select 12, "COLUMN2", "COL" from "TABLE1" where "COL" >= 0
	TEST(ORACLE,
		Insert(TABLE1)(COLUMN1, 12)(COLUMN2)(COL).From(TABLE1).Where(COL >= 0),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COLUMN2\", \"COL\") select 12, \"COLUMN2\", \"COL\" from \"TABLE1\" where \"COL\" >= 0"); // insert into "TABLE1"("COLUMN1", "COLUMN2", "COL") select 12, "COLUMN2", "COL" from "TABLE1" where "COL" >= 0
	TEST(MSSQL,
		Insert(TABLE1)(COLUMN1, 12)(COLUMN2)(COL).From(TABLE1).Where(COL >= 0),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COLUMN2\", \"COL\") select 12, \"COLUMN2\", \"COL\" from \"TABLE1\" where \"COL\" >= 0"); // insert into "TABLE1"("COLUMN1", "COLUMN2", "COL") select 12, "COLUMN2", "COL" from "TABLE1" where "COL" >= 0
	TEST(PGSQL,
		Insert(TABLE1)(COLUMN1, 12)(COLUMN2)(COL).From(TABLE1).Where(COL >= 0),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COLUMN2\", \"COL\") select 12, \"COLUMN2\", \"COL\" from \"TABLE1\" where \"COL\" >= 0"); // insert into "TABLE1"("COLUMN1", "COLUMN2", "COL") select 12, "COLUMN2", "COL" from "TABLE1" where "COL" >= 0
	TEST(FIREBIRD,
		Insert(TABLE1)(COLUMN1, 12)(COLUMN2)(COL).From(TABLE1).Where(COL >= 0),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COLUMN2\", \"COL\") select 12, \"COLUMN2\", \"COL\" from \"TABLE1\" where \"COL\" >= 0"); // insert into "TABLE1"("COLUMN1", "COLUMN2", "COL") select 12, "COLUMN2", "COL" from "TABLE1" where "COL" >= 0
	TEST(DB2,
		Insert(TABLE1)(COLUMN1, 12)(COLUMN2)(COL).From(TABLE1).Where(COL >= 0),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COLUMN2\", \"COL\") select 12, \"COLUMN2\", \"COL\" from \"TABLE1\" where \"COL\" >= 0"); // insert into "TABLE1"("COLUMN1", "COLUMN2", "COL") select 12, "COLUMN2", "COL" from "TABLE1" where "COL" >= 0
// ---------------------------------
	TEST(MY_SQL,
		Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1),
		"insert into `TABLE1`(`COLUMN1`, `COL`) select `COLUMN1`, sum(`COLUMN2`) from `TABLE1` where `COL` >= 0 group by `COLUMN1`"); // insert into `TABLE1`(`COLUMN1`, `COL`) select `COLUMN1`, sum(`COLUMN2`) from `TABLE1` where `COL` >= 0 group by `COLUMN1`
	TEST(SQLITE3,
		Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COL\") select \"COLUMN1\", sum(\"COLUMN2\") from \"TABLE1\" where \"COL\" >= 0 group by \"COLUMN1\""); // insert into "TABLE1"("COLUMN1", "COL") select "COLUMN1", sum("COLUMN2") from "TABLE1" where "COL" >= 0 group by "COLUMN1"
	TEST(ORACLE,
		Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COL\") select \"COLUMN1\", sum(\"COLUMN2\") from \"TABLE1\" where \"COL\" >= 0 group by \"COLUMN1\""); // insert into "TABLE1"("COLUMN1", "COL") select "COLUMN1", sum("COLUMN2") from "TABLE1" where "COL" >= 0 group by "COLUMN1"
	TEST(MSSQL,
		Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COL\") select \"COLUMN1\", sum(\"COLUMN2\") from \"TABLE1\" where \"COL\" >= 0 group by \"COLUMN1\""); // insert into "TABLE1"("COLUMN1", "COL") select "COLUMN1", sum("COLUMN2") from "TABLE1" where "COL" >= 0 group by "COLUMN1"
	TEST(PGSQL,
		Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COL\") select \"COLUMN1\", sum(\"COLUMN2\") from \"TABLE1\" where \"COL\" >= 0 group by \"COLUMN1\""); // insert into "TABLE1"("COLUMN1", "COL") select "COLUMN1", sum("COLUMN2") from "TABLE1" where "COL" >= 0 group by "COLUMN1"
	TEST(FIREBIRD,
		Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COL\") select \"COLUMN1\", sum(\"COLUMN2\") from \"TABLE1\" where \"COL\" >= 0 group by \"COLUMN1\""); // insert into "TABLE1"("COLUMN1", "COL") select "COLUMN1", sum("COLUMN2") from "TABLE1" where "COL" >= 0 group by "COLUMN1"
	TEST(DB2,
		Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COL\") select \"COLUMN1\", sum(\"COLUMN2\") from \"TABLE1\" where \"COL\" >= 0 group by \"COLUMN1\""); // insert into "TABLE1"("COLUMN1", "COL") select "COLUMN1", sum("COLUMN2") from "TABLE1" where "COL" >= 0 group by "COLUMN1"
// ---------------------------------
	TEST(MY_SQL,
		Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1).Having(COLUMN2 > 10),
		"insert into `TABLE1`(`COLUMN1`, `COL`) select `COLUMN1`, sum(`COLUMN2`) from `TABLE1` where `COL` >= 0 group by `COLUMN1` having `COLUMN2` > 10"); // insert into `TABLE1`(`COLUMN1`, `COL`) select `COLUMN1`, sum(`COLUMN2`) from `TABLE1` where `COL` >= 0 group by `COLUMN1` having `COLUMN2` > 10
	TEST(SQLITE3,
		Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1).Having(COLUMN2 > 10),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COL\") select \"COLUMN1\", sum(\"COLUMN2\") from \"TABLE1\" where \"COL\" >= 0 group by \"COLUMN1\" having \"COLUMN2\" > 10"); // insert into "TABLE1"("COLUMN1", "COL") select "COLUMN1", sum("COLUMN2") from "TABLE1" where "COL" >= 0 group by "COLUMN1" having "COLUMN2" > 10
	TEST(ORACLE,
		Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1).Having(COLUMN2 > 10),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COL\") select \"COLUMN1\", sum(\"COLUMN2\") from \"TABLE1\" where \"COL\" >= 0 group by \"COLUMN1\" having \"COLUMN2\" > 10"); // insert into "TABLE1"("COLUMN1", "COL") select "COLUMN1", sum("COLUMN2") from "TABLE1" where "COL" >= 0 group by "COLUMN1" having "COLUMN2" > 10
	TEST(MSSQL,
		Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1).Having(COLUMN2 > 10),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COL\") select \"COLUMN1\", sum(\"COLUMN2\") from \"TABLE1\" where \"COL\" >= 0 group by \"COLUMN1\" having \"COLUMN2\" > 10"); // insert into "TABLE1"("COLUMN1", "COL") select "COLUMN1", sum("COLUMN2") from "TABLE1" where "COL" >= 0 group by "COLUMN1" having "COLUMN2" > 10
	TEST(PGSQL,
		Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1).Having(COLUMN2 > 10),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COL\") select \"COLUMN1\", sum(\"COLUMN2\") from \"TABLE1\" where \"COL\" >= 0 group by \"COLUMN1\" having \"COLUMN2\" > 10"); // insert into "TABLE1"("COLUMN1", "COL") select "COLUMN1", sum("COLUMN2") from "TABLE1" where "COL" >= 0 group by "COLUMN1" having "COLUMN2" > 10
	TEST(FIREBIRD,
		Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1).Having(COLUMN2 > 10),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COL\") select \"COLUMN1\", sum(\"COLUMN2\") from \"TABLE1\" where \"COL\" >= 0 group by \"COLUMN1\" having \"COLUMN2\" > 10"); // insert into "TABLE1"("COLUMN1", "COL") select "COLUMN1", sum("COLUMN2") from "TABLE1" where "COL" >= 0 group by "COLUMN1" having "COLUMN2" > 10
	TEST(DB2,
		Insert(TABLE1)(COLUMN1)(COL, SqlSum(COLUMN2)).From(TABLE1).Where(COL >= 0).GroupBy(COLUMN1).Having(COLUMN2 > 10),
		"insert into \"TABLE1\"(\"COLUMN1\", \"COL\") select \"COLUMN1\", sum(\"COLUMN2\") from \"TABLE1\" where \"COL\" >= 0 group by \"COLUMN1\" having \"COLUMN2\" > 10"); // insert into "TABLE1"("COLUMN1", "COL") select "COLUMN1", sum("COLUMN2") from "TABLE1" where "COL" >= 0 group by "COLUMN1" having "COLUMN2" > 10
// ---------------------------------
	TEST(MY_SQL,
		Insert(TABLE1)(COLUMN1, 1).Where(NotExists(Select(ID).From(TABLE1).Where(COLUMN1 == 1))),
		"insert into `TABLE1`(`COLUMN1`) select 1 where not exists (select `ID` from `TABLE1` where `COLUMN1` = 1)"); // insert into `TABLE1`(`COLUMN1`) select 1 where not exists (select `ID` from `TABLE1` where `COLUMN1` = 1)
	TEST(SQLITE3,
		Insert(TABLE1)(COLUMN1, 1).Where(NotExists(Select(ID).From(TABLE1).Where(COLUMN1 == 1))),
		"insert into \"TABLE1\"(\"COLUMN1\") select 1 where not exists (select \"ID\" from \"TABLE1\" where \"COLUMN1\" = 1)"); // insert into "TABLE1"("COLUMN1") select 1 where not exists (select "ID" from "TABLE1" where "COLUMN1" = 1)
	TEST(ORACLE,
		Insert(TABLE1)(COLUMN1, 1).Where(NotExists(Select(ID).From(TABLE1).Where(COLUMN1 == 1))),
		"insert into \"TABLE1\"(\"COLUMN1\") select 1 from DUAL where not exists (select \"ID\" from \"TABLE1\" where \"COLUMN1\" = 1)"); // insert into "TABLE1"("COLUMN1") select 1 from DUAL where not exists (select "ID" from "TABLE1" where "COLUMN1" = 1)
	TEST(MSSQL,
		Insert(TABLE1)(COLUMN1, 1).Where(NotExists(Select(ID).From(TABLE1).Where(COLUMN1 == 1))),
		"insert into \"TABLE1\"(\"COLUMN1\") select 1 where not exists (select \"ID\" from \"TABLE1\" where \"COLUMN1\" = 1)"); // insert into "TABLE1"("COLUMN1") select 1 where not exists (select "ID" from "TABLE1" where "COLUMN1" = 1)
	TEST(PGSQL,
		Insert(TABLE1)(COLUMN1, 1).Where(NotExists(Select(ID).From(TABLE1).Where(COLUMN1 == 1))),
		"insert into \"TABLE1\"(\"COLUMN1\") select 1 where not exists (select \"ID\" from \"TABLE1\" where \"COLUMN1\" = 1)"); // insert into "TABLE1"("COLUMN1") select 1 where not exists (select "ID" from "TABLE1" where "COLUMN1" = 1)
	TEST(FIREBIRD,
		Insert(TABLE1)(COLUMN1, 1).Where(NotExists(Select(ID).From(TABLE1).Where(COLUMN1 == 1))),
		"insert into \"TABLE1\"(\"COLUMN1\") select 1 where not exists (select \"ID\" from \"TABLE1\" where \"COLUMN1\" = 1)"); // insert into "TABLE1"("COLUMN1") select 1 where not exists (select "ID" from "TABLE1" where "COLUMN1" = 1)
	TEST(DB2,
		Insert(TABLE1)(COLUMN1, 1).Where(NotExists(Select(ID).From(TABLE1).Where(COLUMN1 == 1))),
		"insert into \"TABLE1\"(\"COLUMN1\") select 1 where not exists (select \"ID\" from \"TABLE1\" where \"COLUMN1\" = 1)"); // insert into "TABLE1"("COLUMN1") select 1 where not exists (select "ID" from "TABLE1" where "COLUMN1" = 1)
// ---------------------------------
	TEST(MY_SQL,
		Update(TABLE1)(COLUMN1, 13)(COLUMN2, "world").Where(COL > Date(2007, 1, 1)),
		"update `TABLE1` set `COLUMN1` = 13, `COLUMN2` = 'world' where `COL` > '2007-01-01'"); // update `TABLE1` set `COLUMN1` = 13, `COLUMN2` = 'world' where `COL` > '2007-01-01'
	TEST(SQLITE3,
		Update(TABLE1)(COLUMN1, 13)(COLUMN2, "world").Where(COL > Date(2007, 1, 1)),
		"update \"TABLE1\" set \"COLUMN1\" = 13, \"COLUMN2\" = 'world' where \"COL\" > '2007-01-01'"); // update "TABLE1" set "COLUMN1" = 13, "COLUMN2" = 'world' where "COL" > '2007-01-01'
	TEST(ORACLE,
		Update(TABLE1)(COLUMN1, 13)(COLUMN2, "world").Where(COL > Date(2007, 1, 1)),
		"update \"TABLE1\" set \"COLUMN1\" = 13, \"COLUMN2\" = 'world' where \"COL\" > to_date('2007/1/1', 'SYYYY/MM/DD')"); // update "TABLE1" set "COLUMN1" = 13, "COLUMN2" = 'world' where "COL" > to_date('2007/1/1', 'SYYYY/MM/DD')
	TEST(MSSQL,
		Update(TABLE1)(COLUMN1, 13)(COLUMN2, "world").Where(COL > Date(2007, 1, 1)),
		"update \"TABLE1\" set \"COLUMN1\" = 13, \"COLUMN2\" = 'world' where \"COL\" > convert(datetime, '2007/1/1', 120)"); // update "TABLE1" set "COLUMN1" = 13, "COLUMN2" = 'world' where "COL" > convert(datetime, '2007/1/1', 120)
	TEST(PGSQL,
		Update(TABLE1)(COLUMN1, 13)(COLUMN2, "world").Where(COL > Date(2007, 1, 1)),
		"update \"TABLE1\" set \"COLUMN1\" = 13, \"COLUMN2\" = E'world' where \"COL\" > date '2007-01-01'"); // update "TABLE1" set "COLUMN1" = 13, "COLUMN2" = E'world' where "COL" > date '2007-01-01'
	TEST(FIREBIRD,
		Update(TABLE1)(COLUMN1, 13)(COLUMN2, "world").Where(COL > Date(2007, 1, 1)),
		"update \"TABLE1\" set \"COLUMN1\" = 13, \"COLUMN2\" = 'world' where \"COL\" > '2007-01-01'"); // update "TABLE1" set "COLUMN1" = 13, "COLUMN2" = 'world' where "COL" > '2007-01-01'
	TEST(DB2,
		Update(TABLE1)(COLUMN1, 13)(COLUMN2, "world").Where(COL > Date(2007, 1, 1)),
		"update \"TABLE1\" set \"COLUMN1\" = 13, \"COLUMN2\" = 'world' where \"COL\" > '2007-01-01'"); // update "TABLE1" set "COLUMN1" = 13, "COLUMN2" = 'world' where "COL" > '2007-01-01'
// ---------------------------------
	TEST(MY_SQL,
		Insert(TABLE1)(COL, Select(COLUMN1).From(TABLE1).Where(COLUMN2 == 21).AsValue()),
		"insert into `TABLE1`(`COL`) values ((select `COLUMN1` from `TABLE1` where `COLUMN2` = 21))"); // insert into `TABLE1`(`COL`) values ((select `COLUMN1` from `TABLE1` where `COLUMN2` = 21))
	TEST(SQLITE3,
		Insert(TABLE1)(COL, Select(COLUMN1).From(TABLE1).Where(COLUMN2 == 21).AsValue()),
		"insert into \"TABLE1\"(\"COL\") values ((select \"COLUMN1\" from \"TABLE1\" where \"COLUMN2\" = 21))"); // insert into "TABLE1"("COL") values ((select "COLUMN1" from "TABLE1" where "COLUMN2" = 21))
	TEST(ORACLE,
		Insert(TABLE1)(COL, Select(COLUMN1).From(TABLE1).Where(COLUMN2 == 21).AsValue()),
		"insert into \"TABLE1\"(\"COL\") values ((select \"COLUMN1\" from \"TABLE1\" where \"COLUMN2\" = 21))"); // insert into "TABLE1"("COL") values ((select "COLUMN1" from "TABLE1" where "COLUMN2" = 21))
	TEST(MSSQL,
		Insert(TABLE1)(COL, Select(COLUMN1).From(TABLE1).Where(COLUMN2 == 21).AsValue()),
		"insert into \"TABLE1\"(\"COL\") values ((select \"COLUMN1\" from \"TABLE1\" where \"COLUMN2\" = 21))"); // insert into "TABLE1"("COL") values ((select "COLUMN1" from "TABLE1" where "COLUMN2" = 21))
	TEST(PGSQL,
		Insert(TABLE1)(COL, Select(COLUMN1).From(TABLE1).Where(COLUMN2 == 21).AsValue()),
		"insert into \"TABLE1\"(\"COL\") values ((select \"COLUMN1\" from \"TABLE1\" where \"COLUMN2\" = 21))"); // insert into "TABLE1"("COL") values ((select "COLUMN1" from "TABLE1" where "COLUMN2" = 21))
	TEST(FIREBIRD,
		Insert(TABLE1)(COL, Select(COLUMN1).From(TABLE1).Where(COLUMN2 == 21).AsValue()),
		"insert into \"TABLE1\"(\"COL\") values ((select \"COLUMN1\" from \"TABLE1\" where \"COLUMN2\" = 21))"); // insert into "TABLE1"("COL") values ((select "COLUMN1" from "TABLE1" where "COLUMN2" = 21))
	TEST(DB2,
		Insert(TABLE1)(COL, Select(COLUMN1).From(TABLE1).Where(COLUMN2 == 21).AsValue()),
		"insert into \"TABLE1\"(\"COL\") values ((select \"COLUMN1\" from \"TABLE1\" where \"COLUMN2\" = 21))"); // insert into "TABLE1"("COL") values ((select "COLUMN1" from "TABLE1" where "COLUMN2" = 21))
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(Select(COL).From(TABLE1)),
		"select `COL` from (select `COL` from `TABLE1`)"); // select `COL` from (select `COL` from `TABLE1`)
	TEST(SQLITE3,
		Select(COL).From(Select(COL).From(TABLE1)),
		"select \"COL\" from (select \"COL\" from \"TABLE1\")"); // select "COL" from (select "COL" from "TABLE1")
	TEST(ORACLE,
		Select(COL).From(Select(COL).From(TABLE1)),
		"select \"COL\" from (select \"COL\" from \"TABLE1\")"); // select "COL" from (select "COL" from "TABLE1")
	TEST(MSSQL,
		Select(COL).From(Select(COL).From(TABLE1)),
		"select \"COL\" from (select \"COL\" from \"TABLE1\")"); // select "COL" from (select "COL" from "TABLE1")
	TEST(PGSQL,
		Select(COL).From(Select(COL).From(TABLE1)),
		"select \"COL\" from (select \"COL\" from \"TABLE1\")"); // select "COL" from (select "COL" from "TABLE1")
	TEST(FIREBIRD,
		Select(COL).From(Select(COL).From(TABLE1)),
		"select \"COL\" from (select \"COL\" from \"TABLE1\")"); // select "COL" from (select "COL" from "TABLE1")
	TEST(DB2,
		Select(COL).From(Select(COL).From(TABLE1)),
		"select \"COL\" from (select \"COL\" from \"TABLE1\")"); // select "COL" from (select "COL" from "TABLE1")
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(Select(COL).From(TABLE1).AsTable(TABLE2)),
		"select `COL` from ((select `COL` from `TABLE1`) as `TABLE2`)"); // select `COL` from ((select `COL` from `TABLE1`) as `TABLE2`)
	TEST(SQLITE3,
		Select(COL).From(Select(COL).From(TABLE1).AsTable(TABLE2)),
		"select \"COL\" from (select \"COL\" from \"TABLE1\") as \"TABLE2\""); // select "COL" from (select "COL" from "TABLE1") as "TABLE2"
	TEST(ORACLE,
		Select(COL).From(Select(COL).From(TABLE1).AsTable(TABLE2)),
		"select \"COL\" from ((select \"COL\" from \"TABLE1\") \"TABLE2\")"); // select "COL" from ((select "COL" from "TABLE1") "TABLE2")
	TEST(MSSQL,
		Select(COL).From(Select(COL).From(TABLE1).AsTable(TABLE2)),
		"select \"COL\" from (select \"COL\" from \"TABLE1\") as \"TABLE2\""); // select "COL" from (select "COL" from "TABLE1") as "TABLE2"
	TEST(PGSQL,
		Select(COL).From(Select(COL).From(TABLE1).AsTable(TABLE2)),
		"select \"COL\" from (select \"COL\" from \"TABLE1\") as \"TABLE2\""); // select "COL" from (select "COL" from "TABLE1") as "TABLE2"
	TEST(FIREBIRD,
		Select(COL).From(Select(COL).From(TABLE1).AsTable(TABLE2)),
		"select \"COL\" from ((select \"COL\" from \"TABLE1\") as \"TABLE2\")"); // select "COL" from ((select "COL" from "TABLE1") as "TABLE2")
	TEST(DB2,
		Select(COL).From(Select(COL).From(TABLE1).AsTable(TABLE2)),
		"select \"COL\" from ((select \"COL\" from \"TABLE1\") as \"TABLE2\")"); // select "COL" from ((select "COL" from "TABLE1") as "TABLE2")
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select `COL` from `TABLE1` left outer join (select `COL` from `TABLE1`) on `TABLE1`.`COL` = `TABLE1`.`COLUMN1`"); // select `COL` from `TABLE1` left outer join (select `COL` from `TABLE1`) on `TABLE1`.`COL` = `TABLE1`.`COLUMN1`
	TEST(SQLITE3,
		Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join (select \"COL\" from \"TABLE1\") on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join (select "COL" from "TABLE1") on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(ORACLE,
		Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join (select \"COL\" from \"TABLE1\") on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join (select "COL" from "TABLE1") on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(MSSQL,
		Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join (select \"COL\" from \"TABLE1\") on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join (select "COL" from "TABLE1") on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(PGSQL,
		Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join (select \"COL\" from \"TABLE1\") on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join (select "COL" from "TABLE1") on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join (select \"COL\" from \"TABLE1\") on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join (select "COL" from "TABLE1") on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(DB2,
		Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join (select \"COL\" from \"TABLE1\") on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join (select "COL" from "TABLE1") on "TABLE1"."COL" = "TABLE1"."COLUMN1"
// ---------------------------------
	TEST(MY_SQL,
		Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1).AsTable(TABLE2)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select `COL` from `TABLE1` left outer join ((select `COL` from `TABLE1`) as `TABLE2`) on `TABLE1`.`COL` = `TABLE1`.`COLUMN1`"); // select `COL` from `TABLE1` left outer join ((select `COL` from `TABLE1`) as `TABLE2`) on `TABLE1`.`COL` = `TABLE1`.`COLUMN1`
	TEST(SQLITE3,
		Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1).AsTable(TABLE2)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join (select \"COL\" from \"TABLE1\") as \"TABLE2\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join (select "COL" from "TABLE1") as "TABLE2" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(ORACLE,
		Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1).AsTable(TABLE2)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join ((select \"COL\" from \"TABLE1\") \"TABLE2\") on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join ((select "COL" from "TABLE1") "TABLE2") on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(MSSQL,
		Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1).AsTable(TABLE2)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join (select \"COL\" from \"TABLE1\") as \"TABLE2\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join (select "COL" from "TABLE1") as "TABLE2" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(PGSQL,
		Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1).AsTable(TABLE2)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join (select \"COL\" from \"TABLE1\") as \"TABLE2\" on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join (select "COL" from "TABLE1") as "TABLE2" on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(FIREBIRD,
		Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1).AsTable(TABLE2)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join ((select \"COL\" from \"TABLE1\") as \"TABLE2\") on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join ((select "COL" from "TABLE1") as "TABLE2") on "TABLE1"."COL" = "TABLE1"."COLUMN1"
	TEST(DB2,
		Select(COL).From(TABLE1).LeftJoin(Select(COL).From(TABLE1).AsTable(TABLE2)).On(COL.Of(TABLE1) == COLUMN1.Of(TABLE1)),
		"select \"COL\" from \"TABLE1\" left outer join ((select \"COL\" from \"TABLE1\") as \"TABLE2\") on \"TABLE1\".\"COL\" = \"TABLE1\".\"COLUMN1\""); // select "COL" from "TABLE1" left outer join ((select "COL" from "TABLE1") as "TABLE2") on "TABLE1"."COL" = "TABLE1"."COLUMN1"
// ---------------------------------
	TEST(MY_SQL,
		Select(TABLE1(COL, COLUMN1)).From(TABLE1),
		"select `TABLE1`.`COL`, `TABLE1`.`COLUMN1` from `TABLE1`"); // select `TABLE1`.`COL`, `TABLE1`.`COLUMN1` from `TABLE1`
	TEST(SQLITE3,
		Select(TABLE1(COL, COLUMN1)).From(TABLE1),
		"select \"TABLE1\".\"COL\", \"TABLE1\".\"COLUMN1\" from \"TABLE1\""); // select "TABLE1"."COL", "TABLE1"."COLUMN1" from "TABLE1"
	TEST(ORACLE,
		Select(TABLE1(COL, COLUMN1)).From(TABLE1),
		"select \"TABLE1\".\"COL\", \"TABLE1\".\"COLUMN1\" from \"TABLE1\""); // select "TABLE1"."COL", "TABLE1"."COLUMN1" from "TABLE1"
	TEST(MSSQL,
		Select(TABLE1(COL, COLUMN1)).From(TABLE1),
		"select \"TABLE1\".\"COL\", \"TABLE1\".\"COLUMN1\" from \"TABLE1\""); // select "TABLE1"."COL", "TABLE1"."COLUMN1" from "TABLE1"
	TEST(PGSQL,
		Select(TABLE1(COL, COLUMN1)).From(TABLE1),
		"select \"TABLE1\".\"COL\", \"TABLE1\".\"COLUMN1\" from \"TABLE1\""); // select "TABLE1"."COL", "TABLE1"."COLUMN1" from "TABLE1"
	TEST(FIREBIRD,
		Select(TABLE1(COL, COLUMN1)).From(TABLE1),
		"select \"TABLE1\".\"COL\", \"TABLE1\".\"COLUMN1\" from \"TABLE1\""); // select "TABLE1"."COL", "TABLE1"."COLUMN1" from "TABLE1"
	TEST(DB2,
		Select(TABLE1(COL, COLUMN1)).From(TABLE1),
		"select \"TABLE1\".\"COL\", \"TABLE1\".\"COLUMN1\" from \"TABLE1\""); // select "TABLE1"."COL", "TABLE1"."COLUMN1" from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).Where(BDATE == Date(2011, 12, 9)),
		"select `ID`.`ID`, `ID`.`NAME`, `ID`.`LASTNAME` from `TABLE1` left outer join `TABLE2` on `TABLE2`.`TABLE1_ID` = `TABLE1`.`NAME` where `BDATE` = '2011-12-09'"); // select `ID`.`ID`, `ID`.`NAME`, `ID`.`LASTNAME` from `TABLE1` left outer join `TABLE2` on `TABLE2`.`TABLE1_ID` = `TABLE1`.`NAME` where `BDATE` = '2011-12-09'
	TEST(SQLITE3,
		Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).Where(BDATE == Date(2011, 12, 9)),
		"select \"ID\".\"ID\", \"ID\".\"NAME\", \"ID\".\"LASTNAME\" from \"TABLE1\" left outer join \"TABLE2\" on \"TABLE2\".\"TABLE1_ID\" = \"TABLE1\".\"NAME\" where \"BDATE\" = '2011-12-09'"); // select "ID"."ID", "ID"."NAME", "ID"."LASTNAME" from "TABLE1" left outer join "TABLE2" on "TABLE2"."TABLE1_ID" = "TABLE1"."NAME" where "BDATE" = '2011-12-09'
	TEST(ORACLE,
		Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).Where(BDATE == Date(2011, 12, 9)),
		"select \"ID\".\"ID\", \"ID\".\"NAME\", \"ID\".\"LASTNAME\" from \"TABLE1\" left outer join \"TABLE2\" on \"TABLE2\".\"TABLE1_ID\" = \"TABLE1\".\"NAME\" where \"BDATE\" = to_date('2011/12/9', 'SYYYY/MM/DD')"); // select "ID"."ID", "ID"."NAME", "ID"."LASTNAME" from "TABLE1" left outer join "TABLE2" on "TABLE2"."TABLE1_ID" = "TABLE1"."NAME" where "BDATE" = to_date('2011/12/9', 'SYYYY/MM/DD')
	TEST(MSSQL,
		Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).Where(BDATE == Date(2011, 12, 9)),
		"select \"ID\".\"ID\", \"ID\".\"NAME\", \"ID\".\"LASTNAME\" from \"TABLE1\" left outer join \"TABLE2\" on \"TABLE2\".\"TABLE1_ID\" = \"TABLE1\".\"NAME\" where \"BDATE\" = convert(datetime, '2011/12/9', 120)"); // select "ID"."ID", "ID"."NAME", "ID"."LASTNAME" from "TABLE1" left outer join "TABLE2" on "TABLE2"."TABLE1_ID" = "TABLE1"."NAME" where "BDATE" = convert(datetime, '2011/12/9', 120)
	TEST(PGSQL,
		Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).Where(BDATE == Date(2011, 12, 9)),
		"select \"ID\".\"ID\", \"ID\".\"NAME\", \"ID\".\"LASTNAME\" from \"TABLE1\" left outer join \"TABLE2\" on \"TABLE2\".\"TABLE1_ID\" = \"TABLE1\".\"NAME\" where \"BDATE\" = date '2011-12-09'"); // select "ID"."ID", "ID"."NAME", "ID"."LASTNAME" from "TABLE1" left outer join "TABLE2" on "TABLE2"."TABLE1_ID" = "TABLE1"."NAME" where "BDATE" = date '2011-12-09'
	TEST(FIREBIRD,
		Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).Where(BDATE == Date(2011, 12, 9)),
		"select \"ID\".\"ID\", \"ID\".\"NAME\", \"ID\".\"LASTNAME\" from \"TABLE1\" left outer join \"TABLE2\" on \"TABLE2\".\"TABLE1_ID\" = \"TABLE1\".\"NAME\" where \"BDATE\" = '2011-12-09'"); // select "ID"."ID", "ID"."NAME", "ID"."LASTNAME" from "TABLE1" left outer join "TABLE2" on "TABLE2"."TABLE1_ID" = "TABLE1"."NAME" where "BDATE" = '2011-12-09'
	TEST(DB2,
		Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).Where(BDATE == Date(2011, 12, 9)),
		"select \"ID\".\"ID\", \"ID\".\"NAME\", \"ID\".\"LASTNAME\" from \"TABLE1\" left outer join \"TABLE2\" on \"TABLE2\".\"TABLE1_ID\" = \"TABLE1\".\"NAME\" where \"BDATE\" = '2011-12-09'"); // select "ID"."ID", "ID"."NAME", "ID"."LASTNAME" from "TABLE1" left outer join "TABLE2" on "TABLE2"."TABLE1_ID" = "TABLE1"."NAME" where "BDATE" = '2011-12-09'
// ---------------------------------
	TEST(MY_SQL,
		Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).On(IsNull(BDATE)).Where(BDATE == Date(2011, 12, 9)),
		"select `ID`.`ID`, `ID`.`NAME`, `ID`.`LASTNAME` from `TABLE1` left outer join `TABLE2` on `TABLE2`.`TABLE1_ID` = `TABLE1`.`NAME` and `BDATE` is NULL where `BDATE` = '2011-12-09'"); // select `ID`.`ID`, `ID`.`NAME`, `ID`.`LASTNAME` from `TABLE1` left outer join `TABLE2` on `TABLE2`.`TABLE1_ID` = `TABLE1`.`NAME` and `BDATE` is NULL where `BDATE` = '2011-12-09'
	TEST(SQLITE3,
		Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).On(IsNull(BDATE)).Where(BDATE == Date(2011, 12, 9)),
		"select \"ID\".\"ID\", \"ID\".\"NAME\", \"ID\".\"LASTNAME\" from \"TABLE1\" left outer join \"TABLE2\" on \"TABLE2\".\"TABLE1_ID\" = \"TABLE1\".\"NAME\" and \"BDATE\" is NULL where \"BDATE\" = '2011-12-09'"); // select "ID"."ID", "ID"."NAME", "ID"."LASTNAME" from "TABLE1" left outer join "TABLE2" on "TABLE2"."TABLE1_ID" = "TABLE1"."NAME" and "BDATE" is NULL where "BDATE" = '2011-12-09'
	TEST(ORACLE,
		Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).On(IsNull(BDATE)).Where(BDATE == Date(2011, 12, 9)),
		"select \"ID\".\"ID\", \"ID\".\"NAME\", \"ID\".\"LASTNAME\" from \"TABLE1\" left outer join \"TABLE2\" on \"TABLE2\".\"TABLE1_ID\" = \"TABLE1\".\"NAME\" and \"BDATE\" is NULL where \"BDATE\" = to_date('2011/12/9', 'SYYYY/MM/DD')"); // select "ID"."ID", "ID"."NAME", "ID"."LASTNAME" from "TABLE1" left outer join "TABLE2" on "TABLE2"."TABLE1_ID" = "TABLE1"."NAME" and "BDATE" is NULL where "BDATE" = to_date('2011/12/9', 'SYYYY/MM/DD')
	TEST(MSSQL,
		Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).On(IsNull(BDATE)).Where(BDATE == Date(2011, 12, 9)),
		"select \"ID\".\"ID\", \"ID\".\"NAME\", \"ID\".\"LASTNAME\" from \"TABLE1\" left outer join \"TABLE2\" on \"TABLE2\".\"TABLE1_ID\" = \"TABLE1\".\"NAME\" and \"BDATE\" is NULL where \"BDATE\" = convert(datetime, '2011/12/9', 120)"); // select "ID"."ID", "ID"."NAME", "ID"."LASTNAME" from "TABLE1" left outer join "TABLE2" on "TABLE2"."TABLE1_ID" = "TABLE1"."NAME" and "BDATE" is NULL where "BDATE" = convert(datetime, '2011/12/9', 120)
	TEST(PGSQL,
		Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).On(IsNull(BDATE)).Where(BDATE == Date(2011, 12, 9)),
		"select \"ID\".\"ID\", \"ID\".\"NAME\", \"ID\".\"LASTNAME\" from \"TABLE1\" left outer join \"TABLE2\" on \"TABLE2\".\"TABLE1_ID\" = \"TABLE1\".\"NAME\" and \"BDATE\" is NULL where \"BDATE\" = date '2011-12-09'"); // select "ID"."ID", "ID"."NAME", "ID"."LASTNAME" from "TABLE1" left outer join "TABLE2" on "TABLE2"."TABLE1_ID" = "TABLE1"."NAME" and "BDATE" is NULL where "BDATE" = date '2011-12-09'
	TEST(FIREBIRD,
		Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).On(IsNull(BDATE)).Where(BDATE == Date(2011, 12, 9)),
		"select \"ID\".\"ID\", \"ID\".\"NAME\", \"ID\".\"LASTNAME\" from \"TABLE1\" left outer join \"TABLE2\" on \"TABLE2\".\"TABLE1_ID\" = \"TABLE1\".\"NAME\" and \"BDATE\" is NULL where \"BDATE\" = '2011-12-09'"); // select "ID"."ID", "ID"."NAME", "ID"."LASTNAME" from "TABLE1" left outer join "TABLE2" on "TABLE2"."TABLE1_ID" = "TABLE1"."NAME" and "BDATE" is NULL where "BDATE" = '2011-12-09'
	TEST(DB2,
		Select(ID(ID, NAME, LASTNAME)).From(TABLE1).LeftJoinRef(TABLE2).On(IsNull(BDATE)).Where(BDATE == Date(2011, 12, 9)),
		"select \"ID\".\"ID\", \"ID\".\"NAME\", \"ID\".\"LASTNAME\" from \"TABLE1\" left outer join \"TABLE2\" on \"TABLE2\".\"TABLE1_ID\" = \"TABLE1\".\"NAME\" and \"BDATE\" is NULL where \"BDATE\" = '2011-12-09'"); // select "ID"."ID", "ID"."NAME", "ID"."LASTNAME" from "TABLE1" left outer join "TABLE2" on "TABLE2"."TABLE1_ID" = "TABLE1"."NAME" and "BDATE" is NULL where "BDATE" = '2011-12-09'
// ---------------------------------
	TEST(MY_SQL,
		Select(SqlAll().Of(TABLE1)).From(TABLE1),
		"select `TABLE1`.* from `TABLE1`"); // select `TABLE1`.* from `TABLE1`
	TEST(SQLITE3,
		Select(SqlAll().Of(TABLE1)).From(TABLE1),
		"select \"TABLE1\".* from \"TABLE1\""); // select "TABLE1".* from "TABLE1"
	TEST(ORACLE,
		Select(SqlAll().Of(TABLE1)).From(TABLE1),
		"select \"TABLE1\".* from \"TABLE1\""); // select "TABLE1".* from "TABLE1"
	TEST(MSSQL,
		Select(SqlAll().Of(TABLE1)).From(TABLE1),
		"select \"TABLE1\".* from \"TABLE1\""); // select "TABLE1".* from "TABLE1"
	TEST(PGSQL,
		Select(SqlAll().Of(TABLE1)).From(TABLE1),
		"select \"TABLE1\".* from \"TABLE1\""); // select "TABLE1".* from "TABLE1"
	TEST(FIREBIRD,
		Select(SqlAll().Of(TABLE1)).From(TABLE1),
		"select \"TABLE1\".* from \"TABLE1\""); // select "TABLE1".* from "TABLE1"
	TEST(DB2,
		Select(SqlAll().Of(TABLE1)).From(TABLE1),
		"select \"TABLE1\".* from \"TABLE1\""); // select "TABLE1".* from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(SqlTxt("TEST.*")).From(TABLE1),
		"select TEST.* from `TABLE1`"); // select TEST.* from `TABLE1`
	TEST(SQLITE3,
		Select(SqlTxt("TEST.*")).From(TABLE1),
		"select TEST.* from \"TABLE1\""); // select TEST.* from "TABLE1"
	TEST(ORACLE,
		Select(SqlTxt("TEST.*")).From(TABLE1),
		"select TEST.* from \"TABLE1\""); // select TEST.* from "TABLE1"
	TEST(MSSQL,
		Select(SqlTxt("TEST.*")).From(TABLE1),
		"select TEST.* from \"TABLE1\""); // select TEST.* from "TABLE1"
	TEST(PGSQL,
		Select(SqlTxt("TEST.*")).From(TABLE1),
		"select TEST.* from \"TABLE1\""); // select TEST.* from "TABLE1"
	TEST(FIREBIRD,
		Select(SqlTxt("TEST.*")).From(TABLE1),
		"select TEST.* from \"TABLE1\""); // select TEST.* from "TABLE1"
	TEST(DB2,
		Select(SqlTxt("TEST.*")).From(TABLE1),
		"select TEST.* from \"TABLE1\""); // select TEST.* from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(SqlTxt("123")).From(TABLE1),
		"select 123 from `TABLE1`"); // select 123 from `TABLE1`
	TEST(SQLITE3,
		Select(SqlTxt("123")).From(TABLE1),
		"select 123 from \"TABLE1\""); // select 123 from "TABLE1"
	TEST(ORACLE,
		Select(SqlTxt("123")).From(TABLE1),
		"select 123 from \"TABLE1\""); // select 123 from "TABLE1"
	TEST(MSSQL,
		Select(SqlTxt("123")).From(TABLE1),
		"select 123 from \"TABLE1\""); // select 123 from "TABLE1"
	TEST(PGSQL,
		Select(SqlTxt("123")).From(TABLE1),
		"select 123 from \"TABLE1\""); // select 123 from "TABLE1"
	TEST(FIREBIRD,
		Select(SqlTxt("123")).From(TABLE1),
		"select 123 from \"TABLE1\""); // select 123 from "TABLE1"
	TEST(DB2,
		Select(SqlTxt("123")).From(TABLE1),
		"select 123 from \"TABLE1\""); // select 123 from "TABLE1"
// ---------------------------------
	TEST(MY_SQL,
		Select(ID).From(TABLE1).Where(ID == SqlSetFrom(m)),
		"select `ID` from `TABLE1` where `ID` in ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')"); // select `ID` from `TABLE1` where `ID` in ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')
	TEST(SQLITE3,
		Select(ID).From(TABLE1).Where(ID == SqlSetFrom(m)),
		"select \"ID\" from \"TABLE1\" where \"ID\" in ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')"); // select "ID" from "TABLE1" where "ID" in ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')
	TEST(ORACLE,
		Select(ID).From(TABLE1).Where(ID == SqlSetFrom(m)),
		"select \"ID\" from \"TABLE1\" where \"ID\" in ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')"); // select "ID" from "TABLE1" where "ID" in ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')
	TEST(MSSQL,
		Select(ID).From(TABLE1).Where(ID == SqlSetFrom(m)),
		"select \"ID\" from \"TABLE1\" where \"ID\" in ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')"); // select "ID" from "TABLE1" where "ID" in ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')
	TEST(PGSQL,
		Select(ID).From(TABLE1).Where(ID == SqlSetFrom(m)),
		"select \"ID\" from \"TABLE1\" where \"ID\" in (E'0', E'1', E'2', E'3', E'4', E'5', E'6', E'7', E'8', E'9')"); // select "ID" from "TABLE1" where "ID" in (E'0', E'1', E'2', E'3', E'4', E'5', E'6', E'7', E'8', E'9')
	TEST(FIREBIRD,
		Select(ID).From(TABLE1).Where(ID == SqlSetFrom(m)),
		"select \"ID\" from \"TABLE1\" where \"ID\" in ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')"); // select "ID" from "TABLE1" where "ID" in ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')
	TEST(DB2,
		Select(ID).From(TABLE1).Where(ID == SqlSetFrom(m)),
		"select \"ID\" from \"TABLE1\" where \"ID\" in ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')"); // select "ID" from "TABLE1" where "ID" in ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')
// ---------------------------------
	TEST(MY_SQL,
		SelectAll().From(ID).InnerJoin( SelectAll().From(ID).Where(ID >= Time(2014, 1, 1, 0, 0, 0)).GroupBy(ID).AsTable(ID)),
		"select * from `ID` inner join ((select * from `ID` where `ID` >= '2014-01-01 00:00:00' group by `ID`) as `ID`)"); // select * from `ID` inner join ((select * from `ID` where `ID` >= '2014-01-01 00:00:00' group by `ID`) as `ID`)
	TEST(SQLITE3,
		SelectAll().From(ID).InnerJoin( SelectAll().From(ID).Where(ID >= Time(2014, 1, 1, 0, 0, 0)).GroupBy(ID).AsTable(ID)),
		"select * from \"ID\" inner join (select * from \"ID\" where \"ID\" >= '2014-01-01 00:00:00' group by \"ID\") as \"ID\""); // select * from "ID" inner join (select * from "ID" where "ID" >= '2014-01-01 00:00:00' group by "ID") as "ID"
	TEST(ORACLE,
		SelectAll().From(ID).InnerJoin( SelectAll().From(ID).Where(ID >= Time(2014, 1, 1, 0, 0, 0)).GroupBy(ID).AsTable(ID)),
		"select * from \"ID\" inner join ((select * from \"ID\" where \"ID\" >= to_date('2014/1/1/0', 'SYYYY/MM/DD/SSSSS') group by \"ID\") \"ID\")"); // select * from "ID" inner join ((select * from "ID" where "ID" >= to_date('2014/1/1/0', 'SYYYY/MM/DD/SSSSS') group by "ID") "ID")
	TEST(MSSQL,
		SelectAll().From(ID).InnerJoin( SelectAll().From(ID).Where(ID >= Time(2014, 1, 1, 0, 0, 0)).GroupBy(ID).AsTable(ID)),
		"select * from \"ID\" inner join (select * from \"ID\" where \"ID\" >= convert(datetime, '2014/1/1', 120) group by \"ID\") as \"ID\""); // select * from "ID" inner join (select * from "ID" where "ID" >= convert(datetime, '2014/1/1', 120) group by "ID") as "ID"
	TEST(PGSQL,
		SelectAll().From(ID).InnerJoin( SelectAll().From(ID).Where(ID >= Time(2014, 1, 1, 0, 0, 0)).GroupBy(ID).AsTable(ID)),
		"select * from \"ID\" inner join (select * from \"ID\" where \"ID\" >= timestamp '2014-01-01 00:00:00' group by \"ID\") as \"ID\""); // select * from "ID" inner join (select * from "ID" where "ID" >= timestamp '2014-01-01 00:00:00' group by "ID") as "ID"
	TEST(FIREBIRD,
		SelectAll().From(ID).InnerJoin( SelectAll().From(ID).Where(ID >= Time(2014, 1, 1, 0, 0, 0)).GroupBy(ID).AsTable(ID)),
		"select * from \"ID\" inner join ((select * from \"ID\" where \"ID\" >= '2014-01-01 00:00:00' group by \"ID\") as \"ID\")"); // select * from "ID" inner join ((select * from "ID" where "ID" >= '2014-01-01 00:00:00' group by "ID") as "ID")
	TEST(DB2,
		SelectAll().From(ID).InnerJoin( SelectAll().From(ID).Where(ID >= Time(2014, 1, 1, 0, 0, 0)).GroupBy(ID).AsTable(ID)),
		"select * from \"ID\" inner join ((select * from \"ID\" where \"ID\" >= '2014-01-01 00:00:00' group by \"ID\") as \"ID\")"); // select * from "ID" inner join ((select * from "ID" where "ID" >= '2014-01-01 00:00:00' group by "ID") as "ID")
// ---------------------------------
	TEST(MY_SQL,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE) .As(Select(PARENT_ID, Select(NAME).From(TREENODE).Where(ID == TN(PARENT_ID)).AsValue(), SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (SelectAll().From(TNSIZE)),
		"with recursive `TN`(`ID`, `NAME`, `PARENT_ID`) as (((select `ID`, `NAME`, `PARENT_ID` from `TREENODE` where `PARENT_ID` is NULL) union all (select `TREENODE`.`ID`, `TREENODE`.`NAME`, `TREENODE`.`PARENT_ID` from `TREENODE`, `TN` where `TREENODE`.`PARENT_ID` = `TN`.`ID`))), `TNSIZE` as (select `PARENT_ID`, (select `NAME` from `TREENODE` where `ID` = `TN`.`PARENT_ID`), count(*) from `TN` group by `PARENT_ID`) select * from `TNSIZE`"); // with recursive `TN`(`ID`, `NAME`, `PARENT_ID`) as (((select `ID`, `NAME`, `PARENT_ID` from `TREENODE` where `PARENT_ID` is NULL) union all (select `TREENODE`.`ID`, `TREENODE`.`NAME`, `TREENODE`.`PARENT_ID` from `TREENODE`, `TN` where `TREENODE`.`PARENT_ID` = `TN`.`ID`))), `TNSIZE` as (select `PARENT_ID`, (select `NAME` from `TREENODE` where `ID` = `TN`.`PARENT_ID`), count(*) from `TN` group by `PARENT_ID`) select * from `TNSIZE`
	TEST(SQLITE3,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE) .As(Select(PARENT_ID, Select(NAME).From(TREENODE).Where(ID == TN(PARENT_ID)).AsValue(), SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (SelectAll().From(TNSIZE)),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL union all select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"), \"TNSIZE\" as (select \"PARENT_ID\", (select \"NAME\" from \"TREENODE\" where \"ID\" = \"TN\".\"PARENT_ID\"), count(*) from \"TN\" group by \"PARENT_ID\") select * from \"TNSIZE\""); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL union all select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"), "TNSIZE" as (select "PARENT_ID", (select "NAME" from "TREENODE" where "ID" = "TN"."PARENT_ID"), count(*) from "TN" group by "PARENT_ID") select * from "TNSIZE"
	TEST(ORACLE,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE) .As(Select(PARENT_ID, Select(NAME).From(TREENODE).Where(ID == TN(PARENT_ID)).AsValue(), SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (SelectAll().From(TNSIZE)),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\" as (select \"PARENT_ID\", (select \"NAME\" from \"TREENODE\" where \"ID\" = \"TN\".\"PARENT_ID\"), count(*) from \"TN\" group by \"PARENT_ID\") select * from \"TNSIZE\""); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE" as (select "PARENT_ID", (select "NAME" from "TREENODE" where "ID" = "TN"."PARENT_ID"), count(*) from "TN" group by "PARENT_ID") select * from "TNSIZE"
	TEST(MSSQL,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE) .As(Select(PARENT_ID, Select(NAME).From(TREENODE).Where(ID == TN(PARENT_ID)).AsValue(), SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (SelectAll().From(TNSIZE)),
		"with \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\" as (select \"PARENT_ID\", (select \"NAME\" from \"TREENODE\" where \"ID\" = \"TN\".\"PARENT_ID\"), count(*) from \"TN\" group by \"PARENT_ID\") select * from \"TNSIZE\""); // with "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE" as (select "PARENT_ID", (select "NAME" from "TREENODE" where "ID" = "TN"."PARENT_ID"), count(*) from "TN" group by "PARENT_ID") select * from "TNSIZE"
	TEST(PGSQL,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE) .As(Select(PARENT_ID, Select(NAME).From(TREENODE).Where(ID == TN(PARENT_ID)).AsValue(), SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (SelectAll().From(TNSIZE)),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\" as (select \"PARENT_ID\", (select \"NAME\" from \"TREENODE\" where \"ID\" = \"TN\".\"PARENT_ID\"), count(*) from \"TN\" group by \"PARENT_ID\") select * from \"TNSIZE\""); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE" as (select "PARENT_ID", (select "NAME" from "TREENODE" where "ID" = "TN"."PARENT_ID"), count(*) from "TN" group by "PARENT_ID") select * from "TNSIZE"
	TEST(FIREBIRD,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE) .As(Select(PARENT_ID, Select(NAME).From(TREENODE).Where(ID == TN(PARENT_ID)).AsValue(), SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (SelectAll().From(TNSIZE)),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\" as (select \"PARENT_ID\", (select \"NAME\" from \"TREENODE\" where \"ID\" = \"TN\".\"PARENT_ID\"), count(*) from \"TN\" group by \"PARENT_ID\") select * from \"TNSIZE\""); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE" as (select "PARENT_ID", (select "NAME" from "TREENODE" where "ID" = "TN"."PARENT_ID"), count(*) from "TN" group by "PARENT_ID") select * from "TNSIZE"
	TEST(DB2,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE) .As(Select(PARENT_ID, Select(NAME).From(TREENODE).Where(ID == TN(PARENT_ID)).AsValue(), SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (SelectAll().From(TNSIZE)),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\" as (select \"PARENT_ID\", (select \"NAME\" from \"TREENODE\" where \"ID\" = \"TN\".\"PARENT_ID\"), count(*) from \"TN\" group by \"PARENT_ID\") select * from \"TNSIZE\""); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE" as (select "PARENT_ID", (select "NAME" from "TREENODE" where "ID" = "TN"."PARENT_ID"), count(*) from "TN" group by "PARENT_ID") select * from "TNSIZE"
// ---------------------------------
	TEST(MY_SQL,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(NAME, COUNT) .As(Select(Select(NAME).From(TREENODE).Where(ID == TN(PARENT_ID)).AsValue(), SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Insert(TABLE1)(NAME, NAME)(COL, COUNT).From(TNSIZE)),
		"with recursive `TN`(`ID`, `NAME`, `PARENT_ID`) as (((select `ID`, `NAME`, `PARENT_ID` from `TREENODE` where `PARENT_ID` is NULL) union all (select `TREENODE`.`ID`, `TREENODE`.`NAME`, `TREENODE`.`PARENT_ID` from `TREENODE`, `TN` where `TREENODE`.`PARENT_ID` = `TN`.`ID`))), `TNSIZE`(`NAME`, `COUNT`) as (select (select `NAME` from `TREENODE` where `ID` = `TN`.`PARENT_ID`), count(*) from `TN` group by `PARENT_ID`) insert into `TABLE1`(`NAME`, `COL`) select `NAME`, `COUNT` from `TNSIZE`"); // with recursive `TN`(`ID`, `NAME`, `PARENT_ID`) as (((select `ID`, `NAME`, `PARENT_ID` from `TREENODE` where `PARENT_ID` is NULL) union all (select `TREENODE`.`ID`, `TREENODE`.`NAME`, `TREENODE`.`PARENT_ID` from `TREENODE`, `TN` where `TREENODE`.`PARENT_ID` = `TN`.`ID`))), `TNSIZE`(`NAME`, `COUNT`) as (select (select `NAME` from `TREENODE` where `ID` = `TN`.`PARENT_ID`), count(*) from `TN` group by `PARENT_ID`) insert into `TABLE1`(`NAME`, `COL`) select `NAME`, `COUNT` from `TNSIZE`
	TEST(SQLITE3,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(NAME, COUNT) .As(Select(Select(NAME).From(TREENODE).Where(ID == TN(PARENT_ID)).AsValue(), SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Insert(TABLE1)(NAME, NAME)(COL, COUNT).From(TNSIZE)),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL union all select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"), \"TNSIZE\"(\"NAME\", \"COUNT\") as (select (select \"NAME\" from \"TREENODE\" where \"ID\" = \"TN\".\"PARENT_ID\"), count(*) from \"TN\" group by \"PARENT_ID\") insert into \"TABLE1\"(\"NAME\", \"COL\") select \"NAME\", \"COUNT\" from \"TNSIZE\""); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL union all select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"), "TNSIZE"("NAME", "COUNT") as (select (select "NAME" from "TREENODE" where "ID" = "TN"."PARENT_ID"), count(*) from "TN" group by "PARENT_ID") insert into "TABLE1"("NAME", "COL") select "NAME", "COUNT" from "TNSIZE"
	TEST(ORACLE,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(NAME, COUNT) .As(Select(Select(NAME).From(TREENODE).Where(ID == TN(PARENT_ID)).AsValue(), SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Insert(TABLE1)(NAME, NAME)(COL, COUNT).From(TNSIZE)),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\"(\"NAME\", \"COUNT\") as (select (select \"NAME\" from \"TREENODE\" where \"ID\" = \"TN\".\"PARENT_ID\"), count(*) from \"TN\" group by \"PARENT_ID\") insert into \"TABLE1\"(\"NAME\", \"COL\") select \"NAME\", \"COUNT\" from \"TNSIZE\""); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE"("NAME", "COUNT") as (select (select "NAME" from "TREENODE" where "ID" = "TN"."PARENT_ID"), count(*) from "TN" group by "PARENT_ID") insert into "TABLE1"("NAME", "COL") select "NAME", "COUNT" from "TNSIZE"
	TEST(MSSQL,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(NAME, COUNT) .As(Select(Select(NAME).From(TREENODE).Where(ID == TN(PARENT_ID)).AsValue(), SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Insert(TABLE1)(NAME, NAME)(COL, COUNT).From(TNSIZE)),
		"with \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\"(\"NAME\", \"COUNT\") as (select (select \"NAME\" from \"TREENODE\" where \"ID\" = \"TN\".\"PARENT_ID\"), count(*) from \"TN\" group by \"PARENT_ID\") insert into \"TABLE1\"(\"NAME\", \"COL\") select \"NAME\", \"COUNT\" from \"TNSIZE\""); // with "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE"("NAME", "COUNT") as (select (select "NAME" from "TREENODE" where "ID" = "TN"."PARENT_ID"), count(*) from "TN" group by "PARENT_ID") insert into "TABLE1"("NAME", "COL") select "NAME", "COUNT" from "TNSIZE"
	TEST(PGSQL,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(NAME, COUNT) .As(Select(Select(NAME).From(TREENODE).Where(ID == TN(PARENT_ID)).AsValue(), SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Insert(TABLE1)(NAME, NAME)(COL, COUNT).From(TNSIZE)),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\"(\"NAME\", \"COUNT\") as (select (select \"NAME\" from \"TREENODE\" where \"ID\" = \"TN\".\"PARENT_ID\"), count(*) from \"TN\" group by \"PARENT_ID\") insert into \"TABLE1\"(\"NAME\", \"COL\") select \"NAME\", \"COUNT\" from \"TNSIZE\""); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE"("NAME", "COUNT") as (select (select "NAME" from "TREENODE" where "ID" = "TN"."PARENT_ID"), count(*) from "TN" group by "PARENT_ID") insert into "TABLE1"("NAME", "COL") select "NAME", "COUNT" from "TNSIZE"
	TEST(FIREBIRD,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(NAME, COUNT) .As(Select(Select(NAME).From(TREENODE).Where(ID == TN(PARENT_ID)).AsValue(), SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Insert(TABLE1)(NAME, NAME)(COL, COUNT).From(TNSIZE)),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\"(\"NAME\", \"COUNT\") as (select (select \"NAME\" from \"TREENODE\" where \"ID\" = \"TN\".\"PARENT_ID\"), count(*) from \"TN\" group by \"PARENT_ID\") insert into \"TABLE1\"(\"NAME\", \"COL\") select \"NAME\", \"COUNT\" from \"TNSIZE\""); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE"("NAME", "COUNT") as (select (select "NAME" from "TREENODE" where "ID" = "TN"."PARENT_ID"), count(*) from "TN" group by "PARENT_ID") insert into "TABLE1"("NAME", "COL") select "NAME", "COUNT" from "TNSIZE"
	TEST(DB2,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(NAME, COUNT) .As(Select(Select(NAME).From(TREENODE).Where(ID == TN(PARENT_ID)).AsValue(), SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Insert(TABLE1)(NAME, NAME)(COL, COUNT).From(TNSIZE)),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\"(\"NAME\", \"COUNT\") as (select (select \"NAME\" from \"TREENODE\" where \"ID\" = \"TN\".\"PARENT_ID\"), count(*) from \"TN\" group by \"PARENT_ID\") insert into \"TABLE1\"(\"NAME\", \"COL\") select \"NAME\", \"COUNT\" from \"TNSIZE\""); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE"("NAME", "COUNT") as (select (select "NAME" from "TREENODE" where "ID" = "TN"."PARENT_ID"), count(*) from "TN" group by "PARENT_ID") insert into "TABLE1"("NAME", "COL") select "NAME", "COUNT" from "TNSIZE"
// ---------------------------------
	TEST(MY_SQL,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(ID, COUNT) .As(Select(ID, SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Delete(TREENODE).Where(ID == Select(ID).From(TNSIZE).Where(COUNT == 0))),
		"with recursive `TN`(`ID`, `NAME`, `PARENT_ID`) as (((select `ID`, `NAME`, `PARENT_ID` from `TREENODE` where `PARENT_ID` is NULL) union all (select `TREENODE`.`ID`, `TREENODE`.`NAME`, `TREENODE`.`PARENT_ID` from `TREENODE`, `TN` where `TREENODE`.`PARENT_ID` = `TN`.`ID`))), `TNSIZE`(`ID`, `COUNT`) as (select `ID`, count(*) from `TN` group by `PARENT_ID`) delete from `TREENODE` where `ID` in (select `ID` from `TNSIZE` where `COUNT` = 0)"); // with recursive `TN`(`ID`, `NAME`, `PARENT_ID`) as (((select `ID`, `NAME`, `PARENT_ID` from `TREENODE` where `PARENT_ID` is NULL) union all (select `TREENODE`.`ID`, `TREENODE`.`NAME`, `TREENODE`.`PARENT_ID` from `TREENODE`, `TN` where `TREENODE`.`PARENT_ID` = `TN`.`ID`))), `TNSIZE`(`ID`, `COUNT`) as (select `ID`, count(*) from `TN` group by `PARENT_ID`) delete from `TREENODE` where `ID` in (select `ID` from `TNSIZE` where `COUNT` = 0)
	TEST(SQLITE3,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(ID, COUNT) .As(Select(ID, SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Delete(TREENODE).Where(ID == Select(ID).From(TNSIZE).Where(COUNT == 0))),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL union all select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"), \"TNSIZE\"(\"ID\", \"COUNT\") as (select \"ID\", count(*) from \"TN\" group by \"PARENT_ID\") delete from \"TREENODE\" where \"ID\" in (select \"ID\" from \"TNSIZE\" where \"COUNT\" = 0)"); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL union all select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"), "TNSIZE"("ID", "COUNT") as (select "ID", count(*) from "TN" group by "PARENT_ID") delete from "TREENODE" where "ID" in (select "ID" from "TNSIZE" where "COUNT" = 0)
	TEST(ORACLE,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(ID, COUNT) .As(Select(ID, SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Delete(TREENODE).Where(ID == Select(ID).From(TNSIZE).Where(COUNT == 0))),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\"(\"ID\", \"COUNT\") as (select \"ID\", count(*) from \"TN\" group by \"PARENT_ID\") delete from \"TREENODE\" where \"ID\" in (select \"ID\" from \"TNSIZE\" where \"COUNT\" = 0)"); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE"("ID", "COUNT") as (select "ID", count(*) from "TN" group by "PARENT_ID") delete from "TREENODE" where "ID" in (select "ID" from "TNSIZE" where "COUNT" = 0)
	TEST(MSSQL,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(ID, COUNT) .As(Select(ID, SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Delete(TREENODE).Where(ID == Select(ID).From(TNSIZE).Where(COUNT == 0))),
		"with \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\"(\"ID\", \"COUNT\") as (select \"ID\", count(*) from \"TN\" group by \"PARENT_ID\") delete from \"TREENODE\" where \"ID\" in (select \"ID\" from \"TNSIZE\" where \"COUNT\" = 0)"); // with "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE"("ID", "COUNT") as (select "ID", count(*) from "TN" group by "PARENT_ID") delete from "TREENODE" where "ID" in (select "ID" from "TNSIZE" where "COUNT" = 0)
	TEST(PGSQL,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(ID, COUNT) .As(Select(ID, SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Delete(TREENODE).Where(ID == Select(ID).From(TNSIZE).Where(COUNT == 0))),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\"(\"ID\", \"COUNT\") as (select \"ID\", count(*) from \"TN\" group by \"PARENT_ID\") delete from \"TREENODE\" where \"ID\" in (select \"ID\" from \"TNSIZE\" where \"COUNT\" = 0)"); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE"("ID", "COUNT") as (select "ID", count(*) from "TN" group by "PARENT_ID") delete from "TREENODE" where "ID" in (select "ID" from "TNSIZE" where "COUNT" = 0)
	TEST(FIREBIRD,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(ID, COUNT) .As(Select(ID, SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Delete(TREENODE).Where(ID == Select(ID).From(TNSIZE).Where(COUNT == 0))),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\"(\"ID\", \"COUNT\") as (select \"ID\", count(*) from \"TN\" group by \"PARENT_ID\") delete from \"TREENODE\" where \"ID\" in (select \"ID\" from \"TNSIZE\" where \"COUNT\" = 0)"); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE"("ID", "COUNT") as (select "ID", count(*) from "TN" group by "PARENT_ID") delete from "TREENODE" where "ID" in (select "ID" from "TNSIZE" where "COUNT" = 0)
	TEST(DB2,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(ID, COUNT) .As(Select(ID, SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Delete(TREENODE).Where(ID == Select(ID).From(TNSIZE).Where(COUNT == 0))),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\"(\"ID\", \"COUNT\") as (select \"ID\", count(*) from \"TN\" group by \"PARENT_ID\") delete from \"TREENODE\" where \"ID\" in (select \"ID\" from \"TNSIZE\" where \"COUNT\" = 0)"); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE"("ID", "COUNT") as (select "ID", count(*) from "TN" group by "PARENT_ID") delete from "TREENODE" where "ID" in (select "ID" from "TNSIZE" where "COUNT" = 0)
// ---------------------------------
	TEST(MY_SQL,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(ID, COUNT) .As(Select(ID, SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Update(TABLE1)(COL, Select(COUNT).From(TNSIZE).Where(TNSIZE(ID) == TABLE1(ID)).AsValue())),
		"with recursive `TN`(`ID`, `NAME`, `PARENT_ID`) as (((select `ID`, `NAME`, `PARENT_ID` from `TREENODE` where `PARENT_ID` is NULL) union all (select `TREENODE`.`ID`, `TREENODE`.`NAME`, `TREENODE`.`PARENT_ID` from `TREENODE`, `TN` where `TREENODE`.`PARENT_ID` = `TN`.`ID`))), `TNSIZE`(`ID`, `COUNT`) as (select `ID`, count(*) from `TN` group by `PARENT_ID`) update `TABLE1` set `COL` = ((select `COUNT` from `TNSIZE` where `TNSIZE`.`ID` = `TABLE1`.`ID`))"); // with recursive `TN`(`ID`, `NAME`, `PARENT_ID`) as (((select `ID`, `NAME`, `PARENT_ID` from `TREENODE` where `PARENT_ID` is NULL) union all (select `TREENODE`.`ID`, `TREENODE`.`NAME`, `TREENODE`.`PARENT_ID` from `TREENODE`, `TN` where `TREENODE`.`PARENT_ID` = `TN`.`ID`))), `TNSIZE`(`ID`, `COUNT`) as (select `ID`, count(*) from `TN` group by `PARENT_ID`) update `TABLE1` set `COL` = ((select `COUNT` from `TNSIZE` where `TNSIZE`.`ID` = `TABLE1`.`ID`))
	TEST(SQLITE3,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(ID, COUNT) .As(Select(ID, SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Update(TABLE1)(COL, Select(COUNT).From(TNSIZE).Where(TNSIZE(ID) == TABLE1(ID)).AsValue())),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL union all select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"), \"TNSIZE\"(\"ID\", \"COUNT\") as (select \"ID\", count(*) from \"TN\" group by \"PARENT_ID\") update \"TABLE1\" set \"COL\" = ((select \"COUNT\" from \"TNSIZE\" where \"TNSIZE\".\"ID\" = \"TABLE1\".\"ID\"))"); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL union all select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"), "TNSIZE"("ID", "COUNT") as (select "ID", count(*) from "TN" group by "PARENT_ID") update "TABLE1" set "COL" = ((select "COUNT" from "TNSIZE" where "TNSIZE"."ID" = "TABLE1"."ID"))
	TEST(ORACLE,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(ID, COUNT) .As(Select(ID, SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Update(TABLE1)(COL, Select(COUNT).From(TNSIZE).Where(TNSIZE(ID) == TABLE1(ID)).AsValue())),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\"(\"ID\", \"COUNT\") as (select \"ID\", count(*) from \"TN\" group by \"PARENT_ID\") update \"TABLE1\" set \"COL\" = ((select \"COUNT\" from \"TNSIZE\" where \"TNSIZE\".\"ID\" = \"TABLE1\".\"ID\"))"); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE"("ID", "COUNT") as (select "ID", count(*) from "TN" group by "PARENT_ID") update "TABLE1" set "COL" = ((select "COUNT" from "TNSIZE" where "TNSIZE"."ID" = "TABLE1"."ID"))
	TEST(MSSQL,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(ID, COUNT) .As(Select(ID, SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Update(TABLE1)(COL, Select(COUNT).From(TNSIZE).Where(TNSIZE(ID) == TABLE1(ID)).AsValue())),
		"with \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\"(\"ID\", \"COUNT\") as (select \"ID\", count(*) from \"TN\" group by \"PARENT_ID\") update \"TABLE1\" set \"COL\" = ((select \"COUNT\" from \"TNSIZE\" where \"TNSIZE\".\"ID\" = \"TABLE1\".\"ID\"))"); // with "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE"("ID", "COUNT") as (select "ID", count(*) from "TN" group by "PARENT_ID") update "TABLE1" set "COL" = ((select "COUNT" from "TNSIZE" where "TNSIZE"."ID" = "TABLE1"."ID"))
	TEST(PGSQL,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(ID, COUNT) .As(Select(ID, SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Update(TABLE1)(COL, Select(COUNT).From(TNSIZE).Where(TNSIZE(ID) == TABLE1(ID)).AsValue())),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\"(\"ID\", \"COUNT\") as (select \"ID\", count(*) from \"TN\" group by \"PARENT_ID\") update \"TABLE1\" set \"COL\" = ((select \"COUNT\" from \"TNSIZE\" where \"TNSIZE\".\"ID\" = \"TABLE1\".\"ID\"))"); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE"("ID", "COUNT") as (select "ID", count(*) from "TN" group by "PARENT_ID") update "TABLE1" set "COL" = ((select "COUNT" from "TNSIZE" where "TNSIZE"."ID" = "TABLE1"."ID"))
	TEST(FIREBIRD,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(ID, COUNT) .As(Select(ID, SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Update(TABLE1)(COL, Select(COUNT).From(TNSIZE).Where(TNSIZE(ID) == TABLE1(ID)).AsValue())),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\"(\"ID\", \"COUNT\") as (select \"ID\", count(*) from \"TN\" group by \"PARENT_ID\") update \"TABLE1\" set \"COL\" = ((select \"COUNT\" from \"TNSIZE\" where \"TNSIZE\".\"ID\" = \"TABLE1\".\"ID\"))"); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE"("ID", "COUNT") as (select "ID", count(*) from "TN" group by "PARENT_ID") update "TABLE1" set "COL" = ((select "COUNT" from "TNSIZE" where "TNSIZE"."ID" = "TABLE1"."ID"))
	TEST(DB2,
		WithRecursive(TN)(ID, NAME, PARENT_ID) .As(Select(ID, NAME, PARENT_ID).From(TREENODE).Where(IsNull(PARENT_ID)) + Select(TREENODE(ID, NAME, PARENT_ID)).From(TREENODE, TN) .Where(TREENODE(PARENT_ID) == TN(ID))) .With(TNSIZE)(ID, COUNT) .As(Select(ID, SqlCountRows()) .From(TN) .GroupBy(PARENT_ID)) (Update(TABLE1)(COL, Select(COUNT).From(TNSIZE).Where(TNSIZE(ID) == TABLE1(ID)).AsValue())),
		"with recursive \"TN\"(\"ID\", \"NAME\", \"PARENT_ID\") as (((select \"ID\", \"NAME\", \"PARENT_ID\" from \"TREENODE\" where \"PARENT_ID\" is NULL) union all (select \"TREENODE\".\"ID\", \"TREENODE\".\"NAME\", \"TREENODE\".\"PARENT_ID\" from \"TREENODE\", \"TN\" where \"TREENODE\".\"PARENT_ID\" = \"TN\".\"ID\"))), \"TNSIZE\"(\"ID\", \"COUNT\") as (select \"ID\", count(*) from \"TN\" group by \"PARENT_ID\") update \"TABLE1\" set \"COL\" = ((select \"COUNT\" from \"TNSIZE\" where \"TNSIZE\".\"ID\" = \"TABLE1\".\"ID\"))"); // with recursive "TN"("ID", "NAME", "PARENT_ID") as (((select "ID", "NAME", "PARENT_ID" from "TREENODE" where "PARENT_ID" is NULL) union all (select "TREENODE"."ID", "TREENODE"."NAME", "TREENODE"."PARENT_ID" from "TREENODE", "TN" where "TREENODE"."PARENT_ID" = "TN"."ID"))), "TNSIZE"("ID", "COUNT") as (select "ID", count(*) from "TN" group by "PARENT_ID") update "TABLE1" set "COL" = ((select "COUNT" from "TNSIZE" where "TNSIZE"."ID" = "TABLE1"."ID"))
// ---------------------------------
	TEST(MY_SQL,
		Temporary("TT").As(Select(NAME, LASTNAME).From(TABLE1).Where(COLUMN1 == 12)),
		"create temporary table `TT` as (select `NAME`, `LASTNAME` from `TABLE1` where `COLUMN1` = 12)"); // create temporary table `TT` as (select `NAME`, `LASTNAME` from `TABLE1` where `COLUMN1` = 12)
	TEST(SQLITE3,
		Temporary("TT").As(Select(NAME, LASTNAME).From(TABLE1).Where(COLUMN1 == 12)),
		"create temporary table \"TT\" as (select \"NAME\", \"LASTNAME\" from \"TABLE1\" where \"COLUMN1\" = 12)"); // create temporary table "TT" as (select "NAME", "LASTNAME" from "TABLE1" where "COLUMN1" = 12)
	TEST(ORACLE,
		Temporary("TT").As(Select(NAME, LASTNAME).From(TABLE1).Where(COLUMN1 == 12)),
		"create global temporary table \"TT\" on commit preserve rows as (select \"NAME\", \"LASTNAME\" from \"TABLE1\" where \"COLUMN1\" = 12)"); // create global temporary table "TT" on commit preserve rows as (select "NAME", "LASTNAME" from "TABLE1" where "COLUMN1" = 12)
	TEST(MSSQL,
		Temporary("TT").As(Select(NAME, LASTNAME).From(TABLE1).Where(COLUMN1 == 12)),
		"create temporary table \"TT\" as (select \"NAME\", \"LASTNAME\" from \"TABLE1\" where \"COLUMN1\" = 12)"); // create temporary table "TT" as (select "NAME", "LASTNAME" from "TABLE1" where "COLUMN1" = 12)
	TEST(PGSQL,
		Temporary("TT").As(Select(NAME, LASTNAME).From(TABLE1).Where(COLUMN1 == 12)),
		"create temporary table \"TT\" as (select \"NAME\", \"LASTNAME\" from \"TABLE1\" where \"COLUMN1\" = 12)"); // create temporary table "TT" as (select "NAME", "LASTNAME" from "TABLE1" where "COLUMN1" = 12)
	TEST(FIREBIRD,
		Temporary("TT").As(Select(NAME, LASTNAME).From(TABLE1).Where(COLUMN1 == 12)),
		"create temporary table \"TT\" as (select \"NAME\", \"LASTNAME\" from \"TABLE1\" where \"COLUMN1\" = 12)"); // create temporary table "TT" as (select "NAME", "LASTNAME" from "TABLE1" where "COLUMN1" = 12)
	TEST(DB2,
		Temporary("TT").As(Select(NAME, LASTNAME).From(TABLE1).Where(COLUMN1 == 12)),
		"create temporary table \"TT\" as (select \"NAME\", \"LASTNAME\" from \"TABLE1\" where \"COLUMN1\" = 12)"); // create temporary table "TT" as (select "NAME", "LASTNAME" from "TABLE1" where "COLUMN1" = 12)
// ---------------------------------
	TEST(MY_SQL,
		Update(TABLE1)(COLUMN1, TABLE2(COLUMN2)).From(TABLE2).On(TABLE1(ID) == TABLE2(TABLE1_ID)),
		"update `TABLE1` set `COLUMN1` = `TABLE2`.`COLUMN2`from `TABLE2` on `TABLE1`.`ID` = `TABLE2`.`TABLE1_ID`"); // update `TABLE1` set `COLUMN1` = `TABLE2`.`COLUMN2`from `TABLE2` on `TABLE1`.`ID` = `TABLE2`.`TABLE1_ID`
	TEST(SQLITE3,
		Update(TABLE1)(COLUMN1, TABLE2(COLUMN2)).From(TABLE2).On(TABLE1(ID) == TABLE2(TABLE1_ID)),
		"update \"TABLE1\" set \"COLUMN1\" = \"TABLE2\".\"COLUMN2\"from \"TABLE2\" on \"TABLE1\".\"ID\" = \"TABLE2\".\"TABLE1_ID\""); // update "TABLE1" set "COLUMN1" = "TABLE2"."COLUMN2"from "TABLE2" on "TABLE1"."ID" = "TABLE2"."TABLE1_ID"
	TEST(ORACLE,
		Update(TABLE1)(COLUMN1, TABLE2(COLUMN2)).From(TABLE2).On(TABLE1(ID) == TABLE2(TABLE1_ID)),
		"update \"TABLE1\" set \"COLUMN1\" = \"TABLE2\".\"COLUMN2\"from \"TABLE2\" on \"TABLE1\".\"ID\" = \"TABLE2\".\"TABLE1_ID\""); // update "TABLE1" set "COLUMN1" = "TABLE2"."COLUMN2"from "TABLE2" on "TABLE1"."ID" = "TABLE2"."TABLE1_ID"
	TEST(MSSQL,
		Update(TABLE1)(COLUMN1, TABLE2(COLUMN2)).From(TABLE2).On(TABLE1(ID) == TABLE2(TABLE1_ID)),
		"update \"TABLE1\" set \"COLUMN1\" = \"TABLE2\".\"COLUMN2\"from \"TABLE2\" on \"TABLE1\".\"ID\" = \"TABLE2\".\"TABLE1_ID\""); // update "TABLE1" set "COLUMN1" = "TABLE2"."COLUMN2"from "TABLE2" on "TABLE1"."ID" = "TABLE2"."TABLE1_ID"
	TEST(PGSQL,
		Update(TABLE1)(COLUMN1, TABLE2(COLUMN2)).From(TABLE2).On(TABLE1(ID) == TABLE2(TABLE1_ID)),
		"update \"TABLE1\" set \"COLUMN1\" = \"TABLE2\".\"COLUMN2\"from \"TABLE2\" on \"TABLE1\".\"ID\" = \"TABLE2\".\"TABLE1_ID\""); // update "TABLE1" set "COLUMN1" = "TABLE2"."COLUMN2"from "TABLE2" on "TABLE1"."ID" = "TABLE2"."TABLE1_ID"
	TEST(FIREBIRD,
		Update(TABLE1)(COLUMN1, TABLE2(COLUMN2)).From(TABLE2).On(TABLE1(ID) == TABLE2(TABLE1_ID)),
		"update \"TABLE1\" set \"COLUMN1\" = \"TABLE2\".\"COLUMN2\"from \"TABLE2\" on \"TABLE1\".\"ID\" = \"TABLE2\".\"TABLE1_ID\""); // update "TABLE1" set "COLUMN1" = "TABLE2"."COLUMN2"from "TABLE2" on "TABLE1"."ID" = "TABLE2"."TABLE1_ID"
	TEST(DB2,
		Update(TABLE1)(COLUMN1, TABLE2(COLUMN2)).From(TABLE2).On(TABLE1(ID) == TABLE2(TABLE1_ID)),
		"update \"TABLE1\" set \"COLUMN1\" = \"TABLE2\".\"COLUMN2\"from \"TABLE2\" on \"TABLE1\".\"ID\" = \"TABLE2\".\"TABLE1_ID\""); // update "TABLE1" set "COLUMN1" = "TABLE2"."COLUMN2"from "TABLE2" on "TABLE1"."ID" = "TABLE2"."TABLE1_ID"
// ---------------------------------
	TEST(MY_SQL,
		Select(A(SqlAll())).From(USERS.As(A)) .InnerJoin( Select(Count(SqlAll()), USERNAME, EMAIL).From(USERS) .GroupBy(USERNAME, EMAIL).Having(Count(SqlAll()) > 1) .AsTable(B) ) .On(A(USERNAME) == B(USERNAME) && A(EMAIL) == B(EMAIL)),
		"select `A`.* from `USERS` `A` inner join ((select count(*), `USERNAME`, `EMAIL` from `USERS` group by `USERNAME`, `EMAIL` having count(*) > 1) as `B`) on `A`.`USERNAME` = `B`.`USERNAME` and `A`.`EMAIL` = `B`.`EMAIL`"); // select `A`.* from `USERS` `A` inner join ((select count(*), `USERNAME`, `EMAIL` from `USERS` group by `USERNAME`, `EMAIL` having count(*) > 1) as `B`) on `A`.`USERNAME` = `B`.`USERNAME` and `A`.`EMAIL` = `B`.`EMAIL`
	TEST(SQLITE3,
		Select(A(SqlAll())).From(USERS.As(A)) .InnerJoin( Select(Count(SqlAll()), USERNAME, EMAIL).From(USERS) .GroupBy(USERNAME, EMAIL).Having(Count(SqlAll()) > 1) .AsTable(B) ) .On(A(USERNAME) == B(USERNAME) && A(EMAIL) == B(EMAIL)),
		"select \"A\".* from \"USERS\" \"A\" inner join (select count(*), \"USERNAME\", \"EMAIL\" from \"USERS\" group by \"USERNAME\", \"EMAIL\" having count(*) > 1) as \"B\" on \"A\".\"USERNAME\" = \"B\".\"USERNAME\" and \"A\".\"EMAIL\" = \"B\".\"EMAIL\""); // select "A".* from "USERS" "A" inner join (select count(*), "USERNAME", "EMAIL" from "USERS" group by "USERNAME", "EMAIL" having count(*) > 1) as "B" on "A"."USERNAME" = "B"."USERNAME" and "A"."EMAIL" = "B"."EMAIL"
	TEST(ORACLE,
		Select(A(SqlAll())).From(USERS.As(A)) .InnerJoin( Select(Count(SqlAll()), USERNAME, EMAIL).From(USERS) .GroupBy(USERNAME, EMAIL).Having(Count(SqlAll()) > 1) .AsTable(B) ) .On(A(USERNAME) == B(USERNAME) && A(EMAIL) == B(EMAIL)),
		"select \"A\".* from \"USERS\" \"A\" inner join ((select count(*), \"USERNAME\", \"EMAIL\" from \"USERS\" group by \"USERNAME\", \"EMAIL\" having count(*) > 1) \"B\") on \"A\".\"USERNAME\" = \"B\".\"USERNAME\" and \"A\".\"EMAIL\" = \"B\".\"EMAIL\""); // select "A".* from "USERS" "A" inner join ((select count(*), "USERNAME", "EMAIL" from "USERS" group by "USERNAME", "EMAIL" having count(*) > 1) "B") on "A"."USERNAME" = "B"."USERNAME" and "A"."EMAIL" = "B"."EMAIL"
	TEST(MSSQL,
		Select(A(SqlAll())).From(USERS.As(A)) .InnerJoin( Select(Count(SqlAll()), USERNAME, EMAIL).From(USERS) .GroupBy(USERNAME, EMAIL).Having(Count(SqlAll()) > 1) .AsTable(B) ) .On(A(USERNAME) == B(USERNAME) && A(EMAIL) == B(EMAIL)),
		"select \"A\".* from \"USERS\" as \"A\" inner join (select count(*), \"USERNAME\", \"EMAIL\" from \"USERS\" group by \"USERNAME\", \"EMAIL\" having count(*) > 1) as \"B\" on \"A\".\"USERNAME\" = \"B\".\"USERNAME\" and \"A\".\"EMAIL\" = \"B\".\"EMAIL\""); // select "A".* from "USERS" as "A" inner join (select count(*), "USERNAME", "EMAIL" from "USERS" group by "USERNAME", "EMAIL" having count(*) > 1) as "B" on "A"."USERNAME" = "B"."USERNAME" and "A"."EMAIL" = "B"."EMAIL"
	TEST(PGSQL,
		Select(A(SqlAll())).From(USERS.As(A)) .InnerJoin( Select(Count(SqlAll()), USERNAME, EMAIL).From(USERS) .GroupBy(USERNAME, EMAIL).Having(Count(SqlAll()) > 1) .AsTable(B) ) .On(A(USERNAME) == B(USERNAME) && A(EMAIL) == B(EMAIL)),
		"select \"A\".* from \"USERS\" as \"A\" inner join (select count(*), \"USERNAME\", \"EMAIL\" from \"USERS\" group by \"USERNAME\", \"EMAIL\" having count(*) > 1) as \"B\" on \"A\".\"USERNAME\" = \"B\".\"USERNAME\" and \"A\".\"EMAIL\" = \"B\".\"EMAIL\""); // select "A".* from "USERS" as "A" inner join (select count(*), "USERNAME", "EMAIL" from "USERS" group by "USERNAME", "EMAIL" having count(*) > 1) as "B" on "A"."USERNAME" = "B"."USERNAME" and "A"."EMAIL" = "B"."EMAIL"
	TEST(FIREBIRD,
		Select(A(SqlAll())).From(USERS.As(A)) .InnerJoin( Select(Count(SqlAll()), USERNAME, EMAIL).From(USERS) .GroupBy(USERNAME, EMAIL).Having(Count(SqlAll()) > 1) .AsTable(B) ) .On(A(USERNAME) == B(USERNAME) && A(EMAIL) == B(EMAIL)),
		"select \"A\".* from \"USERS\" \"A\" inner join ((select count(*), \"USERNAME\", \"EMAIL\" from \"USERS\" group by \"USERNAME\", \"EMAIL\" having count(*) > 1) as \"B\") on \"A\".\"USERNAME\" = \"B\".\"USERNAME\" and \"A\".\"EMAIL\" = \"B\".\"EMAIL\""); // select "A".* from "USERS" "A" inner join ((select count(*), "USERNAME", "EMAIL" from "USERS" group by "USERNAME", "EMAIL" having count(*) > 1) as "B") on "A"."USERNAME" = "B"."USERNAME" and "A"."EMAIL" = "B"."EMAIL"
	TEST(DB2,
		Select(A(SqlAll())).From(USERS.As(A)) .InnerJoin( Select(Count(SqlAll()), USERNAME, EMAIL).From(USERS) .GroupBy(USERNAME, EMAIL).Having(Count(SqlAll()) > 1) .AsTable(B) ) .On(A(USERNAME) == B(USERNAME) && A(EMAIL) == B(EMAIL)),
		"select \"A\".* from \"USERS\" \"A\" inner join ((select count(*), \"USERNAME\", \"EMAIL\" from \"USERS\" group by \"USERNAME\", \"EMAIL\" having count(*) > 1) as \"B\") on \"A\".\"USERNAME\" = \"B\".\"USERNAME\" and \"A\".\"EMAIL\" = \"B\".\"EMAIL\""); // select "A".* from "USERS" "A" inner join ((select count(*), "USERNAME", "EMAIL" from "USERS" group by "USERNAME", "EMAIL" having count(*) > 1) as "B") on "A"."USERNAME" = "B"."USERNAME" and "A"."EMAIL" = "B"."EMAIL"
// ---------------------------------
