#ifndef flagNOSSL

#include "WebSSL.h"

NAMESPACE_UPP

extern bool HttpClient_Trace__;

#ifdef _DEBUG
#define LLOG(x)      if(HttpClient_Trace__) RLOG(x); else;
#else
#define LLOG(x)
#endif

HttpsClient::HttpsClient()
{
	secure = true;
}

bool HttpsClient::ProxyConnect()
{
	if(use_proxy) {
		int start_time = msecs();
		int end_time = msecs() + timeout_msecs;
		while(!socket.PeekWrite(1000)) {
			int time = msecs();
			if(time >= end_time) {
				error = NFormat(t_("%s:%d: connecting to host timed out"), socket_host, socket_port);
				Close();
				return false;
			}
		}
		String host_port = host;
		if(port)
			host_port << ':' << port;
		else
			host_port << ":443";
		String request;
		request << "CONNECT " << host_port << " HTTP/1.1\r\n"
		        << "Host: " << host_port << "\r\n";
		if(!IsNull(proxy_username))
			request << "Proxy-Authorization: Basic "
			        << Base64Encode(proxy_username + ':' + proxy_password) << "\r\n";
		request << "\r\n";
		LLOG(request);
		int written = 0;
		while(msecs() - end_time < 0) {
			int nwrite = socket.WriteWait(request.GetIter(written), min(request.GetLength() - written, 1000), 1000);
			if(socket.IsError()) {
				error = Socket::GetErrorText();
				Close();
				return false;
			}
			if((written += nwrite) >= request.GetLength())
				break;
		}
		if(written < request.GetLength()) {
			error = NFormat(t_("%s:%d: timed out sending request to server"), host, port);
			Close();
			return false;
		}
		String line = ReadUntilProgress('\n', start_time, end_time, false);
		LLOG("P< " << line);
		if(socket.IsError()) {
			error = Socket::GetErrorText();
			Close();
			return false;
		}
		if(!line.StartsWith("HTTP") || line.Find(" 2") < 0) {
			error = "Invalid proxy reply: " + line;
			Close();
			return false;
		}
		while(line.GetCount()) {
			line = ReadUntilProgress('\n', start_time, end_time, false);
			if(*line.Last() == '\r')
				line.Trim(line.GetCount() - 1);
			LLOG("P< " << line << " len " << line.GetCount());
			if(socket.IsError()) {
				error = Socket::GetErrorText();
				Close();
				return false;
			}
		}
		use_proxy = false;
		while(!socket.PeekWrite(1000)) {
			int time = msecs();
			if(time >= end_time) {
				error = NFormat(t_("%s:%d: connecting to host timed out"), socket_host, socket_port);
				Close();
				return false;
			}
		}
	}
	return true;
}

bool HttpsClient::IsSecure()
{
	return secure;
}

bool HttpsClient::CreateClientSocket()
{
	if(!secure)
		return HttpClient::CreateClientSocket();
	if(!ssl_context) {
		ssl_context = new SSLContext;
		if(!ssl_context->Create(const_cast<SSL_METHOD *>(SSLv3_client_method()))) {
			error = t_("Error creating SSL context.");
			return false;
		}
	}
	if(!SSLClientSocketUnsecured(socket, *ssl_context, socket_host,
	                             socket_port ? socket_port : DEFAULT_HTTPS_PORT, true, NULL, 0, false)) {
		error = Socket::GetErrorText();
		return false;
	}
	socket.Linger(0);
	if(!ProxyConnect())
		return false;
	SSLSecureSocket(socket);
	return true;
}

END_UPP_NAMESPACE

#endif
