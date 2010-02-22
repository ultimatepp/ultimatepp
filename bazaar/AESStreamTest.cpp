#include <Core/Core.h>
#include <AESStream/AESStream.h>
using namespace Upp;

CONSOLE_APP_MAIN
{
	// Generate cryptographically stable key
	String key(AESRandomString(32));

	// Encryption 
	String sIn,sOut;
	sIn = "qwertyuiop[p\tasdfghjkl;zxcvbnm,./quwiueqiwueoiquweioquweioquweiqwueicuwinuqiweqiwue	pqiueci	eiqniuriryuweyruweyruewrycuwbrurbywuyrwquiercbbcrebrquwey";

	AESEncoderStream aesEncoder(sIn.GetLength(), key); 
	aesEncoder << sIn.Left(10);
	sOut << aesEncoder; //do streamed encoding
	aesEncoder << sIn.Mid(10,10);
	sOut << aesEncoder; //do streamed encoding
	aesEncoder << sIn.Right(sIn.GetLength() - 20); 
	sOut << aesEncoder; //do streamed encoding 
	 
	// Decryption 
	//key.Set(0, 'a'); //uncomment to see what happens with wrong key
	try
	{
		String sDecoded; 
		AESDecoderStream aesDecoder(key);
		
		aesDecoder << sOut.Left(15); //do streamed decoding
		sDecoded << aesDecoder;
		aesDecoder << sOut.Right(sOut.GetLength() - 15); //do streamed decoding
		sDecoded << aesDecoder; 
		
		Cout() << sDecoded;
		if (sDecoded == sIn)
			Cout() << "\n\nOK!"; //check if all converted successfully
		else
			Cout() << "\n\nERROR!";
	}
	catch (const char *xp)
	{
		Cout() << "\n!!Error: " << ToSystemCharset(xp);
	}
	getchar();
}

