#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	const int64 sz = (int64)5 * 1000 * 1000 * 1000;
	String path = GetHomeDirFile("test.bin");
	{
		Buffer<byte> big(sz);
		LOG("Allocated");
		for(int64 i = 0; i < sz; i++) {
			big[i] = i % 123;
		}
		FileOut out(path);
		out.Put64(big, sz);
		LOG("Saved");
	}
	ASSERT(GetFileLength(path) == sz);
	{
		FileIn in(path);
		for(int64 i = 0; i < sz; i++)
		    ASSERT(in.Get() == i % 123);
		LOG("Loaded by bytes");
	}
	{
		Buffer<byte> big(sz);
		FileIn in(path);
		ASSERT(in.GetAll64(big, sz));
		LOG("Loaded by GetAll64");
		for(int64 i = 0; i < sz; i++)
		    ASSERT(big[i] == i % 123);
	}
	DeleteFile(path);
	LOG("Test passed OK");
}
