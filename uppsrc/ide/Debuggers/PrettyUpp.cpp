#include "Debuggers.h"

#ifdef PLATFORM_WIN32

void Pdb::PrettyString(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	bool small = IntAt(val, "chr", 14) == 0;
	p.data_count = small ? IntAt(val, "chr", 15) : IntAt(val, "w", 2);
	adr_t a = (small ? GetAttr(val, "chr") : DeRef(GetAttr(val, "ptr"))).address;
	p.data_type << "char";
	for(int i = 0; i < count; i++)
		p.data_ptr.Add(a + from + i);
	p.kind = TEXT;
}

void Pdb::PrettyWString(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	p.data_count = GetInt64(GetAttr(val, "length"));
	Val q = DeRef(GetAttr(val, "ptr"));
	int sz = SizeOfType(q.type);
	p.data_type.Add(sz == 4 ? "int" : "short int");
	for(int i = 0; i < count; i++)
		p.data_ptr.Add(q.address + from + sz * i);
	p.kind = TEXT;
}

void Pdb::PrettyVector(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	p.data_count = GetInt64(GetAttr(val, "items"));
	Val item = DeRef(GetAttr(val, "vector"));
	int sz = SizeOfType(tparam[0]);
	for(int i = 0; i < count; i++)
		p.data_ptr.Add(item.address + (i + from) * sz);
}

void Pdb::PrettyBiVector(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	p.data_count = GetInt64(GetAttr(val, "items"));
	int start = GetInt(GetAttr(val, "start"));
	int alloc = GetInt(GetAttr(val, "alloc"));
	Val item = DeRef(GetAttr(val, "vector"));
	int sz = SizeOfType(tparam[0]);
	for(int i = 0; i < count; i++) {
		int ii = i + start < alloc ? i + start : i + start - alloc;
		p.data_ptr.Add(item.address + (ii + from) * sz);
	}
}

void Pdb::PrettyIndex(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	return PrettyVector(GetAttr(val, "key"), tparam, from, count, p);
}

void Pdb::PrettyArray(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	PrettyVector(GetAttr(val, "vector"), { "int *" }, from, count, p);
	for(adr_t& a : p.data_ptr)
		a = PeekPtr(a);
}

void Pdb::PrettyBiArray(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	PrettyBiVector(GetAttr(val, "bv"), { "int *" }, from, count, p);
	for(adr_t& a : p.data_ptr)
		a = PeekPtr(a);
}

void Pdb::PrettyMap(Pdb::Pretty& p, Pdb::Pretty& key, Pdb::Pretty& value)
{
	p.data_count = min(key.data_count, value.data_count);
	int n = min(key.data_ptr.GetCount(), value.data_ptr.GetCount());
	for(int i = 0; i < n; i++)
		p.data_ptr << key.data_ptr[i] << value.data_ptr[i];
}

void Pdb::PrettyVectorMap(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	Pretty key, value;
	PrettyIndex(GetAttr(val, "key"), { tparam[0] }, from, count, key);
	PrettyVector(GetAttr(val, "value"), { tparam[1] }, from, count, value);
	PrettyMap(p, key, value);
}

void Pdb::PrettyArrayMap(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	Pretty key, value;
	PrettyIndex(GetAttr(val, "key"), { tparam[0] }, from, count, key);
	PrettyArray(GetAttr(val, "value"), { tparam[1] }, from, count, value);
	PrettyMap(p, key, value);
}

void Pdb::PrettyDate(Pdb::Val val, const Vector<String>&, int64 from, int count, Pdb::Pretty& p)
{
	int day = GetIntAttr(val, "day");
	int month = GetIntAttr(val, "month");
	int year = GetIntAttr(val, "year");
	p.kind = SINGLE_VALUE;
	if(year < -20000)
		p.SetNull();
	else
	if(day >= 0 && day <= 31 && month >= 1 && month <= 12 && year > 1900 && year < 3000)
		p.Text(Format("%04d/%02d/%02d", year, month, day));
}

