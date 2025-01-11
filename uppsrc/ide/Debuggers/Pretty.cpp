#include "Debuggers.h"

#ifdef PLATFORM_WIN32

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
		pretty.Add("Upp::ImageBuffer", { 0, THISFN(PrettyImageBuffer) });
		pretty.Add("Upp::Image", { 0, THISFN(PrettyImg) });
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
		pretty.Add("std::list", { 1, THISFN(PrettyStdList) });
		pretty.Add("std::forward_list", { 1, THISFN(PrettyStdForwardList) });
		pretty.Add("std::deque", { 1, THISFN(PrettyStdDeque) });
		pretty.Add("std::set", { 1, [=](Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdTree(val, true, tparam, from, count, p); }});
		pretty.Add("std::multiset", { 1, [=](Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdTree(val, true, tparam, from, count, p); }});
		pretty.Add("std::map", { 2, [=](Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdTree(val, false, tparam, from, count, p); }});
		pretty.Add("std::multimap", { 2, [=](Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdTree(val, false, tparam, from, count, p); }});
		pretty.Add("std::unordered_set", { 1, [=](Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdUnordered(val, true, tparam, from, count, p); }});
		pretty.Add("std::unordered_multiset", { 1, [=](Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdUnordered(val, true, tparam, from, count, p); }});
		pretty.Add("std::unordered_map", { 2, [=](Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdUnordered(val, false, tparam, from, count, p); }});
		pretty.Add("std::unordered_multimap", { 2, [=](Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdUnordered(val, false, tparam, from, count, p); }});
		pretty.Add("std::atomic", { 1, [=](Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p) { PrettyStdAtomic(val, tparam, from, count, p); }});
	}
	
	type = Filter(type, [](int c) { return c != ' ' ? c : 0; });
	type.Replace("::__1", ""); // CLANG has some weird stuff in names...
	if(PrettyScript(type, val, type_param, from, count, p))
		return true;

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
		result.Cat("[", SLtBlue);
		result.Cat(count == INT64_MAX ? ">10000" : AsString(count), SRed);
		result.Cat("] ", SLtBlue);
	};
	
	Pretty p;
	if(PrettyVal(val, 0, 0, p)) {
		for(const VisualPart& vp : p.text.part)
			result.Cat(vp.text, vp.ink);
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
					if(sz == 2)
						ws.Cat(PeekWord(a));
					else
						ws.Cat(PeekDword(a));
				}
				if(!(flags & MEMBER))
					ResultCount(p.data_count);
				result.Cat(FormatString(sz == 1 ? s : ws.ToString()), SRed);
				if(p.data_count > p.data_ptr.GetCount())
					result.Cat("..", SGray);
			}
		}
		else
		if(p.kind == SINGLE_VALUE) {
			if(p.data_type.GetCount() && p.data_ptr.GetCount())
				Visualise(result, MakeVal(p.data_type[0], p.data_ptr[0]), flags);
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
				Color bc = decode(bc_lvl++ & 3, 0, SGray(), 1, SCyan(), 2, SBrown(), SGreen());
				result.Cat("{", bc);
				for(int i = 0; i < n; i++) {
					if(i)
						result.Cat(", ", SGray);
					for(int j = 0; j < p.data_type.GetCount(); j++) {
						if(j)
							result.Cat(": ", SBlue);
						item[j].address = p.data_ptr[p.separated_types ? n * j + i : ii++];
						if(item[j].type != UNKNOWN)
							Visualise(result, item[j], flags | MEMBER);
					}
				}
				result.Cat("}", bc);
				bc_lvl--;
			}
	
			if(p.data_count > count)
				result.Cat("..", SGray);
		}
		return true;
	}
	return false;
}

#endif
