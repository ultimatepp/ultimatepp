#include "SSH.h"
#include "Malloc.cpp"

namespace Upp {

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::GetName(ssh->otype, ssh->oid) << x); } while(false)
#define LDUMPHEX(x)   do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

// sKeyboardCallback: Authenticates a session, using keyboard-interactive authentication.

static void sKeyboardCallback(const char *name, int name_len, const char *instruction,
	int instruction_len, int num_prompts, const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
	LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses, void **abstract)
{
	SshSession *session = reinterpret_cast<SshSession*>(*abstract);
	for(auto i = 0; i < num_prompts; i++) {
		auto response = session->WhenKeyboard(
			String(name, name_len),
			String(instruction, instruction_len),
			String(prompts[i].text, prompts[i].length)
		);
#ifdef UPP_HEAP
		auto *r = (char*) ssh_malloc(response.GetLength(), abstract);
		memcpy(r, response.Begin(), response.GetLength());
#else
		auto *r = strdup(~response);
#endif
		if(r) {
			responses[i].text   = r;
			responses[i].length = response.GetLength();
		}
	}
}

// sChangePasswordCallback: Requests that the client's password be changed.

static void sChangePasswordCallback(LIBSSH2_SESSION *session, char **pwd, int *len, void **abstract)
{
	String newpwd = reinterpret_cast<SshSession*>(*abstract)->WhenPasswordChange();
#ifdef UPP_HEAP
		*pwd = (char*) ssh_malloc(newpwd.GetLength(), abstract);
		memcpy(*pwd, ~newpwd, newpwd.GetLength());
#else
		*pwd = strdup(~newpwd);
#endif
}

// sX11RequestCallback: Dispatches incoming X11 requests.

static void sX11RequestCallback(LIBSSH2_SESSION *session, LIBSSH2_CHANNEL *channel, char *shost, int sport, void **abstract)
{
	reinterpret_cast<SshSession*>(*abstract)->WhenX11((SshX11Handle) channel);
}

// slibssh2DebugCallback: Allows full-level logging (redirection) of libsssh2 diagnostic messages.

#ifdef flagLIBSSH2TRACE
static void slibssh2DebugCallback(LIBSSH2_SESSION *session, void *context, const char *data, size_t length)
{
	if(!session  || !SSH::sTraceVerbose)
		return;
	auto* ssh_obj = reinterpret_cast<SshSession*>(context);
	RLOG(SSH::GetName(ssh_obj->GetType(), ssh_obj->GetId()) << String(data, int64(length)));
}
#endif

void SshSession::Exit()
{
	if(!session)
		return;

	Run([=]() mutable {
		if(!ssh->session)
			return true;
		int rc = libssh2_session_disconnect(ssh->session, "Disconnecting...");
		if(WouldBlock(rc))
			return false;
		LLOG("Successfully disconnected from the server.");
		return true;
	});

	Run([=]() mutable {
		if(!ssh->session)
			return true;
		int rc = libssh2_session_free(ssh->session);
		if(WouldBlock(rc))
			return false;
		ssh->init    = false;
		ssh->socket  = nullptr;
		ssh->session = nullptr;
		session->connected = false;
		LLOG("Session handles freed.");
		return true;
	}, false);
}

bool SshSession::Connect(const String& url)
{
	UrlInfo u(url);

	auto b = findarg(u.scheme, "ssh", "sftp", "scp", "exec") >= 0 || (u.scheme.IsEmpty() && !u.host.IsEmpty());
	int port = (u.port.IsEmpty() || !b) ? 22 : StrInt(u.port);
	if(b) return Connect(u.host, port, u.username, u.password);
	SetError(-1, "Malformed secure shell URL.");
	return false;
}

