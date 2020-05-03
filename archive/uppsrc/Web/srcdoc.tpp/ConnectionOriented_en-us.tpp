topic "Connection-Oriented Socket Tutorial";
[ $$0,0#00000000000000000000000000000000:Default]
[l17;r17;b4;a4;t167;~~~33;2 $$1,1#59513341195373083833142200425680:Tutorial`-Body1]
[l17;r17;KO0; $$2,2#64182314361042239925864983220390:Tutorial`-Bullets1]
[l17;r17;b4;a8;t167;~~~33;* $$3,3#52748490480400044360925438004072:Tutorial`-Heading1]
[l17;r17;b4;a8;t167;~~~33;*4 $$4,4#44225361735100545001598530750215:Tutorial`-TopicHeading]
[l83;r17;Kt167;~~~33;C@(28.42.200)1 $$5,5#21408285951729133491252488253306:Tutorial`-Code]
[l17;r17;b4;a8;t167;~~~33;*/2 $$6,6#24855341585039799381315381340144:Tutorial`-Headin2]
[l83;r17;Kt167;~~~33;C@(28.127.0)1 $$7,7#81132523355065615153245323750562:Tutorial`-CodeComment]
[{_}%EN-US 
[s4; Connection`-Oriented Socket Tutorial&]
[s3; &]
[s3; First steps&]
[s1; First you must create a Socket object. When you create this 
object, the socket is in an unconnected state; it is not associated 
with any host. In order for the socket to send and receive information, 
it must be bound to a host or destination and port. &]
[s1; &]
[s3; Client Side&]
[s1; To create an outbound connection, use the ClientSocket function 
to bind your socket to a host and port. Client socket returns 
a boolean value, indicating if the connection was successful 
or not.&]
[s6; Example:&]
[s5; Socket my`_sock;&]
[s5; int port`=2000;&]
[s5; String host `= `"192.168.1.2`";&]
[s5; if( ClientSocket(my`_sock, port, host) )&]
[s5; `{&]
[s5; -|my`_sock.Write(`"hello world!`");&]
[s5; -|my`_sock.Close();&]
[s5; `}&]
[s3; &]
[s3; Server Side&]
[s1; Server side applications listen for incoming socket connections. 
A socket server listens and hands off connections to other individual 
sockets (usually separate threads). &]
[s1; To open a port to listen for incoming connections, you use the 
U`+`+ [C ServerSocket] function. [C ServerSocket()] requires as input 
a socket object and the port to listen on.&]
[s6; Example:&]
[s5; Socket accept`_socket, data`_socket;&]
[s5; int port `= 2000;&]
[s7; // Listen for connections using `_accept`_socket;&]
[s5; if( !ServerSocket(accept`_socket, port) )`{&]
[s5; -|throw Exc(`"Couldn`'t bind socket on the local port.`");&]
[s5; `}&]
[s7; // You can do this in a loop to accept many connections:&]
[s5; if( accept`_socket.IsOpen() )`{&]
[s5; -|dword ip`_addr;&]
[s7; -|// Hand off successful connection to `_data`_socket&]
[s5; -|if( !accept`_socket.IsError() `&`& accept`_socket.Accept(data`_socket, 
`&ip`_addr) )&]
[s5; -|`{&]
[s5; -|-|Cout() << `"Connection from `" << FormatIP(m`_ipaddr) << `"`\n`";&]
[s7; -|-|// Read from the socket until it is closed, has an error, 
or you see an end`-of`-file marker&]
[s7; -|-|// (EOF optional and application`-specific)&]
[s5; -|-|while(data`_socket.IsOpen() `&`& !data`_socket.IsEof() `&`& 
!data`_socket.IsError())&]
[s5; -|-|`{&]
[s5; -|-|-|Cout() << data`_socket.Read();&]
[s5; -|-|`}&]
[s5; -|`}&]
[s5; -|Cout() << `"`\n`";&]
[s5; `}&]
[s1; &]
[s1; ServerSocket `"binds`" the socket to the port. You can then 
use the Socket object to read/write data.&]
[s1; &]
[s1; So you can see the [C `_accept`_socket ]gets the connection and 
hands it off to [C `_data`_socket ]with the [C Accept()] method. 
Then [C `_data`_socket] has the connection.&]
[s1; Servers use this method with `"Thread Pools`" of socket connections 
to allow for a certain number of sockets to exist (usually 1 
per thread).&]
[s0; ]