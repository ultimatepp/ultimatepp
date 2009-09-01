#include "TCoreDb.h"

NAMESPACE_UPP

static String GetRandomString(int maxlength = 50000, int minlength = 0)
{
	int len = minlength;
	unsigned span = maxlength - minlength;
	if(span > 0)
	{
		unsigned v = rand();
		if(span >= RAND_MAX)
			v += rand() * RAND_MAX;
		len += rand() % (span + 1);
	}
	StringBuffer s(len);
	for(int i = 0; i < len; i++)
		s[i] = rand();
	return s;
}

void DataFile::FileTest()
{
	String name = ForceExt(GetTempFileName(), ".cdb");
	DataFile file;
	if(!file.Create(name))
	{
		RLOG("Cannot create file '" << name << "'.");
		NEVER();
		return;
	}
	enum
	{
		COUNT   = 100,
		OBJECTS = 1000,
		LENGTH  = 50000,
	};
	Vector<String> values;

	srand(1);
	int i;
	for(i = 0; i < COUNT; i++)
	{
		int x = rand() % (int)OBJECTS;
		String s = GetRandomString(LENGTH);
		values.DoIndex(x) = s;
		file.Set(x, s, 0);
	}
	file.Commit(0);
	file.Compact();
	for(i = 0; i < values.GetCount(); i++)
	{
		String from = file.Get(i);
		if(from != values[i])
		{
			RLOG("DataFile::Set/Get mismatch at #" << i
				<< " (" << from.GetLength() << " / " << values[i].GetLength() << ")");
			NEVER();
		}
	}
	file.Close();
	DeleteFile(name);
}

void DataBase::AllocTest()
{
	RLOG("DataBase::AllocTest");

	String name = ForceExt(GetTempFileName(), ".cdb");
	DataFile df;
	if(!df.Create(name))
	{
		RLOG("Cannot create file '" << name << "'.");
		NEVER();
		return;
	}
	DataBase db;
	int master = db.Create(df, 0);

	Vector< Vector<Value> > rows;

	{ // fill in table
		DataTable& table = db.Add("POKUS");
		table.AddColumn("A", STRING_V);
		table.AddColumn("B", INT_V);
		db.Commit();
		df.Commit(0);

		enum
		{
			COUNT = 1000,
			OBJECTS = 1000,
		};

		srand(1);

		for(int i = 0; i < COUNT; i++)
		{
			if(i % 10 == 0)
				RDUMP(i);
			int x = rand() % (int)OBJECTS;
			if(rows.GetCount() > x && !rows[x].IsEmpty())
			{ // empty / modify row
				if(rand() & 1)
				{
					table.Remove(x);
					rows[x].Clear();
				}
				else
				{
					Vector<Value>& row = rows[x];
					row[0] = GetRandomString();
					row[1] = rand();
					table.Set(x, row);
				}
			}
			else
			{ // add row
				Vector<Value> row;
				row.Add(GetRandomString());
				row.Add(rand());
				x = table.Add(row);
				rows.DoIndex(x) = row;
			}
			if((i & 15) == 0)
				table.Commit();
		}
	}

	db.Commit();
	db.Close();
	df.Compact();
	db.Open(df, master, 0);

	{ // check table
		DataTable& table = db["POKUS"];
		int i = rows.GetCount();
		while(i && rows[i - 1].IsEmpty())
			i--;
		ASSERT(table.GetRowIndex().GetEndIndex() == i);
		while(--i >= 0)
		{
			if(i % 10 == 0)
				RDUMP(i);
			VERIFY(rows[i].IsEmpty() == table.IsEmpty(i));
			if(!rows[i].IsEmpty())
			{
				Vector<Value> row = table.Get(i);
				VERIFY(row[0] == rows[i][0] && row[1] == rows[i][1]);
			}
		}
	}

	db.Close();
	df.Close();

	DeleteFile(name);
}

