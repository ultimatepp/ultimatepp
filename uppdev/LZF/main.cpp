#include "Core/Core.h"
#include <plugin/lz4/lz4.h>
#include <plugin/lzo/lzo.h>

using namespace Upp;

extern String Literals;
extern int LiteralsCount;
extern int Matches;
extern int BigMatches;
extern int BigLiterals;
extern int FarOffsets;
extern int HugeOffsets;
extern int Simple;
extern int Extended;
extern int LargeMatch;
extern int Match32;
extern int Match16;
extern int Match8;
extern int Match4;
extern int LiteralLen[256];
extern int MatchLen[256];

int    LZFCompress(char *out, const char *s, int length);

String LZFCompress(const char *s, int length, String& literals);
String DEECompress(const char *data, const char *end);

CONSOLE_APP_MAIN
{
//	String data = LoadFile("C:/u/aws.data/Viking_Chanarambie_Fenton_With_SurroundingFarms_Backcast_EPEs_14Sep11_v1004_allTI_PCs.blb");
//	String data = LoadFile("C:/u/aws.data/c12_OCOE_env.blb");
//	String data = LoadFile("D:/enwik8");
//	String data = LoadFile("D:/canterbury_last.tar");
//	String data = LoadFile("C:/u/upp.src/uppsrc/CtrlLib/ArrayCtrl.cpp");
//	String data = LoadFile(GetDataFile("LZF.cpp"));
//	String data = LoadFile("C:/u/client/nos74.exe");
	String data = LoadFile("C:/upp/theide.exe");
	
	String l;
	Buffer<char> out(2 * data.GetLength());
	
	{
		RTIMESTOP("LZFCompress");
		int n = LZFCompress(out, ~data, data.GetLength());
		RDUMP(n);
		RDUMP(data.GetLength());
	}
//	RDUMP(cdata.GetCount());

	RDUMP(Match32);
	RDUMP(Match16);
	RDUMP(Match8);
	RDUMP(Match4);
	RDUMP(Matches);
	RDUMP(Extended);
	RDUMP(FarOffsets);
	RDUMP(HugeOffsets);
	RDUMP(LiteralsCount);
	RDUMP(LargeMatch);
	RDUMP(BigMatches);
	RDUMP(BigLiterals);
	RDUMP(Literals.GetCount());
	int n = 0;
	for(int i = 0; i < 256; i++)
		if(LiteralLen[i]) {
			n += LiteralLen[i];
			LOG("LiteralLen" << i << ' ' << LiteralLen[i] << " / " << n);
		}

	RLOG("========");
	n = 0;
	for(int i = 0; i < 256; i++) {
		if(MatchLen[i]) {
			n += MatchLen[i];
			LOG("MatchLen" << i << ' ' << MatchLen[i] << " / " << n);
		}
	}

//	RDUMP(Literals + BigLiterals + LiteralsLen + 2 * Matches + Extended);


//	RDUMP(AsCString(l, 100));
//	RDUMP(l.GetCount());
	int dee = DEECompress(Literals, Literals.End()).GetLength();
	
	SaveFile("d:/literals", Literals);
	
	if(Literals.GetCount() < 10000000)
		RDUMP(ZCompress(Literals).GetCount());

	RDUMP(dee);
	RDUMP(Literals.GetCount() - dee);
	RDUMP(Literals.GetCount());
	RDUMP(data.GetLength());

	RLOG("==========");	
	{
		RTIMESTOP("LZ4");
		RDUMP(LZ4Compress(data).GetLength());
	#ifdef flagLZ4STATS
		RDUMP(lz4stat_LiteralLen);
		RDUMP(lz4stat_Literals);
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

	{
		RTIMESTOP("LZ4 direct");
		Buffer<char> out(2 * data.GetLength());
		RDUMP(LZ4_compress(data, out, data.GetLength()));
	}

}
