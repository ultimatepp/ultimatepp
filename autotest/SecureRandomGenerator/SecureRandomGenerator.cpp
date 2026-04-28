#include <Core/Core.h>
#include <Core/SSL/SSL.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
    StdLogSetup(LOG_COUT | LOG_FILE);
	
	auto Test = [](const String& name, const Function<void()>& fn) {
		String txt = "---" + name + ": ";
		try {
			fn();
			txt << "PASSED";
		}
		catch(...) {
			txt << "FAILED";
		}
		LOG(txt);
	};
	
	Test("Basic functionality", [] {
		ASSERT(SecureNonce(16).GetSize() == 16);
		ASSERT(SecureNonce(64).GetSize() == 64);
		ASSERT(SecureNonce(12).GetSize() == 12);
		ASSERT(!SecureNonce(16).IsEmpty());
		ASSERT(SecureNonce(1).GetSize() == 12);  // Enforce minimum
		ASSERT(SecureRandom(0).GetSize() == 1); // Enforce minimum
	});
	
	Test("Standard nonce helpers, length check", [] {
		ASSERT(GetAESGCMNonce().GetSize() == 12);
		ASSERT(GetChaChaPoly1305Nonce().GetSize() == 12);
		ASSERT(GetTLSNonce().GetSize() == 12);
		ASSERT(GetAESCCMNonce().GetSize() == 13);
		ASSERT(GetJWTNonce().GetSize() == 16);
		ASSERT(GetOAuthNonce().GetSize() == 16);
		ASSERT(GetOCSPNonce().GetSize() == 20);
		ASSERT(GetECDSANonce().GetSize() == 32);
		ASSERT(GetDTLSCookie().GetSize() == 32);
	});
	
	Test("SecureRandom basic checks", [] {
		auto buf = SecureRandom(32);
		ASSERT(buf.GetSize() == 32);
		ASSERT(!buf.IsEmpty());
		
		// Verify it's not all zeros
		bool has_nonzero = false;
		for(size_t i = 0; i < buf.GetSize(); i++) {
			if(buf[i] != 0) {
				has_nonzero = true;
				break;
			}
		}
		ASSERT(has_nonzero);
	});
	
	Test("Uniqueness (single-threaded)", [] {
		const int NONCE_COUNT = 1000;
		Vector<String> nonces;
		nonces.Reserve(NONCE_COUNT);
		
		for(int i = 0; i < NONCE_COUNT; i++) {
			auto buf = SecureNonce(12);
			nonces.Add(String((const char*)~buf, buf.GetSize()));
		}
		
		Sort(nonces);
		for(int i = 1; i < nonces.GetCount(); i++)
			ASSERT(nonces[i] != nonces[i - 1]);
	});
	
	Test("Uniqueness (multi-threaded)", [] {
		const int THREAD_COUNT = CPU_Cores();
		const int NONCES_PER_THREAD = 100000;
		Vector<String> all_nonces;
		
		CoFor(THREAD_COUNT, [&all_nonces](int n) {
			Vector<String> nonces;
			nonces.Reserve(NONCES_PER_THREAD);
			
			for(int i = 0; i < NONCES_PER_THREAD; i++) {
				auto buf = SecureNonce(12);
				nonces.Add(String((const char*)~buf, buf.GetSize()));
			}
			
			CoWork::FinLock();
			all_nonces.AppendPick(pick(nonces));
		});
		
		ASSERT(all_nonces.GetCount() == THREAD_COUNT * NONCES_PER_THREAD);
		Sort(all_nonces);
		
		for(int i = 1; i < all_nonces.GetCount(); i++)
			ASSERT(all_nonces[i] != all_nonces[i - 1]);
	});
	
	Test("Verify nonce internal structure (12-15 byte nonces)", [] {
		auto nonce1 = SecureNonce(12);
		auto nonce2 = SecureNonce(12);
		
		// First 4 bytes (process ID) should be identical
		ASSERT(memcmp(~nonce1, ~nonce2, 4) == 0);
		
		// Next 8 bytes (counter) should differ
		uint64 counter1 = Peek64(~nonce1 + 4);
		uint64 counter2 = Peek64(~nonce2 + 4);
		ASSERT(counter1 != counter2);
		
		// Expect sequential or very close counters
		// Allow for other threads potentially getting nonces in between
		uint64 diff = (counter2 > counter1) ? (counter2 - counter1) : (counter1 - counter2);
		ASSERT(diff <= 100);
	});
	
	Test("Verify nonce internal structure (16+ byte nonces)", [] {
		auto nonce1 = SecureNonce(16);
		auto nonce2 = SecureNonce(16);
		
		// First 8 bytes (process ID) should be identical
		ASSERT(memcmp(~nonce1, ~nonce2, 8) == 0);
		
		// Next 8 bytes (counter) should differ
		uint64 counter1 = Peek64(~nonce1 + 8);
		uint64 counter2 = Peek64(~nonce2 + 8);
		ASSERT(counter1 != counter2);
		
		uint64 diff = (counter2 > counter1) ? (counter2 - counter1) : (counter1 - counter2);
		ASSERT(diff <= 100);
	});
	
	Test("Verify nonce entropy (using chi-square method)", [] {
		const int NONCE_SIZE = 32;                          // Total nonce size
		const int RANDOM_OFFSET = 16;                       // Skip 8B PID + 8B counter
		const int RANDOM_SIZE = NONCE_SIZE - RANDOM_OFFSET;
		const int SAMPLE_COUNT = 1000;
		const double CHI_SQUARE_THRESHOLD = 350.0;          // 99% confidence
		
		String random_bytes;
		random_bytes.Reserve(SAMPLE_COUNT * RANDOM_SIZE);
		
		// Generate samples
		for(int i = 0; i < SAMPLE_COUNT; ++i) {
			auto nonce = SecureNonce(NONCE_SIZE);
			random_bytes.Cat((const char*)(~nonce + RANDOM_OFFSET), RANDOM_SIZE);
		}
		
		// Frequency analysis
		Vector<int> freq(256, 0);
		const byte* data = (const byte*)(const char*)random_bytes;
		for(int i = 0; i < random_bytes.GetLength(); ++i)
			freq[data[i]]++;
		
		// Chi-square test
		double expected = random_bytes.GetLength() / 256.0;
		double chi2 = 0.0;
		for(int count : freq) {
			double delta = count - expected;
			chi2 += (delta * delta) / expected;
		}
		
		ASSERT(chi2 < CHI_SQUARE_THRESHOLD);
	});
	
	Test("Verify different nonce sizes use correct layouts", [] {
		// 12-byte nonce: [4B PID | 8B counter]
		auto nonce12 = SecureNonce(12);
		ASSERT(nonce12.GetSize() == 12);
		
		// 14-byte nonce: [4B PID | 8B counter | 2B random]
		auto nonce14 = SecureNonce(14);
		ASSERT(nonce14.GetSize() == 14);
		
		// 16-byte nonce: [8B PID | 8B counter]
		auto nonce16 = SecureNonce(16);
		ASSERT(nonce16.GetSize() == 16);
		
		// 32-byte nonce: [8B PID | 8B counter | 16B random]
		auto nonce32 = SecureNonce(32);
		ASSERT(nonce32.GetSize() == 32);
		
		// Verify PID portions match where expected
		// For <16 byte nonces, compare first 4 bytes
		ASSERT(memcmp(~nonce12, ~nonce14, 4) == 0);
		
		// For >=16 byte nonces, compare first 8 bytes
		ASSERT(memcmp(~nonce16, ~nonce32, 8) == 0);
	});
	
	Test("Concurrent nonce generation stress test", [] {
		const int THREAD_COUNT = 16;
		const int NONCES_PER_THREAD = 10000;
		std::atomic<int> total_generated{0};
		
		CoFor(THREAD_COUNT, [&total_generated](int n) {
			for(int i = 0; i < NONCES_PER_THREAD; i++) {
				auto nonce = SecureNonce(16);
				ASSERT(nonce.GetSize() == 16);
				ASSERT(!nonce.IsEmpty());
			}
			total_generated += NONCES_PER_THREAD;
		});
		
		ASSERT(total_generated == THREAD_COUNT * NONCES_PER_THREAD);
	});
	
	Test("Helper functions return correct types", [] {
		// Verify all helpers return SecureBuffer<byte>
		auto gcm = GetAESGCMNonce();
		auto chacha = GetChaChaPoly1305Nonce();
		auto tls = GetTLSNonce();
		auto ccm = GetAESCCMNonce();
		auto jwt = GetJWTNonce();
		auto oauth = GetOAuthNonce();
		auto ocsp = GetOCSPNonce();
		auto ecdsa = GetECDSANonce();
		auto dtls = GetDTLSCookie();
		
		// All should be non-empty
		ASSERT(!gcm.IsEmpty());
		ASSERT(!chacha.IsEmpty());
		ASSERT(!tls.IsEmpty());
		ASSERT(!ccm.IsEmpty());
		ASSERT(!jwt.IsEmpty());
		ASSERT(!oauth.IsEmpty());
		ASSERT(!ocsp.IsEmpty());
		ASSERT(!ecdsa.IsEmpty());
		ASSERT(!dtls.IsEmpty());
	});
	
	LOG("=== All tests completed ===");
}