#include <Firebird/Firebird.h>

using namespace Upp;


void TestInt(FBSession& s, const String& field_type)
{
	int i;
	Sql stmt(s);
	stmt.SetStatement(
	    "DROP TABLE test_long_table"
	);
	stmt.Run();
	                  
	// Data definition statement.
	stmt.SetStatement(
		" CREATE TABLE test_long_table( \n"
		"   id integer, \n"
		"   first_field " + field_type + " DEFAULT NULL, \n"
		"   second_field " + field_type + " DEFAULT NULL \n"
		")"
	);
	stmt.Run();

	stmt.SetStatement(
		" INSERT INTO test_long_table \n"
		" VALUES(?, ?, ?)"
	);
		
	for (i = 0; i < 100; ++i)
	{
		stmt.SetParam(0, i);
		stmt.SetParam(1, i * 10);
		stmt.SetParam(2, i * 100);
		stmt.RunX();
	}

	stmt.SetStatement(
		" SET TRANSACTION WAIT ISOLATION LEVEL READ COMMITTED"
	);
	stmt.Run();
	
	int _0 = -1;
	int _1 = -1;
	int _2 = -1;
	
	Ref r0(_0);
	Ref r1(_1);
	Ref r2(_2);

	// SELECT without parameters.
	stmt.SetStatement(
		" SELECT * FROM test_long_table ORDER BY id"
	);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(_1 == i * 10);
		ASSERT(_2 == i * 100);
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());

	// Rerun SELECT.
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		ASSERT(stmt[0] == i);
		ASSERT(stmt[1] == i * 10);
		ASSERT(stmt[2] == i * 100);
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());

	// SELECT with parameters.
	stmt.SetStatement(
		" SELECT * FROM test_long_table WHERE id < ? ORDER BY id"
	);
	stmt.SetParam(0, 50);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(_1 == i * 10);
		ASSERT(_2 == i * 100);
		++i;
	}
	ASSERT(i == 50);
	ASSERT(!stmt.Fetch());

	// Rerun SELECT with parameters.
	stmt.SetParam(0, 25);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(_1 == i * 10);
		ASSERT(_2 == i * 100);
		++i;
	}
	ASSERT(i == 25);
	ASSERT(!stmt.Fetch());

	// Delete all records.
	stmt.SetStatement(
		" DELETE FROM test_long_table"
	);
	stmt.Run();

	// Insert data again.
	stmt.SetStatement(
		" INSERT INTO test_long_table \n"
		" VALUES(?, ?, ?)"
	);
		
	for (i = 0; i < 100; ++i)
	{
		stmt.SetParam(0, i);
		if (i % 2 == 0) {
			stmt.SetParam(1, Null);
			stmt.SetParam(2, i * 100);
		} else {
			stmt.SetParam(1, i * 10);
			stmt.SetParam(2, Null);
		}
		stmt.RunX();
	}
	
	// Run SELECT.
	i = 0;
	const Value vNull(Null);
	
	stmt.SetStatement(
		" SELECT * FROM test_long_table ORDER BY id"
	);
	stmt.Run();
	
	while (stmt.Fetch())
	{
		ASSERT(stmt[0] == i);
		if (i % 2 == 0) {
			ASSERT(stmt[1] == vNull);
			ASSERT(stmt[2] == i * 100);
		} else {
			ASSERT(stmt[1] == i * 10);
			ASSERT(stmt[2] == vNull);
		}
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());
}