void Pdb::PrettyTime(Pdb::Val val, const Vector<String>&, int64 from, int count, Pdb::Pretty& p)
{
	int day = GetIntAttr(val, "day");
	int month = GetIntAttr(val, "month");
	int year = GetIntAttr(val, "year");
	int hour = GetIntAttr(val, "hour");
	int minute = GetIntAttr(val, "minute");
	int second = GetIntAttr(val, "second");
	p.kind = SINGLE_VALUE;
	if(year < -20000)
		p.SetNull();
	else
	if(day >= 1 && day <= 31 && month >= 1 && month <= 12 && year > 1900 && year < 3000 &&
	   hour >= 0 && hour <= 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60)
		p.Text(Format("%04d/%02d/%02d %02d:%02d:%02d", year, month, day, hour, minute, second));
}

void Pdb::PrettyFont(Pdb::Val val, const Vector<String>&, int64 from, int count, Pdb::Pretty& p)
{
	int64 data = GetInt64Attr(val, "data");
	Font fnt;
	memcpy(&fnt, &data, 8);
	p.Text(AsString(fnt));
	p.kind = SINGLE_VALUE;
}

void Pdb::PrettyValueArray_(adr_t a, Pdb::Pretty& p)
{
	Val v = GetAttr(MakeVal("Upp::ValueArray::Data", PeekPtr(a)), "data");
	p.data_type << "Upp::Vector<Upp::Value>";
	p.data_ptr << v.address;
	p.kind = SINGLE_VALUE;
}

void Pdb::PrettyValueArray(Pdb::Val val, const Vector<String>&, int64 from, int count, Pdb::Pretty& p)
{
	PrettyValueArray_(GetAttr(val, "data").address, p);
}

void Pdb::PrettyValueMap_(adr_t a, Pdb::Pretty& p, int64 from, int count)
{
	p.kind = CONTAINER;
	p.data_type << "Upp::Value" << "Upp::Value";

	Pretty key, value;
	Val m = MakeVal("Upp::ValueMap::Data", PeekPtr(a));
	PrettyIndex(GetAttr(m, "key"), { "Upp::Value" }, from, count, key);
	PrettyVector(GetAttr(DeRef(GetAttr(GetAttr(m, "value"), "data")), "data"),
	             { "Upp::Value" }, from, count, value);
	PrettyMap(p, key, value);
}

void Pdb::PrettyValueMap(Pdb::Val val, const Vector<String>&, int64 from, int count, Pdb::Pretty& p)
{
	PrettyValueMap_(GetAttr(val, "data").address, p, from, count);
}

void Pdb::PrettyValue(Pdb::Val val, const Vector<String>&, int64 from, int count, Pdb::Pretty& p)
{
	val = GetAttr(val, "data");
	adr_t a = val.address;
	p.kind = SINGLE_VALUE;
	if(PeekWord(a + 12) && !PeekWord(a + 14)) { // this is not String
		int st = PeekByte(a + 13);
		if(st == 255) { // REF
			st = PeekDword(a + 8);
			if(st == 8) { // WSTRING_V
				a = PeekPtr(a);
				Val v = GetAttr(MakeVal("Upp::RawValueRep<Upp::WString>", a), "v");
				p.data_type << "Upp::WString";
				p.data_ptr << v.address;
			}
			if(st == 6) { // ERROR_V
				p.Text("ErrorValue ", SLtRed());
				a = PeekPtr(a);
				Val v = GetAttr(MakeVal("Upp::RawValueRep<Upp::String>", a), "v");
				p.data_type << "Upp::String";
				p.data_ptr << v.address;
			}
			if(st == 9) // VALUEARRAY_V
				PrettyValueArray_(a, p);
			if(st == 12)
				PrettyValueMap_(a, p, from, count);

			static VectorMap<int, String> single {
				{ 20, "Upp::Complex" }, { 40, "Upp::Font" },
				{ 48, "Upp::Painting" }, { 49, "Upp::Drawing" }, { 150, "Upp::Image" },
				{ POINT64_V, "Upp::Point_<__int64>" }, { POINTF_V, "Upp::Point_<double>" },
				{ SIZE64_V, "Upp::Size_<__int64>" }, { SIZEF_V, "Upp::Size_<double>" },
				{ RECT_V, "Upp::Rect_<int>" },
				{ RECT64_V, "Upp::Rect_<__int64>" }, { RECTF_V, "Upp::Rect_<double>" },
			};
			
			String t = single.Get(st, Null);
			if(t.GetCount()) {
				p.data_type << t;
				p.data_ptr << PeekPtr(a) + (win64 ? 16 : 8);
			}
			return;
		}
		p.data_ptr << a;
		if(st == 3) {
			p.Text("void", SCyan);
			return;
		}
		static VectorMap<int, String> single {
			{ 1, "int" }, { 2, "double" }, { 4, "Upp::Date" }, { 5, "Upp::Time" }, { 10, "int64" },
			{ 11, "bool" }, {39, "Upp::Color" },
			{ POINT_V, "Upp::Point_<int>" }, { SIZE_V, "Upp::Size_<int>" }
		};
		String t = single.Get(st, Null);
		if(t.GetCount())
			p.data_type << t;
	}
	else {
		p.data_ptr << a;
		p.data_type << "Upp::String";
	}
}

