#include "Core/Core.h"
#include <plugin/lz4/lz4.h>
#include <plugin/lzo/lzo.h>

using namespace Upp;

extern int Literals;
extern int LiteralsLen;
extern int Matches;
extern int BigMatches;
extern int BigLiterals;
extern int FarOffsets;

int    LZFCompress(char *out, const char *s, int length);

String LZFCompress(const char *s, int length, String& literals);
String DEECompress(const char *data, const char *end);

CONSOLE_APP_MAIN
{
//	String data = LoadFile("C:/u/aws.data/Viking_Chanarambie_Fenton_With_SurroundingFarms_Backcast_EPEs_14Sep11_v1004_allTI_PCs.blb");
//	String data = LoadFile("C:/u/aws.data/Viking_Chanarambie_Fenton_With_SurroundingFarms_Backcast_EPEs_14Sep11_v1004_allTI_PCs.blb");
	String data = LoadFile("D:/enwik8");
//	String data = LoadFile("C:/u/upp.src/uppsrc/CtrlLib/ArrayCtrl.cpp");
//	String data = LoadFile(GetDataFile("LZF.cpp"));
	String l;
	Buffer<char> out(11 * data.GetLength() / 10);
	RDUMP(LZFCompress(out, ~data, data.GetLength()));
//	RDUMP(cdata.GetCount());

	RDUMP(Literals);
	RDUMP(LiteralsLen);
	RDUMP(Matches);
	RDUMP(BigMatches);
	RDUMP(BigLiterals);
	RDUMP(FarOffsets);
	
	RDUMP(/*Literals + */BigLiterals + LiteralsLen + 2 * Matches + BigMatches + FarOffsets);


//	RDUMP(AsCString(l, 100));
//	RDUMP(l.GetCount());
//	RDUMP(DEECompress(l, l.End()).GetLength());
	RDUMP(data.GetLength());

	RLOG("==========");	
	{
		RTIMESTOP("LZ4");
		RDUMP(LZ4Compress(data).GetLength());
	#ifdef flagLZ4STATS
		RDUMP(lz4stat_LiteralLen);
		RDUMP(lz4stat_Matches);
		RDUMP(lz4stat_BigMatch);
		RDUMP(lz4stat_BigLiteral);
		RDUMP(lz4stat_LiteralLen + 3 * lz4stat_Matches + lz4stat_BigMatch + lz4stat_BigLiteral);
	#endif
	}
	{
		RTIMESTOP("LZO");
		RDUMP(LZOCompress(data).GetLength());
	}
	if(data.GetLength() < 10000000)
		RDUMP(ZCompress(data).GetCount());
}
