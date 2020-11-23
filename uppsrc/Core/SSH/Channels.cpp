#include "SSH.h"

namespace Upp {

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::GetName(ssh->otype, ssh->oid) << x); } while(false)
#define VLOG(x)       if(SSH::sTraceVerbose) LLOG(x);
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

bool SshChannel::Init()
{
	ASSERT(!IsOpen());

	LIBSSH2_CHANNEL *ch = libssh2_channel_open_session(ssh->session);
	if(!ch && !WouldBlock())
		ThrowError(-1);
	if(ch) {
		channel = MakeOne<LIBSSH2_CHANNEL*>(ch);
		LLOG("A new channel is opened.");
	}
	return ch;
}

void SshChannel::Exit()
{
	if(!channel)
		return;

	Run([=]() mutable {
		int rc = libssh2_channel_free(*channel);
		if(!WouldBlock(rc) && rc < 0)
			ThrowError(rc);
		if(!rc) {
			ssh->init = false;
			channel.Clear();
			LLOG("Channel succesfully freed.");
		}
		return !rc;
	}, false);
}

bool SshChannel::Open()
{
	if(IsOpen())
		Close();
	return Run([=]() mutable { return Init(); });
}

bool SshChannel::Close()
{
	return Run([=]() mutable {
		int rc = libssh2_channel_close(*channel);
		if(!WouldBlock(rc) && rc < 0) ThrowError(rc);
		if(!rc) LLOG("Channel close message is sent to the server.");
		return !rc;
	});
}

bool SshChannel::WaitClose()
{
	return Run([=]() mutable {
		int rc = libssh2_channel_wait_closed(*channel);
		if(!WouldBlock(rc) && rc < 0) ThrowError(rc);
		if(!rc)	LLOG("Channel close message is acknowledged by the server.");
		return !rc;
	});
}

bool SshChannel::Request(const String& request, const String& params)
{
	return Run([=]() mutable {
		int rc = libssh2_channel_process_startup(
			*channel,
			request,
			request.GetLength(),
			params.GetLength() ? ~params : nullptr,
			params.GetLength()
		);
		if(!WouldBlock(rc) && rc < 0)
			ThrowError(rc);
		if(!rc)
			LLOG("\"" << request << "\" request (params: " << params << ") is successful.");
		return !rc;
	});
}

bool SshChannel::RequestTerminal(const String& term, int width, int height, const String& modes)
{
	return Run([=]() mutable {
		int rc = libssh2_channel_request_pty_ex(
			*channel,
			~term,
			term.GetLength(),
			~modes,
			modes.GetLength(),
			width,
			height,
			LIBSSH2_TERM_WIDTH_PX,
			LIBSSH2_TERM_HEIGHT_PX
		);
		if(!WouldBlock(rc) && rc < 0)
			ThrowError(rc);
		if(!rc)
			LLOG("Terminal (" << term << ") [W:" << width << ", H:" << height << "] opened.");
		return !rc;
	});
}

bool SshChannel::SetEnv(const String& variable, const String& value)
{
	return Run([=]() mutable {
		int rc = libssh2_channel_setenv(*channel, variable, value);
		if(!WouldBlock(rc) && rc < 0) ThrowError(rc);
		if(!rc)	LLOG("Environment variable '" << variable << "' set to " << value);
		return !rc;
	});
}

bool SshChannel::PutEof()
{
	return Run([=]() mutable {
		int rc = libssh2_channel_send_eof(*channel);
		if(!WouldBlock(rc) && rc < 0) ThrowError(rc);
		if(!rc)	LLOG("EOF message is sent to the server.");
		return !rc;
	});
}

bool SshChannel::GetEof()
{
	return Run([=]() mutable {
		int rc = libssh2_channel_wait_eof(*channel);
		if(!WouldBlock(rc) && rc < 0) ThrowError(rc);
		if(!rc) LLOG("EOF message is acknowledged by the server.");;
		return !rc;
	});
}

bool SshChannel::IsEof()
{
	bool b = false;
	INTERLOCKED
	{
		b = libssh2_channel_eof(*channel) != 0;
	}
	if(b)
		LLOG("EOF received.");
	return b;
}

bool SshChannel::SetTerminalSize(int width, int height)
{
	return Run([=]() mutable { return SetPtySz(width, height) >= 0; });
}

int SshChannel::SetPtySz(int w, int h)
{
	int rc = libssh2_channel_request_pty_size(*channel, w, h);
	if(!WouldBlock(rc) && rc < 0) rc = 1;
	if(rc == 1)	LLOG("Warning: Couldn't set terminal size!");
	if(rc == 0)	LLOG("Terminal size adjusted. [W:" << w << ", H:" << h << "]");
	return rc;
}

bool SshChannel::SetReadWindowSize(uint32 size, bool force)
{
	return Run([=]() mutable { return SetWndSz(size, force); });
}

bool SshChannel::SetWndSz(uint32 size, bool force)
{
	int rc = libssh2_channel_receive_window_adjust2(*channel, size, (unsigned char) force, nullptr);
	if(!WouldBlock(rc) && rc < 0) ThrowError(rc);
	if(!rc) LLOG(Format("Receive window size set is to %d.", AsString(size)));
	return !rc;
}