void TestString(FBSession& s)
{
	int i;
	Sql stmt(s);
	stmt.SetStatement(
	    "DROP TABLE test_str_table"
	);
	stmt.Run();
	                  
	// Data definition statement.
	stmt.SetStatement(
		" CREATE TABLE test_str_table( \n"
		"   id integer, \n"
		"   first_field VARCHAR(255) DEFAULT NULL, \n"
		"   second_field VARCHAR(255) DEFAULT NULL \n"
		")"
	);
	stmt.Run();

	stmt.SetStatement(
		" INSERT INTO test_str_table \n"
		" VALUES(?, ?, ?)"
	);
		
	for (i = 0; i < 100; ++i)
	{
		stmt.SetParam(0, i);
		stmt.SetParam(1, FormatInt(i * 10));
		stmt.SetParam(2, FormatInt(i * 100));
		stmt.RunX();
	}

	int _0 = -1;
	String _1;
	String _2;
	
	Ref r0(_0);
	Ref r1(_1);
	Ref r2(_2);

	// SELECT without parameters.
	stmt.SetStatement(
		" SELECT * FROM test_str_table ORDER BY id"
	);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(_1 == FormatInt(i * 10));
		ASSERT(_2 == FormatInt(i * 100));
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());

	// Rerun SELECT.
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		ASSERT(stmt[0] == i);
		ASSERT(stmt[1] == FormatInt(i * 10));
		ASSERT(stmt[2] == FormatInt(i * 100));
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());

	// SELECT with parameters.
	stmt.SetStatement(
		" SELECT * FROM test_str_table WHERE id < ? ORDER BY id"
	);
	stmt.SetParam(0, 50);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(_1 == FormatInt(i * 10));
		ASSERT(_2 == FormatInt(i * 100));
		++i;
	}
	ASSERT(i == 50);
	ASSERT(!stmt.Fetch());

	// Rerun SELECT with parameters.
	stmt.SetParam(0, 25);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(_1 == FormatInt(i * 10));
		ASSERT(_2 == FormatInt(i * 100));
		++i;
	}
	ASSERT(i == 25);
	ASSERT(!stmt.Fetch());

	// Delete all records.
	stmt.SetStatement(
		" DELETE FROM test_str_table"
	);
	stmt.Run();

	// Insert data again.
	stmt.SetStatement(
		" INSERT INTO test_str_table \n"
		" VALUES(?, ?, ?)"
	);
		
	for (i = 0; i < 100; ++i)
	{
		stmt.SetParam(0, i);
		if (i % 2 == 0) {
			stmt.SetParam(1, Null);
			stmt.SetParam(2, FormatInt(i * 100));
		} else {
			stmt.SetParam(1, FormatInt(i * 10));
			stmt.SetParam(2, Null);
		}
		stmt.RunX();
	}
	
	// Run SELECT.
	i = 0;
	const Value vNull(Null);
	
	stmt.SetStatement(
		" SELECT * FROM test_str_table ORDER BY id"
	);
	stmt.Run();
	
	while (stmt.Fetch())
	{
		ASSERT(stmt[0] == i);
		if (i % 2 == 0) {
			ASSERT(stmt[1] == vNull);
			ASSERT(stmt[2] == FormatInt(i * 100));
		} else {
			ASSERT(stmt[1] == FormatInt(i * 10));
			ASSERT(stmt[2] == vNull);
		}
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());
}

void TestBlob(FBSession& s)
{
	int i;
	Sql stmt(s);
	stmt.SetStatement(
	    "DROP TABLE test_blob_table"
	);
	stmt.Run();
	                  
	// Data definition statement.
	stmt.SetStatement(
		" CREATE TABLE test_blob_table( \n"
		"   id integer, \n"
		"   first_field BLOB DEFAULT NULL, \n"
		"   second_field BLOB DEFAULT NULL \n"
		")"
	);
	stmt.Run();

	stmt.SetStatement(
		" INSERT INTO test_blob_table \n"
		" VALUES(?, ?, ?)"
	);
		
	for (i = 0; i < 100; ++i)
	{
		stmt.SetParam(0, i);
		stmt.SetParam(1, FormatInt(i * 10));
		stmt.SetParam(2, FormatInt(i * 100));
		stmt.RunX();
	}

	int _0 = -1;
	String _1;
	String _2;
	
	Ref r0(_0);
	Ref r1(_1);
	Ref r2(_2);

	// SELECT without parameters.
	stmt.SetStatement(
		" SELECT * FROM test_blob_table ORDER BY id"
	);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(_1 == FormatInt(i * 10));
		ASSERT(_2 == FormatInt(i * 100));
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());

	// Rerun SELECT.
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		ASSERT(stmt[0] == i);
		ASSERT(stmt[1] == FormatInt(i * 10));
		ASSERT(stmt[2] == FormatInt(i * 100));
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());

	// SELECT with parameters.
	stmt.SetStatement(
		" SELECT * FROM test_blob_table WHERE id < ? ORDER BY id"
	);
	stmt.SetParam(0, 50);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(_1 == FormatInt(i * 10));
		ASSERT(_2 == FormatInt(i * 100));
		++i;
	}
	ASSERT(i == 50);
	ASSERT(!stmt.Fetch());

	// Rerun SELECT with parameters.
	stmt.SetParam(0, 25);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(_1 == FormatInt(i * 10));
		ASSERT(_2 == FormatInt(i * 100));
		++i;
	}
	ASSERT(i == 25);
	ASSERT(!stmt.Fetch());

	// Delete all records.
	stmt.SetStatement(
		" DELETE FROM test_blob_table"
	);
	stmt.Run();

	// Insert data again.
	stmt.SetStatement(
		" INSERT INTO test_blob_table \n"
		" VALUES(?, ?, ?)"
	);
		
	for (i = 0; i < 100; ++i)
	{
		stmt.SetParam(0, i);
		if (i % 2 == 0) {
			stmt.SetParam(1, Null);
			stmt.SetParam(2, FormatInt(i * 100));
		} else {
			stmt.SetParam(1, FormatInt(i * 10));
			stmt.SetParam(2, Null);
		}
		stmt.RunX();
	}
	
	// Run SELECT.
	i = 0;
	const Value vNull(Null);
	
	stmt.SetStatement(
		" SELECT * FROM test_blob_table ORDER BY id"
	);
	stmt.Run();
	
	while (stmt.Fetch())
	{
		ASSERT(stmt[0] == i);
		if (i % 2 == 0) {
			ASSERT(stmt[1] == vNull);
			ASSERT(stmt[2] == FormatInt(i * 100));
		} else {
			ASSERT(stmt[1] == FormatInt(i * 10));
			ASSERT(stmt[2] == vNull);
		}
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());
}

