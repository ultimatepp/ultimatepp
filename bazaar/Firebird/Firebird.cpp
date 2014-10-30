#include "Firebird.h"
#include <time.h>

NAMESPACE_UPP

TransAuto::TransAuto(SqlSession& s) : abort(true), isSession(true), session(&s)
{
	session->Begin();
}

TransAuto::TransAuto(Sql& s) : abort(true), isSession(false), sql(&s)
{
	sql->Begin();
}

TransAuto::~TransAuto()
{
	try	{
		if (isSession)
		{
			if(abort) {
				session->Rollback();
			} else {
				session->Commit();
			}
		} else {
			if(abort) {
				sql->Rollback();
			} else {
				sql->Commit();
			}
		}
	} catch(...) {
		// Ignore all exceptions in destructor.
		ASSERT(false);
	}
}

class TransRetain
{
public:
	TransRetain(FBSession& s) : abort(true), session(&s)
	{
	}
	~TransRetain()
	{
		try	{
			if(abort) {
				session->RollbackRetaining();
			} else {
				session->CommitRetaining();
			}
		} catch(...) {
			// Ignore all exceptions in the destructor.
			ASSERT(false);
		}
	}

public:
	operator bool() const { return abort; }
	void Finish() { abort = false; }

protected:
	bool abort:1;
	FBSession* session;
};

