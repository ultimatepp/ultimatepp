#ifndef _XmlRpc_XmlRpc_h
#define _XmlRpc_XmlRpc_h
#include <Core/Core.h>
#include <ValueMap/ValueMap.h>
#include <Web/Web.h>
#include <Web/SSL/WebSSL.h>
/**
* Base class that converts Values to XML. This base class is used for XmlRpcCall and XmlRpcServer to
* convert their Values to the corresponding XML string to send to the other side.
*/
class ValueXmlConvert {
	public:
		typedef ValueXmlConvert CLASSNAME;
		void PrimitiveToXml(XmlNode& xml,const Value& v) const;
		void ValueToXml(XmlNode& xml, Value& v) const;
};

/**
* Class to create calls to a Xml Server. This class is used by XmlRpcClient to generate calls and send them
* to the server. A XmlRpcCall is passed to XmlRpcClient::Execute() to be sent to the server.
*/
class XmlRpcCall : ValueXmlConvert {
	private:
		String method;
		ValueArray va;
		VectorMap<int,int> NumberToValue;

	public:
		typedef XmlRpcCall CLASSNAME;
		XmlRpcCall();
		XmlRpcCall(String _method);
		XmlRpcCall(const char *_method);
		XmlRpcCall& operator() (const Value& v);
		void Clear();
		String ToXml() const;
};

/**
* Class that parses the Xml answers. XmlRpcParser is used by XmlRpcClient and XmlRpcServer to parse the
* incomming traffic and convert it to a Value that can be used to respond (at the server side) or
* returned to the user (client side).
*/
class XmlRpcParser {
	protected:
		Value ParsePrimitive(XmlParser& p);
		Value ParseStruct(XmlParser& p);
		Value ParseArray(XmlParser& p);
		Value Parse(XmlParser& p);

	public:
		typedef XmlRpcParser CLASSNAME;
		XmlRpcParser();

		Value ParseMethodCall(String s);
		Value ParseMethodResponse(String s);
};

/**
* Client class used to connect to a server. This base class is used for XmlRpcClient and XmlRpcServer to
* convert their Values to the corresponding XML string to send to the other side.
*/
class XmlRpcClient {
	private:
		HttpClient http;
		String rawReply;
		XmlRpcParser xmlReply;
		SSLContext sslcontext;
		bool usessl;

	public:
		typedef XmlRpcClient CLASSNAME;
		XmlRpcClient();
		XmlRpcClient(String host, String path, int Port);
		/**
		* enable the use of ssl encryption. enable the use of ssl encryption,
		* the default setting is off
		* @param ssl set the use on if true
		* @warning if no ssl context is set it sets the method to TLS
		* and does NOT verify the peer
		*/
		void UseSSl(bool ssl=true) {usessl=ssl;}
		XmlRpcClient& Host(String h);
		XmlRpcClient& Path(String p);
		XmlRpcClient& Port(int p);
		/**
		* get the ssl context. get the ssl context
		*/
		SSLContext&	GetSSLContext() {return sslcontext;}
		//HttpClient& Get();
		/*
		* returns the raw reply. returns the raw(= non parsed) reply of the server
		*/
		const String& GetRawReply() const	{return rawReply;}
		Value Execute(const XmlRpcCall& call);

};

/**
* Server Methods-Class. This is the base class of Methods that are used by the server to response
* to questions of any clients.
*/
class XmlRpcMethod {
	protected:
		String name;
	public:
		typedef XmlRpcMethod CLASSNAME;
		/**
		* constructs a method without a name. constructs a method without a name, should not be used!
		*/
		XmlRpcMethod()											{name.Clear();}
		/**
		* create a named method. create a method with name \p s. it will be called
		* if the call includes \p s as method name and the signature of the call
		* and the method are the same.
		* @param s the name of the method
		* @see Execute(), Signature()
		*/
		XmlRpcMethod(String s) 									{name = s;}
		/**
		* called if executed. this has to be implemented by any method that should be used.
		* it is called if one of the signatures of the method and the name match
		* @param result a reference to a Value in which after execution the result has to be
		* @param params a ValueArray containing only the parameters, not the methodname.
		* @param ipadd the ip address of the calling client
		* @see Signature()
		*/
		virtual void Execute(Value& result, ValueArray params, dword ipadd)  {}
		/**
		* return help of the message. this method just has to set the String \p help
		* to availeable help. no xml tags are allowed within this help.
		* @param help string to store the help for the message in
		*/
		virtual void Help(String& help)							{}
		/**
		* setting the signature. this method has to set the signature(s) of the methodcall:
		* each Value in the ValueArray \p sig is a signature, which also is a ValueArray, so
		* for each signature one has to add a ValueArray to \p sig.
		* if for example a functions looks like this int something(int x,double y) or
		* like int something(double x, string y, int z), the result would be:
		* @code
		* ValueArray first, second;
		* first.Add(1); //1 is an int, return value
		* first.Add(1); //1 is an int, x
		* first.Add(1.1); //1.1 is a double, y
		*
		* second.Add(1);
		* second.Add(1.1);
		* second.Add("abc");
		* second.Add(1);
		*
		* sig.Add(first);
		* sig.Add(second);
		* @endcode
		*/
		virtual void Signature(ValueArray& sig)					{}
		String GetName() 		 								{return name;}
};