inline
bool AreClose(double actual, double expected, double tolerance)
{
	return (actual >= (expected - tolerance)) && (actual <= (expected + tolerance));
}

void TestDouble(FBSession& s, const String& field_type)
{
	int i;
	Sql stmt(s);
	stmt.SetStatement(
	    "DROP TABLE test_double_table"
	);
	stmt.Run();
	                  
	// Data definition statement.
	stmt.SetStatement(
		" CREATE TABLE test_double_table( \n"
		"   id integer, \n"
		"   first_field " + field_type + " DEFAULT NULL, \n"
		"   second_field " + field_type + " DEFAULT NULL \n"
		")"
	);
	stmt.Run();

	stmt.SetStatement(
		" INSERT INTO test_double_table \n"
		" VALUES(?, ?, ?)"
	);
		
	for (i = 0; i < 100; ++i)
	{
		stmt.SetParam(0, i);
		stmt.SetParam(1, i * 10.01);
		stmt.SetParam(2, i * 100.001);
		stmt.RunX();
	}

	int _0 = -1;
	double _1 = -1.0;
	double _2 = -1.0;
	
	Ref r0(_0);
	Ref r1(_1);
	Ref r2(_2);

	// SELECT without parameters.
	stmt.SetStatement(
		" SELECT * FROM test_double_table ORDER BY id"
	);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(AreClose(_1, i * 10.01, 0.005));
		ASSERT(AreClose(_2, i * 100.001, 0.0005));
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());

	// Rerun SELECT.
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		ASSERT(stmt[0] == i);
		ASSERT(AreClose(stmt[1], i * 10.01, 0.005));
		ASSERT(AreClose(stmt[2], i * 100.001, 0.0005));
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());

	// SELECT with parameters.
	stmt.SetStatement(
		" SELECT * FROM test_double_table WHERE id < ? ORDER BY id"
	);
	stmt.SetParam(0, 50);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(AreClose(_1, i * 10.01, 0.005));
		ASSERT(AreClose(_2, i * 100.001, 0.0005));
		++i;
	}
	ASSERT(i == 50);
	ASSERT(!stmt.Fetch());

	// Rerun SELECT with parameters.
	stmt.SetParam(0, 25);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(AreClose(_1, i * 10.01, 0.005));
		ASSERT(AreClose(_2, i * 100.001, 0.0005));
		++i;
	}
	ASSERT(i == 25);
	ASSERT(!stmt.Fetch());

	// Delete all records.
	stmt.SetStatement(
		" DELETE FROM test_double_table"
	);
	stmt.Run();

	// Insert data again.
	stmt.SetStatement(
		" INSERT INTO test_double_table \n"
		" VALUES(?, ?, ?)"
	);
		
	const Value vNull(Null);
	for (i = 0; i < 100; ++i)
	{
		stmt.SetParam(0, i);
		if (i % 2 == 0) {
			stmt.SetParam(1, vNull);
			stmt.SetParam(2, i * 100.001);
		} else {
			stmt.SetParam(1, i * 10.01);
			stmt.SetParam(2, vNull);
		}
		stmt.RunX();
	}
	
	// Run SELECT.
	i = 0;
	
	stmt.SetStatement(
		" SELECT * FROM test_double_table ORDER BY id"
	);
	stmt.Run();
	
	while (stmt.Fetch())
	{
		ASSERT(stmt[0] == i);
		if (i % 2 == 0) {
			ASSERT(stmt[1] == vNull);
			ASSERT(AreClose(stmt[2], i * 100.001, 0.0005));
		} else {
			ASSERT(AreClose(stmt[1], i * 10.01, 0.005));
			ASSERT(stmt[2] == vNull);
		}
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());
}

