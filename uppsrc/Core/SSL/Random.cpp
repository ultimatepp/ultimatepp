#include "SSL.h"

#define LLOG(x) // DLOG("SecureRandomGenerator: " << x)

namespace Upp {

namespace {

std::atomic<bool>   sForked(false);
std::atomic<uint64> sId(0);
std::atomic<uint64> sCounter(0);
SpinLock            sLock;

constexpr const int NONCE_MIN = 12;
constexpr const int NONCE_STRUCTURED_MIN = 16;

inline void FillRandom(void* ptr, int len)
{
    if(len <= 0)
        return;
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    if(RAND_status() != 1) {
        RAND_poll();
        if(RAND_status() != 1)
            throw Exc("SecureRandom: RNG not seeded");
    }
#endif
    if(RAND_bytes(reinterpret_cast<byte*>(ptr), len) != 1)
        throw Exc("SecureRandom: RAND_bytes failed");
}

void Init()
{
	static_assert(sizeof(uint64) == 8, "Secure random/nonce generator requires 64-bit integers");
	
	SslInitThread();

	ONCELOCK {
		uint32 seed = 0;
		FillRandom(&seed, sizeof(seed));
		sCounter = (uint64) seed;
#ifdef PLATFORM_POSIX
		pthread_atfork(nullptr, nullptr, [] {
			sForked = true;
		#if OPENSSL_VERSION_NUMBER < 0x10100000L
			RAND_cleanup();
		#endif
		});
#endif
	}
}


void HandleFork()
{
#ifdef PLATFORM_POSIX
    if(!sForked.load())
        return;
    // After fork(), child inherits RNG state. We must reseed once to avoid
    // nonce/counter reuse. SpinLock ensures only one thread performs reseed
    // while others wait until state becomes consistent.
    SpinLock::Lock __(sLock);
    if(sForked.load()) {
        uint32 seed = 0;
        FillRandom(&seed, sizeof(seed));
        sCounter = (uint64) seed;
        sId = 0;
        sForked = false;
    }
#endif
}

uint64 GetNonceDomainId()
{
	if(uint64 v = sId.load(); v)
		return v;

	uint64 x = 0;
	FillRandom(&x, sizeof(x));
	if(!x) x = 1;

	uint64 expected = 0;
	if(sId.compare_exchange_strong(expected, x))
		return x;

	return sId.load();
}

uint64 NextCounter()
{
	// simple atomic increment is enough here
	uint64 v = ++sCounter;
	if(v == 0)
		throw Exc("SecureRandom: counter overflow");
	return v;
}

}

SecureBuffer<byte> SecureRandom(int n)
{
	Init();
	HandleFork();
	n = max(1, n);
	SecureBuffer<byte> out(n);
	FillRandom(~out, n);
	return pick(out);
}

SecureBuffer<byte> SecureNonce(int n)
{
	Init();
	HandleFork();
	uint64 did = GetNonceDomainId();
	uint64 cnt = NextCounter();

	n = max(n, NONCE_MIN);
	SecureBuffer<byte> out(n);

	byte *p = ~out;

	// 12-15 byte layout
	// 4 bytes PID | 8 bytes counter | [random tail]
	if(n < NONCE_STRUCTURED_MIN) {
		Poke32(p, (dword) did);
		p += sizeof(dword);
		Poke64(p, (int64) cnt);
		p += sizeof(int64);
		if(int len = n - NONCE_MIN; len > 0)
			FillRandom(p, len);
		return pick(out);
	}

	// 16-byte structured layout
	// 8 bytes PID | 8 bytes counter | [random tail]
	Poke64(p, (int64) did);
	p += sizeof(int64);
	Poke64(p, (int64) cnt);
	p += sizeof(int64);
	if(int len = n - NONCE_STRUCTURED_MIN; len > 0)
		FillRandom(p, len);
	return pick(out);
}


}
