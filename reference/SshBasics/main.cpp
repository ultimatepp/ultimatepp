#include "SshBasics.h"

//#define SSH_KBAUTH		// Enables ssh keyboard authentication method.

#define SSH_SFTP

// Important note: The scp, X11 forwarding and tcp-ip/port forwarding (tunnel) examples will not
// work with the public ssh test server used in this reference example (test.rebex.net). In order
// to run these examples you can set up an easy-to-use ssh server (e.g. OpenSSH) on a local
// machine for testing purposes.

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
#ifdef flagLIBSSH2TRACE
	VerboseLogging();
#else
//	Ssh::Trace();
#endif

	SshSession session;
#if defined(SSH_KBAUTH)
	session.KeyboardAuth();
	session.WhenKeyboard = [](String title, String instructions, String prompt)
	{
		// Title and insctructions are optional and might be empty.
		if(!IsNull(title))
			LOG(title);
		if(!IsNull(instructions))
			LOG(instructions);
		
		Cout() << prompt;
		return ReadSecret(); // "password"
	};
#endif
	if(session.Timeout(30000).Connect("demo:password@test.rebex.net:22")) {
#if   defined(SSH_SFTP)
		SFtpGet(session);
#elif defined(SSH_SFTP_STREAM)
		SFtpStreamGet(session);
#elif defined(SSH_SFTP_TRANSPARENCY)
		SFtpTransparency(session);
#elif defined(SSH_SFTP_MT)
		SFtpAsyncGet(session);
#elif defined(SSH_SFTP_MT_LOOP)
		SFtpAsyncGet2(session);
#elif defined(SSH_EXEC)
		ExecListDir(session);
#elif defined(SSH_EXEC_MT)
		ExecAsyncListDir(session);
#elif defined(SSH_SCP)
		ScpGet(session);
#elif defined(SSH_SHELL)
		ShellConsole(session);
#elif defined(SSH_SHELL_X11)
		X11Forwarding(session);
#elif defined(SSH_TUNNEL)
		ForwardTcpIp(session);
#elif defined(SSH_PICK_SEMANTICS)
		SshPick(session);
#elif defined(SSH_POLYMORPHISM)
		SshPolymorphism(session);
#endif
		return;
	}
	LOG(session.GetErrorDesc());
}