#define TRANSACTION_RETAIN(trans) \
for(TransRetain auto_trans_##__LINE__(trans); \
    auto_trans_##__LINE__; \
    auto_trans_##__LINE__.Finish() )

struct FBValue {
	short       nullInd;
	XSQLVAR*	pXSQLVar;
	
	FBValue(XSQLVAR& v);
	virtual ~FBValue();
	
	bool CanBeNull() const { return pXSQLVar->sqltype & 1; }
	bool IsNull() const { return nullInd == -1; }
	void SetNull(bool flag = true) { nullInd = flag ? -1 : 0; }
	
	virtual void GetValue(Ref r) const = 0;
	virtual void SetValue(const Value& v) = 0;
};

FBValue::FBValue(XSQLVAR& v)
{
	pXSQLVar = &v;
	if (CanBeNull())
	{
		v.sqlind = &nullInd;
		nullInd = -1;
	}
}

FBValue::~FBValue()
{
}

struct FBInt : public FBValue {
	int	value;

	FBInt(XSQLVAR& v);

	virtual void GetValue(Ref r) const;
	virtual void SetValue(const Value& v);
};

FBInt::FBInt(XSQLVAR& v) : FBValue(v)
{
	v.sqltype = SQL_LONG + (v.sqltype & 1);
	v.sqldata = reinterpret_cast<ISC_SCHAR*>(&value);
	v.sqllen = sizeof(value);
}

void FBInt::GetValue(Ref r) const
{
	if (IsNull())
		r = Null;
	else {
		switch(r.GetType()) 
		{
		case INT_V:
			RefInt(r) = value;
			break;
		case DOUBLE_V:
			RefDouble(r) = static_cast<double>(value);
			break;
		case STRING_V:
			RefString(r) = FormatInt(value);
			break;
		case VALUE_V:
			RefValue(r) = Value(value);
			break;
		case WSTRING_V:
			RefWString(r) = FormatInt(value).ToWString();
			break;
		case INT64_V:
			RefInt64(r) = value;
			break;
		case BOOL_V:
			RefBool(r) = value != 0;
			break;
		case DATE_V:
		case TIME_V:
		default:
			ASSERT(false);
			break;
		}
	}
}

void FBInt::SetValue(const Value& v)
{
	SetNull(v.IsNull());

	if (v.IsNull())
		return;
	
	switch (v.GetType())
	{
	case INT_V:
		value = ValueTo<int>(v);
		break;
	case DOUBLE_V:
		value = static_cast<int>(ValueTo<double>(v));
		break;
	case STRING_V:
		value = ScanInt(ValueTo<String>(v));
		break;
//		case DATE_V:
//		case TIME_V:
	case VALUE_V:
		SetValue(ValueTo<Value>(v));
		break;
	case WSTRING_V:
		value = ScanInt(ValueTo<WString>(v).ToString());
		break;
	case INT64_V:
		value = static_cast<int>(ValueTo<int64>(v));
		break;
	case BOOL_V:
		value = ValueTo<bool>(v) ? 1 : 0;
		break;
	default:
		ASSERT(false);
		break;
	}
}

struct FBInt64 : public FBValue {
	int64	value;

	FBInt64(XSQLVAR& v);

	virtual void GetValue(Ref r) const;
	virtual void SetValue(const Value& v);
};

FBInt64::FBInt64(XSQLVAR& v) : FBValue(v)
{
	v.sqltype = SQL_INT64 + (v.sqltype & 1);
	v.sqldata = reinterpret_cast<ISC_SCHAR*>(&value);
	v.sqllen = sizeof(value);
}

void FBInt64::GetValue(Ref r) const
{
	if (IsNull())
		r = Null;
	else {
		switch(r.GetType()) 
		{
		case INT_V:
			RefInt(r) = static_cast<int>(value);
			break;
		case DOUBLE_V:
			RefDouble(r) = static_cast<double>(value);
			break;
		case STRING_V:
			RefString(r) = FormatInt64(value);
			break;
		case VALUE_V:
			RefValue(r) = Value(value);
			break;
		case WSTRING_V:
			RefWString(r) = FormatInt64(value).ToWString();
			break;
		case INT64_V:
			RefInt64(r) = value;
			break;
		case BOOL_V:
			RefBool(r) = value != 0;
			break;
		case DATE_V:
		case TIME_V:
		default:
			ASSERT(false);
			break;
		}
	}
}

void FBInt64::SetValue(const Value& v)
{
	SetNull(v.IsNull());

	if (v.IsNull())
		return;
	
	switch (v.GetType())
	{
	case INT_V:
		value = ValueTo<int>(v);
		break;
	case DOUBLE_V:
		value = static_cast<int64>(ValueTo<double>(v));
		break;
	case STRING_V:
		value = ScanInt64(ValueTo<String>(v));
		break;
//		case DATE_V:
//		case TIME_V:
	case VALUE_V:
		SetValue(ValueTo<Value>(v));
		break;
	case WSTRING_V:
		value = ScanInt64(ValueTo<WString>(v).ToString());
		break;
	case INT64_V:
		value = ValueTo<int64>(v);
		break;
	case BOOL_V:
		value = ValueTo<bool>(v) ? 1 : 0;
		break;
	default:
		ASSERT(false);
		break;
	}
}

struct FBDouble : public FBValue {
	double	value;

	FBDouble(XSQLVAR& v);

	virtual void GetValue(Ref r) const;
	virtual void SetValue(const Value& v);
};

FBDouble::FBDouble(XSQLVAR& v) : FBValue(v)
{
	v.sqltype = SQL_DOUBLE + (v.sqltype & 1);
	v.sqldata = reinterpret_cast<ISC_SCHAR*>(&value);
	v.sqllen = sizeof(value);
	v.sqlscale = 18;
}

void FBDouble::GetValue(Ref r) const
{
	if (IsNull())
		r = Null;
	else {
		switch(r.GetType()) 
		{
		case INT_V:
			RefInt(r) = static_cast<int>(value);
			break;
		case DOUBLE_V:
			RefDouble(r) = value;
			break;
		case STRING_V:
			RefString(r) = FormatDouble(value);
			break;
		case VALUE_V:
			RefValue(r) = Value(value);
			break;
		case WSTRING_V:
			RefWString(r) = FormatDouble(value).ToWString();
			break;
		case INT64_V:
			RefInt64(r) = static_cast<int64>(value);
			break;
		case BOOL_V:
			RefBool(r) = value != 0.0;
			break;
		case DATE_V:
		case TIME_V:
		default:
			ASSERT(false);
			break;
		}
	}
}

void FBDouble::SetValue(const Value& v)
{
	SetNull(v.IsNull());

	if (v.IsNull())
		return;
	
	switch (v.GetType())
	{
	case INT_V:
		value = ValueTo<int>(v);
		break;
	case DOUBLE_V:
		value = ValueTo<double>(v);
		break;
	case STRING_V:
		value = ScanDouble(ValueTo<String>(v));
		break;
//		case DATE_V:
//		case TIME_V:
	case VALUE_V:
		SetValue(ValueTo<Value>(v));
		break;
	case WSTRING_V:
		value = ScanDouble(ValueTo<WString>(v).ToString());
		break;
	case INT64_V:
		value = static_cast<double>(ValueTo<int64>(v));
		break;
	case BOOL_V:
		value = ValueTo<bool>(v) ? 1.0 : 0.0;
		break;
	default:
		ASSERT(false);
		break;
	}
}

// This macro is used to declare structures representing SQL VARCHAR types
#define SQL_VARCHAR(len) struct {short vary_length; char vary_string[(len) + 1];} 

struct FBVarying : public FBValue {
//	enum { max_varchar_size = 8 * 1024 };
	enum { max_varchar_size = 32765 };

	//String	value;
	SQL_VARCHAR(32765) buff;

	FBVarying(XSQLVAR& v);
	
	void SetValue(const String& str);

	virtual void GetValue(Ref r) const;
	virtual void SetValue(const Value& v);
};

FBVarying::FBVarying(XSQLVAR& v) : FBValue(v)
{
	v.sqltype = SQL_VARYING + (v.sqltype & 1);
	v.sqldata = reinterpret_cast<ISC_SCHAR*>(&buff);
	v.sqllen = v.sqllen == 0 ? max_varchar_size : v.sqllen;
}

void FBVarying::SetValue(const String& str)
{
	ASSERT(pXSQLVar);
	buff.vary_length = min<short>(str.GetCount(), pXSQLVar->sqllen);
	strncpy(buff.vary_string, ~str, buff.vary_length);
}

void FBVarying::GetValue(Ref r) const
{
	if (IsNull())
		r = Null;
	else {
		switch(r.GetType()) {
		case INT_V:
			{
				const char* e = buff.vary_string + buff.vary_length;
				RefInt(r) = ScanInt(buff.vary_string, &e);
			}
			break;
		case DOUBLE_V:
			{
				const char* e = buff.vary_string + buff.vary_length;
				RefDouble(r) = ScanDouble(buff.vary_string, &e);
			}
			break;
		case STRING_V:
			RefString(r) = String(buff.vary_string, buff.vary_length);
			break;
		case DATE_V:
			r.SetValue(UPP::Scan(DATE_V, String(buff.vary_string, buff.vary_length)));
			break;
		case TIME_V:
			r.SetValue(UPP::Scan(TIME_V, String(buff.vary_string, buff.vary_length)));
			break;
		case VALUE_V:
			RefValue(r) = Value(String(buff.vary_string, buff.vary_length));
			break;
		case WSTRING_V:
			RefWString(r) = WString(buff.vary_string, buff.vary_length);
			break;
		case INT64_V:
			{
				const char* e = buff.vary_string + buff.vary_length;
				RefInt64(r) = ScanInt64(buff.vary_string, &e);
			}
			break;
		case BOOL_V:
			{
				const char* e = buff.vary_string + buff.vary_length;
				RefBool(r) = ScanInt(buff.vary_string, &e) != 0;
			}
			break;
		default:
			ASSERT(false);
			break;
		}
	}
}

void FBVarying::SetValue(const Value& v)
{
	SetNull(v.IsNull());

	if (v.IsNull())
		return;
	
	switch (v.GetType())
	{
	case INT_V:
		SetValue(AsString(ValueTo<int>(v)));
		break;
	case DOUBLE_V:
		SetValue(AsString(ValueTo<double>(v)));
		break;
	case STRING_V:
		SetValue(ValueTo<String>(v));
		break;
//		case DATE_V:
//		case TIME_V:
	case VALUE_V:
		SetValue(ValueTo<Value>(v));
		break;
	case WSTRING_V:
		SetValue(ValueTo<WString>(v).ToString());
		break;
	case INT64_V:
		SetValue(AsString(ValueTo<int64>(v)));
		break;
	case BOOL_V:
		SetValue(IntStr((int)ValueTo<bool>(v)));
		break;
	default:
		ASSERT(false);
		break;
	}
}

struct FBTimeStamp : public FBValue {
	ISC_TIMESTAMP	value;

	FBTimeStamp(XSQLVAR& v, T_FB& dll);

	virtual void GetValue(Ref r) const;
	virtual void SetValue(const Value& v);
	
	T_FB& dll;
};

FBTimeStamp::FBTimeStamp(XSQLVAR& v, T_FB& dll) : FBValue(v), dll(dll)
{
	v.sqltype = SQL_TIMESTAMP + (v.sqltype & 1);
	v.sqldata = reinterpret_cast<ISC_SCHAR*>(&value);
	v.sqllen = sizeof(value);
}

void FBTimeStamp::GetValue(Ref r) const
{
	if (IsNull())
		r = Null;
	else {
		const LanguageInfo& li = GetLanguageInfo();
		struct tm	time;
		dll.isc_decode_timestamp(&value, &time);
		
		const Time t(time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, 
			time.tm_hour, time.tm_min, time.tm_sec);
		
		switch(r.GetType()) {
		case STRING_V:
			RefString(r) = li.FormatTime(t);
			break;
		case DATE_V:
			RefDate(r) = t;
			break;
		case TIME_V:
			RefTime(r) = t;
			break;
		case VALUE_V:
			RefValue(r) = t;
			break;
		case WSTRING_V:
			RefWString(r) = li.FormatTime(t).ToWString();
			break;
		case INT64_V:
			RefInt64(r) = t.Get();
			break;
		case DOUBLE_V:
			RefDouble(r) = static_cast<double>(t.Get());
			break;
		case INT_V:
		case BOOL_V:
		default:
			ASSERT(false);
			break;
		}
	}
}

void FBTimeStamp::SetValue(const Value& v)
{
	SetNull(v.IsNull());

	if (v.IsNull())
		return;

	Time t;	
	switch (v.GetType())
	{
	case INT_V:
		t.Set(ValueTo<int>(v));
		break;
	case DOUBLE_V:
		t.Set((int64)ValueTo<double>(v));
	case STRING_V:
		t = UPP::Scan(TIME_V, ValueTo<String>(v));
		break;
	case DATE_V:
	{
		const Date& d = ValueTo<Date>(v);
		t = Time(d.year, d.month, d.day);
	}
		break;
	case TIME_V:
		t = ValueTo<Time>(v);
		break;
	case VALUE_V:
		SetValue(ValueTo<Value>(v));
		break;
	case WSTRING_V:
		t = UPP::Scan(TIME_V, ValueTo<WString>(v).ToString());
		break;
	case INT64_V:
		t.Set(ValueTo<int64>(v));
		break;
	case BOOL_V:
	default:
		ASSERT(false);
		break;
	}
		
	struct tm time;
	time.tm_year = t.year - 1900;
	time.tm_mon = t.month - 1;
	time.tm_mday = t.day;
	time.tm_hour = t.hour;
	time.tm_min = t.minute;
	time.tm_sec = t.second;
	time.tm_isdst = -1;

	dll.isc_encode_timestamp(&time, &value);
}

struct FBDate : public FBValue {
	ISC_DATE	value;

	FBDate(XSQLVAR& v, T_FB& dll);

	virtual void GetValue(Ref r) const;
	virtual void SetValue(const Value& v);
	
	T_FB& dll;
};

FBDate::FBDate(XSQLVAR& v, T_FB& dll) : FBValue(v), dll(dll)
{
	v.sqltype = SQL_DATE + (v.sqltype & 1);
	v.sqldata = reinterpret_cast<ISC_SCHAR*>(&value);
	v.sqllen = sizeof(value);
}

void FBDate::GetValue(Ref r) const
{
	if (IsNull())
		r = Null;
	else {
		const LanguageInfo& li = GetLanguageInfo();
		struct tm	time;
		dll.isc_decode_sql_date(&value, &time);
		
		const Date d(time.tm_year + 1900, time.tm_mon + 1, time.tm_mday);
		
		switch(r.GetType()) {
		case STRING_V:
			RefString(r) = li.FormatDate(d);
			break;
		case DATE_V:
			RefDate(r) = d;
			break;
		case TIME_V:
			RefTime(r) = Time(d.year, d.month, d.day);
			break;
		case VALUE_V:
			RefValue(r) = d;
			break;
		case WSTRING_V:
			RefWString(r) = li.FormatDate(d).ToWString();
			break;
		case INT64_V:
			RefInt64(r) = d.Get();
			break;
		case INT_V:
			RefInt(r) = d.Get();
			break;
		case DOUBLE_V:
			RefDouble(r) = d.Get();
			break;
		case BOOL_V:
		default:
			ASSERT(false);
			break;
		}
	}
}

void FBDate::SetValue(const Value& v)
{
	SetNull(v.IsNull());

	if (v.IsNull())
		return;
	
	Date d;	
	switch (v.GetType())
	{
	case INT_V:
		d.Set(ValueTo<int>(v));
		break;
	case DOUBLE_V:
		d.Set((int)ValueTo<double>(v));
	case STRING_V:
		d = UPP::Scan(DATE_V, ValueTo<String>(v));
		break;
	case DATE_V:
		d = ValueTo<Date>(v);
		break;
	case TIME_V:
		d = ValueTo<Time>(v);
		break;
	case VALUE_V:
		SetValue(ValueTo<Value>(v));
		break;
	case WSTRING_V:
		d = UPP::Scan(DATE_V, ValueTo<WString>(v).ToString());
		break;
	case INT64_V:
		d.Set((int)ValueTo<int64>(v));
		break;
	case BOOL_V:
	default:
		ASSERT(false);
		break;
	}
		
	struct tm time;
	time.tm_year = d.year - 1900;
	time.tm_mon = d.month - 1;
	time.tm_mday = d.day;
	time.tm_hour = 0;
	time.tm_min = 0;
	time.tm_sec = 0;
	time.tm_isdst = -1;

	dll.isc_encode_sql_date(&time, &value);
}

struct FBBlob : public FBValue {
	mutable ibpp::SegmentBlob	value;

	FBBlob(FBSession& s, XSQLVAR& v);

	long GetLength() const
	{
		return value.getInfo().getLenght();
	}
	
	String GetEntire() const;
	void SetEntire(const char* data_ptr, int data_len);
	
	virtual void GetValue(Ref r) const;
	virtual void SetValue(const Value& v);
};

FBBlob::FBBlob(FBSession& s, XSQLVAR& v)
: FBValue(v)
, value(s.db, s.tr, s.dll, s.ib_error)
{
	v.sqltype = SQL_BLOB + (v.sqltype & 1);
	v.sqldata = reinterpret_cast<ISC_SCHAR*>(&value.getID());
	v.sqllen = sizeof(GDS_QUAD);
}

String FBBlob::GetEntire() const
{
	// Max value is 2 >> 16 ...
	const unsigned short db_segment_size = 0xFFFF;
	String data;
	unsigned short actualSize;
	char buff[db_segment_size];
	
	value.open();
	while (value.getSegment(actualSize, sizeof(buff), buff))
	    data.Cat(buff, actualSize);
	value.close();
	
	return data;
}

void FBBlob::SetEntire(const char* data_ptr, int data_len)
{
	value.create();
	value.put_segment(data_ptr, data_len);
	value.close();
}

void FBBlob::GetValue(Ref r) const
{
	if (IsNull())
		r = Null;
	else {
		unsigned short actualSize;
		value.open();

		switch(r.GetType()) {
		case INT_V:
			{
				int v;
				if (value.getSegment(actualSize, sizeof(v), reinterpret_cast<char*>(&v)))
					RefInt(r) = v;
			}
			break;
		case DOUBLE_V:
			{
				double v;
				if (value.getSegment(actualSize, sizeof(v), reinterpret_cast<char*>(&v)))
					RefDouble(r) = v;
			}
			break;
		case STRING_V:
			RefString(r) = GetEntire();
			break;
		case DATE_V:
			r.SetValue(UPP::Scan(DATE_V, GetEntire()));
			break;
		case TIME_V:
			r.SetValue(UPP::Scan(TIME_V, GetEntire()));
			break;
		case VALUE_V:
			r.SetValue(GetEntire());
			break;
		case WSTRING_V:
			RefWString(r) = GetEntire().ToWString();
			break;
		case INT64_V:
			{
				int64 v;
				if (value.getSegment(actualSize, sizeof(v), reinterpret_cast<char*>(&v)))
					RefInt64(r) = v;
			}
			break;
		case BOOL_V:
		default:
			ASSERT(false);
			break;
		}

		value.close();
	}
}

void FBBlob::SetValue(const Value& v)
{
	SetNull(v.IsNull());

	if (v.IsNull())
		return;
	
	const LanguageInfo& li = GetLanguageInfo();
	switch (v.GetType())
	{
	case INT_V:
		SetEntire(reinterpret_cast<const char*>(&ValueTo<int>(v)), sizeof(int));
		break;
	case DOUBLE_V:
		SetEntire(reinterpret_cast<const char*>(&ValueTo<double>(v)), sizeof(double));
		break;
	case STRING_V:
		{
			String s = ValueTo<String>(v);
			SetEntire(~s, s.GetCount());
		}
		break;
	case DATE_V:
		{
			String s = li.FormatDate(ValueTo<Date>(v));
			SetEntire(~s, s.GetCount());
		}
		break;
	case TIME_V:
		{
			String s = li.FormatTime(ValueTo<Time>(v));
			SetEntire(~s, s.GetCount());
		}
		break;
	case VALUE_V:
		SetValue(ValueTo<Value>(v));
		break;
	case WSTRING_V:
		{
			String s = ValueTo<WString>(v).ToString();
			SetEntire(~s, s.GetCount());
		}
		break;
	case INT64_V:
		SetEntire(reinterpret_cast<const char*>(&ValueTo<int64>(v)), sizeof(int64));
		break;
	case BOOL_V:
	default:
		ASSERT(false);
		break;
	}
}

struct FBArray : public FBValue {
	ISC_QUAD	value;

	FBArray(XSQLVAR& v) : FBValue(v)
	{
		v.sqltype = SQL_ARRAY + (v.sqltype & 1);
		v.sqldata = reinterpret_cast<ISC_SCHAR*>(&value);
		v.sqllen = sizeof(value);
	}

	virtual void GetValue(Ref r) const
	{
		if (IsNull())
			r = Null;
		else {
			switch(r.GetType()) {
//			case INT_V:
//				{
//					const char* e = buff.vary_string + buff.vary_length;
//					RefInt(r) = ScanInt(buff.vary_string, &e);
//				}
//				break;
//			case DOUBLE_V:
//				{
//					const char* e = buff.vary_string + buff.vary_length;
//					RefDouble(r) = ScanDouble(buff.vary_string, &e);
//				}
//				break;
//			case STRING_V:
//				RefString(r) = String(buff.vary_string, buff.vary_length);
//				break;
//			case DATE_V:
//				r.SetValue(UPP::Scan(DATE_V, String(buff.vary_string, buff.vary_length)));
//				break;
//			case TIME_V:
//				r.SetValue(UPP::Scan(TIME_V, String(buff.vary_string, buff.vary_length)));
//				break;
//			case VALUE_V:
//				r.SetValue(String(buff.vary_string, buff.vary_length));
//				break;
//			case WSTRING_V:
//				RefWString(r) = WString(buff.vary_string, buff.vary_length);
//				break;
//			case INT64_V:
//				{
//					const char* e = buff.vary_string + buff.vary_length;
//					RefInt64(r) = ScanInt64(buff.vary_string, &e);
//				}
//				break;
			case BOOL_V:
			default:
				ASSERT(false);
				break;
			}
		}
	}
	
	virtual void SetValue(const Value& v)
	{
		SetNull(v.IsNull());
	
		if (v.IsNull())
			return;
		
		switch (v.GetType())
		{
		case INT_V:
			break;
		case DOUBLE_V:
		case STRING_V:
		case DATE_V:
		case TIME_V:
		case VALUE_V:
			SetValue(ValueTo<Value>(v));
			break;
		case WSTRING_V:
		case INT64_V:
		case BOOL_V:
		default:
			ASSERT(false);
			break;
		}
	}
};


class FBConnection : public SqlConnection {
public:
	enum StatementType
	{
		stNone,
		stSelect,
		stSelect4Update,
		stInsert,
		stDelete,
		stUpdate,
		stDDL,
		stTrBegin,
		stTrCommit,
		stTrRollback,
//		stCreate,
//		stCreateDB,
//		stDrop,
//		stAlter,
//		stFunction,
//		stSet,
//		stSetTransaction
	};

public:
	FBConnection(FBSession& s);
	
protected:
	virtual void Cancel();
	virtual bool Execute();
	virtual bool Fetch();
	virtual void GetColumn(int i, Ref r) const;
//	virtual Value GetInsertedId() const;
//	virtual int GetRowsProcessed() const;
	virtual SqlSession& GetSession() const;
	virtual String GetUser() const;
	virtual void SetParam(int i, const Value& r);
	virtual String ToString() const;

protected:
    bool IsDML() const { return st == stInsert || st == stDelete || st == stUpdate; }
//    bool IsDDL() const { return st == stCreate || st == stDrop || st == stAlter; }
    bool IsDDL() const { return st == stDDL; }
    bool IsSelect() const { return st == stSelect || st == stSelect4Update; }
    
	StatementType GetST() const { return st; }
	
	void Prepare();
	void CloseCursor();
	void SetupConverters(ibpp::SQLDataArray& from, Array<FBValue>& to) const;
	
protected:
	bool				executed_:1;
	StatementType		st;
	int					bindNum;
	int					defineNum;
	FBSession&			session;
	ibpp::DynamicSQL	dSQL_;
	Array<FBValue>		defineBind;
	Array<FBValue>		bindBind;
};

FBConnection::FBConnection(FBSession& s)
: executed_(false)
, st(stNone)
, session(s)
, dSQL_(s.db, s.tr, s.dll, s.ib_error)
{
}

String FBConnection::GetUser() const
{
	Sql stmt(session);
	String result;
	Ref rResult(result);
	
	stmt.SetStatement(
	    "SELECT current_user FROM rdb$database"
	);
	stmt.RunX();
	if (stmt.Fetch())
		stmt.GetColumn(0, rResult);
	
	return result;
}

void FBConnection::SetParam(int i, const Value& v)
{
	Prepare();
	bindBind[i].SetValue(v);
}

void FBConnection::SetupConverters(ibpp::SQLDataArray& from, Array<FBValue>& to) const
{
	const int size = from.getSize();
	
	for (int i = 0; i < size; ++i)
	{
		XSQLVAR& var = from[i];
		
		switch (var.sqltype & ~1)
		{
		case SQL_TEXT:
		case SQL_VARYING:
			to.Add(new FBVarying(var));
			break;
		case SQL_SHORT:
		case SQL_LONG:
			if (var.sqlscale < 0)
				to.Add(new FBDouble(var));
			else
				to.Add(new FBInt(var));
			break;
		case SQL_INT64:
			if (var.sqlscale < 0)
				to.Add(new FBDouble(var));
			else
				to.Add(new FBInt64(var));
			break;
		case SQL_FLOAT:
		case SQL_D_FLOAT:
		case SQL_DOUBLE:
			to.Add(new FBDouble(var));
			break;
		case SQL_DATE:
		case SQL_TYPE_TIME:
			to.Add(new FBTimeStamp(var, session.dll));
			break;
		case SQL_TYPE_DATE:
			to.Add(new FBDate(var, session.dll));
			break;
		case SQL_BLOB:
			to.Add(new FBBlob(session, var));
			break;
		case SQL_ARRAY:
			to.Add(new FBArray(var));
			break;
//			break;
//		case SQL_QUAD:
//			break;
		default:
			ASSERT(false);
			break;
		}
	}
}

void FBConnection::Prepare()
{
	if (parse)
	{
		CloseCursor();
		
		// Special case. Statement should not be prepared in case of
		// "CREATE DATABASE" and "SET TRANSACTION" 	

		dSQL_.prepare(statement, session.tmpDataArray);
		
		const int stmt_type = dSQL_.getStmtType();
		
		switch(stmt_type) 
		{
		case isc_info_sql_stmt_select:
			st = stSelect;
			break;
		case isc_info_sql_stmt_insert:
			st = stInsert;
			break;
		case isc_info_sql_stmt_update:
			st = stUpdate;
			break;
		case isc_info_sql_stmt_delete:
			st = stDelete;
			break;
		case isc_info_sql_stmt_ddl:
			st = stDDL;
			break;
//		case isc_info_sql_stmt_get_segment:
//			st = ???;
//			break;
//		case isc_info_sql_stmt_put_segment:
//			st = ???;
//			break;
//		case isc_info_sql_stmt_exec_procedure:
//			st = ???;
//			break;
		case isc_info_sql_stmt_start_trans:
			st = stTrBegin;
			break;
		case isc_info_sql_stmt_commit:
			st = stTrCommit;
			break;
		case isc_info_sql_stmt_rollback:
			st = stTrRollback;
			break;
		case isc_info_sql_stmt_select_for_upd:
			st = stSelect4Update;
			break;
//		case isc_info_sql_stmt_set_generator:
//			st = ???;
//			break;
//		case isc_info_sql_stmt_savepoint:
//			st = ???;
//			break;
		default:
			break;
		}
		
		executed_ = false;

		defineBind.Clear();
		defineNum = session.tmpDataArray.getRequredSize();
		defineBind.Reserve(defineNum);
		ibpp::SQLDataArray& out = dSQL_.GetOut();
		out.setSize(defineNum);
		if (defineNum > 0)
		{
			dSQL_.describe_define(out);			
			SetupConverters(out, defineBind);
		}
		
		dSQL_.describe_bind(session.tmpDataArray);
		bindNum = session.tmpDataArray.getRequredSize();
		bindBind.Clear();
		bindBind.Reserve(bindNum);
		ibpp::SQLDataArray& in = dSQL_.GetIn();
		in.setSize(bindNum);
		if (bindNum)
		{
			dSQL_.describe_bind(in);
			SetupConverters(in, bindBind);
		}
		
		parse = false;
	}
}

void FBConnection::CloseCursor()
{
	if (executed_ && IsSelect() && !session.CursorIsClosed())
	{
		dSQL_.close_cursor();
		session.SetCursorIsClosed();
	}
}

bool FBConnection::Execute()
{
	try {
		Prepare();
		
		// Special case. Statement should not be prepared in case of
		// "CREATE DATABASE" and "SET TRANSACTION" 	
//		if (GetST() == stCreateDB || GetST() == stSetTransaction)
//			dSQL_.execute_immediate_no_trans(statement);
//		else 
		if (IsDDL()) {
			// Rollback transaction and start another.
			// This seems to be a default behaviour with other databases.
			session.RollbackRetaining();
			TRANSACTION_RETAIN(session) {
				dSQL_.execute();
			}
		} else if (IsSelect()) {
			CloseCursor();
			dSQL_.execute();
			executed_ = true;
		} else {
			// For EXECUTE PROCEDURE, for example ...
			if(session.IsExplicitTrans())
				dSQL_.execute2();
			else {
				// Commit/Rollback implicitly.
				TRANSACTION_RETAIN(session) {
					dSQL_.execute2();
				}
			}
		}
	} catch(ibpp::DbExc& e) {
		session.SetError(~e, statement, e.GetErrNum());
		return false;
	}
	
	return true;
}

bool FBConnection::Fetch()
{
	ASSERT(executed_);
	const bool rc = dSQL_.fetch();
	session.SetCursorIsClosed(false);
	return rc;
}

void FBConnection::GetColumn(int i, Ref r) const
{
	defineBind[i].GetValue(r);
}

void FBConnection::Cancel()
{
}

String FBConnection::ToString() const
{
	return String();
}

SqlSession& FBConnection::GetSession() const
{
	return session;
}

FBSession::FBSession(const String& dllName)
: SvcConnected(false)
, DbConnected(false)
, TrStarted(false)
, CursIsClosed(true)
, TrExplicit(false)
, tmpDataArray(1)
, dll(FB())
, ib_error(dll)
, svc(dll, ib_error)
, db(dll, ib_error)
, tr(dll, ib_error)
{
	if (!dllName.IsEmpty())
		dll.SetLibName(dllName);
}

FBSession::~FBSession()
{
	if (TrExplicit)
		RollbackInternal();
	else
		CommitInternal();
	
	if (DbConnected)
		db.dettach();
	
	if (SvcConnected)
		svc.detach();
}

void FBSession::Connect(
	const char* dbname,
	const char* host,
	const char* user,
	const char* pswd,
	const String& dllName,
	ibpp::network_protocol_t protocol
	)
{
	if (!dllName.IsEmpty())
		dll.SetLibName(dllName);
	if (!dll.Load())
		throw ibpp::DbExc(dll.GetLibName() + " wasn't found.");

	// Service
	svc.attach(host, user, pswd, protocol);
	SvcConnected = true;

	// Database
	db.setUserName(user);
	db.setPassword(pswd);
	db.attach(dbname);
	DbConnected = true;
	DbName = dbname;
	
	// Transaction
	tr.setVersion3();
	tr.setReadWrite();
	tr.setReadCommitted();
	tr.setRecVersion();
	
	BeginInternal();
}

SqlConnection * FBSession::CreateConnection()
{
	return new FBConnection(*this);
}

Vector<SqlColumnInfo> FBSession::EnumColumns(String /*database*/, String table)
{
	Vector<SqlColumnInfo> result;
	Sql stmt(*this);
	String name;
	int null_flag;
	int field_type;
	int field_width;
	int field_precision;
	int field_scale;
		
	Ref rName(name);
	Ref rNullFlag(null_flag);
	Ref rFieldType(field_type);
	Ref rFieldWidth(field_width);
	Ref rFieldPrecision(field_precision);
	Ref rFieldScale(field_scale);
	
	stmt.SetStatement(
		" SELECT f.rdb$field_name, \n"
		" f.rdb$field_position, \n"
		" f.rdb$null_flag, \n"
		" fs.rdb$field_type, \n"
		" fs.rdb$field_length, \n"
		" fs.rdb$field_precision, \n"
		" fs.rdb$field_scale, \n"
		" fs.rdb$field_sub_type, \n"
		" fs.rdb$computed_source, \n"
		" fs.rdb$character_length \n"
		" FROM rdb$relation_fields f \n"
		" LEFT JOIN rdb$fields fs ON fs.rdb$field_name = f.rdb$field_source \n"
		" WHERE f.rdb$relation_name = ? \n"
		" ORDER BY f.rdb$field_position"
	);
	stmt.SetParam(0, table);
	stmt.RunX();
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, rName);
		stmt.GetColumn(2, rNullFlag);
		stmt.GetColumn(3, rFieldType);
		stmt.GetColumn(4, rFieldWidth);
		stmt.GetColumn(5, rFieldPrecision);
		stmt.GetColumn(6, rFieldScale);

		SqlColumnInfo& ci = result.Add();
		ci.name = name;
		ci.type = field_type;
		ci.width = field_width;
		ci.precision = field_precision;
		ci.scale = field_scale;
		ci.nullable = rNullFlag != 1;
	}
	
	return result;
}

