#include <Core/Core.h>
#include "Brc.brc"
#include <plugin/bz2/bz2.h>
#include <plugin/lz4/lz4.h>
#include <plugin/lzma/lzma.h>
#include <plugin/zstd/zstd.h>

using namespace Upp;

//**

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	String orig = LoadDataFile("Brc.cpp");
	ASSERT(String(brc_plain, brc_plain_length) == orig);
	ASSERT(ZDecompress(brc_zip, brc_zip_length) == orig);
	ASSERT(BZ2Decompress(brc_bz2, brc_bz2_length) == orig);
	ASSERT(LZ4Decompress(brc_lz4, brc_lz4_length) == orig);
	ASSERT(LZMADecompress(brc_lzma, brc_lzma_length) == orig);
	ASSERT(ZstdDecompress(brc_zstd, brc_zstd_length) == orig);
	
	LOG("=============== OK");
}
