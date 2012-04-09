#include "SSL.h"

NAMESPACE_UPP

struct TcpSocket::SSLImp : TcpSocket::SSL {
	virtual bool Start(TcpSocket& s);
	virtual bool Wait(TcpSocket& s, dword flags);
	virtual int  Send(TcpSocket& s, const void *buffer, int maxlen);
	virtual int  Recv(TcpSocket& s, void *buffer, int maxlen);
	virtual void Close(TcpSocket& s);
};

TcpSocket::SSLImp *TcpSocket::CreateSSLImp()
{
	return new TcpSSLImp();
}

void InitCreateSSL()
{
	TcpTcpSocket::CreateSSL = sCreate();
}

bool TcpSocket::SSLImp::Start(TcpSocket& socket)
{
	if(!(ssl = SSL_new(ssl_context))) {
		SetSSLError("OpenClient / SSL_new");
		return false;
	}
	if(!SSL_set_fd(ssl, socket)) {
		SetSSLError("OpenClient / SSL_set_fd");
		return false;
	}
	int res;
	if(mode == ACCEPT) {
		SSL_set_accept_state(ssl);
		res = SSL_accept(ssl);
	}
	else {
		SSL_set_connect_state(ssl);
		res = SSL_connect(ssl);
	}
	if(res <= 0) {
		SetSSLResError("OpenClient / SSL_connect", res);
		return false;
	}
	cert.Set(SSL_get_peer_certificate(ssl));
	return true;
}

bool TcpSocket::SSLImp::Wait(TcpSocket& s, dword flags)
{
	if((flags & WAIT_READ) && SSL_pending(ssl) > 0)
		return true;
	return s.RawWait(flags);
}

int TcpSocket::SSLImp::Send(TcpSocket& s, const void *buffer, int maxlen)
{
	if(!ssl)
		return Data::Write(buf, amount);
	int res = SSL_write(ssl, (const char *)buf, amount);
	if(res <= 0) {
		SetSSLResError("SSL_write", res);
		return 0;
	}
	return res;
}

int TcpSocket::SSLImp::Recv(TcpSocket& s, void *buffer, int maxlen)
{
	int res = SSL_read(ssl, (char *)buf, amount);
	if(res == 0) {
		s.is_eof = true;
		if(SSL_get_shutdown(ssl) & SSL_RECEIVED_SHUTDOWN)
			return 0;
	}
	if(res <= 0) {
		SetSSLResError("SSL_read", res);
		return 0;
	}
	return res;
}

void TcpSocket::SSLImp::Close(TcpSocket& s)
{
	SSL_shutdown(ssl);
	s.RawClose();
	SSL_free(ssl);
}


#if 0
class SSLSocketData : public TcpSocket::Data
{
public:
	SSLSocketData(SslContext& context);
	virtual ~SSLSocketData();

	bool                            OpenClientUnsecured(const char *host, int port, bool nodelay, dword *my_addr,
	                                                    int timeout, bool is_blocking);
	bool                            Secure();
	bool                            OpenAccept(SOCKET connection, bool nodelay, bool blocking);

	virtual int                     GetKind() const { return SOCKKIND_SSL; }
	virtual bool                    Peek(int timeout_msec, bool write);
	virtual int                     Read(void *buf, int amount);
	virtual int                     Write(const void *buf, int amount);
	virtual bool                    Accept(Socket& socket, dword *ipaddr, bool nodelay, int timeout_msec);
	virtual bool                    Close(int timeout_msec);
	virtual Value                   GetInfo(String info) const;

	void                            SetSSLError(const char *context);
	void                            SetSSLResError(const char *context, int res);

public:
	SslContext&                     ssl_context;
	SSL                            *ssl;
	SslCertificate                  cert;
};

SSLSocketData::SSLSocketData(SslContext& ssl_context)
: ssl_context(ssl_context)
{
	SSLInit().AddThread();
	ssl = NULL;
}

SSLSocketData::~SSLSocketData()
{
	Close(0);
}

void SSLSocketData::SetSSLError(const char *context)
{
	if(sock) {
		int code;
		String text = SSLGetLastError(code);
		SetSockError(context, code, text);
	}
}

void SSLSocketData::SetSSLResError(const char *context, int res)
{
	if(sock) {
		int code = SSL_get_error(ssl, res);
		String out;
		switch(code) {
	#define SSLERR(c) case c: out = #c; break;
			SSLERR(SSL_ERROR_NONE)
			SSLERR(SSL_ERROR_SSL)
			SSLERR(SSL_ERROR_WANT_READ)
			SSLERR(SSL_ERROR_WANT_WRITE)
			SSLERR(SSL_ERROR_WANT_X509_LOOKUP)
			SSLERR(SSL_ERROR_SYSCALL)
			SSLERR(SSL_ERROR_ZERO_RETURN)
			SSLERR(SSL_ERROR_WANT_CONNECT)
	#ifdef PLATFORM_WIN32
			SSLERR(SSL_ERROR_WANT_ACCEPT)
	#endif
			default: out = "unknown code"; break;
		}
		SetSockError(context, code, out);
	}
}

bool SSLSocketData::Peek(int timeout_msec, bool write)
{
	if(ssl && !write && SSL_pending(ssl) > 0)
		return true;
	return Data::Peek(timeout_msec, write);
}

