#include <CtrlLib/CtrlLib.h>

#include <Protect/Protect.h>

using namespace Upp;

String GetKey(void)
{
	return ScanHexString("AABBCCDDEEFF00112233445566778899");
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

	OBFUSCATE_END_FUNC;

	return 2 * f + e;
}

GUI_APP_MAIN
{
	bool bad_key = false;
	ON_PROTECT_BAD_KEY(Decrypt)
	{
		bad_key = true;
	}

	if(bad_key && PromptYesNo("Bad key !!&Do you want to continue anyways ?")) {
		Exit(0);
	}
		
	double d = CryptedTest(3, 4);

	d = ObfuscatedTest(3, 4);
	d = ObfuscatedTest(3, 4);
	
	PromptOK("FINISHED OK !!");
}

