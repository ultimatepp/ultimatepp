#include "SSH.h"

namespace Upp {
	
bool SshHosts::Add(const String& host, int port, const Info& info, const String& comment)
{
	return Add(Format("[%s]:%d", host, port), info, comment);
}

bool SshHosts::Add(const String& host, const Info& info, const String& comment)
{
	ASSERT(ssh_session);
	bool b = handle &&
		libssh2_knownhost_addc(
			handle,
			~host,
			NULL,
			~info.key, info.key.GetLength(),
			~comment, comment.GetLength(),
			info.type |
			LIBSSH2_KNOWNHOST_TYPE_PLAIN |
			LIBSSH2_KNOWNHOST_KEYENC_RAW,
			NULL
		) == 0;
	return b ? b : Error();
}

bool SshHosts::Remove(SshHost* host)
{
	ASSERT(ssh_session);
	auto b = handle && libssh2_knownhost_del(handle, host) == 0;
	return b ? b : Error();
}

bool SshHosts::Load(const String& filename)
{
	ASSERT(ssh_session);
	file_path = filename;
	auto b = libssh2_knownhost_readfile(handle, ~file_path, LIBSSH2_KNOWNHOST_FILE_OPENSSH) >= 0;
	return b ? b : Error();
}

bool SshHosts::Save()
{
	return SaveAs(file_path);
}

bool SshHosts::SaveAs(const String& filename)
{
	ASSERT(ssh_session);
	auto b = handle && libssh2_knownhost_writefile(handle, ~filename, LIBSSH2_KNOWNHOST_FILE_OPENSSH) == 0;
	return b ? b : Error();
}

SshHosts::Info SshHosts::Check(const String& host, int port)
{
	ASSERT(ssh_session);
	Info info;
	if(handle) {
		int	   type   = 0;
		size_t length = 0;
		auto*  p = libssh2_session_hostkey(ssh_session, &length, &type);
		if(!p) {
			Error();
			info.status = LIBSSH2_KNOWNHOST_CHECK_FAILURE;
			return pick(info);
		}
		info.status = libssh2_knownhost_checkp(
			handle,
			~host,
			port,
			p,
			length,
			LIBSSH2_KNOWNHOST_TYPE_PLAIN |
			LIBSSH2_KNOWNHOST_KEYENC_RAW,
			NULL
		);
		info.key.Set(p, length);
		switch(type) {
			case LIBSSH2_HOSTKEY_TYPE_RSA:
				info.type = LIBSSH2_KNOWNHOST_KEY_SSHRSA;
				break;
			case LIBSSH2_HOSTKEY_TYPE_DSS:
				info.type = LIBSSH2_KNOWNHOST_KEY_SSHDSS;
				break;
			case LIBSSH2_HOSTKEY_TYPE_UNKNOWN:
				info.type = LIBSSH2_KNOWNHOST_KEY_UNKNOWN;
				break;
			default:
				NEVER();
		}
	}
	return pick(info);
}

Vector<SshHost*> SshHosts::GetHosts()
{
	ASSERT(ssh_session);
	Vector<SshHost*> v;
	SshHost *prev = NULL, *next = NULL;
	int rc = libssh2_knownhost_get(handle, &prev, NULL);
	if(rc >= 0) {
		v.Add(prev);
		if(rc == 0)
			while(rc < 1) {
				rc = libssh2_knownhost_get(handle, &next, prev);
				if(rc < 0) break;
				v.Add(next);
				prev = next;
			}
	}
	if(rc < 0) Error();
	return pick(v);
}

bool SshHosts::Error()
{
	ASSERT(ssh_session);
	String msg = !handle ? "Invalid host handle." : "";
	//_error = ssh_liberror(ssh_session, handle ? 0 : -1, msg);
	return false;
}

SshHosts::SshHosts(SshSession& session)
{
	ASSERT(session.GetHandle());
	ssh_session   = session.GetHandle();
	handle = libssh2_knownhost_init(ssh_session);
}

SshHosts::~SshHosts()
{
	if(ssh_session && handle)
		libssh2_knownhost_free(handle);
}

}