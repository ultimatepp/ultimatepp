#ifndef flagNOSSL

#include "WebSSL.h"

NAMESPACE_UPP

HttpsClient::HttpsClient()
{
	secure = true;
}

bool HttpsClient::CreateClientSocket()
{
	if(!secure)
		return HttpClient::CreateClientSocket();
	if(!ssl_context) {
		ssl_context = new SSLContext;
		if(!ssl_context->Create(SSLv3_client_method())) {
			error = t_("Error creating SSL context.");
			return false;
		}
	}
	if(!SSLClientSocket(socket, *ssl_context, socket_host, socket_port, true, NULL, 0, false)) {
		error = Socket::GetErrorText();
		return false;
	}
	socket.Linger(0);
	return true;
}

END_UPP_NAMESPACE

#endif