void Pdb::PrettyColor(Pdb::Val val, const Vector<String>&, int64 from, int count, Pdb::Pretty& p)
{
	dword color = GetIntAttr(val, "color");
	Color c = Color::FromRaw(color);
	if(IsNull(c))
		p.SetNull();
	else {
		p.Text("\1", c);
		p.Text(" ");
		p.Text(ColorToHtml(c));
	}
	p.kind = SINGLE_VALUE;
}

void Pdb::PrettyRGBA(Pdb::Val val, const Vector<String>&, int64 from, int count, Pdb::Pretty& p)
{
	RGBA rc;
	rc.r = GetIntAttr(val, "r");
	rc.g = GetIntAttr(val, "g");
	rc.b = GetIntAttr(val, "b");
	rc.a = 255;
	
	Color c(rc);

	p.Text("\1", c);
	p.Text(" ");
	p.Text(ColorToHtml(c));
	p.Text(", a: " + AsString(GetIntAttr(val, "a")));
	
	p.kind = SINGLE_VALUE;
}

void Pdb::PrettyImageBuffer(Pdb::Val val, const Vector<String>&, int64 from, int count, Pdb::Pretty& p)
{
	p.kind = SINGLE_VALUE;

	Val sz = GetAttr(val, "size");
	String h;
	h.Cat("\2");
	PrettyImage img;
	img.size.cx = (int)GetIntAttr(sz, "cx");
	img.size.cy = (int)GetIntAttr(sz, "cy");
	img.pixels = DeRef(GetAttr(GetAttr(val, "pixels"), "ptr")).address;

	if(img.pixels && img.size.cx * img.size.cy) {
		RawCat(h, img);
		p.Text(h);
	}
	else
		p.SetNull();

	bool error = img.size.cx < 0 || img.size.cx > 10000 || img.size.cy < 0 || img.size.cy > 10000 || !img.pixels;
	Color c1 = error ? SLtRed() : SLtBlue();
	Color c2 = error ? SLtRed() : SRed();
	p.Text(" [", c1);
	p.Text(AsString(img.size.cx), c2);
	p.Text("x", c1);
	p.Text(AsString(img.size.cy), c2);
	p.Text("]", c1);
}

void Pdb::PrettyImg(Pdb::Val val, const Vector<String>& t, int64 from, int count, Pdb::Pretty& p)
{
	p.kind = SINGLE_VALUE;
	val = DeRef(GetAttr(val, "data"));
	if(val.address)
		PrettyImageBuffer(GetAttr(val, "buffer"), t, from, count, p);
	else
		p.SetNull();
}

#endif
