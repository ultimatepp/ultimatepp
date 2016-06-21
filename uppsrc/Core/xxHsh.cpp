#include "Core.h"

#include "lib/xxhash.h"

namespace Upp {

xxHashStream::xxHashStream(dword seed)
{
	STATIC_ASSERT(sizeof(context) >= sizeof(XXH32_state_t));
	XXH32_reset((XXH32_state_t *)context, seed);
}

void xxHashStream::Out(const void *data, dword size)
{
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
	h.Put(data, len);
	return h.Finish();
}

int xxHash(const String& s)
{
	return xxHash(~s, s.GetCount());
}

};