void TestDatetime(FBSession& s)
{
	int i;
	Sql stmt(s);
	stmt.SetStatement(
	    "DROP TABLE test_datetime_table"
	);
	stmt.Run();
	                  
	// Data definition statement.
	stmt.SetStatement(
		" CREATE TABLE test_datetime_table( \n"
		"   id integer, \n"
		"   first_field TIMESTAMP DEFAULT NULL, \n"
		"   second_field TIMESTAMP DEFAULT NULL \n"
		")"
	);
	stmt.Run();

	stmt.SetStatement(
		" INSERT INTO test_datetime_table \n"
		" VALUES(?, ?, ?)"
	);
		
	for (i = 0; i < 100; ++i)
	{
		stmt.SetParam(0, i);
		stmt.SetParam(1, Time(i+1900+10,  i%12+1, i%28+1, i%24, i%60, i%60));
		stmt.SetParam(2, Time(i+1900+100, i%12+1, i%28+1, i%24, i%60, i%60));
		stmt.RunX();
	}

	int _0 = -1;
	Time _1;
	Time _2;
	
	Ref r0(_0);
	Ref r1(_1);
	Ref r2(_2);

	// SELECT without parameters.
	stmt.SetStatement(
		" SELECT * FROM test_datetime_table ORDER BY id"
	);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(_1 == Time(i+1900+10,  i%12+1, i%28+1, i%24, i%60, i%60));
		ASSERT(_2 == Time(i+1900+100, i%12+1, i%28+1, i%24, i%60, i%60));
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());

	// Rerun SELECT.
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		ASSERT(stmt[0] == i);
		ASSERT(stmt[1] == Time(i+1900+10,  i%12+1, i%28+1, i%24, i%60, i%60));
		ASSERT(stmt[2] == Time(i+1900+100, i%12+1, i%28+1, i%24, i%60, i%60));
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());

	// SELECT with parameters.
	stmt.SetStatement(
		" SELECT * FROM test_datetime_table WHERE id < ? ORDER BY id"
	);
	stmt.SetParam(0, 50);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(_1 == Time(i+1900+10,  i%12+1, i%28+1, i%24, i%60, i%60));
		ASSERT(_2 == Time(i+1900+100, i%12+1, i%28+1, i%24, i%60, i%60));
		++i;
	}
	ASSERT(i == 50);
	ASSERT(!stmt.Fetch());

	// Rerun SELECT with parameters.
	stmt.SetParam(0, 25);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(_1 == Time(i+1900+10,  i%12+1, i%28+1, i%24, i%60, i%60));
		ASSERT(_2 == Time(i+1900+100, i%12+1, i%28+1, i%24, i%60, i%60));
		++i;
	}
	ASSERT(i == 25);
	ASSERT(!stmt.Fetch());

	// Delete all records.
	stmt.SetStatement(
		" DELETE FROM test_datetime_table"
	);
	stmt.Run();

	// Insert data again.
	stmt.SetStatement(
		" INSERT INTO test_datetime_table \n"
		" VALUES(?, ?, ?)"
	);
		
	for (i = 0; i < 100; ++i)
	{
		stmt.SetParam(0, i);
		if (i % 2 == 0) {
			stmt.SetParam(1, Null);
			stmt.SetParam(2, Time(i+1900+100, i%12+1, i%28+1, i%24, i%60, i%60));
		} else {
			stmt.SetParam(1, Time(i+1900+10,  i%12+1, i%28+1, i%24, i%60, i%60));
			stmt.SetParam(2, Null);
		}
		stmt.RunX();
	}
	
	// Run SELECT.
	i = 0;
	const Value vNull(Null);
	
	stmt.SetStatement(
		" SELECT * FROM test_datetime_table ORDER BY id"
	);
	stmt.Run();
	
	while (stmt.Fetch())
	{
		ASSERT(stmt[0] == i);
		if (i % 2 == 0) {
			ASSERT(stmt[1] == vNull);
			ASSERT(stmt[2] == Time(i+1900+100, i%12+1, i%28+1, i%24, i%60, i%60));
		} else {
			ASSERT(stmt[1] == Time(i+1900+10,  i%12+1, i%28+1, i%24, i%60, i%60));
			ASSERT(stmt[2] == vNull);
		}
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());
}