Vector<String> FBSession::EnumDatabases()
{
	Vector<String> result;
	result.Add(DbName);
	return result;
}

Vector<String> FBSession::EnumPrimaryKey(String /*database*/, String table)
{
	Vector<String> result;
	Sql stmt(*this);
	String name;
	Ref rName(name);
	
	stmt.SetStatement(
		" SELECT i.rdb$field_name, \n"
		" i.rdb$field_position \n"
		" FROM rdb$relation_constraints rc, rdb$index_segments i \n"
		" WHERE (i.rdb$index_name = rc.rdb$index_name) \n"
		" AND (rc.rdb$constraint_type = 'PRIMARY KEY') \n"
		" AND (rc.rdb$relation_name = ?) \n"
		" ORDER BY i.rdb$field_position"
	);
	stmt.SetParam(0, table);
	stmt.RunX();
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, rName);
		result.Add(name);
	}
	
	return result;
}

namespace {
	static const char *rwlist[] = {
		"ACTION", "ACTIVE", "ADD",
		"ADMIN", "AFTER", "ALL",
		"ALTER", "AND", "ANY",
		"AS", "ASC", "ASCENDING",
		"AT", "AUTO", "AUTODDL",
		"AVG", "BASED", "BASENAME",
		"BASE_NAME", "BEFORE", "BEGIN",
		"BETWEEN", "BLOB", "BLOBEDIT",
		"BUFFER", "BY", "CACHE",
		"CASCADE", "CAST", "CHAR",
		"CHARACTER", "CHARACTER_LENGTH", "CHAR_LENGTH",
		"CHECK", "CHECK_POINT_LEN", "CHECK_POINT_LENGTH",
		"COLLATE", "COLLATION", "COLUMN",
		"COMMIT", "COMMITTED", "COMPILETIME",
		"COMPUTED", "CLOSE", "CONDITIONAL",
		"CONNECT", "CONSTRAINT", "CONTAINING",
		"CONTINUE", "COUNT", "CREATE",
		"CSTRING", "CURRENT", "CURRENT_DATE",
		"CURRENT_TIME", "CURRENT_TIMESTAMP", "CURSOR",
		"DATABASE", "DATE", "DAY",
		"DB_KEY", "DEBUG", "DEC",
		"DECIMAL", "DECLARE", "DEFAULT",
		"DELETE", "DESC", "DESCENDING",
		"DESCRIBE", "DESCRIPTOR", "DISCONNECT",
		"DISPLAY", "DISTINCT", "DO",
		"DOMAIN", "DOUBLE", "DROP",
		"ECHO", "EDIT", "ELSE",
		"END", "ENTRY_POINT", "ESCAPE",
		"EVENT", "EXCEPTION", "EXECUTE",
		"EXISTS", "EXIT", "EXTERN",
		"EXTERNAL", "EXTRACT", "FETCH",
		"FILE", "FILTER", "FLOAT",
		"FOR", "FOREIGN", "FOUND",
		"FREE_IT", "FROM", "FULL",
		"FUNCTION", "GDSCODE", "GENERATOR",
		"GEN_ID", "GLOBAL", "GOTO",
		"GRANT", "GROUP", "GROUP_COMMIT_WAIT",
		"GROUP_COMMIT_", "WAIT_TIME", "HAVING",
		"HELP", "HOUR", "IF",
		"IMMEDIATE", "IN", "INACTIVE",
		"INDEX", "INDICATOR", "INIT",
		"INNER", "INPUT", "INPUT_TYPE",
		"INSERT", "INT", "INTEGER",
		"INTO", "IS", "ISOLATION",
		"ISQL", "JOIN", "KEY",
		"LC_MESSAGES", "LC_TYPE", "LEFT",
		"LENGTH", "LEV", "LEVEL",
		"LIKE", "LOGFILE", "LOG_BUFFER_SIZE",
		"LOG_BUF_SIZE", "LONG", "MANUAL",
		"MAX", "MAXIMUM", "MAXIMUM_SEGMENT",
		"MAX_SEGMENT", "MERGE", "MESSAGE",
		"MIN", "MINIMUM", "MINUTE",
		"MODULE_NAME", "MONTH", "NAMES",
		"NATIONAL", "NATURAL", "NCHAR",
		"NO", "NOAUTO", "NOT",
		"NULL", "NUMERIC", "NUM_LOG_BUFS",
		"NUM_LOG_BUFFERS", "OCTET_LENGTH", "OF",
		"ON", "ONLY", "OPEN",
		"OPTION", "OR", "ORDER",
		"OUTER", "OUTPUT", "OUTPUT_TYPE",
		"OVERFLOW", "PAGE", "PAGELENGTH",
		"PAGES", "PAGE_SIZE", "PARAMETER",
		"PASSWORD", "PLAN", "POSITION",
		"POST_EVENT", "PRECISION", "PREPARE",
		"PROCEDURE", "PROTECTED", "PRIMARY",
		"PRIVILEGES", "PUBLIC", "QUIT",
		"RAW_PARTITIONS", "RDB$DB_KEY", "READ",
		"REAL", "RECORD_VERSION", "REFERENCES",
		"RELEASE", "RESERV", "RESERVING",
		"RESTRICT", "RETAIN", "RETURN",
		"RETURNING_VALUES", "RETURNS", "REVOKE",
		"RIGHT", "ROLE", "ROLLBACK",
		"RUNTIME", "SCHEMA", "SECOND",
		"SEGMENT", "SELECT", "SET",
		"SHADOW", "SHARED", "SHELL",
		"SHOW", "SINGULAR", "SIZE",
		"SMALLINT", "SNAPSHOT", "SOME",
		"SORT", "SQLCODE", "SQLERROR",
		"SQLWARNING", "STABILITY", "STARTING",
		"STARTS", "STATEMENT", "STATIC",
		"STATISTICS", "SUB_TYPE", "SUM",
		"SUSPEND", "TABLE", "TERMINATOR",
		"THEN", "TIME", "TIMESTAMP",
		"TO", "TRANSACTION", "TRANSLATE",
		"TRANSLATION", "TRIGGER", "TRIM",
		"TYPE", "UNCOMMITTED", "UNION",
		"UNIQUE", "UPDATE", "UPPER",
		"USER", "USING", "VALUE",
		"VALUES", "VARCHAR", "VARIABLE",
		"VARYING", "VERSION", "VIEW",
		"WAIT", "WEEKDAY", "WHEN",
		"WHENEVER", "WHERE", "WHILE",
		"WITH", "WORK", "WRITE",
		"YEAR", "YEARDAY",
	};
}

