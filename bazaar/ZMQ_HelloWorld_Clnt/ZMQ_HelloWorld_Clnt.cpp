#include <ZMQ/ZMQ.h>


#include <Core/Core.h>

using namespace Upp;
//
//  Hello World client
//  Connects REQ socket to tcp://localhost:5555
//  Sends "Hello" to server, expects "World" back
//
//#include <string.h>
//#include <stdio.h>
//#include <unistd.h>

CONSOLE_APP_MAIN{
	String connect_str("tcp://localhost:5555");
	
	zmq::context_t context (1);
	//  Socket to talk to server
	printf ("Connecting to hello world server...\n");
	zmq::socket_t socket (context, ZMQ_REQ);
	socket.connect(~connect_str);

	int request_nbr;
	for (request_nbr = 0; request_nbr != 10; request_nbr++) {
        zmq::message_t request(6);
        memcpy ((void *) request.data (), "Hello", 5);
        
        printf ("Sending Hello %d...\n", request_nbr);
        socket.send(request);
        
        zmq::message_t reply;
        socket.recv (&reply);
        printf ("Received World %d\n", request_nbr);
    }
}

