#include "IPNServer.h"

NAMESPACE_UPP

// constructor - defaults to port 8787 and connection timeout of 5 minutes (300 sec.)
IPNServer::IPNServer(int port) : ScgiServer(port)
{
}

void IPNServer::OnAccepted()
{
	Cout() << "Accepted connection from client " << FormatIP(clientIP) << "\n";
}

void IPNServer::OnRequest()
{
	// we handle just POST requests, GET not allowed
	if(HasPostData())
	{
		// create context for ssl connection
		SSLContext context;
		if(!context.Create(const_cast<SSL_METHOD *>(SSLv3_client_method())))
		{
			Cerr() << "Error creating SSL context.";
			return;
		}

		// re-send back data to paypal site, in order to check
		// if data was from them
		Socket answerSocket;
		if(!SSLClientSocket(answerSocket, context, payServer, payPort))
		{
			Cout() << "Error creating connection to PayPal\n";
			return;
		}
		String req = "cmd=_notify-validate";
		for(int i = 0; i < post.GetCount(); i++)
			req << "&" << ToLower(post.GetKey(i)) << "=" << UrlEncode(post[i]);

		String header;
		header << 
			"POST /cgi-bin/webscr HTTP/1.0\r\n"
			"Content-Type: application/x-www-form-urlencoded\r\n"
			"Content-Length: " << req.GetCount() << "\r\n\r\n"
		;
		answerSocket.Write(header + req);
		
		// wait for PayPal server answer
		req = "";
		while(answerSocket.IsOpen() && !answerSocket.IsEof() && !answerSocket.IsError())
			req << answerSocket.Read();
		answerSocket.Close();
		
		// 'VERIFIED' or 'INVALID' answer are on last line of received HTTP packet;
		// the rest is just HTTP headers and can be discarded
		// in detail, the LAST part of req string must be "VERIFIED"
		if(req.Right(8) == "VERIFIED")
			WhenVerified();
		else
			WhenInvalid();
	}
	else
		Cout() << "No POST data on PayPal request\n";

	// this one avoids "500 server error".... don't know why
	// but paypal do check the http response along with reply packet
	// on direct http access it'll show just a blank page
	clientSock.Write("Content-Type: text/plain\r\n\r\n");
}

VectorMap<String, String> IPNServer::GetVerified() const
{
	VectorMap<String, String> data;
	for(int i = 0; i < post.GetCount(); i++)
		data.Add(ToUpper(post.GetKey(i)), post[i]);
	return data;
}

void IPNServer::OnClosed()
{
	Cout() << "Connection with " << FormatIP(clientIP) << " closed\n";
}

// runs the server
void IPNServer::Run(void)
{
	// runs the SCGI server
	ScgiServer::Run();
}

END_UPP_NAMESPACE