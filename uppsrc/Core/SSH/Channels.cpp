#include "SSH.h"

namespace Upp {

#define LLOG(x)       do { if(SSH::sTrace) RLOG(SSH::GetName(ssh->otype, ssh->oid) << x); } while(false)
#define VLOG(x)       if(SSH::sTraceVerbose) LLOG(x);
#define LDUMPHEX(x)	  do { if(SSH::sTraceVerbose) RDUMPHEX(x); } while(false)

int SshChannel::AdjustChunkSize(int64 sz)
{
	return (int) clamp(sz, (int64) 1, int64(ssh->chunk_size));
}

void SshChannel::Clear()
{
	result     = Null;
	exitcode   = 0;
	exitsignal = Null;
	done       = 0;
	total      = 0;
	Zero(filestat);
}

bool SshChannel::Cleanup(Error& e)
{
	Unlock();
	if(!Ssh::Cleanup(e) || !IsComplexCmd()) {
		return false;
	}
	ssh->ccmd = -1;
	if(*channel) {
		LLOG("Cleaning up...");
		ssh->status = CLEANUP;
		Close();
	}
	return true;
}

bool SshChannel::Init()
{
	*channel = libssh2_channel_open_session(ssh->session);
	if(!*channel && !WouldBlock())
		SetError(-1);
	if(*channel)
		LLOG("A new channel is opened.");
	return *channel != NULL;
}

void SshChannel::Exit()
{
	ComplexCmd(CHANNEL_EXIT, [=] {
		Cmd(CHANNEL_EXIT, [=]() mutable {
			if(!listener)
				return true;
			auto rc = libssh2_channel_forward_cancel(listener);
			if(!WouldBlock(rc) && rc < 0)
				SetError(rc);
			if(rc == 0)
				listener = NULL;
			return rc == 0;
		});
		Cmd(CHANNEL_EXIT, [=]() mutable {
			if(!channel || *channel == NULL)
				return true;
			auto rc = libssh2_channel_free(*channel);
			if(!WouldBlock(rc) && rc < 0)
				SetError(rc);
			if(rc == 0) {
				*channel = NULL;
				LLOG("Channel succesfully freed.");
			}
			return rc == 0;
		});
	});
}

bool SshChannel::Open()
{
	return Cmd(CHANNEL_OPEN, [=]() mutable {
		return SshChannel::Init();
	});
}

bool SshChannel::Close()
{
	return Cmd(CHANNEL_CLOSE, [=]() mutable {
		if(!channel || !*channel)
			return true;
		auto rc = libssh2_channel_close(*channel);
		if(!WouldBlock(rc) && rc < 0)
			SetError(rc);
		if(rc == 0)
			LLOG("Channel close message is sent to the server.");
		return rc == 0;
	});
}

bool SshChannel::CloseWait()
{
	return Cmd(CHANNEL_WAIT, [=]() mutable {
		auto rc = libssh2_channel_wait_closed(*channel);
		if(!WouldBlock(rc) && rc < 0)
			SetError(rc);
		if(rc == 0)
			LLOG("Channel close message is acknowledged by the server.");
		return rc == 0;
	});
}

bool SshChannel::Request(const String& request, const String& params)
{
	return Cmd(CHANNEL_REQUEST, [=]() mutable {
		auto rc = libssh2_channel_process_startup(
			*channel,
			request,
			request.GetLength(),
			params.GetLength() ? ~params : NULL,
			params.GetLength()
		);
		if(!WouldBlock(rc) && rc < 0)
			SetError(rc);
		if(rc == 0)
			LLOG("\"" << request << "\" request (params: " << params << ") is successful.");
		return rc == 0;
	});

}

bool SshChannel::Terminal(const String& term, int width, int height)
{
	return Cmd(CHANNEL_REQUEST, [=]() mutable {
		auto rc = libssh2_channel_request_pty_ex(
			*channel,
			~term,
			term.GetLength(),
			NULL,
			0,
			width,
			height,
			LIBSSH2_TERM_WIDTH_PX,
			LIBSSH2_TERM_HEIGHT_PX
		);
		if(!WouldBlock(rc) && rc < 0)
			SetError(rc);
		if(rc == 0)
			LLOG("Terminal (" << term << ") [W:" << width << ", H:" << height << "] opened.");
		return rc == 0;
	});
}


bool SshChannel::SetEnv(const String& variable, const String& value)
{
	return Cmd(CHANNEL_SET_ENV, [=]() mutable {
		auto rc = libssh2_channel_setenv(*channel, variable, value);
		if(!WouldBlock(rc) && rc < 0)
			SetError(rc);
		if(rc == 0)
			LLOG("Environment variable '" << variable << "' set to " << value);
		return rc == 0;
	});
}

String SshChannel::Get(int64 size, int sid)
{
	Clear();
	Cmd(CHANNEL_READ, [=]() mutable { return ReadString((String&)result, size, sid); });
	return !IsBlocking() ? Null : (String) result;
}

int64 SshChannel::Get(Stream& out, int64 size, int sid)
{
	Clear();
	Cmd(CHANNEL_READ, [=, &out]() mutable { return ReadStream(out, size, sid); });
	return !IsBlocking() ? 0 : (int64) result;
}

String SshChannel::GetLine(int maxlen, int sid)
{
	Clear();
	Cmd(CHANNEL_READ, [=]{
		bool is_eol = false;
		String& s = (String&) result;
		do {
			int c = Read(sid);
			if(c == -1)
				break;
			else
				ssh->start_time = msecs();
			if(c == '\r')
				continue;
			if(s.GetLength() >= maxlen)
				s = Null;
			is_eol = c == '\n';
			if(!is_eol)
				s.Cat(c);
		}
		while(!is_eol && !IsEof() && !IsTimeout() && IsBlocking());
		return is_eol || IsEof();
	});
	return !IsBlocking() ? Null : (String) result;
}

int SshChannel::GetNow(int sid)
{
	Cmd(CHANNEL_READ, [=]() mutable {
		int c = Read(sid);
		if(c >= 0) result = c;
		return c >= 0;
	});
	return !IsBlocking() ? -1 : (int) result;
}

int SshChannel::GetNow(void* buffer, int sid)
{
	Clear();
	Cmd(CHANNEL_READ, [=]() mutable {
		result = Read(buffer, ssh->chunk_size, sid);
		return true;
	});
	return !IsBlocking() ? -1 : (int) result;
}

int64 SshChannel::Put(const String& s, int sid)
{
	Clear();
	Cmd(CHANNEL_WRITE, [=, &s]() mutable { return WriteString(s, s.GetLength(), sid); });
	return !IsBlocking() ? Null : (int64) result;
}

int64 SshChannel::Put(Stream& in, int64 size, int sid)
{
	Clear();
	Cmd(CHANNEL_WRITE, [=, &in]() mutable { return WriteStream(in, size, sid); });
	return !IsBlocking() ? 0 : (int64) result;
}

bool SshChannel::PutNow(char c, int sid)
{
	return Cmd(CHANNEL_READ, [=]() mutable { return Write(c, sid); });
}

int SshChannel::PutNow(const void* buffer, int64 size, int sid)
{
	Cmd(CHANNEL_WRITE, [=]() mutable {
		result = Write(buffer, size, sid);
		return true;
	});
	return !IsBlocking() ? -1 : (int) result;
}

int SshChannel::Read(void *buffer, int64 len, int sid)
{
	auto n = libssh2_channel_read_ex(*channel, sid, (char*) buffer, len);
	if(n < 0 && !WouldBlock(n))
		SetError(n);
	if(n > 0) {
		done += n;
		total += done;
		if(WhenProgress(done, len))
			SetError(-1, "Read aborted.");
		ssh->start_time = msecs();
		VLOG("Read stream #" << sid << ": " << n << " bytes read.");
	}
	if(IsEof())
		LLOG("Read stream #" << sid << ": EOF.");
	return n;
}

int SshChannel::Read(int sid)
{
	char c;
	auto rc = libssh2_channel_read_ex(*channel, sid, &c, 1);
	if(!WouldBlock(rc) && rc < 0) SetError(rc);
	return rc == 1 ? int(c) : -1;
}

bool SshChannel::ReadString(String& s, int64 len, int sid, bool nb)
{
	return ReadContent(
		[&s](const void* buf, int len){
			s.Cat((const char*) buf, len);
		},
		len,
		sid,
		nb
	);
}

bool SshChannel::ReadStream(Stream& s, int64 len, int sid, bool nb)
{
	return ReadContent(
		[=, &s](const void* buf, int len){
			s.Put(buf, len);
			result = done;
		},
		len,
		sid,
		nb
	);
}

bool SshChannel::ReadContent(Event<const void*,int>&& consumer, int64 len, int sid, bool nb)
{
	auto l = AdjustChunkSize(len - done);
	Buffer<char> buffer(l);

	int n = 0;
	do {
		n = Read(buffer, l, sid);
		if(n < 0)
			return false;
		if(n > 0) {
			if(WhenContent)
				WhenContent(buffer, n);
			else
				consumer(buffer, n);
		}
	}
	while(!IsEof() && done < len && !nb);
	return true;
}

int SshChannel::Write(const void* buffer, int64 len, int sid)
{
	done = 0;
	auto n = libssh2_channel_write_ex(*channel, sid, (const char*) buffer + done , len - done);
	if(n < 0 && !WouldBlock(n))
		SetError(n);
	if(WouldBlock(n))
		return 0;
	if(n > 0) {
		done += n;
		total += done;
		if(WhenProgress(done, len))
			SetError(-1, "Write aborted.");
		ssh->start_time = msecs();
		VLOG("Write stream #" << sid << ": " << n << " bytes written.");
	}
	if(IsEof())
		LLOG("Write stream #" << sid << ": EOF.");
	return n;
}

bool SshChannel::Write(char c, int sid)
{
	auto rc = libssh2_channel_write_ex(*channel, sid, &c, 1);
	if(!WouldBlock(rc) && rc < 0) SetError(rc);
	return rc == 1;
}

bool SshChannel::WriteString(const String& s, int64 len, int sid, bool nb)
{
	int n = 0;
	do {
		n = Write(s.Begin() + total, len - total, sid);
		if(n > 0)
			result = done;
		if(n == 0)
			return false;
	}
	while(!IsEof() && total < len && !nb);
	return true;
}

bool SshChannel::WriteStream(Stream& s, int64 len, int sid, bool nb)
{
	int n = 0;
	do {
		auto l = AdjustChunkSize(len - done);
		String ss = s.Get(l);
		n = Write(ss.Begin(), ss.GetLength(), sid);
		if(n > 0 && n < ss.GetLength()) {
			s.SeekCur(-(ss.GetLength() - n));
			result = (int64) s.GetPos();
		}
		if(!n)
			return false;
	}
	while(!s.IsEof() && done < len && !nb);
	return true;
}

bool SshChannel::SendEof0()
{
	auto rc = libssh2_channel_send_eof(*channel);
	if(!WouldBlock(rc) && rc < 0)
		SetError(rc);
	if(rc == 0)
		LLOG("EOF message is sent to the server.");
	return rc == 0;
}

bool SshChannel::RecvEof0()
{
	auto rc = libssh2_channel_wait_eof(*channel);
	if(!WouldBlock(rc) && rc < 0)
		SetError(rc);
	if(rc == 0)
		LLOG("EOF message is acknowledged by the server.");
	return rc == 0;
}

bool SshChannel::SetWndSz(int64 size, bool force)
{
	auto rc = libssh2_channel_receive_window_adjust2(*channel, size, (char)force, NULL);
	if(!WouldBlock(rc) && rc < 0)
		SetError(rc);
	if(rc == 0)
		LLOG(Format("Receive window size set is to %d.", size));
	return rc == 0;
}

int SshChannel::SetPtySz(int w, int h)
{
	auto rc = libssh2_channel_request_pty_size(*channel, w, h);
	if(!WouldBlock(rc) && rc < 0) rc = 1;
	if(rc == 1)	LLOG("Warning: Couldn't set terminal size.");
	if(rc == 0)	LLOG("Terminal size adjusted. [W:" << w << ", H:" << h << "]");
	return rc;
}

bool SshChannel::SendEof()
{
	return Cmd(CHANNEL_EOF, [=]() mutable {
		return SendEof0();
	});
}

bool SshChannel::RecvEof()
{
	return Cmd(CHANNEL_EOF, [=]() mutable {
		return RecvEof0();
	});
}

bool SshChannel::SendRecvEof()
{
	return ComplexCmd(CHANNEL_EOF, [=]() mutable {
		SendEof();
		RecvEof();
	});
}

bool SshChannel::IsEof()
{
	auto rc = libssh2_channel_eof(*channel);
	return rc != 0;
}

bool SshChannel::SetTerminalSize(int width, int height)
{
	return Cmd(CHANNEL_PTY_SIZE, [=]() mutable {
		return SetPtySz(width, height);
	});
}

bool SshChannel::SetReadWindowSize(int64 size, bool force)
{
	return Cmd(CHANNEL_WIN_SIZE, [=]() mutable {
		return SetWndSz(size, force);
	});
}

int64 SshChannel::GetReadWindowSize()
{
	return (int64) libssh2_channel_window_read(*channel);
}

int64 SshChannel::GetWriteWindowSize()
{
	return (int64) libssh2_channel_window_write(*channel);
}

int SshChannel::GetExitCode()
{
	if(*channel && *channel) {
		exitcode = libssh2_channel_get_exit_status(*channel);
		LLOG("Exit code: " << exitcode);
	}
	return exitcode;
}

String SshChannel::GetExitSignal()
{
	if(*channel && *channel) {
		Buffer<char*> sig(64, 0);
		auto rc = libssh2_channel_get_exit_signal(*channel, sig, NULL, NULL, NULL, NULL, NULL);
		exitsignal = *sig;
		LLOG("Exit signal: " << exitsignal);
	}
	return exitsignal;
}

dword SshChannel::EventWait(int fd, dword events, int tv)
{
	SocketWaitEvent we;
	we.Add(fd, events);
	return we.Wait(tv) > 0 ? we[0] : 0;
}

bool SshChannel::ProcessEvents(String& input)
{
	Buffer<char> buffer(ssh->chunk_size);
	auto len = Read(buffer, ssh->chunk_size);

	ReadWrite(input, buffer, len);
	
	while(!input.IsEmpty()) {
		len = Write(~input, input.GetLength());
		if(len == input.GetLength())
			input.Clear();
		else
		if(len > 0)
			input.Remove(0, len);
		else
			break;
	}
	return IsEof();
}

void SshChannel::ReadWrite(String& in, const void* out, int out_len)
{
}

bool SshChannel::Lock()
{
	if(*lock == 0) {
		LLOG("Channel serialization lock acquired.");
		*lock = ssh->oid;
	}
	return *lock == ssh->oid;
}

void SshChannel::Unlock()
{
	if(*lock == ssh->oid) {
		*lock = 0;
		LLOG("Channel serialization lock released.");
	}
}

SshChannel::SshChannel(SshSession& session)
: Ssh()
{
	ssh->otype		= CHANNEL;
	ssh->session	= session.GetHandle();
	ssh->socket		= &session.GetSocket();
	ssh->timeout	= session.GetTimeout();
	ssh->waitstep   = session.GetWaitStep();
	ssh->wait		= Proxy(session.WhenWait);
	
	channel.Create();
	*channel = NULL;
	listener = NULL;
	lock     = session.GetLockPtr();
	Clear();
}

SshChannel::~SshChannel()
{
	if(channel) { // Picked?
		Ssh::Exit();
		Unlock();
		Exit();
	}
}
}