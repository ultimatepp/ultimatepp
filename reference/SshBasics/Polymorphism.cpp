#include "SshBasics.h"

// SshPolymorphism:
// Demonstrates polymorphism and RTTI for Ssh objects.

void SshPolymorphism(SshSession& session)
{
	constexpr const char *path = "/readme.txt";
	
	Array<Ssh> channels;
	
	channels.Create<Scp>(session);
	channels.Create<SFtp>(session);
	channels.Create<SshExec>(session);

	for(Ssh& channel : channels){
		if(channel.Is<Scp>()) {
			LOG("\nFound: Scp object");
			LOG("-----------------\n");
			LOG(channel.To<Scp>().LoadFile(path));
		}
		else
		if(channel.Is<SFtp>()) {
			LOG("\nFound: Sftp object");
			LOG("------------------\n");
			LOG(channel.To<SFtp>().GetInfo(path).GetName());
		}
		else
		if(channel.Is<SshExec>()) {
			LOG("\nFound: Exec object");
			LOG("------------------\n");
			String out, err;
			channel.To<SshExec>().Execute("ls -l", out, err);
			LOG(out);
			LOG(err);
		}
		if(channel.IsError()) {
			LOG("Operation failed. Reason: " << channel.GetErrorDesc());
		}
	
	}
}
