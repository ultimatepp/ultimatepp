#include <CtrlLib/CtrlLib.h>
#include <Core/SSH/SSH.h>

using namespace Upp;

// SFtpFileSel:
// This example demonstrates FileSel integration of SFtp class,
// using FileSystemInfo interface.

GUI_APP_MAIN
{
	const char *url = "demo:password@test.rebex.net:22";

	Progress pi;
	pi.Title("Connecting to ssh server");
	pi.SetTotal(5);
	pi.Create();

	bool refresh_gui = true; // FileSel refreshes its file list on every single GUI event.
	                         // In most cases this is not possible for an sftp connection,
	                         // because of the network latency. This switch is a workaround
	                         // for this particular problem, and allows the calls to the sftp
	                         // subsystem to be in sync with FileSel requests at the cost of a
	                         // minimal GUI lag in FileSel.

	SshSession session;
	session.WhenPhase = [&refresh_gui, &pi] (int phase) {
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
			pi.Close();
			refresh_gui = false;
			break;
		}
	};
	session.WhenWait = [&refresh_gui, &pi] { if(refresh_gui) pi.ProcessEvents(); };
	if(session.Timeout(30000).Connect(url)) {
		SFtp sftp(session);
		SFtpFileSystemInfo sfsi(sftp);
		FileSel fsel;
		fsel.Filesystem(sfsi);
		fsel.BaseDir(sftp.GetDefaultDir());
		while(fsel.Asking(false).ExecuteOpen("Select a file to download (Select cancel to quit)")) {
			pi.Reset();
			refresh_gui = true;
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
			refresh_gui = false;
		}
	}
	else
		ErrorOK(DeQtf(session.GetErrorDesc()));
}
