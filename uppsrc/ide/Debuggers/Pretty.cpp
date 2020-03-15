#include "Debuggers.h"

#ifdef PLATFORM_WIN32

// THIS is currently hardwired to the current version of U++ Core...

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
	adr_t a = DeRef(GetAttr(val, "ptr")).address;
	p.data_type << "short int";
	for(int i = 0; i < count; i++)
		p.data_ptr.Add(a + from + 2 * i);
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

void Pdb::PrettyStdString(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	adr_t a;
	int size;
	bool w = tparam[0] == "wchar_t";
	if(HasAttr(val, "__r_")) { // CLANG variant
		Val r = GetAttr(GetAttr(val, "__r_"), "__value_");
		Val s = GetAttr(r, "__s");
		size = GetByteAttr(s, "__size_");
		if(size & 1) {
			Val l = GetAttr(r, "__l");
			size = GetIntAttr(l, "__size_");
			a = DeRef(GetAttr(l, "__data_")).address;
		}
		else {
			size = size >> 1;
			a = s.address + 1 + w;
		}
	}
	else {
		Val q = GetAttr(GetAttr(val, "_Mypair"), "_Myval2");
		size = GetIntAttr(q, "_Mysize");
		int res = GetIntAttr(q, "_Myres");
		a = GetAttr(GetAttr(q, "_Bx"), "_Buf").address;
		if(res >= (w ? 8 : 16))
			a = PeekPtr(a);
	}
	p.data_count = size;
	p.data_type << (w ? "short int" : "char");
	int sz = w + 1;
	for(int i = 0; i < count; i++)
		p.data_ptr.Add(a + sz * (from + i));
	p.kind = TEXT;
}

void Pdb::PrettyStdVector(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	Val begin, end;
	if(HasAttr(val, "__begin_")) { // CLANG's std::vector
		begin = DeRef(GetAttr(val, "__begin_"));
		end = DeRef(GetAttr(val, "__end_"));
	}
	else {
		Val q = GetAttr(GetAttr(val, "_Mypair"), "_Myval2");
		begin = DeRef(GetAttr(q, "_Myfirst"));
		end = DeRef(GetAttr(q, "_Mylast"));
	}
	int sz = SizeOfType(tparam[0]);
	p.data_count = (end.address - begin.address) / sz;
	for(int i = 0; i < count; i++)
		p.data_ptr.Add(begin.address + (i + from) * sz);
}

void Pdb::TraverseTree(bool set, Pdb::Val head, Val node, int64& from, int& count, Pdb::Pretty& p, int depth)
{
	if(depth > 40) // avoid problems if tree is damaged
		return;
	if(depth && node.address == head.address || count <= 0) // we are at the end or have read enough items
		return;
	TraverseTree(set, head, DeRef(GetAttr(node, "_Left")), from, count, p, depth + 1);
	if(node.address != head.address) {
		if(from == 0) {
			Val v = GetAttr(node, "_Myval");
			if(set)
				p.data_ptr.Add(v.address);
			else {
				p.data_ptr.Add(GetAttr(v, "first").address);
				p.data_ptr.Add(GetAttr(v, "second").address);
			}
			count--;
		}
		else
			from--;
	}
	TraverseTree(set, head, DeRef(GetAttr(node, "_Right")), from, count, p, depth + 1);
}

void Pdb::TraverseTreeClang(bool set, int nodet, Val node, int64& from, int& count, Pdb::Pretty& p, int depth)
{
	if(depth > 40 || count <= 0) // avoid problems if tree is damaged
		return;

	Val left = DeRef(GetAttr(node, "__left_"));
	if(left.address)
		TraverseTreeClang(set, nodet, left, from, count, p, depth + 1);

	node.type = nodet;
	Val data = GetAttr(node, "__value_");
	if(from == 0) {
		if(set)
			p.data_ptr.Add(data.address);
		else {
			Val cc = GetAttr(data, "__cc");
			p.data_ptr.Add(GetAttr(cc, "first").address);
			p.data_ptr.Add(GetAttr(cc, "second").address);
		}
		count--;
	}
	else
		from--;

	Val right = DeRef(GetAttr(node, "__right_"));
	if(right.address)
		TraverseTreeClang(set, nodet, right, from, count, p, depth + 1);
}

