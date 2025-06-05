#include <Core/Core.h>
#include <Core/SSL/SSL.h>

using namespace Upp;

template<typename T>
bool IsZeroed(const T* ptr, size_t count)
{
	auto bytes = reinterpret_cast<const byte*>(ptr);
	for(size_t i = 0; i < count * sizeof(T); ++i) {
		if(bytes[i] != 0) {
			LOG(Format("Memory not zeroed at offset %d: found value 0x%02X", (int) i, (int) bytes[i]));
			return false;
		}
	}
	return true;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT | LOG_FILE);
	
	auto Test = [](const String& name, const Function<void()>& fn) {
		String txt = "---" + name + ": ";
		fn();
		LOG(txt << "PASSED");
	};

	Test("SecureZero: Basic integer array", [&]{
		int buffer[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		SecureZero(buffer);
		ASSERT(IsZeroed(buffer, 10));
	});

	Test("SecureZero: Empty array (edge case)", [&]{
		int buffer[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		SecureZero(buffer, 0);
		ASSERT(buffer[0] == 1 && buffer[9] == 10); // Should not change anything
	});

	Test("SecureZero: NULL pointer (edge case)", [&]{
		// This SHOULD NOT crash
		SecureZero<int>(nullptr, 10);
	});

	Test("SecureZero: Byte-by-byte zeroing", [&]{
		byte buffer[7] = {1, 2, 3, 4, 5, 6, 7}; // Odd size to test boundary cases
		SecureZero(buffer, 7);
		ASSERT(IsZeroed(buffer, 7));
	});

	Test("SecureZero: Complex types", [&]{
		struct TestStruct {
			int a;
			double b;
			char c[10];
		} buffer[5];

		for(int i = 0; i < 5; ++i) {
			buffer[i].a = i;
			buffer[i].b = i * 3.14;
			memset(buffer[i].c, 'A' + i, 10);
		}
		
		SecureZero(buffer, 5);
		ASSERT(IsZeroed(buffer, 5));
	});
	
	Test("SecureZero: Partial array zeroing", [&]{
		int buffer[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		SecureZero(buffer + 3, 4); // Zero elements 3, 4, 5, 6
		ASSERT(buffer[0] == 1);
		ASSERT(buffer[1] == 2);
		ASSERT(buffer[2] == 3);
		ASSERT(buffer[3] == 0);
		ASSERT(buffer[4] == 0);
		ASSERT(buffer[5] == 0);
		ASSERT(buffer[6] == 0);
		ASSERT(buffer[7] == 8);
		ASSERT(buffer[8] == 9);
		ASSERT(buffer[9] == 10);
	});

	Test("SecureZero: Resistance to compiler optimization", [&]{
		const int BUFSIZE = 1024;
		Buffer<byte> buffer(BUFSIZE, 0xFF);
		SecureZero(~buffer, BUFSIZE);
		volatile byte checksum = 0;
		for(int i = 0 ; i < BUFSIZE; i++)
			checksum ^= buffer[i];
		ASSERT(checksum == 0);
	});

	Test("SecureZero: Overlapping memory regions", [&] {
		byte buffer[16];
		memset(buffer, 0xAB, 16);
		SecureZero(buffer + 4, 8); // Zero middle 8 bytes
		for(int i = 0; i < 4; ++i) ASSERT(buffer[i] == 0xAB);
		for(int i = 4; i < 12; ++i) ASSERT(buffer[i] == 0x00);
		for(int i = 12; i < 16; ++i) ASSERT(buffer[i] == 0xAB);
	});
		
	Test("SecureBuffer: Basic functionality", [&]{
		SecureBuffer<int> buffer(10);
		ASSERT(buffer.GetSize() == 10);
		buffer[0] = 42;
		ASSERT(buffer[0] == 42);
		buffer.Clear();
		ASSERT(buffer.GetSize() == 0);
	});

	Test("SecureBuffer: Pick semantics", [&]{
		SecureBuffer<int> buffer1(5);
		buffer1[0] = 123;
		SecureBuffer<int> buffer2 = pick(buffer1);
		ASSERT(buffer2.GetSize() == 5);
		ASSERT(buffer2[0] == 123);
		ASSERT(buffer1.GetSize() == 0);  // NOLINT
	});

	Test("SecureBuffer: Zeroing verification (security critical)", [&]{
		struct TestStruct {
			int a = 0xDEADBEEF;
			char b[8] = "TEST";
		};
		SecureBuffer<TestStruct> buffer(1);
		buffer[0].a = 0xCAFEBABE;
		strcpy(buffer[0].b, "SECRET");
		TestStruct *ptr = ~buffer;
		buffer.Zero();
		ASSERT(IsZeroed(ptr, buffer.GetSize()));
	});

	Test("SecureBuffer: Multiple clear calls", [&]{
		SecureBuffer<int> buffer(4);
		buffer[0] = 123;
		buffer.Clear();
		buffer.Clear(); // Should be safe
		ASSERT(buffer.GetSize() == 0);
	});
	
	Test("SecureBuffer: Edge case", [&]{
        SecureBuffer<char> buffer(0);
        ASSERT(buffer.GetSize() == 0);
        SecureBuffer<double> largebuffer(10000);
        ASSERT(largebuffer.GetSize() == 10000);
	});
}