bool SshSession::Connect(const String& host, int port, const String& user, const String& password)
{
	IpAddrInfo ipinfo;

	if(!Run([=, &ipinfo] () mutable {
		if(host.IsEmpty())
			ThrowError(-1, "Host is not specified.");
		ssh->session = nullptr;
		session->socket.Timeout(0);
		if(!WhenProxy) {
			ipinfo.Start(host, port);
			LLOG(Format("Starting DNS sequence locally for %s:%d", host, port));
		}
		else
			LLOG("Proxy plugin found. Attempting to connect via proxy...");
		WhenPhase(WhenProxy ? PHASE_CONNECTION : PHASE_DNS);
		return true;
	})) goto Bailout;

	if(!WhenProxy) {
		if(!Run([=, &ipinfo] () mutable {
			if(ipinfo.InProgress())
				return false;
			if(!ipinfo.GetResult())
				ThrowError(-1, "DNS lookup failed.");
			WhenPhase(PHASE_CONNECTION);
			return true;
		})) goto Bailout;

		if(!Run([=, &ipinfo] () mutable {
			if(!session->socket.Connect(ipinfo))
				return false;
			ipinfo.Clear();
			return true;
		})) goto Bailout;

		if(!Run([=, &ipinfo] () mutable {
			if(!session->socket.WaitConnect())
				return false;
			LLOG("Successfully connected to " << host <<":" << port);
			return true;
		})) goto Bailout;
	}
	else {
		if(!Run([=] () mutable {
			if(!WhenProxy())
				ThrowError(-1, "Proxy connection attempt failed.");
			LLOG("Proxy connection to " << host << ":" << port << " is successful.");
			return true;
		})) goto Bailout;
	}

	if(!Run([=] () mutable {
#ifdef UPP_HEAP
			LLOG("Using Upp's memory managers.");
			ssh->session = libssh2_session_init_ex((*ssh_malloc), (*ssh_free), (*ssh_realloc), this);
#else
			LLOG("Using libssh2's memory managers.");
			ssh->session = libssh2_session_init_ex(nullptr, nullptr, nullptr, this);
#endif
			if(!ssh->session)
				ThrowError(-1, "Failed to initalize libssh2 session.");
#ifdef flagLIBSSH2TRACE
			if(libssh2_trace_sethandler(ssh->session, this, &slibssh2DebugCallback))
				LLOG("Warning: Unable to set trace (debug) handler for libssh2.");
			else {
				libssh2_trace(ssh->session, SSH::sTraceVerbose);
				LLOG("Verbose debugging mode enabled.");
			}
#endif
			libssh2_session_set_blocking(ssh->session, 0);
			ssh->socket = &session->socket;
			LLOG("Session successfully initialized.");
			WhenConfig();
			libssh2_session_flag(ssh->session, LIBSSH2_FLAG_COMPRESS, (int) session->compression);
			LLOG("Compression is " << (session->compression ? "enabled." : "disabled."));
			WhenPhase(PHASE_HANDSHAKE);
			return true;
	})) goto Bailout;

	while(!session->iomethods.IsEmpty()) {
		if(!Run([=] () mutable {
			int    method = session->iomethods.GetKey(0);
			String mnames = GetMethodNames(method);
			int rc = libssh2_session_method_pref(ssh->session, method, ~mnames);
			if(!WouldBlock(rc) && rc < 0) ThrowError(rc);
			if(!rc && !session->iomethods.IsEmpty()) {
				LLOG("Transport method: #" << method << " is set to [" << mnames << "]");
				session->iomethods.Remove(0);
			}
			return !rc;
		})) goto Bailout;
	}

	if(!Run([=] () mutable {
			int rc = libssh2_session_handshake(ssh->session, session->socket.GetSOCKET());
			if(!WouldBlock(rc) && rc < 0) ThrowError(rc);
			if(!rc) {
				LLOG("Handshake successful.");
				WhenPhase(PHASE_AUTHORIZATION);
				if(WhenVerify && !WhenVerify(host, port))
					ThrowError(-1);
			}
			return !rc;
	})) goto Bailout;

	if(!Run([=] () mutable {
			session->authmethods = libssh2_userauth_list(ssh->session, user, user.GetLength());
			if(IsNull(session->authmethods)) {
				if(libssh2_userauth_authenticated(ssh->session)) {
					LLOG("Server @" << host << " does not require authentication!");
					WhenPhase(PHASE_SUCCESS);
					session->connected = true;
					return session->connected;
				}
				else
				if(!WouldBlock())
					ThrowError(-1);
				return false;
			}
			LLOG("Authentication methods list successfully retrieved: [" << session->authmethods << "]");
			WhenAuth();
			return true;
	})) goto Bailout;

	if(session->connected)
		goto Finalize;

	if(!Run([=] () mutable {
			int rc = -1;
			switch(session->authmethod) {
				case PASSWORD:
					rc = libssh2_userauth_password_ex(
							ssh->session,
							~user,
							 user.GetLength(),
							~password,
							 password.GetLength(),
							 WhenPasswordChange
								? &sChangePasswordCallback
									: nullptr);
					break;
				case PUBLICKEY: {
					// Allow  computation of public key from private key.
					const char *pubkey = !IsNull(session->pubkey) ? ~session->pubkey : nullptr;
					rc = session->keyfile
					?	libssh2_userauth_publickey_fromfile(
							 ssh->session,
							~user,
							 pubkey,
							~session->prikey,
							~session->phrase)
					:	libssh2_userauth_publickey_frommemory(
							 ssh->session,
							~user,
							 user.GetLength(),
							 pubkey,
							~session->pubkey.GetLength(),
							~session->prikey,
							 session->prikey.GetLength(),
							~session->phrase);
					break;
				}
				case HOSTBASED: {
					if(!session->keyfile)
						ThrowError(-1, "Keys cannot be loaded from memory.");
					else {
					// Allow  computation of public key from private key.
					const char *pubkey = !IsNull(session->pubkey) ? ~session->pubkey : nullptr;
					rc = libssh2_userauth_hostbased_fromfile(
							 ssh->session,
							~user,
							 pubkey,
							~session->prikey,
							~session->phrase,
							~host);
					}
					break;
				}
				case KEYBOARD:
					rc = libssh2_userauth_keyboard_interactive(
						ssh->session,
						~user,
						&sKeyboardCallback);
					break;
				case SSHAGENT:
					rc = TryAgent(user);
					break;
				default:
					NEVER();

			}
			if(rc != 0 && !WouldBlock(rc))
				ThrowError(rc);
			if(rc == 0 && libssh2_userauth_authenticated(ssh->session)) {
				LLOG("Client succesfully authenticated.");
				WhenPhase(PHASE_SUCCESS);
				session->connected = true;
			}
			return	session->connected;
	})) goto Bailout;

Finalize:
#ifdef PLATFORM_POSIX
	libssh2_session_callback_set2(ssh->session, LIBSSH2_CALLBACK_X11, (libssh2_cb_generic*) sX11RequestCallback);
	LLOG("X11 dispatcher is set.");
#endif
	return true;

Bailout:
	LLOG("Connection attempt failed. Bailing out...");
	Exit();
	return false;
}

