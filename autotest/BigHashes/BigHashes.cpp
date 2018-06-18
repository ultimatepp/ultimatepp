#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	#define GB I64(1024)*1024*1024
	int64 n;
	
	for(int pass = 0; pass < 3; pass++) {
		SeedRandom(0);
		Sha1Stream sha1;
		Md5Stream md5;
		Sha256Stream sha256;
		int64 count = decode(pass, 0, 123456, 1, 3 * GB, 5 * GB);
		DLOG("----- Pass " << pass << ", count: " << count);
		for(n = 0; n < count; n++) {
			int x = (byte)Random();
			sha1.Put(x);
			md5.Put(x);
			sha256.Put(x);
		}
	
		String md5r = md5.FinishString();
		DLOG("md5: " << md5r);
		ASSERT(md5r == decode(pass, 0, "e91a97e4fe738ecea789dad3a7ca34bb",
		                            1, "1beb984259455ff4423e4eadf71c02da",
		                               "2a5d494fcf9d8d82696b5a784dcc8bc8"));
		String sha1r = sha1.FinishString();
		DLOG("sha1: " << sha1r);
		ASSERT(sha1r == decode(pass, 0, "b712bc5326e2237520f41557b83dd52b8c14b840",
		                             1, "9d8efa91dfd213cf9cd9736d52f0d16decd16468",
		                                "b70f7af81f12d3f3c42a23f6f4b5ce2490caeaa6"));
		String sha256r = sha256.FinishString();
		DLOG("sha256: " << sha256r);
		ASSERT(sha256r == decode(pass, 0, "852f79ad4abe264bbb4e330f811a09729f17f3fbbf608cbf0e5a926f4c35b164",
		                               1, "d9f1b7c8ad2db41f9d82afb538be7c3ce5bb6719f05e74de6b35a66f8568c880",
		                                  "a81f96bee9f56bd5ba3f17f47ee343eb0a642a553114eeedd63d4cc771ed8783"));
	}
	
	DLOG("--------------- OK");
}

#if 0

md5
e91a97e4fe738ecea789dad3a7ca34bb  small.bin
1beb984259455ff4423e4eadf71c02da  3gb.bin
2a5d494fcf9d8d82696b5a784dcc8bc8  5gb.bin

sha1
b712bc5326e2237520f41557b83dd52b8c14b840 123456
9d8efa91dfd213cf9cd9736d52f0d16decd16468 3gb
b70f7af81f12d3f3c42a23f6f4b5ce2490caeaa6 5gb

sha256
852f79ad4abe264bbb4e330f811a09729f17f3fbbf608cbf0e5a926f4c35b164  small.bin
d9f1b7c8ad2db41f9d82afb538be7c3ce5bb6719f05e74de6b35a66f8568c880  3gb.bin
a81f96bee9f56bd5ba3f17f47ee343eb0a642a553114eeedd63d4cc771ed8783  5gb.bin

#endif