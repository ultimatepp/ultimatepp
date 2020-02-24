#include "SSH.h"

namespace Upp {

int SshExec::Execute(const String& cmd, String& out, String& err)
{
	if(RequestExec(cmd)) {
		int size = max(ssh->chunk_size,  1024);
		out = Get(size);
		err = GetStdErr(size);
		if(Shut(IsError() ? GetErrorDesc() : Null))
			return  GetExitCode();
	}
	return GetError();
}

int SshExecute(SshSession& session, const String& cmd, String& out, String& err)
{
	return SshExec(session).Execute(cmd, out, err);
}

int SshExecute(SshSession& session, const String& cmd, String& out)
{
	String err;
	int rc = SshExec(session).Execute(cmd, out, err);
	if(!IsNull(err))
		out.Cat(err);
	return rc;
}

String SshExecute(SshSession& session, const String& cmd)
{
	String out, err;
	return SshExecute(session, cmd, out, err) ? String::GetVoid(): out;
}

}