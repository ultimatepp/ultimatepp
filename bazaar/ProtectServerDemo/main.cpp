#include <ProtectServer/ProtectServer.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	ProtectServer server;
	
	// setup database
	server
		.SetAppKey(ScanHexString("AABBCCDDEEFF00112233445566778899"))
		.GetDB()
			.SetHost("localhost")
			.SetDBName("protect_demo")
			.SetUserName("protect_user")
			.SetPassword("protect")
	;
	server
		.GetSmtp()
			.Host("localhost")
			.Port(25)
//			.Auth("user", "password")		// use if mail server requires auth
	;
	server
		.SetWelcomeSubject("ProtectDemo user activation")
		.SetWelcomeBody("<HTML><BODY>Welcome, %USER%,<BR><BR> Your activation code is %ACTIVATIONKEY%<BR><BR><BR></BODY></HTML>")
	;
	
	// setup key
	server.SetCommunicationKey(ScanHexString("aabbccddeeff00112233445566778899"));
	
	// setup cypher
	server.SetCypher(new Snow2);

	server.Run();
}
