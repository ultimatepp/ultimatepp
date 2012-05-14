#include "Core.h"

NAMESPACE_UPP

//#BLITZ_APPROVE

INITBLOCK {
	Value::Register<Uuid>("Uuid");
}

void Uuid::Serialize(Stream& s) {
	int version = 0;
	s / version % a % b %c % d;
}

void Uuid::Jsonize(JsonIO& jio)
{
	String h;
	if(jio.IsStoring()) {
		h = Format(*this);
		jio.Set(h);
	}
	else
		*this = ScanUuid((String)jio.Get());
}

Uuid Uuid::Create() {
	Uuid ud;
	ud.a = Random();
	ud.b = Random();
	ud.c = Random();
	ud.d = Random();
	return ud;
}

String Format(const Uuid& id) {
	return Sprintf("%08X%08X%08X%08X", id.a, id.b, id.c, id.d);
}

String FormatWithDashes(const Uuid& id) {
	return Sprintf("%08X-%04X-%04X-%04X-%04X%08X", id.a, (id.b & 0xFFFF0000) >> 16, id.b & 0x0000FFFF,
		(id.c & 0xFFFF0000) >> 16, id.c & 0x0000FFFF, id.d);
}


dword scanX(const char *s)
{
    dword r = 0;
    for(int i = 0; i < 8; i++) {
        r = (r << 4) | (*s >= '0' && *s <= '9' ?      *s - '0' :
                        *s >= 'A' && *s <= 'F' ? 10 + *s - 'A' :
                        *s >= 'a' && *s <= 'f' ? 10 + *s - 'a' : 0);
        s++;
    }
    return r;
}

Uuid ScanUuid(const char *s)
{
	Uuid id;
	String xu;
	while(*s) {
		if(IsXDigit(*s))
			xu.Cat(*s);
		s++;
	}
	if(xu.GetCount() < 32)
		return Null;
	id.a = scanX(~xu);
	id.b = scanX(~xu + 8);
	id.c = scanX(~xu + 16);
	id.d = scanX(~xu + 24);
	return id;
}

void Uuid::Xmlize(XmlIO& xio)
{
	String h;
	if(xio.IsStoring())
		h = Format(*this);
	xio.Attr("value", h);
	if(xio.IsLoading())
		*this = ScanUuid(h);
}

String Uuid::ToString() const
{
	return Format(*this);
}

String Uuid::ToStringWithDashes() const
{
	return FormatWithDashes(*this);
}

String Dump(const Uuid& id) {
	return "UUID: " + Format(id);
}

struct UuidValueGenClass : ValueGen
{
	virtual Value Get() {
		return Format(Uuid::Create());
	}
};

ValueGen& UuidValueGen()
{
	return Single<UuidValueGenClass>();
}

END_UPP_NAMESPACE
