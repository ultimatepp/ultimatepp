#include "Debuggers.h"

#ifdef PLATFORM_WIN32

// TODO: from->int64

void Pdb::PrettyString(Pdb::Val val, const Vector<String>& tparam, int from, int count, Pdb::Pretty& p)
{
	bool small = IntAt(val, "chr", 14) == 0;
	p.data_count = small ? IntAt(val, "chr", 15) : IntAt(val, "w", 2);
	adr_t a = (small ? GetAttr(val, "chr") : DeRef(GetAttr(val, "ptr"))).address;
	p.data_type << "char";
	for(int i = 0; i < count; i++)
		p.data_ptr.Add(a + from + i);
	p.kind = TEXT;
}

void Pdb::PrettyWString(Pdb::Val val, const Vector<String>& tparam, int from, int count, Pdb::Pretty& p)
{
	p.data_count = GetInt(GetAttr(val, "length"));
	adr_t a = DeRef(GetAttr(val, "ptr")).address;
	p.data_type << "short int";
	for(int i = 0; i < count; i++)
		p.data_ptr.Add(a + from + 2 * i);
	p.kind = TEXT;
}

void Pdb::PrettyVector(Pdb::Val val, const Vector<String>& tparam, int from, int count, Pdb::Pretty& p)
{
	p.data_count = GetInt(GetAttr(val, "items"));
	Val item = DeRef(GetAttr(val, "vector"));
	int sz = SizeOfType(tparam[0]);
	for(int i = 0; i < count; i++)
		p.data_ptr.Add(item.address + (i + from) * sz);
}

void Pdb::PrettyIndex(Pdb::Val val, const Vector<String>& tparam, int from, int count, Pdb::Pretty& p)
{
	return PrettyVector(GetAttr(val, "key"), tparam, from, count, p);
}

void Pdb::PrettyArray(Pdb::Val val, const Vector<String>& tparam, int from, int count, Pdb::Pretty& p)
{
	PrettyVector(GetAttr(val, "vector"), { "int *" }, from, count, p);
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

void Pdb::PrettyVectorMap(Pdb::Val val, const Vector<String>& tparam, int from, int count, Pdb::Pretty& p)
{
	Pretty key, value;
	PrettyIndex(GetAttr(val, "key"), { tparam[0] }, from, count, key);
	PrettyVector(GetAttr(val, "value"), { tparam[1] }, from, count, value);
	PrettyMap(p, key, value);
}

void Pdb::PrettyArrayMap(Pdb::Val val, const Vector<String>& tparam, int from, int count, Pdb::Pretty& p)
{
	Pretty key, value;
	PrettyIndex(GetAttr(val, "key"), { tparam[0] }, from, count, key);
	PrettyArray(GetAttr(val, "value"), { tparam[1] }, from, count, value);
	PrettyMap(p, key, value);
}

void Pdb::PrettyValue(Pdb::Val val, const Vector<String>& tparam, int from, int count, Pdb::Pretty& p)
{
	p.kind = SINGLE_VALUE;
	val = GetAttr(val, "data");
	adr_t a = val.address;
	p.data_ptr << a;
	if(PeekWord(a + 12) && !PeekWord(a + 14)) { // this is not String
		int st = PeekByte(a + 13);
	
		String t = decode(st, 1, "int", 2, "double", 4, "Upp::Date", 5, "Upp::Time",
		                      10, "int64", 11, "bool", "");
		if(t.GetCount())
			p.data_type << t;
	}
	else
		p.data_type << "Upp::String";
}

bool Pdb::PrettyData(Visual& result, Pdb::Val val, int expandptr, int slen)
{
	const Type& t = GetType(val.type);
	
	current_modbase = t.modbase; // so that we do not need to pass it as parameter in Pretty routines

	int count = 0;
	
	auto ResultCount = [&](int count) {
		result.Cat("[", SLtBlue);
		result.Cat(AsString(count), SRed);
		result.Cat("] ", SLtBlue);
	};
	
	auto ResultText = [&](const String& x)
	{
		result.Cat(AsCString(x), SRed);
		if(count > 200)
			result.Cat("..", SGray);
	};

/*
	if(t.name == "Upp::String") {
		bool small = IntAt(val, "chr", 14) == 0;
		count = small ? IntAt(val, "chr", 15) : IntAt(val, "w", 2);
		ResultCount(count);
		ResultText(ReadString((small ? GetAttr(val, "chr") : DeRef(GetAttr(val, "ptr"))).address,
		                      min(count, 200), true));
		return true;
	}

	if(t.name == "Upp::WString") {
		count = GetInt(GetAttr(val, "length"));
		ResultCount(count);
		ResultText(ReadWString(DeRef(GetAttr(val, "ptr")).address, min(count, 200), true).ToString());
		return true;
	}
*/
	
	String type = t.name;
	Vector<String> type_param;
	int q = type.Find('<');
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

	ONCELOCK {
		pretty.Add("Upp::Value", { 0, THISFN(PrettyValue) });
		pretty.Add("Upp::String", { 0, THISFN(PrettyString) });
		pretty.Add("Upp::WString", { 0, THISFN(PrettyWString) });
		pretty.Add("Upp::Vector", { 1, THISFN(PrettyVector) });
		pretty.Add("Upp::Array", { 1, THISFN(PrettyArray) });
		pretty.Add("Upp::Index", { 1, THISFN(PrettyIndex) });
		pretty.Add("Upp::VectorMap", { 2, THISFN(PrettyVectorMap) });
		pretty.Add("Upp::ArrayMap", { 2, THISFN(PrettyArrayMap) });
	}
DDUMP(type);
	int ii = pretty.Find(type);
	while(ii >= 0) {
		auto pr = pretty[ii];
		if(type_param.GetCount() >= pr.a) {
			Pretty p;
			p.kind = CONTAINER;
			p.data_type.Append(type_param, 0, pr.a);
			pr.b(val, type_param, 0, 0, p);
			
			if(p.kind == TEXT) {
				int count = (int)min(p.data_count, (int64)200);
				pr.b(val, type_param, 0, count, p);
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
					ResultText(sz == 1 ? s : ws.ToString());
				}
			}
			else
			if(p.kind == SINGLE_VALUE) {
				pr.b(val, type_param, 0, 1, p);
				if(p.data_type.GetCount() && p.data_ptr.GetCount()) {
					Val item;
					(TypeInfo &)item = GetTypeInfo(p.data_type[0]);
					item.address = p.data_ptr[0];
					Visualise(result, item, expandptr - 1, slen);
				}
				else
					Visualise(result, val, expandptr - 1, slen, false);
			}
			else { // CONTAINER
				int count = (int)min(p.data_count, (int64)40);
				p.data_ptr.Clear();
				pr.b(val, type_param, 0, count, p);
	
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
								result.Cat(", ", SGray);
							item[j].address = p.data_ptr[ii++];
							Visualise(result, item[j], expandptr - 1, slen);
						}
					}
				}
		
				if(p.data_count > count)
					result.Cat("..", SGray);
			}
			return true;
		}
		ii = pretty.FindNext(ii);
	}
	return false;
}

#endif
