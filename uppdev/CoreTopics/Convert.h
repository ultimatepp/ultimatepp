unsigned      stou(const char *ptr, void *endptr = NULL, unsigned base = 10);
inline unsigned stou(const byte *ptr, void *endptr = NULL, unsigned base = 10) { return stou((const char *)ptr, endptr, base); }
unsigned      stou(const wchar *ptr, void *endptr = NULL, unsigned base = 10);

uint64        stou64(const char *s, void *endptr = NULL, unsigned base = 10);
uint64        stou64(const wchar *s, void *endptr = NULL, unsigned base = 10);

int           ScanInt(const char *ptr, const char **endptr = NULL, int radix = 10);
int           ScanInt(const wchar *ptr, const wchar **endptr = NULL, int radix = 10);

int64         ScanInt64(const char *ptr, const char **endptr = NULL, int base = 10);

double        ScanDouble(const char *ptr, const char **endptr = NULL, bool accept_comma = true);
double        ScanDouble(const wchar *ptr, const wchar **endptr = NULL, bool accept_comma = true);

Value         StrIntValue(const char *s);
inline int    StrInt(const char* s) { return ScanInt(s); }
inline double IntDbl(int i)         { return IsNull(i) ? double(Null) : double(i); }
inline int    DblInt(double d)      { return IsNull(d) ? int(Null) : fround(d); }
Value         StrDblValue(const char* s);
inline double StrDbl(const char* s) { return ScanDouble(s); }

Value NotNullError();

class Convert {
public:
#ifdef flagSO
	Convert();
	virtual ~Convert();
#else
	virtual ~Convert() {}
#endif

	virtual Value  Format(const Value& q) const;
	virtual Value  Scan(const Value& text) const;
	virtual int    Filter(int chr) const;

	Value operator()(const Value& q) const              { return Format(q); }
};

const Convert& StdConvert();

String StdFormat(const Value& q);

class ConvertInt : public Convert {
public:
	virtual Value Scan(const Value& text) const;
	virtual int   Filter(int chr) const;

protected:
	int64 minval, maxval;
	bool  notnull;

public:
	ConvertInt& MinMax(int _min, int _max)        { minval = _min; maxval = _max; return *this; }
	ConvertInt& Min(int _min)                     { minval = _min; return *this; }
	ConvertInt& Max(int _max)                     { maxval = _max; return *this; }
	ConvertInt& NotNull(bool b = true)            { notnull = b; return *this; }
	ConvertInt& NoNotNull()                       { return NotNull(false); }
	int         GetMin() const                    { return (int)minval; }
	int         GetMax() const                    { return (int)maxval; }
	bool        IsNotNull() const                 { return notnull; }

#ifdef flagSO
	ConvertInt(int minval = -INT_MAX, int maxval = INT_MAX, bool notnull = false);
	virtual ~ConvertInt();
#else
	ConvertInt(int minval = -INT_MAX, int maxval = INT_MAX, bool notnull = false)
		: minval(minval), maxval(maxval), notnull(notnull) {}
#endif
};

const ConvertInt& StdConvertInt();
const ConvertInt& StdConvertIntNotNull();

struct ConvertInt64 : public ConvertInt {
	ConvertInt& MinMax(int64 _min, int64 _max)    { minval = _min; maxval = _max; return *this; }
	ConvertInt& Min(int64 _min)                   { minval = _min; return *this; }
	ConvertInt& Max(int64 _max)                   { maxval = _max; return *this; }
	int64       GetMin() const                    { return minval; }
	int64       GetMax() const                    { return maxval; }

#ifdef flagSO
	ConvertInt64(int64 minval = -INT64_MAX, int64 maxval = INT64_MAX, bool notnull = false);
	virtual ~ConvertInt64();
#else
	ConvertInt64(int64 minval = -INT64_MAX, int64 maxval = INT64_MAX, bool notnull = false) {
		MinMax(minval, maxval); NotNull(notnull);
	}
#endif
};

class ConvertDouble : public Convert {
public:
	virtual Value Format(const Value& q) const;
	virtual Value Scan(const Value& text) const;
	virtual int   Filter(int chr) const;

protected:
	double      minval, maxval;
	bool        notnull;
	String      pattern;

public:
	ConvertDouble& Pattern(const char *p)            { pattern = p; return *this; }
	ConvertDouble& MinMax(double _min, double _max)  { minval = _min; maxval = _max; return *this; }
	ConvertDouble& Min(double _min)                  { minval = _min; return *this; }
	ConvertDouble& Max(double _max)                  { maxval = _max; return *this; }
	ConvertDouble& NotNull(bool b = true)            { notnull = b; return *this; }
	ConvertDouble& NoNotNull()                       { return NotNull(false); }
	double         GetMin() const                    { return minval; }
	double         GetMax() const                    { return maxval; }
	bool           IsNotNull() const                 { return notnull; }

