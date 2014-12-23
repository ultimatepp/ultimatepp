#include <Esc/Esc.h>


NAMESPACE_UPP

#define LTIMING(x)  // RTIMING(x)

void ESC_count(EscEscape& e)
{
	e = e[0].GetCount();
}

void ESC_is_void(EscEscape& e)
{
	e = e[0].IsVoid();
}

void ESC_is_number(EscEscape& e)
{
	e = e[0].IsNumber();
}

void ESC_is_array(EscEscape& e)
{
	e = e[0].IsArray();
}

void ESC_is_map(EscEscape& e)
{
	e = e[0].IsMap();
}

void ESC_int(EscEscape& e)
{
	if(e[0].IsNumber())
		e = (int)e[0].GetNumber();
}

void ESC_to_string(EscEscape& e)
{
	e = e[0].ToString();
}

void ESC_to_number(EscEscape& e)
{
	if(e[0].IsArray()) {
		double d = ScanDouble((String)e[0]);
		if(!IsNull(d)) {
			if(d <= INT64_MAX && d >= INT64_MIN)
				e = ScanInt64((String)e[0]);
			else
				e = d;
		}
	}
}

void ESC_rand(EscEscape& e)
{
	e = rand();
}

void ESC_keys(EscEscape& e)
{
	e.CheckMap(0);
	EscValue v;
	const VectorMap<EscValue, EscValue>& m = e[0].GetMap();
	for(int i = 0; i < m.GetCount(); i++)
		if(!m.IsUnlinked(i))
			v.ArrayAdd(m.GetKey(i));
	e = v;
}

void ESC_values(EscEscape& e)
{
	e.CheckMap(0);
	EscValue v;
	const VectorMap<EscValue, EscValue>& m = e[0].GetMap();
	for(int i = 0; i < m.GetCount(); i++)
		if(!m.IsUnlinked(i))
			v.ArrayAdd(m[i]);
	e = v;
}

void ESC_reverse(EscEscape& e)
{
	e.CheckArray(0);
	const Vector<EscValue>& a = e[0].GetArray();
	EscValue r;
	for(int i = a.GetCount() - 1; i >= 0; i--)
		r.ArrayAdd(a[i]);
	e = r;
}

struct EscCmp {
	Esc *esc;
	bool operator()(const EscValue& a, const EscValue& b) const {
		return esc->DoCompare(a, b, "< (sort)") < 0;
	}
};

Vector<int> EscGetSortOrder(EscEscape& e)
{
	e.CheckArray(0);
	const Vector<EscValue>& va = e[0].GetArray();
	EscCmp cmp;
	cmp.esc = &e.esc;
	return GetSortOrder(va, cmp);
}

void ESC_sort(EscEscape& e)
{
	Vector<int> so = EscGetSortOrder(e);
	EscValue r;
	for(int i = 0; i < so.GetCount(); i++)
		r.ArrayAdd(e[0].ArrayGet(so[i]));
	e = r;
}

void ESC_order(EscEscape& e)
{
	Vector<int> so = EscGetSortOrder(e);
	EscValue r;
	for(int i = 0; i < so.GetCount(); i++)
		r.ArrayAdd(so[i]);
	e = r;
}

// ---------------------------

void ESC_mid(EscEscape& e)
{
	e.CheckArray(0);
	int pos = e.Int(1);
	int count = e.Int(2);
	if(pos < 0 || pos + count > e[0].GetCount())
		e.ThrowError("out of bounds in call to 'mid'");
	e.ret_val = e[0].ArrayGet(pos, count);
}

void ESC_exists(EscEscape& e)
{
	e.CheckMap(0);
	e = !e[0].MapGet(e[1]).IsVoid();
}

struct ESC_FileOut : public EscHandle {
	FileStream file;
	void Put(EscEscape& e)         { if(file) file.Put(e.Int(0)); }
	void PutLine(EscEscape& e)     { if(file) file.PutLine(String(e[0])); }
	void Close(EscEscape& e)       { if(file) file.Close(); }

	typedef ESC_FileOut CLASSNAME;

	ESC_FileOut(EscEscape& e, EscValue& v, int style) {
		file.Open(String(e[0]), style);
		v.Escape("Put(a)", this, THISBACK(Put));
		v.Escape("PutLine(a)", this, THISBACK(PutLine));
		v.Escape("Close()", this, THISBACK(Close));
	}
};

