#include "XmlRpc.h"
/**
* constructor. constructs just a new client.
*/
XmlRpcClient::XmlRpcClient() {
	usessl=false;
}
/**
* constructor, setting host, path and port. constructs a new client and sets the host (either
* a name or an ip address), its path and port to connect to later on.
* @param host the hostname or ip address of the server to contact later
* @param path the path of the eg. php file that answers the calls
* @param port the portnumber to connect to
* @see Execute()
*/
XmlRpcClient::XmlRpcClient(String host, String path, int port) {
	usessl=false;
	Host(host);
	Path(path);
	Port(port);
};
/*
HttpClient& XmlRpcClient::Get() {
	return http;
};
*/
/**
* set the host to connect to. set the host to connect to during method call.
* @param h the hostname or ip address of the server
*/
XmlRpcClient& XmlRpcClient::Host(String h) {
	http.Host(h);
	return *this;
}
/**
* set the path of the server. set the path of the server to connect to during method call.
* @param p the path of the server eg "/xmlserver/index.php"
*/
XmlRpcClient& XmlRpcClient::Path(String p) {
	http.Path(p);
	return *this;
}
/**
* set the port to connect to. set the tcp-port to connect to during method call.
* @param p the tcp-port of the server
*/
XmlRpcClient& XmlRpcClient::Port(int p) {
	http.Port(p);
	return *this;
}

/**
* execute a method call. execute the methodcall passed as argument.
* @param call the method to call at the given server, path and port.
* @see Host(), Path(), Port(), XmlRpcCall
*/
Value XmlRpcClient::Execute(const XmlRpcCall& call) {
	String request, xml;

	http.ClearHeaders();
	xml = call.ToXml();
	request << "User-Agent: Ultimate++ XML-RPC 1.0\r\n";
	request << "Host: "<<http.host<<"\r\n";
	request << "Content-Length: "<< (int) xml.GetLength()<<"\r\n";
	request << "Content-Type: text/xml\r\n\r\n";
	http.Post();
	http.std_headers = false;
	http.Headers(request);
	http.AddHeaders(xml);

	rawReply.Clear();
	if(usessl) {
		if(sslcontext.IsEmpty()) {
			SSL_METHOD *meth=TLSv1_client_method();
			sslcontext.Create(meth);
			sslcontext.VerifyPeer(false);
		}
		if(!SSLClientSocket(http.socket,sslcontext,http.host,http.port))
			return ErrorValue("SSL connection failed");
	}
	rawReply = http.Execute();

	if(http.GetStatusCode() == 200) {
		return xmlReply.ParseMethodResponse(rawReply);
	}
	return ErrorValue(NFormat("http status-code != 200 (%d)",http.GetStatusCode()));
};