void SshSession::Disconnect()
{
	Exit();
}

SFtp SshSession::CreateSFtp()
{
	ASSERT(ssh && ssh->session);
	return pick(SFtp(*this));
}

SshChannel SshSession::CreateChannel()
{
	ASSERT(ssh && ssh->session);
	return pick(SshChannel(*this));
}

SshExec SshSession::CreateExec()
{
	ASSERT(ssh && ssh->session);
	return pick(SshExec(*this));
}

Scp SshSession::CreateScp()
{
	ASSERT(ssh && ssh->session);
	return pick(Scp(*this));
}

SshTunnel SshSession::CreateTunnel()
{
	ASSERT(ssh && ssh->session);
	return pick(SshTunnel(*this));
}

SshShell SshSession::CreateShell()
{
	ASSERT(ssh && ssh->session);
	return pick(SshShell(*this));
}

String SshSession::GetHostKeyHash(int type, int length) const
{
	String hash;
	if(ssh->session) {
		hash = libssh2_hostkey_hash(ssh->session, type);
		if(hash.GetLength() > length)
			hash.TrimLast(hash.GetLength() - length);
	}
	return hash;
}

ValueMap SshSession::GetMethods() const
{
	ValueMap methods;
	if(ssh->session) {
		for(int i = METHOD_EXCHANGE; i <= METHOD_SLANGUAGE; i++) {
			const char **p = nullptr;
			auto rc = libssh2_session_supported_algs(ssh->session, i, &p);
			if(rc > 0) {
				auto& v = methods(i);
				for(int j = 0; j < rc; j++) {
					v << p[j];
				}
				libssh2_free(ssh->session, p);
			}
		}
	}
	return pick(methods);
}

String SshSession::GetMethodNames(int type) const
{
	String names;
	const Value& v = session->iomethods[type];
	if(IsValueArray(v)) {
		for(int i = 0; i < v.GetCount(); i++)
			names << v[i].To<String>() << (i < v.GetCount() - 1 ? "," : "");
	}
	else names << v;
	return pick(names);
}

int SshSession::TryAgent(const String& username)
{
	LLOG("Attempting to authenticate via ssh-agent...");
	auto agent = libssh2_agent_init(ssh->session);
	if(!agent)
		ThrowError(-1, "Couldn't initialize ssh-agent support.");
	if(libssh2_agent_connect(agent)) {
		libssh2_agent_free(agent);
		ThrowError(-1, "Couldn't connect to ssh-agent.");
	}
	if(libssh2_agent_list_identities(agent)) {
		FreeAgent(agent);
		ThrowError(-1, "Couldn't request identities to ssh-agent.");
	}
	libssh2_agent_publickey *id = nullptr, *previd = nullptr;

	for(;;) {
		auto rc = libssh2_agent_get_identity(agent, &id, previd);
		if(rc < 0) {
			FreeAgent(agent);
			ThrowError(-1, "Unable to obtain identity from ssh-agent.");
		}
		if(rc != 1) {
			if(libssh2_agent_userauth(agent, ~username, id)) {
				LLOG(Format("Authentication with username %s and public key %s failed.",
							username, id->comment));
			}
			else {
				LLOG(Format("Authentication with username %s and public key %s succeesful.",
							username, id->comment));
				break;
			}
		}
		else {
			FreeAgent(agent);
			ThrowError(-1, "Couldn't authenticate via ssh-agent");
		}
		previd = id;
	}
	FreeAgent(agent);
	return 0;
}

void SshSession::FreeAgent(SshAgent* agent)
{
	libssh2_agent_disconnect(agent);
	libssh2_agent_free(agent);
}


SshSession& SshSession::Keys(const String& prikey, const String& pubkey, const String& phrase, bool fromfile)
{
    session->prikey  = prikey;
    session->pubkey  = pubkey;
    session->phrase  = phrase;
    session->keyfile = fromfile;
    return *this;
}

SshSession::SshSession()
: Ssh()
{
    session.Create();
    ssh->otype           = SESSION;
    ssh->whenwait        = Proxy(WhenWait);
    session->authmethod  = PASSWORD;
    session->connected   = false;
    session->keyfile     = true;
    session->compression = false;
 }

SshSession::~SshSession()
{
	Exit();
}
}