void Pdb::PrettyStdTree(Pdb::Val val, bool set, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p)
{
	if(HasAttr(val, "__tree_")) {
		String nodet;
		if(set)
			nodet << "std::__1::__tree_node<" << tparam[0] << ",void *>";
		else {
			nodet = "std::__1::__tree_node<std::__1::__value_type<" << tparam[0] << "," << tparam[1];
			if(*nodet.Last() == '>')
				nodet << ' ';
			nodet << ">,void *>";
		}
		Val tree = GetAttr(val, "__tree_");
		Val value = GetAttr(GetAttr(tree, "__pair1_"), "__value_");
		p.data_count =GetInt64Attr(GetAttr(tree, "__pair3_"), "__value_");
		Val node = DeRef(GetAttr(value, "__left_"));
		TraverseTreeClang(set, GetTypeInfo(nodet).type, node, from, count, p, 0);
	}
	else {
		val = GetAttr(GetAttr(GetAttr(val, "_Mypair"), "_Myval2"), "_Myval2");
		p.data_count = GetIntAttr(val, "_Mysize");
		Val head = DeRef(GetAttr(val, "_Myhead")); // points to leftmost element (!)
		Val top = DeRef(GetAttr(head, "_Left"));
		for(int i = 0; i < 40; i++) { // find topmost node, i is depth limit
			Val v = DeRef(GetAttr(top, "_Parent"));
			if(v.address == head.address)
				break;
			top = v;
		}
		TraverseTree(set, head, top, from, count, p, 0);
	}
}

Pdb::Val Pdb::MakeVal(const String& type, adr_t address)
{
	Val item;
	(TypeInfo &)item = GetTypeInfo(type);
	item.address = address;
	return item;
}

bool Pdb::PrettyVal(Pdb::Val val, int64 from, int count, Pretty& p)
{
	ASSERT(count < 100000);
	
	if(val.type < 0)
		return false;

	const Type& t = GetType(val.type);
	
	
	current_modbase = t.modbase; // so that we do not need to pass it as parameter in Pretty routines

	String type = t.name;
	if(type.TrimStart("Upp::WithDeepCopy<")) {
		type.TrimEnd(">");
		type = TrimRight(type);
	}
	int q = type.Find('<');
	Vector<String> type_param; // need to be in 'orignal' form, otherwise GetTypeInfo fails
	if(q >= 0) {
		int e = type.ReverseFind('>');
		if(e >= 0) {
			int lvl = 0;
			int i = q + 1;
			int b = i;
			while(i < type.GetCount()) {
				if(findarg(type[i], ',', '>') >= 0 && lvl == 0) {
					if(i > b)
						type_param.Add(TrimBoth(type.Mid(b, i - b)));
					b = i + 1;
				}
				if(type[i] == '<')
					lvl++;
				else
				if(type[i] == '>') {
					if(lvl == 0)
						break;
					lvl--;
				}
				i++;
			}
			
			type = TrimBoth(type.Mid(0, q));
		}
	}

	if(pretty.GetCount() == 0) {
		pretty.Add("Upp::Date", { 0, THISFN(PrettyDate) });
		pretty.Add("Upp::Time", { 0, THISFN(PrettyTime) });
		pretty.Add("Upp::Color", { 0, THISFN(PrettyColor) });
		pretty.Add("Upp::RGBA", { 0, THISFN(PrettyRGBA) });
		pretty.Add("Upp::Font", { 0, THISFN(PrettyFont) });
		pretty.Add("Upp::ValueArray", { 0, THISFN(PrettyValueArray) });
		pretty.Add("Upp::ValueMap", { 0, THISFN(PrettyValueMap) });
		pretty.Add("Upp::Value", { 0, THISFN(PrettyValue) });
		pretty.Add("Upp::String", { 0, THISFN(PrettyString) });
		pretty.Add("Upp::WString", { 0, THISFN(PrettyWString) });
		pretty.Add("Upp::Vector", { 1, THISFN(PrettyVector) });
		pretty.Add("Upp::BiVector", { 1, THISFN(PrettyBiVector) });
		pretty.Add("Upp::Array", { 1, THISFN(PrettyArray) });
		pretty.Add("Upp::BiArray", { 1, THISFN(PrettyBiArray) });
		pretty.Add("Upp::Index", { 1, THISFN(PrettyIndex) });
		pretty.Add("Upp::VectorMap", { 2, THISFN(PrettyVectorMap) });
		pretty.Add("Upp::ArrayMap", { 2, THISFN(PrettyArrayMap) });

		pretty.Add("std::basic_string", { 1, THISFN(PrettyStdString) });
		pretty.Add("std::vector", { 1, THISFN(PrettyStdVector) });
		pretty.Add("std::set", { 1, [=](Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdTree(val, true, tparam, from, count, p); }});
		pretty.Add("std::multiset", { 1, [=](Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdTree(val, true, tparam, from, count, p); }});
		pretty.Add("std::map", { 2, [=](Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdTree(val, false, tparam, from, count, p); }});
		pretty.Add("std::multimap", { 2, [=](Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdTree(val, false, tparam, from, count, p); }});
	}
	
	type = Filter(type, [](int c) { return c != ' ' ? c : 0; });
	type.Replace("::__1", ""); // CLANG has some weird stuff in names...
	

	int ii = pretty.Find(type);
	if(ii >= 0) {
		const auto& pr = pretty[ii];
		if(type_param.GetCount() >= pr.a) {
			p.kind = CONTAINER;
			p.data_type.Append(type_param, 0, pr.a);
			pr.b(val, type_param, from, count, p);
			return true;
		}
	}
	return false;
}

