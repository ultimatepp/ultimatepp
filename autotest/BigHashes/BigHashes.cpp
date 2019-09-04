#include <Core/Core.h>

using namespace Upp;

#define ETALONS

const String etalons[] = {
	"886052cf6394b49259fed024cc9c2485",
	"ad46b0777df728bb9c3f9a0d4f00c4c6aa55e69b",
	"c4791ec92d9d35736ac52d86eab42ceb0e7e9c60180e384d7f59ae99542d556e",
	"765e4f447b58a644e1be444f79fce5a8",
	"78fd0af23fe279ea154f71890f12506092ff25af",
	"1865f704adc9257fc98a166240b7f77f534b4b856dee034e7a0d074c2ad35234",
	"84587f60a5365aff009358b6acd816f1",
	"7849c1e148f9c2bf78cb3824354226fae37d9492",
	"fc52faf60e6ba1cc6bf4cb3880fad5889ab3449386174e1bf9ad9678182dc87c",
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
