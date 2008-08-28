#include "Core.h"

NAMESPACE_UPP

//#BLITZ_APPROVE

void Uuid::Serialize(Stream& s) {
	int version = 0;
	s / version % a % b %c % d;
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

String Uuid::ToString() const
{
	return Format(*this);
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
