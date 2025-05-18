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
		ASSERT(!SecureNonce(16).IsVoid());
		ASSERT(!SecureNonce(64).IsVoid());
		ASSERT(SecureNonce(1).GetLength() == 12);
	});

	Test("Standard nonce helpers, length check", [] {
        ASSERT(GetAESGCMNonce().GetLength() == 12);
        ASSERT(GetChaChaPoly1305Nonce().GetLength() == 12);
        ASSERT(GetTLSNonce().GetLength() == 12);
        ASSERT(GetAESCCMNonce().GetLength() == 13);
        ASSERT(GetJWTNonce().GetLength() == 16);
        ASSERT(GetOAuthNonce().GetLength() == 16);
        ASSERT(GetOCSPNonce().GetLength() == 20);
        ASSERT(GetECDSANonce().GetLength() == 32);
        ASSERT(GetDTLSCookie().GetLength() == 32);
	});
	
	Test("Uniqueness (single-threaded)", [] {
		const int NONCE_COUNT = 1000;
		Vector<String> nonces;
		nonces.Reserve(NONCE_COUNT);
		for(int i = 0; i < NONCE_COUNT; i++)
			nonces.Add() = SecureNonce(12);
		Sort(nonces);
		for(int i = 1; i < nonces.GetCount() - 1; i++)
			ASSERT(nonces[i] != nonces[i - 1]);
	});

	Test("Uniqueness (multi-threaded)", [] {
        const int THREAD_COUNT = CPU_Cores();
        const int NONCES_PER_THREAD = 100000;
        Vector<String> all_nonces;
        CoFor(THREAD_COUNT, [&all_nonces](int n) {
			Vector<String> nonces;
			nonces.Reserve(NONCES_PER_THREAD);
			for(int i = 0; i < NONCES_PER_THREAD; i++)
				nonces.Add() = SecureNonce(12);
			CoWork::FinLock();
			all_nonces.AppendPick(pick(nonces));
        });
        ASSERT(all_nonces.GetCount() == THREAD_COUNT * NONCES_PER_THREAD);
		Sort(all_nonces);
		for(int i = 1; i < all_nonces.GetCount() - 1; i++)
			ASSERT(all_nonces[i] != all_nonces[i - 1]);
	});

	Test("Verify nonce internal structure", [] {
        String nonce1 = SecureNonce(16);
        String nonce2 = SecureNonce(16);

        // First 4 bytes (process ID) should be identical
        ASSERT(memcmp(~nonce1, ~nonce2, 4) == 0);

        // Next 8 bytes (counter) should differ and should be sequential
        uint64 counter1 = Peek64le(~nonce1 + 4);
        uint64 counter2 = Peek64le(~nonce2 + 4);
        ASSERT(counter1 != counter2);

        // We expect sequential or very close counters in most cases
        // But allow for other threads potentially getting nonces in between
        uint64 diff = (counter2 > counter1) ? (counter2 - counter1) : (counter1 - counter2);
        ASSERT(diff <= 100); // Allow for some concurrent access in tests
	});

	Test("Verify nonce entropy (using chi-square method)", [] {
	    const int NONCE_SIZE = 32;                          // Total nonce size
	    const int RANDOM_OFFSET = 12;                       // Skip PUID + counter
	    const int RANDOM_SIZE = NONCE_SIZE - RANDOM_OFFSET;
	    const int SAMPLE_COUNT = 1000;
	    const double CHI_SQUARE_THRESHOLD = 350.0;          // 99% confidence
    
	    String random_bytes;
	    random_bytes.Reserve(SAMPLE_COUNT * RANDOM_SIZE);
	
	    // Generate samples (length guaranteed by SecureNonce)
	    for(int i = 0; i < SAMPLE_COUNT; ++i) {
	        String nonce = SecureNonce(NONCE_SIZE);
	        random_bytes.Cat(nonce.Mid(RANDOM_OFFSET, RANDOM_SIZE));
	    }
	
	    // Frequency analysis
	    Vector<int> freq(256, 0);
	    const byte* data = random_bytes;
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
}
