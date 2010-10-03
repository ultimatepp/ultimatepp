#include <CtrlLib/CtrlLib.h>

#include <Protect/Protect.h>

using namespace Upp;

String GetKey(void)
{
	// WARNING -- TO PUT A NULL BYTE (0X00) INSIDE KEYSTRING
	// REQUIRES SOME ADDITIONAL WORK !
	String k = "\xAA\xBB\xCC\xDD\xEE\xFF";
	k.Cat('\x00');
	k += "\x11\x22\x33\x44\x55\x66\x77\x88\x99";
	return k;
}

void Decrypt(byte *start, size_t len, byte const *nonce, size_t nonceLen)
{
	PROTECT_DECRYPT ( start, len, GetKey(), nonce, nonceLen );
}

double CryptedTest(double d, double e)
{
	PROTECT_START_FUNC(Decrypt);
	
	double f;
	f = d * e;

	PromptOK("CryptedTest DECRYPTED SUCCESFULLY!!!");
	return 2 * f + e;

	PROTECT_END_FUNC;
}

double ObfuscatedTest(double d, double e)
{

	// WARNING -- DON'T PUT ANY return STATEMENT BETWEEN
	// OBFUSCATE_START and OBFUSCATE_END
	OBFUSCATE_START_FUNC;

	double f;
	f = d * e;

	PromptOK("ObfuscatedTest DEOBFUSCATED SUCCESFULLY!!!");

	OBFUSCATE_END_FUNC;

	return 2 * f + e;
}

GUI_APP_MAIN
{
	ON_PROTECT_BAD_KEY(Decrypt)
	{
		bool res = PromptYesNo("Bad key !!&Do you want to continue anyways ?");
		if(!res)
			exit(0);
	}
		
	double d = CryptedTest(3, 4);
	d = ObfuscatedTest(3, 4);
	d = ObfuscatedTest(3, 4);
	
	PromptOK("FINISHED OK !!");
	
}

