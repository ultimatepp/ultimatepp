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
	DDUMP(done);
}

void Test(int size, dword cmask, int fixed, int rndmask)
{
	for(int crc = 0; crc < 2; crc++)
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
				DDUMP(crc);
				DLOG("COMPRESS: " << data.GetCount());
			
				Zlib zip;
				zip.GZip(gzip);
				zip.ChunkSize(cs == 0 ? 128 : cs == 1 ? 2048 : 65536).CRC(crc).Compress();
				Put(zip, data, fixed, rndmask);
				zip.End();
				String deflated = zip;
				DDUMP(deflated.GetCount());
				
				dword crc = zip.GetCRC();
				
	//			LOGHEXDUMP(~deflated, deflated.GetCount());
				
				String z = gzip ? GZCompress(data) : ZCompress(data);
				
				DDUMP(z.GetCount());
	//			LOGHEXDUMP(~z, z.GetCount());
	
				if(crc)
					ASSERT(crc == CRC32(data));
				ASSERT(deflated == z);
			
				DLOG("====================");
				DLOG("DECOMPRESS: " << deflated.GetCount());
				String zz = gzip ? GZDecompress(deflated) : ZDecompress(deflated);
				DDUMP(zz.GetCount());
				zip.CRC(crc).Decompress();
				Put(zip, deflated, fixed, rndmask);
				zip.End();
				String inflated = zip;
				ASSERT(!zip.IsError());
				DDUMP(inflated.GetCount());
				ASSERT(inflated == data);
				if(crc)
					ASSERT(zip.GetCRC() == crc);
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
	TimeStop tm;
	String gzf = LoadFile(GetHomeDirFile("mbox.gz"));
	Zlib z;
	z.GZip().Decompress();
	z.Put(gzf);
	z.End();
	DDUMP(~z);
	DDUMP(z.GetGZipComment());
	DDUMP(z.GetGZipName());
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
	LOG("===================== OK " << tm);
}
