#include "Core.h"

#ifdef PLATFORM_WIN32

#define Ptr Ptr_
#include <objbase.h>
#undef Ptr

#pragma comment(lib, "ole32.lib")

#endif

NAMESPACE_UPP

//#BLITZ_APPROVE

void Uuid::Serialize(Stream& s) {
	int version = 0;
	s / version % a % b %c % d;
}

#ifdef PLATFORM_WIN32

Uuid Uuid::Create() {
	Uuid uuid;
	CoCreateGuid((GUID *)&uuid);
	return uuid;
}
#endif

#if defined(PLATFORM_POSIX)

static StaticCriticalSection sUuidLock;

Uuid Uuid::Create() {
	CriticalSection::Lock _(sUuidLock);
	static int fd;
	ONCELOCK {
		fd = open("/dev/urandom", O_RDONLY);
		ASSERT(fd > 0);
	}
	static byte uuid[sizeof(Uuid)];
	read(fd, uuid, sizeof(Uuid));
	return *(Uuid *)uuid;
}

#endif

String Format(const Uuid& id) {
	return Sprintf("%08X%08X%08X%08X", id.a, id.b, id.c, id.d);
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