static Date anchor_date(2000, 1, 1);
enum { ROW_COUNT = 100000 };

static void CreateDataBase(const String& fn, int& aobj, int& bobj, int& cobj, int& dobj, int& eobj, int& fobj, int& iobj)
{
	DataFile df;
#ifdef NO_CACHE
	df.SetCache(16384, 50000, 50000);
#endif//NO_CACHE
	if(!df.Create(fn))
	{
		RLOG("error creating file '" << fn << "'");
		NEVER();
		return;
	}

	DataColumnOf<int> a, b;
	DataColumnOf<String> c;
	DataColumnOf<Date>   d;
	DataColumnOf<Time>   e;
	DataColumnOf<bool>   f;
	DataRowIndex index;

#ifdef NO_CACHE
	a.SetCacheSize(0);
	b.SetCacheSize(0);
	c.SetCacheSize(0);
	d.SetCacheSize(0);
	e.SetCacheSize(0);
	f.SetCacheSize(0);
#endif//NO_CACHE

	aobj = a.Create(df, 1);
	bobj = b.Create(df, 2);
	cobj = c.Create(df, 3);
	dobj = d.Create(df, 4);
	eobj = e.Create(df, 5);
	fobj = f.Create(df, 6);
	iobj = index.Create(df, 7);

/*
	a.SetCount(COUNT);
	b.SetCount(COUNT);
	c.SetCount(COUNT);
	d.SetCount(COUNT);
	e.SetCount(COUNT);
*/

	for(int i = 0; i < ROW_COUNT; i++)
	{
		if(i % 1000 == 0)
			RLOG("Writing " << i);
//		LOG("#" << i);
//		if(i == 49001)
//			__asm int 3
		int x = index.Add();
		a.Set(x, i);
		b.Set(x, -i);
		String vc = "ab" + IntStr(i) + "cd";
//		if(i % 1000 == 0)
//			vc.Cat('*', 10 * i);
		c.Set(x, vc);
		d.Set(x, anchor_date + i);
		e.Set(x, ToTime(anchor_date) + i);
		f.Set(x, i & 1);
	}

	a.Commit(); a.Close();
	b.Commit(); b.Close();
	c.Commit(); c.Close();
	d.Commit(); d.Close();
	e.Commit(); e.Close();
	f.Commit(); f.Close();
	index.Commit(); index.Close();

	df.Commit(0);
	df.Close();
}

static void CheckColumnA(DataFile& df, int aobj)
{
	DataColumnOf<int> a;
	a.Open(df, aobj, 1);
	for(int i = 0; i < a.GetCount(); i++)
	{
		if((int)a[i] != i)
		{
			String s;
			s << "Mismatch: a[" << i << "] = " << StdFormat(a[i]) << ", " << 3 * i << " expected";
			RLOG(s);
			NEVER();
		}
		a.Set(i, 3 * i);
	}
	a.Commit();
}

static void CheckColumnA2(DataFile& df, int aobj)
{
	DataColumnOf<int> a;
	a.Open(df, aobj, 1);
	for(int i = 0; i < a.GetCount(); i++)
		if((int)a[i] != 3 * i)
		{
			String s;
			s << "Mismatch: a[" << i << "] = " << StdFormat(a[i]) << ", " << 3 * i << " expected";
			RLOG(s);
			NEVER();
		}
}

static void CheckColumnB(DataFile& df, int bobj)
{
	DataColumnOf<int> b;
	b.Open(df, bobj, 2);
	for(int i = 0; i < b.GetCount(); i++)
	{
		if((int)b[i] != -i)
		{
			String s;
			s << "Mismatch: b[" << i << "] = " << StdFormat(b[i]) << ", " << -i << " expected";
			RLOG(s);
			NEVER();
		}
		b.Set(i, -i * i);
	}
	b.Commit();
}

