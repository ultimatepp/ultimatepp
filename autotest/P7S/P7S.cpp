#include <Core/Core.h>

using namespace Upp;

// openssl req -newkey rsa:2048 -new -nodes -x509 -days 3650 -keyout key.pem -out cert.pem

CONSOLE_APP_MAIN
{
	String dir = GetHomeDirFile("autotest.p7s");

	DeleteFolderDeep(dir);
	DirectoryCreate(dir);
	
	String content = AppendFileName(dir, "content.txt");
	SaveFile(content, "Hello world!");
	
	DUMP(SHA1String(content));
	
	String signature = AppendFileName(dir, "signature.der");
	
	String cert = LoadDataFile("public.pem");
	String pkey = LoadDataFile("private.pem");
	
	SaveFile(AppendFileName(dir, "public.pem"), cert);
	SaveFile(AppendFileName(dir, "private.pem"), pkey);

	String sgn = GetP7Signature(LoadFile(content), cert, pkey);
	ASSERT(sgn.GetCount());
	SaveFile(signature, sgn);
	
	const char *cmd =
	"openssl smime -verify -CAfile ~/autotest.p7s/public.pem "
	"-content ~/autotest.p7s/content.txt "
	"-inform der -in ~/autotest.p7s/signature.der";
	
	LOG(cmd);
	
	int code = system(cmd);
	
	DUMP(code);
	
	ASSERT(code == 0);
	
	DeleteFolderDeep(dir);
	
	LOG("===================== OK");
}
