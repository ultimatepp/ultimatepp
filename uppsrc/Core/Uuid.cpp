#include "Core.h"

namespace Upp {

//#BLITZ_APPROVE

INITBLOCK {
	Value::Register<Uuid>("Uuid");
}

void Uuid::Serialize(Stream& s) {
	int version = 0;
	s / version % v[0] % v[1];
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

void Uuid::New()
{
	do
		Random64(v, 2);
	while(IsNullInstance());
}

String Format(const Uuid& id) {
	return Sprintf("%08X%08X%08X%08X", LODWORD(id.v[0]), HIDWORD(id.v[0]), LODWORD(id.v[1]), HIDWORD(id.v[1]));
}

String FormatWithDashes(const Uuid& id) {
	return Sprintf("%08X-%04X-%04X-%04X-%04X%08X", LODWORD(id.v[0]),
	               HIWORD(HIDWORD(id.v[0])), LOWORD(HIDWORD(id.v[0])),
	               HIWORD(LODWORD(id.v[1])), LOWORD(LODWORD(id.v[1])),
	               HIDWORD(id.v[1]));
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
	id.v[0] = MAKEQWORD(scanX(~xu), scanX(~xu + 8));
	id.v[1] = MAKEQWORD(scanX(~xu + 16), scanX(~xu + 24));
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

}
