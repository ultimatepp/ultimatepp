#include "Core.h"

#include "lib/xxhash.h"

namespace Upp {

xxHashStream::xxHashStream(dword seed)
{
	STATIC_ASSERT(sizeof(context) >= sizeof(XXH32_state_t));
	Reset(seed);
}

void xxHashStream::Reset(dword seed)
{
	XXH32_reset((XXH32_state_t *)context, seed);
}

void xxHashStream::Out(const void *data, dword size)
{
	RTIMING("xxHash");
	XXH32_update((XXH32_state_t *)context, data, size);
}

int xxHashStream::Finish()
{
	Flush();
	return XXH32_digest((XXH32_state_t *)context);
}

int xxHash(const void *data, size_t len)
{
	xxHashStream h;
	h.Put64(data, len);
	return h.Finish();
}

int xxHash(const String& s)
{
	return xxHash(~s, s.GetCount());
}

};