static void CheckColumnB2(DataFile& df, int bobj)
{
	DataColumnOf<int> b;
	b.Open(df, bobj, 2);
	for(int i = 0; i < b.GetCount(); i++)
		if((int)b[i] != -i * i)
		{
			String s;
			s << "Mismatch: b[" << i << "] = " << StdFormat(b[i]) << ", " << -i * i << " expected";
			RLOG(s);
			NEVER();
		}
}

static void CheckColumnC(DataFile& df, int cobj)
{
	DataColumnOf<String> c;
	c.Open(df, cobj, 3);
	for(int i = 0; i < c.GetCount(); i++)
	{
//		if(i == 179540)
//			__asm int 3
		String t = c[i], e = "ab" + IntStr(i) + "cd";
//		if(i % 1000 == 0)
//			e.Cat('*', 10 * i);
		if(t != e)
		{
			String s;
			s << "Mismatch: c[" << i << "] = " << StdFormat(c[i]) << ", " << e << " expected";
			RLOG(s);
			NEVER();
		}
		c.Set(i, "ab" + IntStr(i) + "cd_ef" + IntStr(-i));
	}
	c.Commit();
}

static void CheckColumnC2(DataFile& df, int cobj)
{
	DataColumnOf<String> c;
	c.Open(df, cobj, 3);
	for(int i = 0; i < c.GetCount(); i++)
	{
		String t = c[i], e = "ab" + IntStr(i) + "cd_ef" + IntStr(-i);
		if(t != e)
		{
			String s;
			s << "Mismatch: c[" << i << "] = " << StdFormat(c[i]) << ", " << e << " expected";
			RLOG(s);
			NEVER();
		}
	}
}

static void CheckColumnD(DataFile& df, int dobj)
{
	DataColumnOf<Date> d;
	d.Open(df, dobj, 4);
	for(int i = 0; i < d.GetCount(); i++)
	{
		if(Date(d[i]) != anchor_date + i)
		{
			String s;
			s << "Mismatch: d[" << i << "] = " << StdFormat(d[i]) << ", " << Format(anchor_date + i) << " expected";
			RLOG(s);
			NEVER();
		}
		d.Set(i, anchor_date - 2 * i);
	}
	d.Commit();
}

static void CheckColumnD2(DataFile& df, int dobj)
{
	DataColumnOf<Date> d;
	d.Open(df, dobj, 4);
	for(int i = 0; i < d.GetCount(); i++)
		if(Date(d[i]) != anchor_date - 2 * i)
		{
			String s;
			s << "Mismatch: d[" << i << "] = " << StdFormat(d[i])
			<< ", " << Format(anchor_date - 2 * i) << " expected";
			RLOG(s);
			NEVER();
		}
}

static void CheckColumnE(DataFile& df, int eobj)
{
	DataColumnOf<Time> e;
	e.Open(df, eobj, 5);
	for(int i = 0; i < e.GetCount(); i++)
	{
		Time dest = ToTime(anchor_date) + i;
		if(Time(e[i]) != dest)
		{
			String s;
			s << "Mismatch: e[" << i << "] = " << StdFormat(e[i])
			<< ", " << Format(dest) << " expected";
			RLOG(s);
			NEVER();
		}
		e.Set(i, ToTime(anchor_date) - 2 * i);
	}
	e.Commit();
}

static void CheckColumnE2(DataFile& df, int eobj)
{
	DataColumnOf<Time> e;
	e.Open(df, eobj, 5);
	for(int i = 0; i < e.GetCount(); i++)
	{
		Time dest = ToTime(anchor_date) - 2 * i;
		if(Time(e[i]) != dest)
		{
			String s;
			s << "Mismatch: e[" << i << "] = " << StdFormat(e[i]) << ", "
			<< Format(dest) << " expected";
			RLOG(s);
			NEVER();
		}
	}
}

