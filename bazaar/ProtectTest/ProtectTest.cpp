#include <Core/Core.h>
#include <Cypher/Snow2.h>

#include <Protect/Protect.h>

using namespace Upp;

One<Cypher> GetCypher(uint8_t const *nonce, int nonceSize)
{
	String key = ScanHexString("AABBCCDDEEFF00112233445566778899");
	One<Cypher> res = new Snow2((uint8_t const *)~key, key.GetCount(), nonce, nonceSize);
	return res;
}


void encrypted(int x)
{
	PROTECT_START_FUNC(GetCypher);

	Cerr() << "2 * X = " << 2 * x << "\n";

	PROTECT_END_FUNC;
}

/*
int squared(int x)
{
	PROTECT_START_FUNC(GetCypher);
	return x*x;
	PROTECT_END_FUNC;
}
*/

int squared(int x)
{
	volatile static bool __decrypted = false;
	PROTECT_DECRYPT_CODE(__decrypted, GetCypher);
	PROTECT_DUMMY1();
	PROTECT_DUMMY1();
	PROTECT_DUMMY1();
	PROTECT_DUMMY1();
	PROTECT_DUMMY2();
	PROTECT_DUMMY2();
	PROTECT_DUMMY1();
	if(__PROTECT_DUMMY_BOOL)
		goto __PROTECT_LABEL_1;

	return x*x;

	__PROTECT_LABEL_1:
	PROTECT_DUMMY1();
	PROTECT_DUMMY1();
	PROTECT_DUMMY1();
	PROTECT_DUMMY1();
	PROTECT_DUMMY2();
	PROTECT_DUMMY1();
	PROTECT_DUMMY1();
}


void encrypted2(int x)
{
	PROTECT_START_FUNC(GetCypher);
	Cerr() << "X * X = " << squared(x) << "\n";
	PROTECT_END_FUNC;
}

void obfuscated(String const &s)
{
	OBFUSCATE_START_FUNC;

	Cerr() << "S is : " << s << "\n";

	OBFUSCATE_END_FUNC;
}

CONSOLE_APP_MAIN
{
	// check if protection key is ok; if not abort
	ON_PROTECT_BAD_KEY(GetCypher) {
		Cerr() << "Bad key, aborting\n";
		Exit(1);
	}

	// run an encrypted function, twice
	encrypted(5);
	encrypted(10);
	
	// run second encrypted function, twice
	encrypted2(5);
	encrypted2(10);
	
	// run an obfuscated function, twice
	obfuscated("Hello");
	obfuscated("Massimo");
	
	// this data will be encrypted and can be read by PROTECT_DECRYPT_DATA call
	const char *encryptedData = PROTECT_DATA_START "This data was encrypted\n" PROTECT_DATA_END;
	Cerr() << PROTECT_DECRYPT_DATA(GetCypher, encryptedData);
}
