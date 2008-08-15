#include <Sic/Sic.h>

struct SVal::SValMap {
	Atomic               refcount;
	ArrayMap<SVal, SVal> map;

	void     Retain()                      { AtomicInc(refcount); }
	void     Release()                     { if(AtomicDec(refcount) == 0) delete this; }

	SValMap *Copy();
	SValMap *Clone();

	SValMap()                              { AtomicWrite(refcount, 1); }
};

struct SVal::SLambdaRef : SLambda {
	Atomic   refcount;
	void     Retain()       { AtomicInc(refcount); }
	void     Release()      { if(AtomicDec(refcount) == 0) delete this; }
	SLambdaRef()            { refcount = 1; }
};

SVal::SValMap *SVal::SValMap::Copy()
{
	SValMap *c = new SValMap;
	c->map <<= map;
	return c;
}

SVal::SValMap *SVal::SValMap::Clone()
{
	if(AtomicRead(refcount) == 1)
		return this;
	SValMap *c = Copy();
	Release();
	return c;
}

SVal::SVal()
{
	type = SV_VOID;
}

void SVal::Free()
{
	string.Clear();
	if(type == SV_MAP)
		map->Release();
	if(type == SV_LAMBDA)
		lambda->Release();
}

SVal::~SVal()
{
	Free();
}

void SVal::Assign(const SVal& s)
{
	type = s.type;
	if(type == SV_MAP) {
		map = s.map;
		ipos = s.ipos;
		map->Retain();
	}
	else
	if(type == SV_LAMBDA) {
		lambda = s.lambda;
		lambda->Retain();
	}
	else {
		string = s.string;
		number = s.number;
	}
}

void SVal::operator=(const SVal& s)
{
	Free();
	Assign(s);
}

const ArrayMap<SVal, SVal>& SVal::GetMap() const
{
	ASSERT(IsMap());
	return map->map;
}

void SVal::SetMap()
{
	if(type == SV_MAP)
		map = map->Clone();
	else {
		map = new SValMap();
		ipos = 0;
		type = SV_MAP;
	}
}

SVal::SVal(const SVal& s)
{
	Assign(s);
}

void SVal::operator=(const Nuller&)
{
	Free();
	type = SV_VOID;
}

void SVal::operator=(double n)
{
	Free();
	type = SV_NUMBER;
	number = n;
}

void SVal::operator=(const String& s)
{
	Free();
	type = SV_STRING;
	string = s;
}

SVal::SVal(const String& s)
{
	type = SV_STRING;
	string = s;
}

SVal::SVal(const Nuller& s)
{
	type = SV_VOID;
}

SVal::SVal(double n)
{
	type = SV_NUMBER;
	number = n;
}

SLambda& SVal::CreateLambda()
{
	Free();
	lambda = new SLambdaRef;
	type = SV_LAMBDA;
	return *lambda;
}

const SLambda& SVal::GetLambda() const
{
	ASSERT(IsLambda());
	return *lambda;
}

SVal& SVal::Set(SVal key)
{
	SetMap();
	if(key.IsNumber())
		ipos = key.GetNumber() + 1;
	return map->map.GetAdd(key);
}

SVal& SVal::Add()
{
	SetMap();
	return map->map.GetAdd(ipos++);
}

unsigned SVal::GetHashValue() const
{
	switch(type) {
	case SV_NUMBER:
		return ::GetHashValue(number);
	case SV_STRING:
		return ::GetHashValue(string);
	case SV_MAP:
		unsigned hash = 0;
		for(int i = 0; i < map->map.GetCount(); i++)
			hash ^= ::GetHashValue(map->map.GetKey(i)) ^ ::GetHashValue(map->map[i]);
		return hash;
	}
	return 0;
}

bool SVal::operator==(const SVal& a) const
{
	switch(type) {
	case SV_VOID:
		return a.type == SV_VOID;
	case SV_NUMBER:
		return a.type == SV_NUMBER && number == a.number;
	case SV_STRING:
		return a.type == SV_STRING && string == a.string;
	case SV_MAP:
		if(a.type != SV_MAP) return false;
		if(map->map.GetCount() != a.map->map.GetCount())
			return false;
		for(int i = 0; i < map->map.GetCount(); i++) {
			SVal key = map->map.GetKey(i);
			int q = a.map->map.Find(key);
			if(q < 0)
				return false;
			if(map->map[i] != a.map->map[q])
				return false;
		}
		return true;
	}
	return false;
}

String SVal::Dump() const
{
	String r;
	int i;
	switch(type) {
	case SV_NUMBER:
		return Format("%g", number);
	case SV_STRING:
		return AsCString(string);
	case SV_LAMBDA:
		r = "@(";
		for(i = 0; i < lambda->arg.GetCount(); i++) {
			if(i)
				r << ", ";
			if(lambda->ref[i])
				r << "&";
			r << lambda->arg[i];
		}
		r << ")\n" << lambda->code;
		return r;
	case SV_MAP:
		r << "{ ";
		for(i = 0; i < map->map.GetCount(); i++) {
			if(i)
				r << ", ";
			r << map->map.GetKey(i).Dump() << ":" << map->map[i].Dump();
		}
		r << " }";
		return r;
	}
	return "void";
}

bool IsTrue(const SVal& a)
{
	if(a.IsNumber())
		return a.GetNumber();
	if(a.IsString())
		return a.GetString().GetLength();
	if(a.IsMap())
		return a.GetMap().GetCount();
	return false;
}