static void CheckColumnF(DataFile& df, int fobj)
{
	DataColumnOf<bool> f;
	f.Open(df, fobj, 6);
	for(int i = 0; i < ROW_COUNT; i++)
	{
		if(CalcType<bool>::ValueTo(f[i]) != bool(i & 1))
		{
			String s;
			s << "Mismatch: f[" << i << "] = " << StdFormat(f[i]) << ", " << (i & 1) << " expected";
			RLOG(s);
			NEVER();
		}
		f.Set(i, CalcType<bool>::ToValue(i & 2));
	}
	f.Commit();
}

static void CheckColumnF2(DataFile& df, int fobj)
{
	DataColumnOf<bool> f;
	f.Open(df, fobj, 6);
	for(int i = 0; i < ROW_COUNT; i++)
	{
		bool b = i & 2;
		if(CalcType<bool>::ValueTo(f[i]) != b)
		{
			String s;
			s << "Mismatch: f[" << i << "] = " << StdFormat(f[i]) << ", " << b << " expected";
			RLOG(s);
			NEVER();
		}
	}
}

static void CheckIndex(DataFile& df, int iobj)
{
	DataRowIndex index;
	index.Open(df, iobj, 6);
	RLOG("Index count = " << index.GetCount() << ", first empty = "
		<< index.GetFirstEmpty() << ", end index = " << index.GetEndIndex());
	Index<int> to_remove;
	srand(1);
	int top = index.GetEndIndex();
	int rem = top / 4;
	to_remove.Reserve(rem);
	while(to_remove.GetCount() < rem)
	{
		int r = rand() + rand() * RAND_MAX;
		to_remove.FindAdd(r % top);
	}
	RLOG("Removing " << rem << " entries...");
	index.Remove(to_remove.GetKeys());
	int i;
	for(i = 0; i < top; i++)
		if(index.IsEmpty(i) != (to_remove.Find(i) >= 0))
		{
			NEVER();
		}
	RLOG("Index count = " << index.GetCount() << ", first empty = "
		<< index.GetFirstEmpty() << ", end index = " << index.GetEndIndex());
	RLOG("Reallocating items...");
	Vector<int> sorted;
	sorted <<= to_remove.GetKeys();
	Sort(sorted);
	for(i = 0; i < rem; i++)
	{
		int alloc = index.Add();
		ASSERT(alloc == sorted[i]);
	}
	RLOG("Index count = " << index.GetCount() << ", first empty = "
		<< index.GetFirstEmpty() << ", end index = " << index.GetEndIndex());
}

static void CheckDataBase(const String& fn, int aobj, int bobj, int cobj, int dobj, int eobj, int fobj, int iobj)
{
	DataFile df;
	df.SetCache(16384, 100000, 100000);

	if(!df.Open(fn))
	{
		RLOG("error opening file '" << fn << "'");
		NEVER();
		return;
	}

	/*
	{
		String ex = AppendFileName(GetTempPath(), "pokus.cdi");
		FileOut fo(ex);
		if(!fo.IsOpen())
			CmdError("error creating file '" + ex + "'");

		df.Export(fo);
	}
	*/

/*
	RLOG("Checking RowIndex...");
	CheckIndex(df, iobj);
	RLOG("Checking A...");
	CheckColumnA(df, aobj);
	RLOG("Checking B...");
	CheckColumnB(df, bobj);
*/
	RLOG("Checking C...");
	CheckColumnC(df, cobj);
/*
	RLOG("Checking D...");
	CheckColumnD(df, dobj);
	RLOG("Checking E...");
	CheckColumnE(df, eobj);
	RLOG("Checking F...");
	CheckColumnF(df, fobj);
*/
	df.Commit(0);
	df.Close();
}

