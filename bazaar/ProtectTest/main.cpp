#include <CtrlLib/CtrlLib.h>

#include <Protect/Protect.h>

using namespace Upp;

String GetKey(void)
{
	return "\xAA\xBB\xCC\xDD\xEE\xFF";

}

bool Decrypt(byte *start, size_t len)
{
	return PROTECT_DECRYPT ( start, len, GetKey() );
}

void testfn1(void)
{
	PROTECT_START_FUNC(Decrypt);

	PromptOK("testfn1 DECRYPTED SUCCESFULLY!!!");
	return;

	PROTECT_END_FUNC;
}

void testfn2(void)
{
	PROTECT_START_FUNC(Decrypt);

	PromptOK("testfn2 DECRYPTED SUCCESFULLY!!!");
	return;

	PROTECT_END_FUNC;
}

GUI_APP_MAIN
{
	testfn1();
	testfn2();
}