int SshChannel::GetExitCode()
{
	int rc = 0;
	INTERLOCKED
	{
		rc = libssh2_channel_get_exit_status(*channel);
	}
	LLOG("Exit code: " << rc);
	return rc;
}

String SshChannel::GetExitSignal()
{
	char *sig = (char *)"none";
	size_t len = 0;
	String s;
	INTERLOCKED
	{
		libssh2_channel_get_exit_signal(*channel, &sig, &len, nullptr, nullptr, nullptr, nullptr);
	}
	s.Set(sig, len);
	LLOG("Exit signal: " << s);
	return s;
}

int SshChannel::Get(void *ptr, int size, int sid)
{
	done = 0;
	Run([=]() mutable {
		while(done < size && !IsEof() && !IsTimeout()) {
			int rc = Read(ptr, size, sid);
			if(rc < 0) return false;
			if(!rc) break;
			UpdateClient();
		}
		return true;
	});
	return GetDone();
}

String SshChannel::Get(int size, int sid)
{
	StringBuffer sb(size);
	int len = Get(~sb, size, sid);
	sb.SetCount(len);
	return pick(String(sb));
}

String SshChannel::GetLine(int maxlen, int sid)
{
	done = 0;
	String line;
	Run([=, &line]{
		bool eol = false;
		do {
			int c = Read(sid);
			if(c == -1)
				break;
			ssh->start_time = msecs();
			if(c == '\r')
				continue;
			if(line.GetLength() >= maxlen)
				line = Null;
			eol = c == '\n';
			if(!eol) {
				line.Cat(c);
				done++;
			}
		}
		while(!eol && !IsEof() && !IsTimeout());
		return eol || IsEof();
	});
	return line;
}

int SshChannel::Put(const void *ptr, int size, int sid)
{
	done = 0;
	Run([=]() mutable {
		while(done < size && !IsEof() && !IsTimeout()) {
			int rc = Write(ptr, size, sid);
			if(rc < 0) return false;
			if(!rc) break;
			UpdateClient();
		}
		return true;
	});
	return GetDone();
}

int SshChannel::Read(void *ptr, int size, int sid)
{
	int sz = min(size - done, ssh->chunk_size);

	int rc = static_cast<int>(
		libssh2_channel_read_ex(*channel, sid, (char*) ptr + done, size_t(sz))
		);
	if(rc < 0 && !WouldBlock(rc)) {
		ThrowError(rc);
	}
	else
	if(rc > 0) {
		done += rc;
		ssh->start_time = msecs();
		VLOG("Read stream #" << sid << ": " << rc << " bytes read.");
	}
	return rc;
}

int SshChannel::Read(int sid)
{
	char c;
	done = 0;
	return Read(&c, 1, sid) == 1 ? int(c) : -1;
}

int SshChannel::Write(const void *ptr, int size, int sid)
{
	int sz = min(size - done, ssh->chunk_size);

	int rc = static_cast<int>(
		libssh2_channel_write_ex(*channel, sid, (const char*) ptr + done, size_t(sz))
		);
	if(rc < 0 && !WouldBlock(rc)) {
		ThrowError(rc);
	}
	else
	if(rc > 0) {
		done += rc;
		ssh->start_time = msecs();
		VLOG("Write stream #" << sid << ": " << rc << " bytes written.");
	}
	return rc;
}

bool SshChannel::Write(char c, int sid)
{
	done = 0;
	return Write(&c, 1, sid) == 1;
}

dword SshChannel::EventWait(int fd, dword events, int tv)
{
	SocketWaitEvent we;
	we.Add(fd, events);
	return we.Wait(tv) > 0 ? we[0] : 0;
}

bool SshChannel::ProcessEvents(String& input)
{
	Buffer<char> buffer(ssh->chunk_size, 0);

	return Run([=, &buffer, &input]{
		done = 0;
		int len = Read(buffer, ssh->chunk_size);
		ReadWrite(input, buffer, len);
		while(!input.IsEmpty() && InProgress()) {
			done = 0;
			len = Write(~input, input.GetLength());
			if(len <= 0)
				break;
			input.Remove(0, len);
		}
		return IsEof();
	});
}


bool SshChannel::Shut(const String& msg, bool nowait)
{
	bool eof = false;
	if(PutEof() && !nowait)
		eof = GetEof();
	if(Close() && eof)
		WaitClose();
	if(!IsNull(msg))
		SetError(-1, msg);
	return !IsError();
}

SshChannel::SshChannel(SshSession& session)
: done(0)
{
	ssh->otype		= CHANNEL;
	ssh->session	= session.GetHandle();
	ssh->socket		= &session.GetSocket();
	ssh->timeout	= session.GetTimeout();
	ssh->waitstep   = session.GetWaitStep();
	ssh->whenwait	= Proxy(session.WhenWait);
}

SshChannel::~SshChannel()
{
	Exit();
}
}