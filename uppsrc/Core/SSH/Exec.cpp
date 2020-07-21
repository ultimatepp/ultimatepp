#include "SSH.h"

namespace Upp {

int SshExec::Execute(const String& cmd, String& out, String& err)
{
	if(RequestExec(cmd)) {
		ReadStdOut(out);
		ReadStdErr(err);
		if(Shut(IsError() ? GetErrorDesc() : Null))
			return  GetExitCode();
	}
	return GetError();
}

void SshExec::ReadStdOut(String& out)
{
	String s;
	while(!(s = Get(ssh->chunk_size)).IsEmpty())
		out.Cat(s);
}

void SshExec::ReadStdErr(String& err)
{
	String s;
	while(!(s = GetStdErr(ssh->chunk_size)).IsEmpty())
		err.Cat(s);
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