Vector<String> FBSession::EnumReservedWords()
{
	Vector<String> result;
	result.SetCount(__countof(rwlist));
	for(int i = 0; i < __countof(rwlist); i++)
		result[i] = rwlist[i];
	return result;
}

Vector<String> FBSession::EnumSequences(String /*database*/)
{
	Vector<String> result;
	Sql stmt(*this);
	String name;	
	Ref rName(name);
	
	stmt.SetStatement(
	    "SELECT RDB$GENERATOR_NAME FROM RDB$GENERATORS WHERE (RDB$SYSTEM_FLAG is NULL) or (RDB$SYSTEM_FLAG <> 1)"
	);
	stmt.RunX();
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, rName);
		result.Add(name);
	}
	
	return result;
}

Vector<String> FBSession::EnumTables(String /*database*/)
{
	Vector<String> result;
	Sql stmt(*this);
	String name;	
	Ref rName(name);
	
	stmt.SetStatement(
	    "SELECT RDB$RELATION_NAME FROM RDB$RELATIONS WHERE (RDB$VIEW_BLR IS NULL) AND (RDB$SYSTEM_FLAG = 0)"
	);
	stmt.RunX();
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, rName);
		result.Add(name);
	}
	
	return result;
}

Vector<String> FBSession::EnumUsers()
{
	using namespace ibpp;
	using namespace ibpp::svc_cmd;
	
	Vector<String> result;
	get_users users(svc);
	
	users.execute();
	const Vector<user_info>& ui = users.get_info();
	
	for(int i = 0; i < ui.GetCount(); ++i)
		result.Add(ui[i].user_name);
	
	return result;
}

