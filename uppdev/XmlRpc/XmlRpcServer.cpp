#include "XmlRpc.h"
#include "XmlRpcSysMethods.h"

#define	EMETHOD			100
#define EBADREQUEST 	400
#define ENOTFOUND		404
#define ETIMEOUT		408
#define EINTERNAL 		500
#define EIMPLEMENTED	501


/**
* constructs a new Xmlrpc server. it constructs a new XmlRpc server, clears the list of Methods (including SysMethods).
*/
XmlRpcServer::XmlRpcServer() {
	sysMethodsLoaded=sysMethods=false;
	Clear();
	//sslcontext.UseCertificate(String certificate¸ String private_key¸ bool cert_asn1 = false)
}
/**
* clears the method list. this method clears the list of methods provided by the server and reloads the SysMethods,
* if they were loaded before.
*/
void XmlRpcServer::Clear() {
	sslcontext.Clear();
	loadSysMethods(false);
	methods.Clear();
	loadSysMethods(sysMethods);
}
/**
* loads the SysMethods. this method loads or unloads the special system methods provided by the server.
* @param load if set the methods will be loaded else they will be unloaded
*/
void XmlRpcServer::loadSysMethods(bool load) {
	if(load) {
		if(sysMethodsLoaded)
			return;
		methods.Add() = new XmlRpcSysListMethods("listMethods",&methods);
		methods.Add() = new XmlRpcSysMethodHelp("methodHelp",&methods);
		methods.Add() = new XmlRpcSysMethodSignature("methodSignature",&methods);
		sysMethodsLoaded=true;
	}else {
		if(!sysMethodsLoaded)
			return;
		for(int i=0;i<methods.GetCount();i++) {
			if(methods.At(i)->GetName().Left(7) == "system.")
				delete methods[i];
				methods.Remove(i);
				i--;
		}
		sysMethodsLoaded=false;
	}
}
/**
* set wether SysMethods should be provided. if called with \p on = true the Sysmethods will be
* loaded and even reloaded after clearing the non system messages with Clear(). The systme messages that
* are provided at the moment are: system.listMethods, system.methodHelp, system.methodSignature.
* @param on if set SystemMethods will be provided by the server
*/
void XmlRpcServer::SysMethods(bool on) {
	sysMethods=on;
	loadSysMethods(on);
}
/**
* add a usermethod to the server. one can add own methods to the server by calling this method.
* The provided Method will be called if the name and one of the signatures of an incoming request
* match.
* Only methods with non empty names that don't start with "system." are allowed!
* @param m a pointer to a XmlRpcMethod derived object
* @returns reference to the current server
* @see XmlRpcMethod
*/
XmlRpcServer& XmlRpcServer::Add(XmlRpcMethod* m) {
	if(m->GetName().IsEmpty())
		return *this;
	if(m->GetName().Left(7)=="system.")
	   return *this;
	methods.Add(m);
	return *this;
}
/**
* open a socket. open a socket, bind to it and listen.
* @param port port to listen at
* @param listen_count number of clients in count
* @param ssl use ssl encryption, default is no ssl encryption. If ssl should be used but the ssl
* context is not set XmlRpcServer trys to load the ConfigFile("cacert.pem") and ConfigFile("pkey.pem")
* if this fails the server wont start.
* @warning if ssl context is not set the peer won't be veryfied
* @returns true if succesfull, false else
*/
bool XmlRpcServer::Open(int port, int listen_count , bool ssl) {
	server_port=port;
	listen=listen_count;
	use_ssl=ssl;
	if(ssl)  {
		if(sslcontext.IsEmpty()) {
			SSL_METHOD *meth = TLSv1_server_method();
			sslcontext.Create(meth);
			sslcontext.VerifyPeer(false);
			String certfile(ConfigFile("cacert.pem")), keyfile(ConfigFile("pkey.pem"));
			if(!FileExists(certfile) || !FileExists(keyfile)) {
				String debug;
				debug << "unable to find: ";
				if(!FileExists(certfile))
					debug << certfile << " ";
				if(!FileExists(keyfile))
					debug << keyfile;
				debug <<"\n";
				LOG(debug);
				return false;
			}
			String cert(LoadFile(certfile));
			String pkey(LoadFile(keyfile));
			sslcontext.UseCertificate(cert,pkey);
		}
		return SSLServerSocket(socket, sslcontext, server_port, false, listen, true);
	}
	else
		return ServerSocket(socket, server_port, false, listen, true);
}
/**
* close the socket. close the socket
*/
void XmlRpcServer::Close() {
	socket.Close();
}
/**
* close and reopen the socket. close and reopen the socket by calling Close() and Open().
*/
bool XmlRpcServer::Reopen() {
	Close();
	return Open(server_port,listen,use_ssl);
}
/**
* wait for connections. wait for one incomming connection return after max \p msec milliseconds
* of waiting or after accepting a connection or after an error.
* @param msec time to wait for one connection before returning
*/
void XmlRpcServer::RunOnce(int msec) {
	if(!socket.IsOpen())
		return;
	Socket s;
	if(socket.Peek(msec)) {
		XmlRpcConnectionInfo ci;

		if(socket.Accept(s,&ci.ipadd,true,0)) {
			ci.timeout=10000;
			Thread().Run(THISBACK2(HandleRequest,s,RawToValue(ci)));
		} else {
				RLOG(NFormat("accept error: %d %s",socket.GetErrorCode(),socket.GetErrorText()));
		}

	}
	if(socket.IsError())
		RLOG(NFormat("socket error: %d %s",socket.GetErrorCode(),socket.GetErrorText()));
}
/**
* wait for connections. wait for incomming connections, accept them and pass them to
* HandleRequest() and wait again in an infinite loop.
* @see RunOnce()
*/
void XmlRpcServer::Run() {
	if(!socket.IsOpen())
		return;

	while(true) {
		RunOnce(1000);
	}
}
/*
* handle incomming requests. parse and response to incomming method calls.
* @param s the socket the connection is on
* @param ipadd the ip address of the calling client
* @param value a Value containing ConnectionInfo
*/
void XmlRpcServer::HandleRequest(Socket &s, Value &value) {
	XmlRpcConnectionInfo ci = ValueTo<XmlRpcConnectionInfo>(value);

	String data, response;
	LOG("\n\ngot a Request");
	data = s.Read(ci.timeout);
	if(data.IsEmpty()) {
		Error(s,ETIMEOUT,"Request Timeout");
		return;
	}
	XmlRpcParser p;
	XmlNode n;
	Value v;
	v=p.ParseMethodCall(data);
	if(v.IsError()) {
		Error(s,EBADREQUEST,"Bad Request: unable to understand xml method call");
		return;
	}

	ValueArray params(v);
	if(params.GetCount()<1 || params[0].GetType() != STRING_V) {
		Error(s,EBADREQUEST,"Bad Request: unable to understand xml method call");
		return;
	}
	for(int i=0;i<methods.GetCount();i++) {
		if(methods[i]->GetName()==params[0]) {
			CheckSignatureAndRun(s,methods[i],params, ci.ipadd);
			return;
		}
	}
	if(response.IsEmpty())
		Error(s,ENOTFOUND,"Not Found: the requested method was not found");

}
/**
* check for a signature and run the method. check wether the XmlRpcMethod object has a signature
* equal to the one of the call and if so run the method and send the response
* @param s socket the connection is on
* @param m method that should be run
* @param params the methodname and the parameter received within the method call
* @param ipadd the ip address of the calling client
*/
void XmlRpcServer::CheckSignatureAndRun(Socket& s, XmlRpcMethod* m, ValueArray params, dword ipadd) {
	ValueArray va;
	m->Signature(va);
	LOG(NFormat("function:  %s",m->GetName()));
	LOG(NFormat("Signature of function: %s",Value(va).ToString()));
	LOG(NFormat("Signature of call: %s",Value(params).ToString()));

	if(va.IsEmpty()) {
		Error(s,EINTERNAL,"internal server error");
		return;
	}

	for(int i=0; i<va.GetCount(); i++) {
		if(va[i].GetType()!=VALUEARRAY_V) {
			Error(s,EINTERNAL,"internal server error");
			return;
		}
		ValueArray vaa(va[i]);
		if(vaa.GetCount()<1) {
			Error(s,EINTERNAL,"internal server error ");
			return;
		}
		if(vaa.GetCount() == (params.GetCount())) {
			bool execute=true;
			for(int j=1;j<vaa.GetCount() && execute;j++)
				execute = execute && (vaa.Get(j).GetType() == params.Get(j).GetType());
			if(execute) {
				LOG("found given signature");
				Value result;
				params.Remove(0);
				m->Execute(result,params, ipadd);
				LOG(NFormat("Result: %s",result.ToString()));
				if(!result.IsError()) {
					SendResponse(s,result);
					return;
				}
				Error(s,EMETHOD,NFormat("Method Error: %s",GetErrorText(result)));
				return;
			}
		}
	}
	Error(s,EBADREQUEST,"Bad Request: wrong method signature");
}
/**
* create xml response. create xml response.
* @param v the value to be send
* @return returns the string containing the xml representation of v
*/
String XmlRpcServer::Response(Value& v) {
	XmlNode n;
	XmlNode&xml = n("methodResponse")("params")("param").Add("value");
	ValueToXml(xml,v);
	return  AsXML(n,XML_HEADER);
}
/**
* create an error response. create an error response with \p num as error number and \p desc as
* description of the error
* @param s socket the connection is on
* @param num the error number
* @param desc the description of the error
*/
void XmlRpcServer::Error(Socket& s, int num, String desc) {
	XmlNode n;
	LOG(NFormat("ERROR: %s (%d)",desc,num));
	Value errorcode(num);
	XmlNode& code=n("methodResponse")("fault")("value")("struct").Add("member");
	code.Add("name").Add().CreateText("faultCode");
	code.Add("value").Add("int").Add().CreateText(errorcode.ToString());
	XmlNode& text=n("methodResponse")("fault")("value")("struct").Add("member");
	text.Add("name").Add().CreateText("faultString");
	text.Add("value").Add("string").Add().CreateText(desc);
	SendResponse(s,AsXML(n,XML_HEADER));
}
/**
* send http response. send a response with a http header and the data in \p response.
* @param s socket the connection is on
* @param response a string sent as body (should be an xml document)
*/
void XmlRpcServer::SendResponse(Socket& s, String response) {
	String out;
	out <<"HTTP/1.0 200 OK\r\n";
	out <<"Server: Ultimate++ XmlRPC Server/any OS\r\n";
	out <<"Connection: close\r\n";
	out <<"Content-Length: "<<response.GetLength()<<"\r\n";
	out <<"Content-Type: text/xml\r\n\r\n";
	out << response;
	s.Write(out);
	LOG("sent");
	s.Close();
	LOG("closed");
}
