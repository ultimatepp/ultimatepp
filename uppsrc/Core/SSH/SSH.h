#ifndef _SSH_SSH_h
#define _SSH_SSH_h

#include <Core/Core.h>

#ifdef PLATFORM_POSIX
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#endif


#include "libssh2/libssh2_setup.h"
#include "libssh2/libssh2.h"
#include "libssh2/libssh2_sftp.h"
#include "libssh2/libssh2_publickey.h"

namespace Upp {

INITIALIZE(SSH);

using SFtpHandle   = LIBSSH2_SFTP_HANDLE*;
using SFtpAttrs    = LIBSSH2_SFTP_ATTRIBUTES;
using ScpAttrs     = libssh2_struct_stat;
using SshHost      = libssh2_knownhost;
using SshAgent     = LIBSSH2_AGENT;
using SshX11Handle = LIBSSH2_CHANNEL*;

namespace SSH {
    extern bool sTrace;
    extern int  sTraceVerbose;
    extern String GetName(int type, int64 id);
}

class Ssh;
class SshSession;
class SFtp;
class SshChannel;
class Scp;
class SshExec;
class SshTunnel;
class SshShell;
class SshHosts;

#include "Core.h"
#include "Session.h"
#include "SFtp.h"
#include "Channels.h"
#include "Hosts.h"
}
#endif
