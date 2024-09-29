#ifndef _SshBasic_SshBasic_h
#define _SshBasic_SshBasic_h

#include <Core/Core.h>
#include <Core/SSH/SSH.h>

using namespace Upp;

void SFtpGet(SshSession& session);
void SFtpStreamGet(SshSession& session);
void SFtpTransparency(SshSession& session);
void SFtpAsyncGet(SshSession& session);
void SFtpAsyncGet2(SshSession& session);
void ExecListDir(SshSession& session);
void ExecAsyncListDir(SshSession& session);
void ShellConsole(SshSession& session);
void ScpGet(SshSession& session);
void ForwardTcpIp(SshSession& session);
void X11Forwarding(SshSession& session);
void SshPick(SshSession& session);
void SshPolymorphism(SshSession& session);
void TraceVerbose();

#endif