bool Pdb::VisualisePretty(Visual& result, Pdb::Val val, dword flags)
{
	auto ResultCount = [&](int64 count) {
		if(!(flags & MEMBER)) {
			result.Cat("[", SLtBlue);
			result.Cat(AsString(count), SRed);
			result.Cat("] ", SLtBlue);
		}
	};
	
	Pretty p;
	if(PrettyVal(val, 0, 0, p)) {
		if(p.kind == TEXT) {
			int count = (int)min(p.data_count, (int64)200);
			Pretty p;
			PrettyVal(val, 0, count, p);
			if(p.data_type.GetCount()) {
				String s;
				WString ws;
				int sz = SizeOfType(p.data_type[0]);
				for(adr_t a : p.data_ptr) {
					if(sz == 1)
						s.Cat(PeekByte(a));
					else
						ws.Cat(PeekWord(a));
				}
				ResultCount(p.data_count);
				result.Cat(FormatString(sz == 1 ? s : ws.ToString()), SRed);
				if(p.data_count > p.data_ptr.GetCount())
					result.Cat("..", SGray);
			}
		}
		else
		if(p.kind == SINGLE_VALUE) {
			Pretty p;
			PrettyVal(val, 0, 1, p);
			if(p.text.part.GetCount())
				for(const VisualPart& vp : p.text.part)
					result.Cat(vp.text, vp.ink);
			else
			if(p.data_type.GetCount() && p.data_ptr.GetCount())
				Visualise(result, MakeVal(p.data_type[0], p.data_ptr[0]), flags);
			else
				Visualise(result, val, flags | RAW);
		}
		else { // CONTAINER
			int count = (int)min(p.data_count, (int64)40);
			Pretty p;
			PrettyVal(val, 0, count, p);

			ResultCount(p.data_count);
			if(p.data_type.GetCount()) {
				Buffer<Val> item(p.data_type.GetCount());
				for(int i = 0; i < p.data_type.GetCount(); i++) {
					(TypeInfo &)item[i] = GetTypeInfo(p.data_type[i]);
					item[i].context = val.context;
				}
				int ii = 0;
				int n = p.data_ptr.GetCount() / p.data_type.GetCount();
				for(int i = 0; i < n; i++) {
					if(i)
						result.Cat(", ", SGray);
					for(int j = 0; j < p.data_type.GetCount(); j++) {
						if(j)
							result.Cat(": ", SBlue);
						item[j].address = p.data_ptr[ii++];
						if(item[j].type != UNKNOWN)
							Visualise(result, item[j], flags | MEMBER);
					}
				}
			}
	
			if(p.data_count > count)
				result.Cat("..", SGray);
		}
		return true;
	}
	return false;
}

#endif
