#include "Entropy.h"

#include <plugin/lz4/lz4.h>

CONSOLE_APP_MAIN
{
	String data = LoadFile("D:/enwik8");
//	String data = LoadFile("D:/20131117_ST_ZZSZ.xml");
//	String data = LoadFile("C:/xxx/Nos.exe");
//	String data = LoadFile("C:/u/aws.data/Viking_Chanarambie_Fenton_With_SurroundingFarms_Backcast_EPEs_14Sep11_v1004_allTI_PCs.blb");
		
	String cdata = Compress(data.Begin(), data.End());
	
//	String ddata = Decompress(cdata, cdata.GetCount());
	
	RDUMP(cdata.GetCount());
	RLOG("Ratio " << cdata.GetCount() * 100.0 / data.GetCount() << "%");
	
	{
		RTIMING("LZ4");
		cdata = LZ4Compress(data);
		RLOG("Ratio " << cdata.GetCount() * 100.0 / data.GetCount() << "%");
	}
}