Vector<String> FBSession::EnumViews(String /*database*/)
{
	Vector<String> result;
	Sql stmt(*this);
	String name;	
	Ref rName(name);
	
	stmt.SetStatement(
	    "SELECT RDB$RELATION_NAME FROM RDB$RELATIONS WHERE NOT (RDB$VIEW_BLR IS NULL)"
	);
	stmt.RunX();
	while (stmt.Fetch())
	{
		stmt.GetColumn(0, rName);
		result.Add(name);
	}
	
	return result;
}

void FBSession::RollbackTo(const String& savepoint)
{
	Sql stmt(*this);

	stmt.SetStatement(
	    "ROLLBACK TO SAVEPOINT " + savepoint
	);
	stmt.RunX();
}

String FBSession::Savepoint()
{
	static dword i;
	i = (i + 1) & 0x8fffffff;
	String s = Sprintf("SESSION_SAVEPOINT_%d", i);
	Sql stmt(*this);

	stmt.SetStatement(
	    "SAVEPOINT " + s
	);
	stmt.RunX();
	return s;
}

inline
static bool IsIdentChar(int s)
{
	return IsAlNum(s) || s == '_';
}

inline
static void SkipSpaces(const char*& s)
{
	while (IsSpace(*s)) ++s;
}

class BackTrack
{
public:
	BackTrack(const char*& s) : Pos(s), BtPos(s) {}
	