void ESC_OpenFileOut(EscEscape& e)
{
	EscValue v;
	ESC_FileOut *f = new ESC_FileOut(e, v, FileStream::CREATE);
	if(f->file)
		e = v;
}

void ESC_OpenFileAppend(EscEscape& e)
{
	EscValue v;
	ESC_FileOut *f = new ESC_FileOut(e, v, FileStream::APPEND);
	if(f->file)
		e = v;
}

struct ESC_FileIn : public EscHandle {
	FileIn file;
	void IsEof(EscEscape& e)       { e = file.IsEof(); }
	void Get(EscEscape& e)         { e = file.Get(); }
	void GetLine(EscEscape& e)     { e = file.GetLine(); }
	void Close(EscEscape& e)       { if(file) file.Close(); }

	typedef ESC_FileIn CLASSNAME;

	ESC_FileIn(EscEscape& e, EscValue& v) {
		file.Open(String(e[0]));
		v.Escape("IsEof()", this, THISBACK(IsEof));
		v.Escape("Get()", this, THISBACK(Get));
		v.Escape("GetLine()", this, THISBACK(GetLine));
		v.Escape("Close()", this, THISBACK(Close));
	}
};

void ESC_OpenFileIn(EscEscape& e)
{
	EscValue v;
	ESC_FileIn *f = new ESC_FileIn(e, v);
	if(f->file)
		e = v;
}

// ---------------------------

void ESC_sin(EscEscape& e)
{
	e = sin(e[0].GetNumber());
}

void ESC_cos(EscEscape& e)
{
	e = cos(e[0].GetNumber());
}


bool IsDate(const EscValue& v)
{
	return v.HasNumberField("year") && v.HasNumberField("month") && v.HasNumberField("day");
}

bool IsTime(const EscValue& v)
{
	return IsDate(v) && v.HasNumberField("hour") && v.HasNumberField("minute") && v.HasNumberField("second");
}

void SIC_IsDate(EscEscape& e)
{
	e = IsDate(e[0]);
}

void SIC_IsTime(EscEscape& e)
{
	e = IsTime(e[0]);
}

void SIC_GetSysTime(EscEscape& e)
{
	Time tm = GetSysTime();
	EscValue v;
	v.MapSet("year", (int)tm.year);
	v.MapSet("month", (int)tm.month);
	v.MapSet("day", (int)tm.day);
	v.MapSet("hour", (int)tm.hour);
	v.MapSet("minute", (int)tm.minute);
	v.MapSet("second", (int)tm.second);
	e = v;
}

void ESC_ToLower(EscEscape& e)
{
	String s = e[0];
	e = ToLower(s);
}

void ESC_ToUpper(EscEscape& e)
{
	String s = e[0];
	e = ToUpper(s);
}

void StdLib(ArrayMap<String, EscValue>& global)
{
	Escape(global, "is_number(value)", ESC_is_number);
	Escape(global, "is_array(value)", ESC_is_array);
	Escape(global, "is_map(value)", ESC_is_map);
	Escape(global, "is_void(value)", ESC_is_void);
	Escape(global, "int(value)", ESC_int);
	Escape(global, "to_string(value)", ESC_to_string);
	Escape(global, "to_number(value)", ESC_to_number);
	Escape(global, "count(value)", ESC_count);
	Escape(global, "keys(map)", ESC_keys);
	Escape(global, "values(map)", ESC_values);
	Escape(global, "rand()", ESC_rand);
	Escape(global, "reverse(array)", ESC_reverse);
	Escape(global, "sort(array)", ESC_sort);
	Escape(global, "order(array)", ESC_order);

	Escape(global, "ToUpper(value)", ESC_ToUpper);
	Escape(global, "ToLower(value)", ESC_ToLower);

	Escape(global, "len(x)", ESC_count);
	Escape(global, "mid(array, pos, count)", ESC_mid);
	Escape(global, "exists(map, key)", ESC_exists);

	Escape(global, "OpenFileOut(x)", ESC_OpenFileOut);
	Escape(global, "OpenFileAppend(x)", ESC_OpenFileOut);
	Escape(global, "OpenFileIn(x)", ESC_OpenFileIn);

	Escape(global, "GetSysTime()", SIC_GetSysTime);
	Escape(global, "IsDate(x)", SIC_IsDate);
	Escape(global, "IsTime(x)", SIC_IsTime);

	Escape(global, "sin(value)", ESC_sin);
	Escape(global, "cos(value)", ESC_cos);
}

END_UPP_NAMESPACE