void TestBool(FBSession& s, const String& field_type, bool str_type = false)
{
	int i;
	Sql stmt(s);
	stmt.SetStatement(
	    "DROP TABLE test_bool_table"
	);
	stmt.Run();
	                  
	// Data definition statement.
	stmt.SetStatement(
		" CREATE TABLE test_bool_table( \n"
		"   id integer, \n"
		"   first_field " + field_type + " DEFAULT NULL, \n"
		"   second_field " + field_type + " DEFAULT NULL \n"
		")"
	);
	stmt.Run();

	stmt.SetStatement(
		" INSERT INTO test_bool_table \n"
		" VALUES(?, ?, ?)"
	);
		
	for (i = 0; i < 100; ++i)
	{
		stmt.SetParam(0, i);
		stmt.SetParam(1, (i%2 == 0));
		stmt.SetParam(2, (i%2 == 1));
		stmt.RunX();
	}

	int _0 = -1;
	bool _1 = false;
	bool _2 = false;
	
	Ref r0(_0);
	Ref r1(_1);
	Ref r2(_2);

	// SELECT without parameters.
	stmt.SetStatement(
		" SELECT * FROM test_bool_table ORDER BY id"
	);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(_1 == (i%2 == 0));
		ASSERT(_2 == (i%2 == 1));
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());

	if (!str_type)
	{
		// Rerun SELECT.
		stmt.Run();
		i = 0;
		while (stmt.Fetch())
		{
			ASSERT(stmt[0] == i);
			ASSERT(stmt[1] == (i%2 == 0));
			ASSERT(stmt[2] == (i%2 == 1));
			++i;
		}
		ASSERT(i == 100);
		ASSERT(!stmt.Fetch());
	}

	// SELECT with parameters.
	stmt.SetStatement(
		" SELECT * FROM test_bool_table WHERE id < ? ORDER BY id"
	);
	stmt.SetParam(0, 50);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(_1 == (i%2 == 0));
		ASSERT(_2 == (i%2 == 1));
		++i;
	}
	ASSERT(i == 50);
	ASSERT(!stmt.Fetch());

	// Rerun SELECT with parameters.
	stmt.SetParam(0, 25);
	
	stmt.Run();
	i = 0;
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, r0);
		stmt.GetColumn(1, r1);
		stmt.GetColumn(2, r2);

		ASSERT(_0 == i);
		ASSERT(_1 == (i%2 == 0));
		ASSERT(_2 == (i%2 == 1));
		++i;
	}
	ASSERT(i == 25);
	ASSERT(!stmt.Fetch());

	// Delete all records.
	stmt.SetStatement(
		" DELETE FROM test_bool_table"
	);
	stmt.Run();

	// Insert data again.
	stmt.SetStatement(
		" INSERT INTO test_bool_table \n"
		" VALUES(?, ?, ?)"
	);
		
	for (i = 0; i < 100; ++i)
	{
		stmt.SetParam(0, i);
		if (i % 2 == 0) {
			stmt.SetParam(1, Null);
			stmt.SetParam(2, false);
		} else {
			stmt.SetParam(1, true);
			stmt.SetParam(2, Null);
		}
		stmt.RunX();
	}
	
	// Run SELECT.
	i = 0;
	const Value vNull(Null);
	
	stmt.SetStatement(
		" SELECT * FROM test_bool_table ORDER BY id"
	);
	stmt.Run();
	
	while (stmt.Fetch())
	{
		ASSERT(stmt[0] == i);
		
		if (str_type)
		{
			if (i % 2 == 0) {
				ASSERT(stmt[1] == vNull);
				ASSERT(stmt[2] == "0");
			} else {
				ASSERT(stmt[1] == "1");
				ASSERT(stmt[2] == vNull);
			}
		}
		else
		{
			if (i % 2 == 0) {
				ASSERT(stmt[1] == vNull);
				ASSERT(stmt[2] == false);
			} else {
				ASSERT(stmt[1] == true);
				ASSERT(stmt[2] == vNull);
			}
		}
		++i;
	}
	ASSERT(i == 100);
	ASSERT(!stmt.Fetch());
}