	bool Check(bool v) const
	{
		if (!v)
			Pos = BtPos;
		
		return v;
	}
	
private:
	const char*& Pos;
	const char*  BtPos;
};

static bool scmpw(const char*& s, const char *w) 
{
	BackTrack bt(s);
	
	for(;;) {
		if(*w == '\0')
			return bt.Check(!IsIdentChar(*s));
		if(ToUpper(*s++) != ToUpper(*w++))
			return bt.Check(false);
	}
}

static bool SkipOrAlter(const char*& s)
{
	SkipSpaces(s);
	
	if (scmpw(s, "or"))
	{
		SkipSpaces(s);
	
		scmpw(s, "alter");
	}
	
	return true;
}

bool FirebirdPerformScript(const String& text, StatementExecutor& exe, Gate2<int, int> progress_canceled)
{
	String statement;
	String term(";");
	String new_term;
	const int size = text.GetCount();
	const char* s = text;
	bool found;

	do {
		found = false;
		bool found_block = true;
		bool prev_ident_char = false;
		bool execute = false;
		int level = 0;
		const char* b = s;

		while (*s) {
			// Multi-line comment.
			if (*s == '/' && *(s+1) == '*') {
				s += 2;
				while (*s && (*s++ != '*' || *s != '/'));
				if (*s) ++s;
			}
			// One-line comment.
			else if (*s == '-' && *(s+1) == '-') {
				s += 2;
				while (*s && *s != '\n') ++s;
				if (*s) ++s;
			}
			// String. 
			else if (*s == '\'') {
				while(*++s && (*s != '\'' || *++s == '\''));
				if (*s) ++s;
			}
			else if (scmpw(s, term) && level == 0) {
				if (!new_term.IsEmpty()) {
					term = new_term;
					new_term = Null;
					found = true;
					// Statement starts from here.
					b = s;
					break;
				}
				else if (found_block) {
					statement.Cat(b, s);
					found = true;
					execute = true;
					break;
				}
			}
			else if (!prev_ident_char && level == 0 && scmpw(s, "set")) {
				SkipSpaces(s);
				
				if (scmpw(s, "term"))
				{
					SkipSpaces(s);
					const char* b = s;
					while (!IsSpace(*s)) ++s;
					new_term.Cat(b, s);
				}
			}
			else if (!prev_ident_char && scmpw(s, "begin")) {
				found_block = true;
				++level;
			}
			else if (!prev_ident_char && scmpw(s, "end")) {
				--level;
			}
			else if (!prev_ident_char && scmpw(s, "alter") || scmpw(s, "recreate") || (scmpw(s, "create") && SkipOrAlter(s)))
			{
				SkipSpaces(s);
				
				if (scmpw(s, "procedure") || scmpw(s, "trigger"))
					found_block = false;
			}
			else
			{
				prev_ident_char = IsIdentChar(*s);
				++s;
			}
		}
		
		if (found && execute)
		{
			if(!exe.Execute(statement)) return false;
			statement = Null;
		}	
	} while (found);
	
	return true;
}

