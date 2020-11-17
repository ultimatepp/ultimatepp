#include <CtrlLib/CtrlLib.h>
#include <Core/SSH/SSH.h>

using namespace Upp;

// SFtpFileSel:
// This example demonstrates file selector integration of SFtp class,
// using the FileSystemInfo interface.

GUI_APP_MAIN
{
	String url = "demo:password@test.rebex.net:22";

	if(!EditTextNotNull(url, "SFTP server", "Url"))
		return;
		
	Progress pi;
	pi.Title("Connecting to SFTP server");
	pi.Set(0, 5);
	pi.Create();

	SshSession session;
	session.WhenPhase = [&pi] (int phase) {
		switch(phase) {
		case SshSession::PHASE_DNS:
			pi.SetText("Resolving name...");
			pi.SetPos(1);
			break;
		case SshSession::PHASE_CONNECTION:
			pi.SetText("Connecting...");
			pi.SetPos(2);
			break;
		case SshSession::PHASE_HANDSHAKE:
			pi.SetText("Starting protocol handshake...");
			pi.SetPos(3);
			break;
		case SshSession::PHASE_AUTHORIZATION:
			pi.SetText("Authenticating...");
			pi.SetPos(4);
			break;
		case SshSession::PHASE_SUCCESS:
			pi.SetText("Client successfully connected to server.");
			pi.SetPos(5);
			break;
		}
	};
	
	session.WhenWait = [&pi] { pi.ProcessEvents(); };
	if(session.Timeout(30000).Connect(url)) {
		pi.Close();
		SFtp sftp(session);
		sftp.WhenWait = []{}; // "NOOP"
		SFtpFileSystemInfo sfsi(sftp);
		FileSel fsel;
		fsel.Filesystem((FileSystemInfo&) sfsi);
		fsel.BaseDir(sftp.GetDefaultDir());
		while(fsel.NoAsking().ExecuteOpen("Select a file to download (Select cancel to quit)")) {
			pi.Reset();
			String path = fsel.Get();
			sftp.WhenProgress = [&pi] (int64 done, int64 total)
			{
				pi.SetText(
					Format("%s of %s is transferred",
						FormatFileSize(done),
						FormatFileSize(total)
					)
				);
				return pi.SetCanceled(int(done), int(total));
			};
			pi.Title("Downloading " << GetFileName(path));
			pi.Create();
			String f = sftp.LoadFile(path);
			if(sftp.IsError())
				ErrorOK(DeQtf(sftp.GetErrorDesc()));
			else
				SaveFile(NativePath(AppendFileName(GetCurrentDirectory(), GetFileName(path))), f);
			pi.Close();
		}
	}
	else
		ErrorOK(DeQtf(session.GetErrorDesc()));
}
