#include <Core/Core.h>

using namespace Upp;

void Put(Zlib& zip, const String& data, int fixed, int rndmask)
{
	int done = 0;
	while(done < data.GetCount()) {
		int n = min(fixed ? fixed : (rand() & rndmask), data.GetCount() - done);
		zip.Put(~data + done, n);
		done += n;
	}
}

void Test(int size, dword cmask, int fixed, int rndmask)
{
	for(int gzip = 0; gzip < 2; gzip++)
		for(int cs = 0; cs < 3; cs++) {
			String data;
			for(int i = 0; i < size; i++) {
				data.Cat(rand());
				if((rand() & cmask) == 0)
					data.Cat("Something to test compression too...");
			}
		
			DLOG("====================");
			DDUMP(fixed);
			DDUMP(gzip);
			DLOG("COMPRESS: " << data.GetCount());
		
			Zlib zip;
			zip.GZip(gzip);
			zip.ChunkSize(cs == 0 ? 128 : cs == 1 ? 2048 : 65536).Crc().Compress();
			DDUMP(zip.out.GetCount());
			Put(zip, data, fixed, rndmask);
			DDUMP(zip.out.GetCount());
			zip.End();
			DDUMP(zip.out.GetCount());
			
			String deflated = zip.out;
			dword crc = zip.GetCrc();
			
			DDUMP(deflated.GetCount());
//			LOGHEXDUMP(~deflated, deflated.GetCount());
			
			String z = gzip ? GZCompress(data) : ZCompress(data);
			
			DDUMP(z.GetCount());
//			LOGHEXDUMP(~z, z.GetCount());

			ASSERT(crc == CRC32(data));
			ASSERT(deflated == z);
		
			DLOG("====================");
			DLOG("DECOMPRESS: " << deflated.GetCount());
			String zz = gzip ? GZDecompress(deflated) : ZDecompress(deflated);
			DDUMP(zz.GetCount());
			zip.Crc().Decompress();
			Put(zip, deflated, fixed, rndmask);
			zip.End();
			DDUMP(zip.out.GetCount());
			ASSERT(zip.out == data);
			ASSERT(zip.GetCrc() == crc);
		}
}

void Test2(int n, int fixed, int rndmask)
{
	Test(n, 0xffffffff, fixed, rndmask);
	Test(n, 255, fixed, rndmask);
	Test(n, 15, fixed, rndmask);
	Test(n, 1, fixed, rndmask);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String gzf = LoadFile(GetHomeDirFile("mbox.gz"));
	Zlib z;
	z.GZip().Decompress();
	z.Put(gzf);
	z.End();
	DDUMP(z.out);
	DDUMP(GZDecompress(gzf));
	
	for(int x = 3; x < 10000000; x += x) {
		Test(x, 0xffffffff, 1, 2047);
		Test(x, 0xffffffff, 3, 2047);
		Test(x, 0xffffffff, 128, 2047);
		Test(x, 0xffffffff, 2048, 2047);
		Test(x, 0xffffffff, 8192, 2047);
		Test(x, 0xffffffff, 100000, 2047);
		Test(x, 0xffffffff, 0, 2047);
	}
	LOG("*************************************");
	LOG("EVERYTHING IS OK");
}