/**
* Server class used to run a Xml-Rpc Server. This class implements a complete Server with extendeable functionality.
* a new Method is Added by creating a new class derived from XmlRpcMethod and add it by using the Add() function.
*/
class XmlRpcServer : ValueXmlConvert {
	public:
		/**
		* base class of server system methods. Internal Methods of the server should be derived from this class.
		* it is pretty the same as the XmlRpcMethod (actually it is derived from it) with two exceptions:
		* a "system." is added to the method name, the pointer to the method array is passed to this methods.
		* they can only be loaded within the server, not from external, like user methods.
		* @see loadSysMethods(), XmlRpcMethod
		*/
		class XmlRpcSysMethod : public XmlRpcMethod {
			protected:
				Vector<XmlRpcMethod *> *methods;
			public:
			XmlRpcSysMethod()										{}
			XmlRpcSysMethod(String s, Vector<XmlRpcMethod *>* _methods) : XmlRpcMethod("system."+s)	{methods=_methods;}
		};

		/**
		* connection information. connection informations are stored in this struct
		* @param ipadd ip address of the client
		* @param timeout max time to wait [ms]
		*/
		struct XmlRpcConnectionInfo {
			dword ipadd;
			int timeout;
		};

	protected:
		Vector<XmlRpcMethod *> methods;
		int server_port,listen;
		bool use_ssl,sysMethodsLoaded,sysMethods;
		SSLContext sslcontext;
		Socket socket;

		void	CheckHttpHeader(String& s);
		void 	CheckSignatureAndRun(Socket& s, XmlRpcMethod* m, ValueArray params, dword ipadd);
		String 	Response(Value& v);
		void	SendResponse(Socket &s, String s);
		/*
		* send http response. call SendResponse() avter calling Response() for value \p v.
		* @param s socket the connection is on
		* @param v value to be sent (non error)
		*/
		void	SendResponse(Socket &s, Value& v) 	{SendResponse(s,Response(v));}
		void 	Error(Socket& s, int code, String desc);
		void 	loadSysMethods(bool load=true);

	public:
		typedef XmlRpcServer CLASSNAME;
		XmlRpcServer();
		XmlRpcServer& Add(XmlRpcMethod* m);
		void	Clear();
		/**
		* get the ssl context. get the ssl context
		*/
		SSLContext&	GetSSLContext() {return sslcontext;}
		bool	Open(int port, int listen_count = 5, bool ssl=false);
		bool	Reopen();
		void	Close();
		void 	RunOnce(int msec);
		void	Run();
		void	SysMethods(bool on=true);
		/**
		* return wether the system methods are loaded.return wether the system methods are loaded
		*/
		bool	IsSysMethodsLoaded()			{return sysMethodsLoaded;}

		void	HandleRequest(Socket &s, Value &v);
		/**
		* return wether socket is open.return wether socket is open
		*/
		bool	IsOpen() const                 { return socket.IsOpen(); }
		/**
		* return wether socket had an error.return wether socket had an error
		*/
		bool	IsError() const                { return socket.IsError(); }
		/**
		* return the port listening on.return the port listening on
		*/
		int		GetPort() const                { return server_port; }
		/**
		* get the server socket. get the server socket
		*/
		Socket&	GetSocket()                    { return socket; }
};

/**
* @mainpage XML Remote Procedure Call implementation
* @version R23
*
* this is a XML rpc implementation for <a href="http://www.ultimatepp.org">Ultimate++</a>, the implementation follows the specification
* found at http://www.xmlrpc.com/spec
* @section Server
* a server can be created by using the XmlRpcServer class and its methods:
* @code
* //small server (NOT WORKING because XmlRpcMethod needs to be implemented)
* CONSOLE_APP_MAIN {
* 	XmlRpcMethod m("hello");		//create the hello method (in this case with NO FUNCTIONALITY and wrong signature
* 	XmlRpcServer server; 			//create the server
* 	server.SysMethods();			//switch on sys methods
* 	server.Add(&m);				//add the method "hello"
* 	server.Open(8080);			//listen on port 8080
* 	server.Run();				//and run
* }
* @endcode
* @see XmlRpcServer, XmlRpcMethod
*
* @section Client
* a client can be implemented by using the XmlRpcClient class.
* @code
* //function checking for methods on the server side
* void CheckForMethods(Value &v) {
*	XmlRpcClient xmlClient;			//client
* 	XmlRpcCall call("system.listMethods");	//call with sys method "listMethods"
*
* 	xmlClient.Host("www.ultimatepp.org");	//set Host
* 	xmlClient.Path("/server/index.php");	//set path
* 	xmlClient.Port(80);			//and port to connect to
* 	v = xmlClient.Execute(call);		//run the call
* }
* @endcode
* @see XmlRpcClient, XmlRpcCall
*/
#endif
