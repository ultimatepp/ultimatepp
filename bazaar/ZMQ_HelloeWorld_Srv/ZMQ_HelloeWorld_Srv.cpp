
//
//  Hello World server in C++
//  Binds REP socket to tcp://*:5555
//  Expects "Hello" from client, replies with "World"
//
#include <ZMQ/ZMQ.h>
//#include <unistd.h>
//#include <stdio.h>
//#include <string.h>
#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN{
	String  bind_str("tcp://*:5555");
	
	//  Prepare our context and socket
	zmq::context_t context (1);
	zmq::socket_t socket (context, ZMQ_REP);
	socket.bind (~bind_str);

	while (true) {
		zmq::message_t request;
		
		//  Wait for next request from client
		socket.recv (&request);
		printf ("Received Hello");

		//  Do some 'work'
		Sleep(1000);

		//  Send reply back to client
		zmq::message_t reply (6);
		memcpy ((void *) reply.data (), "World", 5);
		socket.send (reply);
	}
}

