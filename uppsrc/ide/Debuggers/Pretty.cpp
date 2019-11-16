#include "Debuggers.h"

#if 0
bool Pdb::PrettyVector(Pdb::Val val, const String& type, const Vector<String>& tparam,
                       Thread& Pretty& p, int from, int count)
{
	if(type == "Upp::Vector" && HasT(0)) {
		p.count = GetInt(GetAttr(val, "items"), ctx);
	
		Val item = DeRef(GetAttr(val, "vector"), ctx);
		int sz = SizeOfType(tparam[0]);
		p.type.Add(tparam[0]);
		
		for(int i = 0; i < count; i++)
			p.address.Add(item.address + (i + from) * count);
		return true;
	}
	
	return false;
}

bool Pdb::PrettyIndex(Pdb::Val val, const String& type, const Vector<String>& tparam,
                      Thread& Pretty& p, int from, int count)
{
}

	if(type == "Upp::Index" && HasT(0)) {
		val = GetAttr(val, "key");
		type = "Upp::Vector";
		return PrettyVector
	}
#endif

bool Pdb::Pretty(Visual& result, Pdb::Val val, int expandptr, int slen)
{
	const Type& t = GetType(val.type);

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
	
	String type = t.name;
	Vector<int> tparam;
	Vector<int> tref;
	int q = type.Find('<');
	if(q >= 0) {
		int e = type.ReverseFind('>');
		if(e >= 0) {
			int lvl = 0;
			int i = q + 1;
			int b = i;
			while(i < type.GetCount()) {
				if(findarg(type[i], ',', '>') >= 0 && lvl == 0) {
					if(i > b) {
						String tp = TrimBoth(type.Mid(b, i - b));
						int ref = 0;
						while(tp.Find("const") >= 0)
							tp.Replace("const", "");
						for(;;) {
							int q = tp.Find('*');
							if(q < 0) break;
							tp.Remove(q, 1);
							ref++;
						}
						while(tp.Find("  ") >= 0)
							tp.Replace("  ", " ");
						tparam.Add(FindType(t.modbase, tp));
						tref.Add(ref);
					}
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
	
	auto HasT = [&] (int i) { return i  < tparam.GetCount() && !IsNull(tparam[i]); };
	
	if(type == "Upp::Index" && HasT(0)) {
		val = GetAttr(val, "key");
		type = "Upp::Vector";
	}
	
	if(type == "Upp::Vector" && HasT(0)) {
		count = GetInt(GetAttr(val, "items"));
		ResultCount(count);

		Val item = DeRef(GetAttr(val, "vector"));
		item.type = tparam[0];
		item.ref = tref[0];
		int sz = SizeOfType(item.type);

		int n = min(count, 40);
		for(int i = 0; i < n; i++) {
			if(i)
				result.Cat(", ", SGray);
			Visualise(result, item, expandptr - 1, slen);
			item.address += sz;
		}

		if(count > n)
			result.Cat("..", SGray);
		
		return true;
	}

	if(type == "Upp::VectorMap" && HasT(0) && HasT(1)) {
		Val key = GetAttr(GetAttr(val, "key"), "key");
		Val value = GetAttr(val, "value");

		count = GetInt(GetAttr(key, "items"));
		ResultCount(count);
		
		Val key_item = DeRef(GetAttr(key, "vector"));
		key_item.type = tparam[0];
		key_item.ref = tref[0];
		int key_sz = SizeOfType(key_item.type);
		
		Val item = DeRef(GetAttr(value, "vector"));
		item.type = tparam[1];
		item.ref = tref[1];
		int sz = SizeOfType(item.type);
		
		int n = min(count, 40);
		for(int i = 0; i < n; i++) {
			if(i)
				result.Cat(", ", SGray);
			Visualise(result, key_item, expandptr - 1, slen);
			result.Cat(": ", SGray);
			key_item.address += key_sz;
			Visualise(result, item, expandptr - 1, slen);
			item.address += sz;
		}

		if(count > n)
			result.Cat("..", SGray);
		
		return true;
	}
	
	if(type == "Upp::Array" && HasT(0)) {
		Val vector = GetAttr(val, "vector");
		count = GetInt(GetAttr(vector, "items"));
		ResultCount(count);

		Val item = DeRef(GetAttr(vector, "vector"));
		item.type = tparam[0];
		item.ref = tref[0] + 1;

		int n = min(count, 40);
		for(int i = 0; i < n; i++) {
			if(i)
				result.Cat(", ", SGray);
			Visualise(result, DeRef(item), expandptr - 1, slen);
			item.address += win64 ? 8 : 4;
		}

		if(count > n)
			result.Cat("..", SGray);
	
		return true;
	}
	
	if(type == "Upp::ArrayMap" && HasT(0) && HasT(1)) {
		Val key = GetAttr(GetAttr(val, "key"), "key");

		count = GetInt(GetAttr(key, "items"));
		ResultCount(count);
		
		Val key_item = DeRef(GetAttr(key, "vector"));
		key_item.type = tparam[0];
		key_item.ref = tref[0];
		int key_sz = SizeOfType(key_item.type);
		
		Val value = GetAttr(val, "value");
		Val vector = GetAttr(value, "vector");
		Val item = DeRef(GetAttr(vector, "vector"));
		item.type = tparam[1];
		item.ref = tref[1] + 1;
		
		int n = min(count, 40);
		for(int i = 0; i < n; i++) {
			if(i)
				result.Cat(", ", SGray);
			Visualise(result, key_item, expandptr - 1, slen);
			result.Cat(": ", SGray);
			key_item.address += key_sz;
			Visualise(result, DeRef(item), expandptr - 1, slen);
			item.address += win64 ? 8 : 4;
		}

		if(count > n)
			result.Cat("..", SGray);
		
		return true;
	}

	return false;
}