RunScript FBSession::GetRunScript() const
{
	return &FirebirdPerformScript;
}

void FBSession::Begin()
{
	BeginInternal();
	TrExplicit = true;
}

void FBSession::Commit()
{
	CommitRetaining();
	TrExplicit = false;
}

void FBSession::Rollback()
{
	RollbackRetaining();
	TrExplicit = false;
}

void FBSession::CommitRetaining()
{
	if (TrStarted)
	{
		tr.commit_retaining();
		SetCursorIsClosed(); // By transaction.
	}
}

void FBSession::RollbackRetaining()
{
	if (TrStarted)
	{
		tr.rollback_retaining();
		SetCursorIsClosed(); // By transaction.
	}
}

void FBSession::BeginInternal()
{
	if (!TrStarted)
	{
		tr.start(db);
		TrStarted = true;
	}
}

void FBSession::CommitInternal()
{
	if (TrStarted)
	{
		tr.commit();
		TrStarted = false;
		SetCursorIsClosed(); // By transaction.
	}
}

void FBSession::RollbackInternal()
{
	if (TrStarted)
	{
		tr.rollback();
		TrStarted = false;
		SetCursorIsClosed(); // By transaction.
	}
}

bool FBSession::IsOpen() const
{
	return DbConnected;
}

Value FBSequence::Get() {
	static SqlId ONE_RECORD_TABLE("RDB$DATABASE");
#ifndef NOAPPSQL
	Sql sql(session ? *session : SQL.GetSession());
#else
	ASSERT(session);
	Sql sql(*session);
#endif

	sql.SetStatement(
	    "SELECT GEN_ID(" + seq.ToString() + ", 1) FROM RDB$DATABASE"
	);
	
	if (sql.Run() && sql.Fetch())
		return sql[0];
	
	return ErrorValue();
}

END_UPP_NAMESPACE