class Exec : public StatementExecutor {
public:
	virtual bool Execute(const String& stmt) {
		Cout() << stmt << EOL; 
		return true;
	}
};

CONSOLE_APP_MAIN
{
	const Vector<String>& cmd_line = CommandLine();
	if (cmd_line.GetCount() > 1 && FileExists(cmd_line[0]) && FileExists(cmd_line[1]))
	{
		FBSession s;
		s.Connect(
			cmd_line[1],
			NULL,
			"SYSDBA",
			"masterkey",
			cmd_line[0]
			);

#if 0
		const char* script;
		RunScript runscript = s.GetRunScript();
		Exec exec;
		
		script =
			" /* comment */"
			;
			
		script =
			" /* comment 01 */ \n"
			" /* comment 02 */"
			;

		script =
			" /* SELECT rdb$type_name, rdb$type FROM rdb$types */"
			;

		script =
			" -- comment 01 \n"
			" -- comment 02 "
			;

		script =
			" 'string' "
			;

		script =
			"'s''t''r'''"
			;

		script =
			" DROP PROCEDURE test_procedure_01;"
			;

		script =
			" CREATE OR ALTER PROCEDURE test_procedure_01 (in_param integer) returns (out_param integer) \n"
			" as begin out_param = in_param + 1; end; \n"
			;

		script =
			" CREATE OR ALTER PROCEDURE test_procedure_01 (in_param integer) returns (out_param integer) \n"
			" as begin out_param = in_param + 1; end; \n"
			" DROP PROCEDURE test_procedure_01;"
			;

		script =
			"/* Display an org-chart. \n"
			"* \n"
			"*   Parameters: \n"
			"*       -- \n"
			"*   Returns: \n"
			"*       parent department \n"
			"*       department name \n"
			"*       department manager \n"
			"*       manager’s job title \n"
			"*       number of employees in the department */ \n"
			"CREATE PROCEDURE ORG_CHART \n"
			"  RETURNS (HEAD_DEPT CHAR(25), DEPARTMENT CHAR(25), \n"
			"      MNGR_NAME CHAR(20), TITLE CHAR(5), EMP_CNT INTEGER) \n"
			"  AS \n"
			"      DECLARE VARIABLE mngr_no INTEGER; \n"
			"      DECLARE VARIABLE dno CHAR(3); \n"
			"  BEGIN \n"
			"      FOR SELECT H.DEPARTMENT, D.DEPARTMENT, D.MNGR_NO, D.DEPT_NO \n"
			"          FROM DEPARTMENT D \n"
			"          LEFT OUTER JOIN DEPARTMENT H ON D.HEAD_DEPT = H.DEPT_NO \n"
			"          ORDER BY D.DEPT_NO \n"
			"          INTO :head_dept, :department, :mngr_no, :dno \n"
			"      DO \n"
			"          BEGIN \n"
			"              IF (:mngr_no IS NULL) THEN \n"
			"                  BEGIN \n"
			"                      MNGR_NAME = '--TBH--'; \n"
			"                       TITLE = ''; \n"
			"                  END \n"
			"              ELSE \n"
			"                  SELECT FULL_NAME, JOB_CODE \n"
			"                       FROM EMPLOYEE \n"
			"                      WHERE EMP_NO = :mngr_no \n"
			"                       INTO :mngr_name, :title; \n"
			"                  SELECT COUNT(EMP_NO) \n"
			"                       FROM EMPLOYEE \n"
			"                      WHERE DEPT_NO = :dno \n"
			"                       INTO :emp_cnt; \n"
			"                  SUSPEND; \n"
			"          END \n"
			"  END; ";

		script =
			"SET TERM !! ; \n"
			"SET TERM ; !! \n"
			;

		script =
			"SET TERM !! ; \n"
			"CREATE TRIGGER SAVE_SALARY_CHANGE FOR EMPLOYEE \n"
			"  AFTER UPDATE AS \n"
			"  BEGIN \n"
			"        IF (OLD.SALARY <> NEW.SALARY) THEN \n"
			"        INSERT INTO SALARY_HISTORY \n"
			"        (EMP_NO, CHANGE_DATE, UPDATER_ID, OLD_SALARY, PERCENT_CHANGE) \n"
			"            VALUES (OLD.EMP_NO, ’now’, USER,OLD.SALARY, \n"
			"             (NEW.SALARY - OLD.SALARY) * 100 / OLD.SALARY); \n"
			"  END !! \n"
			"SET TERM ; !!"
			;

		(*runscript)(script, exec, CNULL);
#endif

		Sql sql(s);
		
#if 1
		TRANSACTION(sql) {
		}
		
		TestBool(s, "bigint");
		TRANSACTION(s) {
			TestBool(s, "bigint");
		}

		TestBool(s, "integer");
		TRANSACTION(s) {
			TestBool(s, "integer");
		}

		TestBool(s, "numeric(1)");
		TRANSACTION(s) {
			TestBool(s, "numeric(1)");
		}
	
		TestBool(s, "double precision");
		TRANSACTION(s) {
			TestBool(s, "double precision");
		}
	
		TestBool(s, "varchar(1)", true);
		TRANSACTION(s) {
			TestBool(s, "varchar(1)", true);
		}
	
		TestBool(s, "char(1)", true);
		TRANSACTION(s) {
			TestBool(s, "char(1)", true);
		}

		TestInt(s, "INTEGER");
		TRANSACTION(s) {
			TestInt(s, "INTEGER");
		}
		
		TestInt(s, "SMALLINT");
		TRANSACTION(s) {
			TestInt(s, "SMALLINT");
		}
		
		TestString(s);
		TRANSACTION(s) {
			TestString(s);
		}
		
		TestDouble(s, "DOUBLE PRECISION");
		TRANSACTION(s) {
			TestDouble(s, "DOUBLE PRECISION");
		}

		TestDouble(s, "FLOAT");
		TRANSACTION(s) {
			TestDouble(s, "FLOAT");
		}

		TestDouble(s, "DECIMAL(18, 4)");
		TRANSACTION(s) {
			TestDouble(s, "DECIMAL(18, 4)");
		}

		TestDouble(s, "NUMERIC(18, 4)");
		TRANSACTION(s) {
			TestDouble(s, "NUMERIC(18, 4)");
		}

		TestDatetime(s);
		TRANSACTION(s) {
			TestDatetime(s);
		}
	
		TestBlob(s);
		TRANSACTION(s) {
			TestBlob(s);
		}
#endif

#if 0
		// Should be tested against examples\empbuild\EMPLOYEE.FDB
		FBSequence seq("CUST_NO_GEN", s);
		Value seq_v = seq.Get();
		int seq_i = seq_v;
	
		Cout() << "Users:" << EOL;
		Vector<String> users = s.EnumUsers();
		for(int i = 0; i < users.GetCount(); ++i)
			Cout() << users[i] << EOL;

		Cout() << "Views:" << EOL;
		Vector<String> views = s.EnumViews("");
		for(int i = 0; i < views.GetCount(); ++i)
			Cout() << views[i] << EOL;

		Cout() << "Tables:" << EOL;
		Vector<String> tables = s.EnumTables("");
		for(int i = 0; i < tables.GetCount(); ++i)
			Cout() << tables[i] << EOL;

		Cout() << "Sequences:" << EOL;
		Vector<String> sequences = s.EnumSequences("");
		for(int i = 0; i < sequences.GetCount(); ++i)
			Cout() << sequences[i] << EOL;

		Cout() << "PrimaryKey:" << EOL;
		Vector<String> pk_cols = s.EnumPrimaryKey("", "JOB");
		for(int i = 0; i < pk_cols.GetCount(); ++i)
			Cout() << pk_cols[i] << EOL;

		Cout() << "Columns:" << EOL;
		Vector<SqlColumnInfo> cols = s.EnumColumns("", "JOB");
		for(int i = 0; i < cols.GetCount(); ++i)
			Cout() << cols[i].name << EOL;

#endif
	}
}

