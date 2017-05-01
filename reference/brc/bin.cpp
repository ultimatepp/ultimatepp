#include <Core/Core.h>
#include "bin.brc"
#include <plugin/bz2/bz2.h>
#include <plugin/lzma/lzma.h>
#include <plugin/lz4/lz4.h>
#include <plugin/zstd/zstd.h>

using namespace Upp;

//**

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	LOG("=== PLAIN");
	LOG(String(brc_plain, brc_plain_length));
	LOG("=== ZIP");
	LOG(ZDecompress(brc_zip, brc_zip_length));
	LOG("=== BZ2");
	LOG(BZ2Decompress(brc_bz2, brc_bz2_length));
	LOG("=== LZ4");
	LOG(LZ4Decompress(brc_lz4, brc_lz4_length));
	LOG("=== LZMA");
	LOG(LZMADecompress(brc_lzma, brc_lzma_length));
	LOG("=== Zstd");
	LOG(ZstdDecompress(brc_zstd, brc_zstd_length));
	
	for(int i = 0; i < bin_array_count; i++) {
		LOG("=== ARRAY " << i);
		LOG(ZDecompress(bin_array[i], bin_array_length[i]));
	}

	for(int i = 0; i < bin_mask_count; i++) {
		LOG("=== MASK " << i);
		LOG(LZ4Decompress(bin_mask[i], bin_mask_length[i]));
	}
}
