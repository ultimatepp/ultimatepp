#include <Core/Core.h>

using namespace Upp;

// #define ETALONS

const String etalons[] = {
	"39bcb6992e461b269b95b3bda303addf",
	"5ed456c5db4efe92e7b23f99d63ae74af321a577",
	"1c238112c74cd6e5afaf8993f49ef016fa00e7b4bc51548ee33affc90741c3cc",
	"c698c87fb53058d493492b61f4c74189",
	"6e7f6dca8def40df0b21f58e11c1a41c3e000285",
	"305b66a59d15b252092fbda9d09711230c429f351897cbd430e7b55a35fd3b97",
	"ec4bcc8776ea04479b786e063a9ace45",
	"13edccc7871c2016fbe8a2a0d808e19a90fbfc63",
	"7f06c62352aebd8125b2a1841e2b9e1ffcbed602f381c3dcb3200200e383d1d5",
};

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	#define GB I64(1024)*1024*1024
	int64 n;
	
	const String *e = etalons;
	for(int pass = 0; pass < 3; pass++) {
		SeedRandom(0);
		Sha1Stream sha1;
		Md5Stream md5;
		Sha256Stream sha256;
		int64 count = decode(pass, 0, 123456, 1, 3 * GB, 5 * GB);
		DLOG("----- Pass " << pass << ", count: " << count);
	#ifdef ETALONS
		String fn = GetHomeDirFile("hash_xxxx__.bin.tmp");
		FileOut file(fn);
	#endif
		for(n = 0; n < count; n++) {
			int x = (byte)Random();
		#ifdef ETALONS
			file.Put(x);
		#else
			sha1.Put(x);
			md5.Put(x);
			sha256.Put(x);
		#endif
		}

	#ifdef ETALONS
		file.Close();
		for(const char *op : { "md5sum", "sha1sum", "sha256sum" }) {
			String s = Sys(String() << op << ' ' << fn);
			s.Trim(s.Find(' '));
			LOG(AsCString(s) << ',');
		}
		DeleteFile(fn);
	#else
		#define TEST(x) { String s = x.FinishString(); String t = *e++; LOG(s << " " << t); ASSERT(s == t); }
		TEST(md5);
		TEST(sha1);
		TEST(sha256);
	#endif
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