	ConvertDouble(double minval = DOUBLE_NULL_LIM, double maxval = -DOUBLE_NULL_LIM,
		          bool notnull = false);
#ifdef flagSO
	virtual ~ConvertDouble();
#endif
};

const ConvertDouble& StdConvertDouble();
const ConvertDouble& StdConvertDoubleNotNull();

class ConvertDate : public Convert {
public:
	virtual Value Scan(const Value& text) const;
	virtual int   Filter(int chr) const;

protected:
	Date minval, maxval;
	bool notnull;

public:
	ConvertDate& MinMax(Date _min, Date _max)      { minval = _min; maxval = _max; return *this; }
	ConvertDate& Min(Date _min)                    { minval = _min; return *this; }
	ConvertDate& Max(Date _max)                    { maxval = _max; return *this; }
	ConvertDate& NotNull(bool b = true)            { notnull = b; return *this; }
	ConvertDate& NoNotNull()                       { return NotNull(false); }
	Date         GetMin() const                    { return minval; }
	Date         GetMax() const                    { return maxval; }
	bool         IsNotNull() const                 { return notnull; }

	ConvertDate(Date minval = Date::Low(), Date maxval = Date::High(), bool notnull = false);
	virtual ~ConvertDate();
};

const ConvertDate& StdConvertDate();
const ConvertDate& StdConvertDateNotNull();

class ConvertTime : public Convert {
public:
	virtual Value Scan(const Value& text) const;
	virtual int   Filter(int chr) const;
	virtual Value Format(const Value& q) const;

protected:
	Time minval, maxval;
	bool notnull;
	bool seconds;

public:
	ConvertTime& MinMax(Time _min, Time _max)      { minval = _min; maxval = _max; return *this; }
	ConvertTime& NotNull(bool b = true)            { notnull = b; return *this; }
	ConvertTime& NoNotNull()                       { return NotNull(false); }
	ConvertTime& Seconds(bool b = true)            { seconds = b; return *this; }
	ConvertTime& NoSeconds()                       { return Seconds(false); }
	Time         GetMin() const                    { return minval; }
	Time         GetMax() const                    { return maxval; }
	bool         IsNotNull() const                 { return notnull; }

	ConvertTime(Time minval = ToTime(Date::Low()), Time maxval = ToTime(Date::High()), bool notnull = false);
	virtual ~ConvertTime();
};

const ConvertTime& StdConvertTime();
const ConvertTime& StdConvertTimeNotNull();

class ConvertString : public Convert {
public:
	virtual Value Scan(const Value& text) const;

protected:
	int  maxlen;
	bool notnull;

public:
	ConvertString& MaxLen(int _maxlen)             { maxlen = _maxlen; return *this; }
	int            GetMaxLength() const            { return maxlen; }
	ConvertString& NotNull(bool b = true)          { notnull = b; return *this; }
	ConvertString& NoNotNull()                     { return NotNull(false); }
	bool           IsNotNull() const               { return notnull; }

#ifdef flagSO
	ConvertString(int maxlen = INT_MAX, bool notnull = false);
	virtual ~ConvertString();
#else
	ConvertString(int maxlen = INT_MAX, bool notnull = false)
		: maxlen(maxlen), notnull(notnull) {}
#endif
};

const ConvertString& StdConvertString();
const ConvertString& StdConvertStringNotNull();

class NoConvertClass : public Convert {
public:
	virtual Value  Format(const Value& q) const;
};

const NoConvertClass& NoConvert();

class MapConvert : public Convert {
public:
	virtual Value  Format(const Value& q) const;

protected:
	VectorMap<Value, Value> map;

public:
	void         Clear()                                 { map.Clear(); }
	MapConvert&  Add(const Value& a, const Value& b)     { map.Add(a, b); return *this; }

	int          GetCount() const                        { return map.GetCount(); }
	int          Find(const Value& v) const              { return map.Find(v); }
	const Value& GetKey(int i) const                     { return map.GetKey(i); }
	const Value& GetValue(int i) const                   { return map[i]; }
	const Value& operator[](int i) const                 { return map[i]; }

	virtual ~MapConvert() {}
};

class JoinConvert : public Convert {
public:
	virtual Value Format(const Value& v) const;

protected:
	struct Item {
		int            pos;
		const Convert *convert;
		String         text;
	};
	Array<Item> item;

	int NextPos() const;

public:
	JoinConvert& Add(const char *text);
	JoinConvert& Add(int pos, const Convert& cv);
	JoinConvert& Add(int pos);
	JoinConvert& Add(const Convert& cv);
	JoinConvert& Add();
};

class FormatConvert : public Convert {
public:
	virtual Value Format(const Value& v) const;

private:
	String format;

public:
	void   SetFormat(const char *fmt)           { format = fmt; }
};
