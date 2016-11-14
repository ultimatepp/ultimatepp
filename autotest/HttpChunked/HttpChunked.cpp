#include <Core/Core.h>

using namespace Upp;

// base source : http://www.tcpipguide.com/free/t_HTTPDataLengthIssuesChunkedTransfersandMessageTrai-3.htm
String chunked_with_trailer = "HTTP/1.1 200 OK\r\nDate: Mon, 22 Mar 2004 11:15:03 GMT\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\nTrailer: Expires\r\n\r\n29\r\n<html><body><p>The file you requested is \r\n5\r\n3,400\r\n23\r\nbytes long and was last modified:\r\n\r\n1d\r\nSat, 20 Mar 2004 21:12:00 GMT\r\n13\r\n.</p></body></html>\r\n0\r\nExpires: Sat, 27 Mar 2004 21:12:00 GMT\r\n\r\n";

// base source : https://en.wikipedia.org/wiki/Chunked_transfer_encoding?oldid=430331176
String chunked_without_trailer = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nTransfer-Encoding: chunked\r\n\r\n26\r\nThis is the data in the first chunk\r\n6\r\n1C\r\nand this is the second one\r\n\r\n3\r\ncon\r\n8\r\nsequence\r\n0\r\n\r\n";

static void Server(String r)
{
	TcpSocket   server;
	if(server.Listen(4000, 10)) {

		TcpSocket socket;
		LOG("Waiting...");
		bool b = socket.Accept(server);
		if(b) {
			LOG("Connection accepted");
			HttpHeader http;
			http.Read(socket);
			socket.Put(r);
			socket.Close();
		}
	}
}


CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	HttpRequest::Trace();
	
	Thread a;

	LOG("chunked_without_trailer");
	LOG("****************");
	LOG(chunked_without_trailer);
	LOG("---------------");

	a.Run(callback1(Server, chunked_without_trailer));
	Sleep(200);
	HttpRequest r1("localhost:4000");
	ASSERT(r1.GET().Execute() == "This is the data in the first chunk\r\n6and this is the second one\r\nconsequence");
	a.Wait();
	
	LOG("----------------------------------------------------------");
	LOG("chunked_with_trailer");
	LOG("****************");
	LOG(chunked_with_trailer);
	LOG("---------------");

	a.Run(callback1(Server, chunked_with_trailer));
	HttpRequest r2("localhost:4000");
	ASSERT(r2.GET().Execute() == "<html><body><p>The file you requested is 3,400bytes long and was last modified:\r\nSat, 20 Mar 2004 21:12:00 GMT.</p></body></html>");
	a.Wait();

	LOG("=============== OK");
	
}
