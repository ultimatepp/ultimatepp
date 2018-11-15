#include "SshBasics.h"

// TraceVerbose:
// To activate verbose logging, you need to set the LIBSSH2TRACE flag via
// TheIDE->Main Configuration settings.

void TraceVerbose()
{
	Ssh::TraceVerbose(
//      LIBSSH2_TRACE_SOCKET    |
//      LIBSSH2_TRACE_KEX       |
        LIBSSH2_TRACE_AUTH      |
        LIBSSH2_TRACE_CONN      |
//      LIBSSH2_TRACE_SCP       |
//      LIBSSH2_TRACE_SFTP      |
//      LIBSSH2_TRACE_PUBLICKEY |
        LIBSSH2_TRACE_ERROR
	);
}