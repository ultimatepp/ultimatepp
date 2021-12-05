#ifndef _SvoValue_SvoValue_h_
#define _SvoValue_SvoValue_h_

#include <Painter/Painter.h>

using namespace Upp;

void  DumpNumber(const Value& v);
Value Opt0();
void  Opt();
void CheckString();
void CheckDateTime();
void CheckValueMap();
void OtherChecks();
void CheckRawPickValue();
void CheckRawValue();
void CheckCreateRawValue();

template <class T>
void CheckNumber()
{
	RLOG("========================================================");
	RLOG("CheckNumber " << typeid(T).name());
	Value v = (T)123;
	Value v2 = (T)0;
	Value vn = (T)Null;
	Value dv = 123.0;
	ASSERT(IsNumber(v));
	ASSERT(!IsError(v));
	ASSERT(!IsVoid(v));
	ASSERT(!IsString(v));
	ASSERT(!IsDateTime(v));
	ASSERT(vn.Is<bool>() || GetHashValue(v) == GetHashValue(dv));
	ASSERT(v != v2);
	ASSERT(v != vn);
	ASSERT(IsNull(vn) || vn.Is<bool>());
	DumpNumber(v);
	DumpNumber(vn);
	Value v1 = v;
	RDUMP(v1);
	RDUMP(v2);
	RDUMP(v1 != v2);
	ASSERT(v1 != v2);
	v2 = v1;	
	RDUMP(v1);
	RDUMP(v2);
	RDUMP(v1 == v2);
	ASSERT(v1 == v2);
	Opt();
}

template <class T>
void CheckType(const T& x, bool checkhash = false)
{
	RLOG("========================================================");
	RLOG("CheckType " << typeid(T).name() << " = " << x);
	String fn;
	Value vf;
	Value tt = x;
	bool isvoid = tt.IsVoid();
	if(!isvoid) {
		fn = ConfigFile(AsString(tt.GetType()) + "#"
		                + Filter(AsString(x), [](int c) { return IsAlNum(c) ? c : 0; }));
		DDUMP(fn);
		if(FileExists(fn)) {
			LoadFromFile(vf, fn);
			if(IsValueMap(tt)) {
				RDUMP(ValueMap(vf));
				ASSERT(ValueMap(vf) == ValueMap(x));
			}
			else
			if(tt.Is<Drawing>()) {
				PaintingPainter dw1(100, 100), dw2(100, 100);
				dw1.DrawDrawing(0, 0, tt);
				dw2.DrawDrawing(0, 0, vf);
				Painting a = dw1.GetResult();
				Painting b = dw2.GetResult();
			//	DDUMPHEX(StoreAsString(a));
			//	DDUMPHEX(StoreAsString(b));
				ASSERT(a == b);
			}
			else {
				RDUMP(vf.To<T>());
				ASSERT(vf.To<T>() == x);
			}
		}
	}
	Value v = x;
	RDUMP(v);
	ASSERT((T)v == x);
	ASSERT(isvoid ? v.IsVoid() : v.Is<T>());
	if(!isvoid) {
		StoreToFile(v, fn);
		RDUMP(GetValueTypeNo<T>());
		ASSERT(v.GetType() == GetValueTypeNo<T>());
		if(!tt.Is<ValueArray>() && !tt.Is<ValueMap>())
			ASSERT(v.To<T>() == x);
		ASSERT((T)(v) == x);
		if(!tt.Is<bool>() && !tt.Is<ValueArray>() && !tt.Is<ValueMap>()) {
			Value hh;
			for(int i = 0; i < 2; i++) {
				ASSERT(IsNull(hh.Get<T>()));
				ASSERT(IsNull((T)hh));
				hh = (int)Null;
			}
		}
	}
	if(!vf.IsVoid())
		ASSERT(vf == v);
	String h = StoreAsString(v);
	Value vv;
	LoadFromString(vv, h);
	RDUMP(vv);
	ASSERT(vv == v);
	T xx = vv;
	RDUMP(xx);
	ASSERT(vv == v);

	RDUMP(GetHashValue(v));
	RDUMP(GetHashValue(xx));
	ASSERT(!checkhash || GetHashValue(v) == GetHashValue(xx));
	
	Value vn = (T)Null;
	ASSERT(IsNull(vn) || vn.Is<bool>());

	DLOG("----------");
	RDUMP(v);
	String xml = StoreAsXML(v, "test");
	RDUMP(xml);
	vv = Null;
	LoadFromXML(vv, xml);
	RDUMP(vv);
	ASSERT(vv == v);
	
	String json = StoreAsJson(v);
	RDUMP(json);
	vv = Null;
	LoadFromJson(vv, json);
	RDUMP(vv);
	ASSERT(vv == v);

	if(!tt.Is<bool>()) {
		DLOG("----------");
		v = Null;
	
		RDUMP(v);
		String data = StoreAsString(v);
		vv = x;
		LoadFromString(vv, data);
		RDUMP(vv);
		ASSERT(IsNull(vv));
	
		RDUMP(v);
		xml = StoreAsXML(v, "test");
		RDUMP(xml);
		vv = x;
		LoadFromXML(vv, xml);
		RDUMP(vv);
		ASSERT(IsNull(vv));
		
		json = StoreAsJson(v);
		RDUMP(json);
		vv = x;
		LoadFromJson(vv, json);
		RDUMP(vv);
		ASSERT(IsNull(vv));
	
		DLOG("----------");
		T tx = Null;
		ASSERT(IsNull(tx));
		data = StoreAsString(tx);
		T ty = x;
		LoadFromString(ty, data);
		RDUMP(ty);
		ASSERT(IsNull(ty));
	
		RDUMP(v);
		xml = StoreAsXML(tx, "test");
		RDUMP(xml);
		ty = x;
		LoadFromXML(ty, xml);
		RDUMP(ty);
		ASSERT(IsNull(ty));
		
		json = StoreAsJson(tx);
		RDUMP(json);
		ty = x;
		LoadFromJson(ty, json);
		RDUMP(ty);
		ASSERT(IsNull(vv));

		RDUMP(v);
		tx = x;
		xml = StoreAsXML(tx, "test");
		RDUMP(xml);
		ty = Null;
		LoadFromXML(ty, xml);
		RDUMP(ty);
		ASSERT(StoreAsXML(tx, "X") == StoreAsXML(x, "X"));
		
		json = StoreAsJson(tx);
		RDUMP(json);
		ty = Null;
		LoadFromJson(ty, json);
		RDUMP(ty);
		ASSERT(StoreAsJson(tx) == StoreAsJson(x));
		ASSERT(IsNull(vv));
	}
}

#endif