static void CheckDataBase2(const String& fn, int aobj, int bobj, int cobj, int dobj, int eobj, int fobj, int iobj)
{
	DataFile df;
	df.SetCache(16384, 100000, 100000);

	if(!df.Open(fn))
	{
		RLOG("error opening file '" << fn << "'");
		NEVER();
		return;
	}

//	df.Commit(0);
	RLOG("Database file size = " << df.GetFileSize() << ", overhead = " << df.GetOverhead() << "%");
	RLOG("Compacting...\n");
//	df.Compact();
	RLOG("Database file size = " << df.GetFileSize() << ", overhead = " << df.GetOverhead() << "%");

/*
	RLOG("Checking A/2...");
	CheckColumnA2(df, aobj);
	RLOG("Checking B/2...");
	CheckColumnB2(df, bobj);
*/
	RLOG("Checking C/2...");
	CheckColumnC2(df, cobj);
/*
	RLOG("Checking D/2...");
	CheckColumnD2(df, dobj);
	RLOG("Checking E/2...");
	CheckColumnE2(df, eobj);
	RLOG("Checking F/2...");
	CheckColumnF2(df, fobj);
*/
	df.Commit(0);
	df.Close();
}

static void RawDataFileTest()
{
	String fn = ForceExt(GetTempFileName(), ".cdb");
	int aobj, bobj, cobj, dobj, eobj, fobj, iobj;
	CreateDataBase(fn, aobj, bobj, cobj, dobj, eobj, fobj, iobj);

	CheckDataBase(fn, aobj, bobj, cobj, dobj, eobj, fobj, iobj);
	CheckDataBase2(fn, aobj, bobj, cobj, dobj, eobj, fobj, iobj);
	DeleteFile(fn);
}

static void StructuredDataBaseTest(const String& fn)
{
	DataFile df;
	if(!df.Create(fn))
		throw ObjectExc("cannot create file '" + fn + "'");
	DataBase db;
	int i = db.Create(df, 0);
	if(i)
		throw ObjectExc("database creation error");
	DataTable& table = db.Add("TestTable");
	table.AddColumn("ColumnInt",    INT_V, DataTableColumn::NUMBER_INDEX);
	table.AddColumn("ColumnDouble", DOUBLE_V);
	table.AddColumn("ColumnString", STRING_V);

	{
		StringStream ss;
		db.Describe(ss);
		RLOG(ss.GetResult());
	}

	for(i = 0; i < 100000; i++)
	{
		if(i % 1000 == 0)
			RLOG(i);
		Vector<Value> v;
		v << i << sin((double)i) << ("hello world " + IntStr(i));
		table.Add(v);
	}

	{
		StringStream ss;
		db.Describe(ss);
		RLOG(ss.GetResult());
	}

	{
		DataSet items = table.Column(0).GetRange(50, 60);
		VppLog() << "Range <50, 60>: ";
		for(int i = 0; i < items.GetCount(); i++)
			VppLog() << items[i] << ' ';
		RLOG("");
	}

	{
		double min, max;
		table.Column(0).GetMinMax(min, max);
		RLOG("MinMax(0) = <" << min << ", " << max << ">");
		table.Column(1).GetMinMax(min, max);
		RLOG("MinMax(1) = <" << min << ", " << max << ">");
	}

	db.Commit();
	df.Commit(0);
}

static void DataBaseTest2(const String& fn)
{
	DataFile df;
	if(!df.Open(fn))
		throw ObjectExc("cannot open file '" + fn + "'");
	DataBase db;
	db.Open(df, 0, 0);
	StringStream ss;
	db.Describe(ss);
	RLOG(ss.GetResult());
}

void DataBase::ColumnTest()
{
	RLOG("DataBase::ColumnTest");

	RawDataFileTest();
}

void DataBase::StructuredTest()
{
	String name = ForceExt(GetTempFileName(), ".cdb");
	StructuredDataBaseTest(name);
	DataBaseTest2(name);
	DeleteFile(name);
}

void DataBase::FullTest()
{
	DataFile::FileTest();
	AllocTest();
	ColumnTest();
	StructuredTest();
}

END_UPP_NAMESPACE