int SSLSocketData::Read(void *buf, int amount)
{
	if(!ssl)
		return Data::Read(buf, amount);
	int res = SSL_read(ssl, (char *)buf, amount);
	if(res == 0) {
		is_eof = true;
		if(SSL_get_shutdown(ssl) & SSL_RECEIVED_SHUTDOWN)
			return 0;
	}
	if(res <= 0)
		SetSSLResError("SSL_read", res);
#ifndef NOFAKEERROR
	if(fake_error && res > 0) {
		if((fake_error -= res) <= 0) {
			fake_error = 0;
			SetSockError("SSL_read", 0, "fake error");
			return -1;
		}
		else
			RLOG("SSLSocketData::Read: fake error after " << fake_error);
	}
#endif
	return res;
}

int SSLSocketData::Write(const void *buf, int amount)
{
	if(!ssl)
		return Data::Write(buf, amount);
	int res = SSL_write(ssl, (const char *)buf, amount);
	if(res <= 0)
		SetSSLResError("SSL_write", res);
	return res;
}

bool SSLSocketData::OpenClientUnsecured(const char *host, int port, bool nodelay, dword *my_addr,
                                        int timeout, bool blocking)
{
	return Data::OpenClient(host, port, nodelay, my_addr, timeout, /*blocking*/true);
}

bool SSLSocketData::Secure()
{
	if(!(ssl = SSL_new(ssl_context)))
	{
		SetSSLError("OpenClient / SSL_new");
		return false;
	}
	if(!SSL_set_fd(ssl, socket))
	{
		SetSSLError("OpenClient / SSL_set_fd");
		return false;
	}
	SSL_set_connect_state(ssl);
	int res = SSL_connect(ssl);
	if(res <= 0)
	{
		SetSSLResError("OpenClient / SSL_connect", res);
		return false;
	}
	cert.Set(SSL_get_peer_certificate(ssl));
	return true;
}

bool SSLSocketData::OpenAccept(SOCKET conn, bool nodelay, bool blocking)
{
	Attach(conn, nodelay, blocking);
	if(!(ssl = SSL_new(ssl_context)))
	{
		SetSSLError("Accept / SSL_new");
		return false;
	}
	if(!SSL_set_fd(ssl, socket))
	{
		SetSSLError("Accept / SSL_set_fd");
		return false;
	}
	SSL_set_accept_state(ssl);
	int res = SSL_accept(ssl);
	if(res <= 0)
	{
		SetSSLResError("Accept / SSL_accept", res);
		return false;
	}
	cert.Set(SSL_get_peer_certificate(ssl));
	return true;
}

bool SSLSocketData::Accept(Socket& socket, dword *ipaddr, bool nodelay, int timeout_msec)
{
	SOCKET connection = AcceptRaw(ipaddr, timeout_msec);
	if(connection == INVALID_SOCKET)
		return false;
	One<SSLSocketData> data = new SSLSocketData(ssl_context);
	if(!data->OpenAccept(connection, nodelay, is_blocking))
		return false;
	socket.Attach(-data);
	return true;
}

bool SSLSocketData::Close(int timeout_msec)
{
	if(ssl)
		SSL_shutdown(ssl);
	bool res = Data::Close(timeout_msec);
	if(ssl) {
		SSL_free(ssl);
		ssl = NULL;
	}
	return res;
}

Value SSLSocketData::GetInfo(String info) const
{
	if(info == SSLInfoCipher())           return SSL_get_cipher(ssl);
	if(info == SSLInfoCertAvail())        return cert.IsEmpty() ? 0 : 1;
	if(info == SSLInfoCertVerified())     return SSL_get_verify_result(ssl) == X509_V_OK ? 1 : 0;
	if(info == SSLInfoCertSubjectName())  return cert.IsEmpty() ? String::GetVoid() : cert.GetSubjectName();
	if(info == SSLInfoCertIssuerName())   return cert.IsEmpty() ? String::GetVoid() : cert.GetIssuerName();
	if(info == SSLInfoCertNotBefore())    return cert.IsEmpty() ? Date(Null)        : cert.GetNotBefore();
	if(info == SSLInfoCertNotAfter())     return cert.IsEmpty() ? Date(Null)        : cert.GetNotAfter();
	if(info == SSLInfoCertVersion())      return cert.IsEmpty() ? int(Null)         : cert.GetVersion();
	if(info == SSLInfoCertSerialNumber()) return cert.IsEmpty() ? String::GetVoid() : cert.GetSerialNumber();

	return Data::GetInfo(info);
}

bool SSLServerSocket(Socket& socket, SslContext& ssl_context, int port, bool nodelay, int listen_count, bool blocking)
{
	One<SSLSocketData> data = new SSLSocketData(ssl_context);
	if(!data->OpenServer(port, nodelay, listen_count, blocking))
		return false;
	socket.Attach(-data);
	return true;
}

bool SSLClientSocket(Socket& socket, SslContext& ssl_context, const char *host, int port, bool nodelay,
                     dword *my_addr, int timeout, bool blocking)
{
	One<SSLSocketData> data = new SSLSocketData(ssl_context);
	if(!data->OpenClient(host, port, nodelay, my_addr, timeout, blocking))
		return false;
	if(!data->Secure())
		return false;
	socket.Attach(-data);
	return true;
}

bool SSLClientSocketUnsecured(Socket& socket, SslContext& ssl_context, const char *host,
                              int port, bool nodelay, dword *my_addr, int timeout,
                              bool is_blocking)
{
	One<SSLSocketData> data = new SSLSocketData(ssl_context);
	if(data->OpenClientUnsecured(host, port, nodelay, my_addr, timeout, is_blocking)) {
		socket.Attach(-data);
		return true;
	}
	return false;
}

bool SSLSecureSocket(Socket& socket)
{
	SSLSocketData *sd = dynamic_cast<SSLSocketData *>(~socket.data);
	if(!sd)
		return false;
	return sd->Secure();
}
#endif

END_UPP_NAMESPACE
