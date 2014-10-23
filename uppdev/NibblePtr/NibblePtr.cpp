#include "Entropy.h"

#include <plugin/lz4/lz4.h>

CONSOLE_APP_MAIN
{
	String data = LoadFile("C:/u/upp.src/uppsrc/CtrlLib/ArrayCtrl.cpp");
//	String data = LoadFile("C:/xxx/enwik8.txt");
//	String data = LoadFile("D:/20131117_ST_ZZSZ.xml");
//	String data = LoadFile("C:/u/client/Nos.exe");
//	String data = LoadFile("C:/u/Nos.version/90/Nos.exe");
//	String data = LoadFile("C:/u/aws.data/Viking_Chanarambie_Fenton_With_SurroundingFarms_Backcast_EPEs_14Sep11_v1004_allTI_PCs.blb");


	String cdata = Compress(data.Begin(), data.End());
	
//	String ddata = Decompress(cdata, cdata.GetCount());
	
	RDUMP(cdata.GetCount());
	RLOG("DEE Ratio " << cdata.GetCount() * 100.0 / data.GetCount() << "%");

	
	{
		RLOG("=====================");
		String cdata;
		{
			RTIMESTOP("LZ4");
			cdata = LZ4Compress(data);
		}
		RLOG("LZ4 Ratio " << cdata.GetCount() * 100.0 / data.GetCount() << "%");
		LOG("-- OFFSETS -------");

		int sm = 0;
		for(int i = 0; i < 256; i++) 
			if(matchOffsetStat[i]) {
				LOG(i * 256 << " - " << i * 256 + 255 << " " << matchOffsetStat[i] << "  / " << sm);
				sm += matchOffsetStat[i];
			}
		
		LOG("-- LENGTHS -------");
		sm = 0;
		for(int i = 0; i < 1024; i++)
			if(matchLengthStat[i]) {
				LOG(i << " " << matchLengthStat[i] << "  / " << sm);
				sm += matchLengthStat[i];
			}
		DUMP(matchLengthBig);

		LOG("-- LITERAL LENGTHS -------");
		sm = 0;
		for(int i = 0; i < 1024; i++)
			if(litLengthStat[i]) {
				LOG(i << " " << litLengthStat[i] << "  / " << sm);
				sm += litLengthStat[i];
			}
		DUMP(litLengthStatBig);
		DUMP(literals);
	}
	
	if(1) {
		RLOG("=====================");
		RTIMESTOP("LZV");
		Buffer<byte> out(2 * data.GetLength());
		byte h[16384 * 4];
		int q = LZVCompress((byte *)~data, out, h, data.GetLength(), 2 * data.GetLength());
		RLOG("LZV Ratio " << q * 100.0 / data.GetCount() << "%");
	}
}
