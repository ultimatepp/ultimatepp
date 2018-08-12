#include "SSH.h"

namespace Upp {

int SshExec::Execute(const String& cmd, String& out, String& err)
{
	if(RequestExec(cmd)) {
		int size = max(ssh->chunk_size,  1024);
		out = Get(size);
		err = GetStdErr(size);
		if(!IsError()		&&
			PutGetEof()		&&
			Close()			&&
			WaitClose())
			return  GetExitCode();
	}
	return GetError();
}
}
