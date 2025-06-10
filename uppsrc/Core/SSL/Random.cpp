#include "SSL.h"

// High-assurance random/nonce generator:
// 1) Unique nonces (min 12 bytes) compliant with NIST SP 800-38D
// 2) Collision resistance at 2^64 (128-bit entropy) when nonce size >= 16 bytes

#define LLOG(x) // DLOG("SecureRandomGenerator: " << x)

namespace Upp {


namespace {

std::atomic<bool>   sForked(false);
std::atomic<dword>  sPid(0);
std::atomic<uint64> sCounter(0);

constexpr const int NONCE_MIN = 12;

template<typename T>
bool SSLRandom(T* buffer, int n)
{
	// Check RNG state
	if(RAND_status() != 1) {
		RAND_poll(); // Emergency reseed
		if(RAND_status() != 1)
			return false;
	}
	return RAND_bytes(reinterpret_cast<byte*>(buffer), max(1, n)) == 1;
}

dword GetProcessUniqueId()
{
	// Setup process unique id if not set
	if(sPid.load(std::memory_order_acquire) == 0) {
		dword uid = 0;
#ifdef PLATFORM_WIN32
		DWORD pid = GetCurrentProcessId();
		if(FILETIME ctime, nop1, nop2, nop3; GetProcessTimes(
							GetCurrentProcess(),
							&ctime,
							&nop1,
							&nop2,
							&nop3)) {
			uid = pid ^ ctime.dwLowDateTime;
		}
		else {
			uid = pid;
		}
		uid ^= GetTickCount();
#else
		uid = (dword) getpid() ^ GetTickCount();
#endif
		byte q[4];
		if(!SSLRandom(&q, 4))
			throw Exc("Failed to generate random number");
		// We support only LE machines.
		uid ^= Peek32le(&q);
		if(uid == 0)
			uid = 1;
		dword expected = 0;
		if(sPid.compare_exchange_strong(
			expected,
			uid,
			std::memory_order_release,
			std::memory_order_relaxed))
			return uid; // Minor optimization
	}
	return sPid.load(std::memory_order_acquire);
}

void SRandomSeed()
{
	byte q[8];
	if(!SSLRandom(&q, 8))
		throw Exc("Failed to generate random number");
	sCounter.store(Peek64le(&q), std::memory_order_relaxed);
}

void SRandomInit()
{
    static_assert(sizeof(uint64) == 8,
                "Upp::SecureRandomGenerator: Expected 64-bit uint64");

	SslInitThread();

	ONCELOCK {
		SRandomSeed();
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

uint64 SRandomNext()
{
	for(;;) {
		uint64 cnt = sCounter.load(std::memory_order_acquire);
		if(cnt == UINT64_MAX) // Never allow wraparound.
			throw Exc("Counter overflow, nonce space exhausted");
		// Let's occasionally add a small random jump (a well known technique)
		if((cnt & 0xFFF) == 0) {
			byte jump[8];
			if(SSLRandom(&jump, 8)) {
				uint64 newcnt = cnt + (Peek64le(&jump) & 0xFFFF);
				if(sCounter.compare_exchange_strong(cnt, newcnt,
													std::memory_order_acq_rel,
													std::memory_order_acquire))
					return newcnt;
				continue; // race, retry
			}
		}
		uint64 next = cnt + 1;
		if(sCounter.compare_exchange_weak(cnt, next,
										std::memory_order_acq_rel,
										std::memory_order_acquire))
			return next;
		// Else: CAS failed, retry with updated cnt
	}
}

}

String SecureRandom(int n)
{
	try {
		SRandomInit();
		Buffer<byte> q(n);
		if(!SSLRandom(~q, n))
			throw Exc("Failed to generate random number");
		return String(q, n);
	}
	catch(const Exc& e) {
		LLOG(e);
	}
	catch(...) {
		LLOG("Unknown exception");
	}
	return String::GetVoid();
}

String SecureNonce(int n)
{
	try {
		SRandomInit();
#ifdef PLATFORM_POSIX
		if(sForked.load(std::memory_order_acquire) && Thread::IsMain()) {
			SRandomSeed();
			sForked.store(false, std::memory_order_release);
		}
#endif
		n = max(NONCE_MIN, n);
		String nonce;
		nonce.Reserve(n);
		RawCat(nonce, GetProcessUniqueId());                     // 4 bytes: process unique id
		RawCat(nonce, SRandomNext());                            // 8 bytes: incremented counter
		// Remaining bytes filled with OpenSSL randomness
		if(n > NONCE_MIN) {
			int rlen = n - NONCE_MIN;
			Buffer<byte> q(rlen);
			if(!SSLRandom(~q, rlen))
				throw Exc("Failed to generate random number");
			nonce.Cat(q, rlen);
		}
		return nonce;
	}
	catch(const Exc& e) {
		LLOG(e);
	}
	catch(...) {
		LLOG("Unknown exception");
	}
	return String::GetVoid();
}

}
