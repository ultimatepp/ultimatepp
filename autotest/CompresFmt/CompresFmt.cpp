#include <Core/Core.h>
#include <plugin/lzma/lzma.h>
#include <plugin/lzma/lzma.h>
#include <plugin/lz4/lz4.h>
#include <plugin/zstd/zstd.h>
#include <plugin/zstd_legacy/zstd.h>
#include <plugin/z/z.h>
#include <plugin/bz2/bz2.h>

using namespace Upp;

void SaveDataFile(const char *fn, const String& data)
{
	SaveFile(GetDataFile(fn), data);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String data = LoadDataFile("alice30.txt");

	ASSERT(ZstdDecompress(ZstdCompress(data)) == data);


	if(!FileExists(GetDataFile("alice30.gz"))) {
		LOG("Reference files not found. Press a key to create them");
		ReadStdIn();
		
		SaveDataFile("alice30.gz",  GZCompress(data));
		SaveDataFile("alice30.bz2", BZ2Compress(data));
		SaveDataFile("alice30.lz4", LZ4Compress(data));
		SaveDataFile("alice30.lzma", LZMACompress(data));
		SaveDataFile("alice30.zstd", ZstdCompress(data));
		SaveDataFile("alice30_legacy.zstd", Legacy::ZstdCompress(data));
	}
	
	ASSERT(GZDecompress(LoadDataFile("alice30.gz")) == data);
	ASSERT(BZ2Decompress(LoadDataFile("alice30.bz2")) == data);
	ASSERT(LZ4Decompress(LoadDataFile("alice30.lz4")) == data);
	ASSERT(LZMADecompress(LoadDataFile("alice30.lzma")) == data);
	ASSERT(ZstdDecompress(LoadDataFile("alice30.zstd")) == data);
	ASSERT(Legacy::ZstdDecompress(LoadDataFile("alice30_legacy.zstd")) == data);

	ASSERT(GZDecompress(GZCompress(data)) == data);
	ASSERT(BZ2Decompress(BZ2Compress(data)) == data);
	ASSERT(LZ4Decompress(LZ4Compress(data)) == data);
	ASSERT(LZMADecompress(LZMACompress(data)) == data);
	ASSERT(ZstdDecompress(ZstdCompress(data)) == data);
	ASSERT(Legacy::ZstdDecompress(Legacy::ZstdCompress(data)) == data);
	
	LOG("=================== OK");
}
